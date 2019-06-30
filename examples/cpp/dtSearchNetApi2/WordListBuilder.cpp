#include "stdafx.h"

#include "WordListBuilder.h"

using namespace System;

namespace dtSearch { namespace Engine
{
	WordListBuilder::WordListBuilder ()
	{	CountCreate(otWordListBuilder);
		m_nthWordNum = 0;
		Count = 0;
		LastError = dtSearch::Engine::ErrorCodes::dtsErOK;
		m_fOpen = false;
		m_builder = new dtsWordListBuilder ();
		m_searchFilter = nullptr;
		Flags = (WordListBuilderFlags)0;
		m_lastFlags = Flags;
	}

	WordListBuilder::~WordListBuilder ()
	{	CountDelete(otWordListBuilder);
		
	}

	bool WordListBuilder::OpenIndex (String^ indexName)
	{	beforeList();

		// Close index if necessary
		if (m_fOpen)
			CloseIndex();

		// Convert the index name into UTF8 and pass it to openIndex of dtsWordListBuilder
		DStringProperty sPath;
		sPath.set(indexName);

		long result = m_builder->openIndex(sPath);
		m_fOpen = (result == SUCCESS);
		LastError = (ErrorCodes) m_builder->getLastError();

		return m_fOpen;
	}

	void WordListBuilder::CloseIndex ()
	{
		// Close index if it is open and reset the open status
		if (m_fOpen)
			m_builder->closeIndex();
		m_fOpen = false;
	}


	long WordListBuilder::ListWords (String^ cursorWord, long range)
	{	debugLog(".NET WordListBuilder::ListWords");
		// Convert the cursor word to UTF8 and pass to listWords of dtsWordListBuilder
		DStringProperty cw;
		cw.set(cursorWord);
		
		beforeList();
		m_builder->listWords(cw, range);
		afterList();

		return Count;
	}

	void WordListBuilder::afterList() {
		Count = m_builder->getCount();
		LastError = (dtSearch::Engine::ErrorCodes) m_builder->getLastError();
		debugLog("Count: ", Count);
		if (LastError != dtSearch::Engine::ErrorCodes::dtsErOK) 
			debugLog("Error: ", (int) LastError);
		m_nthWordNum = -1;
		}

	String^ WordListBuilder::GetNthWord (long whichWord)
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

	String^ WordListBuilder::GetNthWordField (long whichWord)
	{
		gotoWord (whichWord);
		return m_nthWordField;
	}

	long WordListBuilder::ListMatchingWords (String^ toMatch, long nLimit, SearchFlags searchFlags, long fuzziness)
	{	debugLog(".NET WordListBuilder::ListMatchingWords");
		DStringProperty  tm;
		tm.set(toMatch);
		beforeList();
		
		m_builder->listMatchingWords(tm, nLimit, (long) searchFlags, fuzziness);
		afterList();

		return Count;
	}

	long WordListBuilder::ListFieldValues (String^ field, String^ toMatch, long nLimit)
	{	debugLog(".NET WordListBuilder::ListFieldValues");
		DStringProperty  sToMatch;
		sToMatch.set(toMatch);
		
		DStringProperty sField;
		sField.set(field);
		beforeList();
		m_builder->listFieldValues(sField, sToMatch, nLimit);
		afterList();


		return Count;
	}
	
	// If a search filter is attached to this WordListBuilder, pass it to the unmanaged
	// implementation.
	void WordListBuilder::applySearchFilter()
	{	if (m_searchFilter != nullptr) {
			dtsSearchFilter *pFilter = m_searchFilter->getImplementation();
			if (pFilter) {
				dtsSearchFilterHandle hFilter = pFilter->getHandle();
				if (hFilter)
					m_builder->setSearchFilter(hFilter);
				}
			}
	}

	long WordListBuilder::ListFields ()
	{	beforeList();
		m_builder->listFields (-1);
		afterList();
		return Count;
	}
	
	bool WordListBuilder::Sort(SortFlags flags) 
	{	return m_builder->sort((long) flags);
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
		if ((whichWord < 0) || (whichWord >= Count))
		{
			m_nthWord = gcnew String ("");
			m_nthWordCount = 0;
			m_nthWordDocCount = 0;
			m_nthWordField = nullptr;
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
	
	void WordListBuilder::beforeList() {
		if (m_lastFlags != Flags) {
			m_builder->setFlags((int) Flags);
			m_lastFlags = Flags;
			}
		}
	
}}
