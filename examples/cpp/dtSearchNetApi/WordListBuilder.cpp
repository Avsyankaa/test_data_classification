#include "stdafx.h"

#include "WordListBuilder.h"

using namespace System;

namespace dtSearch { namespace Engine
{
	WordListBuilder::WordListBuilder ()
	{	CountCreate(otWordListBuilder);
		m_nthWordNum = 0;
		m_Count = 0;
		m_fOpen = false;
		m_builder = new dtsWordListBuilder ();
	}

	WordListBuilder::~WordListBuilder ()
	{	CountDelete(otWordListBuilder);
		Dispose(false);
	}

	bool WordListBuilder::OpenIndex (String* indexName)
	{
		// Close index if necessary
		if (m_fOpen)
			CloseIndex();

		// Convert the index name into UTF8 and pass it to openIndex of dtsWordListBuilder
		DStringProperty sPath;
		sPath.set(indexName);

		long result = m_builder->openIndex(sPath);
		m_fOpen = (result == SUCCESS);

		return m_fOpen;
	}

	void WordListBuilder::CloseIndex ()
	{
		// Close index if it is open and reset the open status
		if (m_fOpen)
			m_builder->closeIndex();
		m_fOpen = false;
	}


	long WordListBuilder::ListWords (String* cursorWord, long range)
	{	debugLog(".NET WordListBuilder::ListWords");
		// Convert the cursor word to UTF8 and pass to listWords of dtsWordListBuilder
		DStringProperty cw;
		cw.set(cursorWord);
		
		m_builder->listWords(cw, range);
		m_Count = m_builder->getCount();
		m_nthWordNum = -1;
		debugLog(".NET WordListBuilder::ListWords Returning count: ", m_Count);
		return m_Count;
	}

	String* WordListBuilder::GetNthWord (long whichWord)
	{
		gotoWord (whichWord);
		return m_nthWord;
	}
	
	long WordListBuilder::GetNthWordCount (long whichWord)
	{
		gotoWord (whichWord);
		return m_nthWordCount;
	}

	long WordListBuilder::GetNthWordDocCount (long whichWord)
	{
		gotoWord (whichWord);
		return m_nthWordDocCount;
	}

	String* WordListBuilder::GetNthWordField (long whichWord)
	{
		gotoWord (whichWord);
		return m_nthWordField;
	}

	long WordListBuilder::ListMatchingWords (String* toMatch, long nLimit, SearchFlags searchFlags, long fuzziness)
	{	debugLog(".NET WordListBuilder::ListMatchingWords");
		DStringProperty  tm;
		tm.set(toMatch);
		m_builder->listMatchingWords(tm, nLimit, searchFlags, fuzziness);
		m_Count = m_builder->getCount();
		m_nthWordNum = -1;
		debugLog(".NET WordListBuilder::ListMatchingWords returning count", m_Count);
		return m_Count;
	}

	long WordListBuilder::ListFields ()
	{
		m_builder->listFields (-1);
		m_Count = m_builder->getCount();
		m_nthWordNum = -1;
		return m_Count;
	}

	// ---- PROTECTED METHODS BELOW ----
	//
	
	void WordListBuilder::DisposeUnmanagedItems()
	{	if (m_builder) {
			dtsWordListBuilder *toDelete = m_builder;
			m_builder = 0;
			delete toDelete;
			}
	}
		

	void WordListBuilder::gotoWord (long whichWord)
	{	debugLog(".NET WordListBuilder::gotoWord", whichWord);
		if ((whichWord < 0) || (whichWord >= m_Count))
		{
			m_nthWord = new String ("");
			m_nthWordCount = 0;
			m_nthWordDocCount = 0;
			m_nthWordField = NULL;
			return;
		}

		if (whichWord == m_nthWordNum)
			return;

		// Allocate a buffer of unmanaged memory and treat it as a char*
		char buf[512];
		long hitCount = 0;
		long docCount = 0;
		buf[0] = 0;
		m_builder->getWord(whichWord, buf, sizeof buf, hitCount, docCount);
		debugLog("   Word = ", buf);
		m_nthWord = DString::allocNetString(buf);
		m_nthWordNum = whichWord;
		m_nthWordCount = hitCount;
		m_nthWordDocCount = docCount;

		buf[0] = 0;
		m_builder->getField(whichWord, buf, sizeof buf);
		if (!strIsBlank(buf))
			debugLog("    Field = ", buf);
		m_nthWordField = DString::allocNetString(buf);

	}

}}
