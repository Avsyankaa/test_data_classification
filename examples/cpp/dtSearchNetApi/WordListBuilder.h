#pragma once

#include "stdafx.h"
#include "Macros.h"
#include "Utils.h"
#include "Constants.h"

using namespace System;
using namespace System::Collections;
using namespace System::Runtime::InteropServices;
using namespace System::Threading;

#pragma managed

namespace dtSearch { namespace Engine
{

	/// <summary>
	/// Builds a scrolling list of indexed words a program can update as a user types in a search request.
	/// </summary>
	public __gc class WordListBuilder : public DisposableObject
	{
	public:

		WordListBuilder ();
		~WordListBuilder();

		/// <summary>
		/// Number of words in the list
		/// </summary>
		IMPLEMENT_INT_GET (Count);

		/// <summary>
		/// Open the index at IndexPath and return true if the index was opened successfully
		/// </summary>
		bool OpenIndex (String* indexName);
			//M:OpenIndex(System.String)

		/// <summary>
		/// Close the index.
		/// </summary>
		void CloseIndex ();
			//M:CloseIndex

		/// <summary>
		/// List words surrounding cursorWord in the index
		/// </summary>
		/// <param name="cursorWord">The word that will appear in the middle of the list</param>
		/// <param name="range">Number of words to find before and after cursorWord in the index</param>
		/// <returns>
		/// Number of words in the list
		/// </returns>
		long ListWords (String* cursorWord, long range);
			//M:ListWords(System.String,System.Int32)


		/// <summary>
		/// Get the text of a word in the word list.
		/// </summary>
		/// <param name="whichWord">Number from 0 to Count-1 indicating which word to get</param>
		/// <returns>
		/// Text of the word
		/// </returns>
		String* GetNthWord (long whichWord);
			//M:GetNthWord(System.Int32)

		/// <summary>
		/// Get the number of times this word occurs in the index
		/// </summary>
		/// <param name="whichWord">Number from 0 to Count-1 indicating which word to get</param>
		/// <returns>
		/// Number of times this word was found in all documents in this index
		/// </returns>
		long GetNthWordCount (long whichWord);
			//M:GetNthWordCount(System.Int32)

		/// <summary>
		/// Get the number of documents in which this word occurs
		/// </summary>
		/// <param name="whichWord">Number from 0 to Count-1 indicating which word to get</param>
		/// <returns>
		/// Number of documents in which this word occurs.
		/// </returns>
		long GetNthWordDocCount (long whichWord);
			//M:GetNthWordDocCount(System.Int32)


		/// <summary>
		/// Get field associated with a word
		/// </summary>
		String* GetNthWordField (long whichWord);
			//M:GetNthWordField(System.Int32)


		/// <summary>
		/// List all words in the index that match an expression
		/// </summary>
		/// <param name="toMatch">The expression to match</param>
		/// <param name="nLimit">Maximum number of words to find</param>
		/// <param name="searchFlags">Flags indicating how to match the words</param>
		/// <param name="fuzziness">If searchFlags includes dtsSearchFuzzy, use fuzziness to indicate the
		///         degree of fuzziness</param>
		/// <returns>
		/// Number of words found.
		/// </returns>
		long ListMatchingWords (String* toMatch, long nLimit, SearchFlags searchFlags, long fuzziness);
			//M:ListMatchingWords(System.String,System.Int32,dtSearch.Engine.SearchFlags,System.Int32)

		/// <summary>
		/// List all fields in the index
		/// </summary>
		long ListFields ();
			//M:ListFields

	protected:
		FIELD_INT (Count);

	protected:

		long m_nthWordNum;
		long m_nthWordDocCount;
		bool m_fOpen;
		dtsWordListBuilder* m_builder;
		String* m_nthWord;
		String* m_nthWordField;
		long m_nthWordCount;


		void gotoWord (long iWord);
		virtual void DisposeUnmanagedItems();
	};
}
}
