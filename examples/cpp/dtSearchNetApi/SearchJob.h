#pragma once

#include "stdafx.h"
#include "Macros.h"
#include "JobErrorInfo.h"
#include "SearchResults.h"
#include "SearchFilter.h"
#include "Server.h"
#include "Constants.h"
#include "NetJobBase.h"

using namespace System;
using namespace System::Collections;
using namespace System::Collections::Specialized;
using namespace System::Runtime::InteropServices;
using namespace System::Threading;

// ------------------ Unmanaged code -----------------------
#pragma unmanaged

class NetSearchJob;

// ------------------ Managed (.NET) code -----------------------
#pragma managed

namespace dtSearch { namespace Engine {
		/// <summary>
		/// Information about a document retrieved in a search.
		/// </summary>
		/// <remarks>
		///<para>
		/// SearchResultsItem is returned through the OnFound callback
		/// in ISearchSearchStatusHandler, and from SearchResults.
		///</para>
		///<para>
		/// Information returned through OnFound will be incomplete
		/// if the dtsSearchDelayDocInfo flag was used in the search,
		/// because the callback is called without reading document
		/// information records from the index.
		///</para>
		/// </remarks>
	public __gc class SearchResultsItem {
		public:

		/// <summary>
		/// Name of the document as it is stored in the index.
		/// </summary>
		String *Filename;
			//F:Filename

		/// <summary>
		/// If non-blank, a user-friendly name to display for the document (for example, the title of an HTML document).
		/// </summary>
		String *DisplayName;
			//F:DisplayName

		/// <summary>
		/// The first 80 text characters of the document.
		/// </summary>
		String *Title;
			//F:Title

		/// <summary>
		/// The folder where the document is located.
		/// </summary>
		String *Location;
			//F:Location

		/// <summary>
		/// The name of the document, without the path.
		/// </summary>
		String *ShortName;
			//F:ShortName

		/// <summary>
		/// Number of hits found in this document.
		/// </summary>
		int HitCount;
			//F:HitCount

		/// <summary>
		/// Word offsets of hits found in this document.
		/// </summary>
		int Hits __gc[];
			//F:Hits

		/// <summary>
		/// Number of words in this document when it was indexed.
		/// </summary>
		int WordCount;
			//F:WordCount

		/// <summary>
		/// Size of this document when it was indexed.
		/// </summary>
		int Size;
			//F:Size

		/// <summary>
		/// Relevance score for this document.
		/// </summary>
		int Score;
			//F:Score

		/// <summary>
		/// An integer that uniquely identifies each document in an index.
		/// </summary>
		/// <remarks>
		///<para>
		/// Each document in an index is assigned a unique integer identifier called a "DocId" or document id.  The first document added to an index has the DocId 1, and subsequent documents will have sequentially numbered DocIds 2, 3, 4, and so forth.  When a document is reindexed, its DocId is "cancelled" and a new DocId is assigned.  Compressing an index renumbers all of the DocIds, so after an index has been compressed, a document's DocId may change.
		///</para>
		///<para>
		/// DocIds are used in:
		/// <list type="bullet">
		/// <item><description>
		/// SearchFilter objects, which use DocIds to identify the documents selected in the filter.
		/// </description></item>
		/// <item><description>
		/// SearchResults.AddDoc, which lets you add a document to a SearchResults list by providing the index path and the DocId.   You can use this to get the properties of the document from its DocId.
		/// </description></item>
		/// <item><description>
		/// The IndexJob.ToRemoveListName list, which lets you remove documents from an index either by name or by DocId.
		/// </description></item>
		/// </list>
		///</para>
		/// </remarks>
		int DocId;
			//F:DocId

		/// <summary>
		/// Integer identifying the index that the document was retrieved from, in the IndexesToSearch array
		/// </summary>
		int WhichIndex;
			//F:WhichIndex

		/// <summary>
		/// Type id of the document
		/// </summary>
		TypeId TypeId;
			//F:TypeId

		/// <summary>
		/// dtSearch Engine build number used to index this document
		/// </summary>
		int IndexedBy;
			//F:IndexedBy

		/// <summary>
		/// Full path to the index this document was found in.
		/// </summary>
		String *IndexRetrievedFrom;
			//F:IndexRetrievedFrom

		/// <summary>
		/// Hits-in-context string from SearchReportJob.
		/// </summary>
		/// <remarks>
		/// Stored by SearchReportJob if dtsReportStoreInResults flag was used when the SearchReportJob was executed
		/// </remarks>
		String *Synopsis;
			//F:Synopsis

		/// <summary>
		/// Details on words matched in this document.
		/// </summary>
		/// <remarks>
		/// The HitDetails array is generated when the dtsSearchWantHitDetails search flag is used.
		/// </remarks>
		String *HitDetails __gc[];
			//F:HitDetails

		/// <summary>
		/// By-word summary of hits found in this document.
		/// </summary>
		/// <remarks>
		/// The HitsByWord array is generated when the dtsSearchWantHitsByWord search flag is used in a search.
		/// </remarks>
		String *HitsByWord __gc[];
			//F:HitsByWord

		/// <summary>
		/// Field-value pairs found in this document when the document was indexed.
		/// </summary>
		/// <remarks>
		/// When a document is indexed, dtSearch will store any field-value pairs found in the document
		/// when the field name matches one of the <see cref="P:dtSearch.Engine.Indexjob.StoredFields">StoredFields</see>
		/// in the IndexJob.
		/// </remarks>
		System::Collections::Specialized::StringDictionary *UserFields;
			//F:UserFields

		/// <summary>
		/// Relevance score for this document, expressed as a percentage (0-100) of the highest-scoring document in the search.
		/// </summary>
		int ScorePercent;
			//F:ScorePercent

		/// <summary>
		/// Number of hits in this document, counting each phrase as one hit.  This is only available if the dtsSearchWantHitsByWord
		/// flag was set in SearchJob.SearchFlags.
		/// </summary>
		int PhraseCount;
			//F:PhraseCount

		/// <summary>
		/// Modification date of the document (UTC)
		/// </summary>
		DateTime ModifiedDate;
			//F:ModifiedDate

		/// <summary>
		/// Creation date of the document (UTC)
		/// </summary>
		DateTime CreatedDate;
			//F:CreatedDate

		/// <summary>
		/// Use to prevent an item from being added to SearchResults in the OnFound callback.
		/// </summary>
		/// <remarks>
		/// In the OnFound callback in ISearchStatusHandler, set VetoThisItem = true to prevent
		/// the item from being included in search results.
		/// </remarks>
		bool VetoThisItem;
			//F:VetoThisItem
		};

