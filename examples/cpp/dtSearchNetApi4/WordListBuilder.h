#pragma once

#include "stdafx.h"
#include "Macros.h"
#include "Utils.h"
#include "Constants.h"
#include "SearchFilter.h"

using namespace System;
using namespace System::Collections;
using namespace System::Runtime::InteropServices;
using namespace System::Threading;

#pragma managed

namespace dtSearch { namespace Engine
{

	/// <summary>
	/// WordListBuilder provides a way to list words, field names, or
	/// field values in an index
	/// </summary>
	/// <remarks>
	/// WordListBuilder is intended for quick enumeration of words,
	/// field names, or field values in an index.
	/// 
	/// Two ways of listing words are provided, one for listing the
	/// words before and after a word in an index, and one for
	/// listing words that match a search term in an index.
	/// 
	/// The scrolling list of indexed words that updates as a user
	/// enters a search request in dtSearch Desktop is implemented
	/// using WordListBuilder's ListWords method. The &quot;Browse
	/// Words&quot; dialog box in dtSearch Desktop that lists words
	/// matching an expression is implemented using the
	/// ListMatchingWords method.
	/// 
	/// Listing of field values only lists values of fields that were
	/// designated as EnumerableFields when the documents were
	/// indexed.
	/// 
	/// For speed, WordListBuilder does not actually enumerate the
	/// references for each word and instead relies on counts
	/// incrementally stored in the index. Therefore, the reported
	/// counts may include artifacts of the indexing process such as
	/// reindexed or removed documents, so the counts may be higher
	/// than the actual count of references in the index. Compressing
	/// an index will remove these extra references.
	/// 
	/// To improve performance in cases where the same field values
	/// have to be enumerated repeatedly with different
	/// SearchFilters, you can set the flag
	/// dtsWordListEnableFieldValuesCache in Flags. This will make
	/// ListFieldValues calls from the same WordListBuilder faster at
	/// the cost of substantial memory use, because the field values
	/// and occurrences in the index will be stored in memory until
	/// the WordListBuilder is deleted. The amount of memory required
	/// is proportional to the number of documents in the index times
	/// the number of values in the enumerable field that is cached.
	/// </remarks>                                                   
	public ref class WordListBuilder : public DisposableObject
	{
	public:

		WordListBuilder ();
		~WordListBuilder();

		/// <summary>
		/// Number of words in the list
		/// </summary>
		property int Count;

		/// <summary>
		/// Zero if the last operation succeeded, or an ErrorCodes value if
		///	the last operation failed 
		/// </summary>
		property dtSearch::Engine::ErrorCodes LastError;
		
		/// <summary>
		/// Flags from the WordListBuilderFlags enumeration to control the behavior of the 
		///	WordListBuilder
		/// </summary>
		property dtSearch::Engine::WordListBuilderFlags Flags;


		/// <summary>
		/// Open the index at IndexPath and return true if the index was opened successfully
		/// </summary>
		bool OpenIndex (String^ indexName);

		/// <summary>
		/// Close the index.
		/// </summary>
		void CloseIndex ();

		/// <summary>
		/// List words surrounding cursorWord in the index
		/// </summary>
		/// <param name="cursorWord">The word that will appear in the middle of the list</param>
		/// <param name="range">Number of words to find before and after cursorWord in the index</param>
		/// <returns>
		/// Number of words in the list
		/// </returns>
		long ListWords (String^ cursorWord, long range);


		/// <summary>
		/// Get the text of a word in the word list.
		/// </summary>
		/// <param name="whichWord">Number from 0 to Count-1 indicating which word to get</param>
		/// <returns>
		/// Text of the word
		/// </returns>
		String^ GetNthWord (long whichWord);

		/// <summary>
		/// Get the number of times this word occurs in the index
		/// </summary>
		/// <param name="whichWord">Number from 0 to Count\-1 indicating
		///                         which word to get</param>
		/// <returns>
		/// Number of times this word was found in all documents in this
		/// index.
		/// 
		/// \Note: To make WordListBuilder fast, it does not actually
		/// enumerate all of the references for each word, so in some
		/// cases the count can include artifacts of the indexing process
		/// such as removed or reindexed documents, which can make the
		/// reported count higher than the actual count in the index. Compressing
		/// an index will eliminate these extra references.
		/// </returns>                                                           
		long GetNthWordCount (long whichWord);

		/// <summary>
		/// Get the number of documents in which this word occurs
		/// </summary>
		/// <param name="whichWord">Number from 0 to Count\-1 indicating
		///                         which word to get</param>
		/// <returns>
		/// Number of documents in which this word occurs.
		/// 
		/// \Note: To make WordListBuilder fast, it does not actually
		/// enumerate all of the references for each word, so in some
		/// cases the count can include artifacts of the indexing process
		/// such as removed or reindexed documents, which can make the
		/// reported count higher than the actual count in the index. Compressing
		/// an index will eliminate these extra references.
		/// </returns>                                                           
		long GetNthWordDocCount (long whichWord);

		/// <summary>
		/// Get field associated with a word
		/// </summary>
		String^ GetNthWordField (long whichWord);


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
		long ListMatchingWords (String^ toMatch, long nLimit, SearchFlags searchFlags, long fuzziness);

		/// <summary>
		/// List all field values in the index that match an expression
		/// </summary>
		/// <param name="field">The field to enumerate (the field must
		///                     have been designated as an
		///                     EnumerableField when the index was
		///                     built)</param>
		/// <param name="toMatch">The expression to match</param>
		/// <param name="nLimit">Maximum number of words to find</param>
		/// <returns>
		/// Number of words found.
		/// </returns>
		/// <remarks>
		/// A SearchFilter can be used with ListFieldValues to limit the
		/// results to values in a set of documents. Use SetFilter to
		/// attach a SearchFilter to the WordListBuilder.
		/// 
		/// When a SearchFilter is used, all counts returned by
		/// WordListBuilder are document counts. For example, if a word
		/// \occurs 20 times in 7 documents, the count returned will be 7
		/// for both GetNthWordCount and GetNthWordDocCount. 
		/// </remarks>                                                   
		long ListFieldValues(String^ Field, String^ toMatch, long nLimit);

		/// <summary>
		/// List all fields in the index
		/// </summary>
		long ListFields ();

		/// <summary>
		/// Limit results of ListMatchingWords or ListFieldValues to the
		/// documents in a SearchFilter
		/// </summary>
		/// <remarks>
		/// ListWords is not affected by SetFilter. Only
		/// ListMatchingWords and ListFieldValues limit their results to
		/// the items in the filter.
		/// 
		/// When a SearchFilter is used, all counts returned by
		/// WordListBuilder are document counts. For example, if a word
		/// \occurs 20 times in 7 documents, the count returned will be 7
		/// for both GetNthWordCount and GetNthWordDocCount. 
		/// </remarks>                                                   
		void SetFilter(SearchFilter^ newFilter) {
			m_searchFilter = newFilter;
			applySearchFilter();
			}

		/// <summary>
		/// Sort the word list by document count or term
		/// </summary>			
		/// <remarks>
		///    The following SortFlags are supported:  dtsSortByWord, and dtsSortByDocCount.
		///  Use dtsSortAscending to specify ascending order.  Otherwise the sort will be in
		///  descending order.  Sort() only works with ListFieldValues and ListMatchingWords.
		/// </remarks>
		bool Sort(SortFlags flags);
	protected:

		long m_nthWordNum;
		long m_nthWordDocCount;
		bool m_fOpen;
		dtsWordListBuilder* m_builder;
		String^ m_nthWord;
		String^ m_nthWordField;
		long m_nthWordCount;
		WordListBuilderFlags m_lastFlags;
		SearchFilter^ m_searchFilter;


		void beforeList();
		void afterList();
		void gotoWord (long iWord);
		virtual void DisposeUnmanagedItems() override;
		
		void applySearchFilter();
	};
}
}