		/// <summary>
		/// Interface for SearchJob.StatusHandler.
		/// </summary>
		/// <remarks>
		/// Use SearchJob.StatusHandler to request a callback each time a document is found,
		/// and to monitor the progress of a search.
		/// </remarks>
	public __gc __interface ISearchStatusHandler
	{
			/// <summary>
			/// Called each time a document is found
			/// </summary>
		void OnFound(SearchResultsItem *item) = 0;
			//M:OnFound(dtSearch.Engine.SearchResultsItem)

			/// <summary>
			/// Called when an indexed is searched
			/// </summary>
		void OnSearchingIndex(String *index) = 0;
			//M:OnSearchingFile(System.String)

			/// <summary>
			/// Called when a file is searched in an unindexed search.
			/// </summary>
		void OnSearchingFile(String *filename) = 0;
			//M:OnSearchingIndex(System.String)

			/// <summary>
			/// Called during a search to check whether the search should be halted
			/// </summary>
			/// <returns>
			/// Return dtsAbort to abort the search, or dtsContinue to let the
			/// search continue
			/// </returns>
		AbortValue CheckForAbort() = 0;
			//M:CheckForAbort()

	};

	/// <summary>
	/// Use to search indexes or to search without an index.
	/// </summary>
	public __gc class SearchJob : public NetJobBase {

	public:

		SearchJob ();
		~SearchJob ();

		/// <summary>
		/// What to search for.
		/// </summary>
		IMPLEMENT_STRING (Request);

		/// <summary>
		/// The full path of each index to search.
		/// </summary>
		IMPLEMENT_STRCOL (IndexesToSearch);

		/// <summary>
		/// Filename filters to select documents to search in an unindexed search.
		/// </summary>
		IMPLEMENT_STRCOL (IncludeFilters);
		/// <summary>
		/// Filename filters to exclude documents from the search in an unindexed search.
		/// </summary>
		IMPLEMENT_STRCOL (ExcludeFilters);

		/// <summary>
		/// Additional boolean conditions that a document must meet to be retrieved in a search.
		/// </summary>
		IMPLEMENT_STRING (BooleanConditions)

		/// <summary>
		/// File conditions that a document must meet to be retrieved in a search.
		/// </summary>
		IMPLEMENT_STRING (FileConditions);

		/// <summary>
		/// Weight to assign to hits in each field when ranking hits
		/// </summary>
		IMPLEMENT_STRING (FieldWeights)


		/// <summary>
		/// In an unindexed search, the directories or files to search.
		/// </summary>
		/// <remarks>
		/// <para>
		/// To specify a directory and its subdirectories,
		/// add &lt;+&gt; (less than, plus, greater than) following the directory name.
		/// </para>
		/// <para>
		/// To specify one or more files, add the filenames to the FoldersToSearch list.
		/// This is more efficient
		/// than specifying the folder name in FoldersToSearch and the filename in IncludeFilters,
		/// because it eliminates the need for a directory scan to locate the file.
		/// </para>
		/// </remarks>
		IMPLEMENT_STRCOL (FoldersToSearch);

		/// <summary>
		/// Set to a non-zero value to make the search automatically stop when this many documents were found.
		/// </summary>
		/// <remarks>
		/// AutoStopLimit causes a search to halt automatically when a specified number of documents have been found. This provides a way to limit the resources consumed by searches that retrieve a very large number of documents. When the number of files retrieved reaches the autoStopLimit value, the search will immediately halt, and the code dtsErSearchLimitReached will be returned through the error handler for the search job. If the AutoStopLimit is 0, the search will not halt.
		/// </remarks>
		IMPLEMENT_INT (AutoStopLimit);

		/// <summary>
		/// If non-zero, the engine will match words that are close to but not identical to a search term.
		/// </summary>
		/// <remarks>
		/// Fuzziness can range from 1 to 10.  The higher the fuzziness value, the more differences
		/// are tolerated when matching words, and the closer to the front of the word these differences
		/// can occur.
		/// </remarks>
		IMPLEMENT_INT (Fuzziness);
		/// <summary>
		/// Limit the maximum size of search results to a specified number of files.
		/// </summary>
		/// <remarks>
		///<para>
		/// When MaxFilesToRetrieve is non-zero, it controls the maximum size of the SearchResults
		/// that can be returned after a search.  The most relevant documents from all matching
		/// documents in the index will be included in SearchResults.
		///</para>
		///<para>
		/// Regardless of the MaxFilesToRetrieve setting, SearchJob.FileCount will return the a count
		/// that includes <i>all</i> matching documents.  MaxFilesToRetrieve limits the number
		/// of items saved in SearchResults, but it does not stop the search before all matching
		/// documents have been identified.
		///</para>
		///<para>
		/// For example, if you set MaxFilesToRetrieve=10 and AutoStopLimit=5000, then SearchResults will
		/// contain the 10 most relevant documents from the first 5000 found.  Documents after the first 5000
		/// found will not be considered, because AutoStopLimit=5000 will force the search to halt
		/// after 5000 matching documents are found.
		///</para>
		///<para>
		/// If the dtsSearchSelectMostRecent flag is set in SearchFlags, the most <i>recent</i> documents
		/// will be returned instead of the most relevant.
		///</para>
		/// </remarks>
		IMPLEMENT_INT (MaxFilesToRetrieve);

		/// <summary>
		/// Flags controlling various search options.
		/// </summary>
		IMPLEMENT_TYPE (dtSearch::Engine::SearchFlags, SearchFlags);

		/// <summary>
		///  Set to a non-zero value to force the search to halt after a specified time.
		/// </summary>
		IMPLEMENT_INT (TimeoutSeconds);

		/// <summary>
		/// After a search, contains the number of hits in all files retrieved.
		/// </summary>
		IMPLEMENT_INT_GET (HitCount);

		/// <summary>
		/// After a search, contains the number of files retrieved (this is the
		/// total number of documents matched, regardless of the MaxFilesToRetrieve setting).
		/// </summary>
		IMPLEMENT_INT_GET (FileCount);

		/// <summary>
		/// Contains any error messages generated during the search.
		/// </summary>
		IMPLEMENT_TYPE_GET (JobErrorInfo*, Errors);

		/// <summary>
		/// Returns the results of the search.
		/// </summary>
		IMPLEMENT_TYPE_GET (SearchResults*, Results);

		/// <summary>
		/// A SearchFilter with all matching documents selected
		/// </summary>
		/// <remarks>
		///<para>
		/// If WantResultsAsFilter was set to true before the search was executed, ResultsAsFilter
		///  will hold a SearchFilter based on the results of the search.  Each document retrieved
		/// in the search will be selected in the filter.
		///</para>
		/// </remarks>
		IMPLEMENT_TYPE_GET (SearchFilter*, ResultsAsFilter);

		/// <summary>
		/// Object to receive callback notifications when documents are found.
		/// </summary>
		IMPLEMENT_TYPE_SET (ISearchStatusHandler*, StatusHandler)

		/// <summary>
		/// Checks whether a SearchJob started using ExecuteInThread is done.
		/// </summary>
		bool IsThreadDone ();
			//M:IsThreadDone

		/// <summary>
		/// Apply a search filter to the search that can be used to exclude or permit specified documents to be returned in search results.
		/// </summary>
		void SetFilter (SearchFilter* newFilter);
			//M:SetFilter(dtSearch.Engine.SearchFilter)

		/// <summary>
		/// Executes the search and returns when the search is complete.
		/// </summary>
		bool Execute ();
			//M:Execute

		/// <summary>
		/// Starts a separate thread to execute the IndexJob and returns immediately.
		/// </summary>
		void ExecuteInThread ();
			//M:ExecuteInThread

		/// <summary>
		/// Returns true immediately if the thread is done; otherwise
		/// waits up to millisecondsToWait for the thread to finish,
		/// then returns false if the thread is not done yet.
		/// </summary>
		/// <param name="millisecondsToWait">Number of milliseconds to wait, if the thread is not done yet.</param>
		bool IsThreadDoneWait (long millisecondsToWait);
			//M:IsThreadDoneWait(System.Int32)

		/// <summary>
		/// Aborts a SearchJob started using ExecuteInThread.
		/// </summary>
		void AbortThread ();
			//M:AbortThread

		/// <summary>
		/// If set to true before a search, after the search ResultsAsFilter will contain
		/// a SearchFilter with all of the documents retrieved in this search.
		/// </summary>
		IMPLEMENT_BOOLEAN(WantResultsAsFilter)
	protected:

		FIELD_STRING (Request);
		FIELD_STRCOL (IndexesToSearch);
		FIELD_STRCOL (IncludeFilters);
		FIELD_STRCOL (ExcludeFilters);
		FIELD_STRING (BooleanConditions)
		FIELD_STRING (FileConditions);
		FIELD_STRING (FieldWeights);
		FIELD_STRCOL (FoldersToSearch);

		FIELD_INT (AutoStopLimit);
		FIELD_INT (Fuzziness);
		FIELD_INT (MaxFilesToRetrieve);
		FIELD_TYPE (dtSearch::Engine::SearchFlags, SearchFlags);
		FIELD_INT (TimeoutSeconds);
		FIELD_INT_GET (HitCount);
		FIELD_INT_GET (FileCount);

		FIELD_TYPE_GET (SearchResults*, Results);
		FIELD_TYPE_GET (SearchFilter*, ResultsAsFilter);

		FIELD_BOOLEAN (DelayDocInfo);
		FIELD_BOOLEAN (ExternalSynonyms);
		FIELD_BOOLEAN (NaturalLanguage);
		FIELD_BOOLEAN (OutputListingHits);
		FIELD_BOOLEAN (PhonicSearching);
		FIELD_BOOLEAN (RegularExpression);
		FIELD_BOOLEAN (Stemming);
		FIELD_BOOLEAN (Synonyms);
		FIELD_BOOLEAN (UserSynonyms);
		FIELD_BOOLEAN (WantResultsAsFilter);
		FIELD_BOOLEAN (WordNetRelated);
		FIELD_BOOLEAN (WordNetSynonyms);
		FIELD_TYPE_SET (ISearchStatusHandler*, StatusHandler)


	protected:

		dtsSearchFilter* m_searchFilter;
		bool m_fNeedResults;
		bool m_resultsReady;

		void makeJob(NetSearchJob& sj);

		void DisposeUnmanagedItems();
		void DisposeManagedItems();

		void freeFilter() {
			if (m_searchFilter) {
				delete m_searchFilter;
				m_searchFilter = 0;
				}
			}
	};
}
}
