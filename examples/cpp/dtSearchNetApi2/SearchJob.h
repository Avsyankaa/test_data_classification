#pragma once

#include "stdafx.h"
#include "Macros.h"
#include "JobErrorInfo.h"
#include "SearchResults.h"
#include "SearchFilter.h"
#include "IndexCache.h"
#include "Server.h"
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
		/// SearchResultsItem is returned through the OnFound callback in
		/// ISearchSearchStatusHandler, and from SearchResults.
		///
		/// Information returned through OnFound will be incomplete if
		/// the dtsSearchDelayDocInfo flag was used in the search,
		/// because the callback is called without reading document
		/// information records from the index.
		///
		///
		/// </remarks>
	public ref class SearchResultsItem {
		public:

		/// <summary>
		/// Name of the document as it is stored in the index.
		/// </summary>
		String ^Filename;
			
		/// <summary>
		/// If non-blank, a user-friendly name to display for the document (for example, the title of an HTML document).
		/// </summary>
		String ^DisplayName;
			
		/// <summary>
		/// The first 80 text characters of the document.
		/// </summary>
		String ^Title;
			
		/// <summary>
		/// The folder where the document is located.
		/// </summary>
		String ^Location;
			
		/// <summary>
		/// The name of the document, without the path.
		/// </summary>
		String ^ShortName;
			
		/// <summary>
		/// Number of hits found in this document.
		/// </summary>
		int HitCount;
			
		/// <summary>
		/// Word offsets of hits found in this document.
		/// </summary>
		array<int> ^Hits;
			
		/// <summary>
		/// Number of words in this document when it was indexed.
		/// </summary>
		int WordCount;
			
		/// <summary>
		/// Size of this document when it was indexed.
		/// </summary>
		int Size;
			
		/// <summary>
		/// Relevance score for this document.
		/// </summary>
		int Score;
			
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
			
		/// <summary>
		/// Integer identifying the index that the document was retrieved from, in the IndexesToSearch array
		/// </summary>
		int WhichIndex;
			
		/// <summary>
		/// Type id of the document
		/// </summary>
		TypeId TypeId;
			
		/// <summary>
		/// dtSearch Engine build number used to index this document
		/// </summary>
		int IndexedBy;
			

		/// <summary>
		/// Full path to the index this document was found in.
		/// </summary>
		String ^IndexRetrievedFrom;
			

		/// <summary>
		/// Hits-in-context string from SearchReportJob.
		/// </summary>
		/// <remarks>
		/// Stored by SearchReportJob if dtsReportStoreInResults flag was used when the SearchReportJob was executed
		/// </remarks>
		String ^Synopsis;
			

		/// <summary>
		/// Details on words matched in this document.
		/// </summary>
		/// <remarks>
		/// The HitDetails array is generated when the dtsSearchWantHitDetails search flag is used.
		/// </remarks>
		array<String ^> ^HitDetails;
			

		/// <summary>
		/// By-word summary of hits found in this document.
		/// </summary>
		/// <remarks>
		/// The HitsByWord array is generated when the dtsSearchWantHitsByWord search flag is used in a search.
		/// </remarks>
		array<String ^> ^HitsByWord;
			

		/// <summary>
		/// Field-value pairs found in this document when the document
		/// was indexed.
		/// </summary>
		/// <remarks>
		/// When a document is indexed, dtSearch will store any
		/// field-value pairs found in the document when the field name
		/// matches one of the StoredFields in the IndexJob used to index
		/// this document.
		/// 
		/// \Example:
		/// <code>
		///     SearchResultsItem item = m_results.CurrentItem;
		///     string subject = item.UserFields[&quot;Subject&quot;];
		/// </code>
		/// \See Also:
		/// 
		/// <see cref="Retrieving Fields in Search Results" /> in the
		/// dtSearch Engine help file, dtSearchApiRef.chm 
		/// </remarks>                                                   
		System::Collections::Specialized::StringDictionary ^UserFields;
			

		/// <summary>
		/// Relevance score for this document, expressed as a percentage (0-100) of the highest-scoring document in the search.
		/// </summary>
		int ScorePercent;
			

		/// <summary>
		/// Number of hits in this document, counting each phrase as one hit.  This is only available if the dtsSearchWantHitsByWord
		/// flag was set in SearchJob.SearchFlags.
		/// </summary>
		int PhraseCount;
			

		/// <summary>
		/// Modification date of the document (UTC)
		/// </summary>
		DateTime ModifiedDate;
			

		/// <summary>
		/// Creation date of the document (UTC)
		/// </summary>
		DateTime CreatedDate;
			

		/// <summary>
		/// Use to prevent an item from being added to SearchResults in the OnFound callback.
		/// </summary>
		/// <remarks>
		/// In the OnFound callback in ISearchStatusHandler, set VetoThisItem = true to prevent
		/// the item from being included in search results.
		/// </remarks>
		bool VetoThisItem;
			

		};


	/// <summary>
	/// Information about a document being searched in an unindexed search (part of SearchProgressInfo)
	/// </summary>
	public ref class SearchFileInfo {
		public:
		/// <summary>
		/// Name of the file
		/// </summary>
		String ^Name;
		/// <summary>
		/// File format expressed as a string
		/// </summary>
		String ^Type;
		/// <summary>
		/// If the file could not be opened, a message indicating the reason
		/// </summary>
		String ^OpenFailMessage;
		/// <summary>
		/// Size of the file in bytes
		/// </summary>
		Int64 Size;
		/// <summary>
		/// Number of words parsed from the file
		/// </summary>
		Int32 WordCount;
		/// <summary>
		/// File format expressed as a TypeId
		/// </summary>
		TypeId TypeId;
		/// <summary>
		/// Information about how this document was indexed.
		/// </summary>
		FileInfoFlags Flags;
		};

/// <summary>
	/// State of an index update
	/// </summary>
	public ref class SearchProgressInfo {
		public:
		SearchProgressInfo() {
			File = gcnew SearchFileInfo;
			}
		/// <summary>
		/// A MessageCode indicating the reason for the update message
		/// </summary>
		MessageCode UpdateType;
		/// <summary>
		/// Location of the index being searched
		/// </summary>
		String ^IndexPath;
		/// <summary>
		/// Information about the file currently being searched (unindexed search only)
		/// </summary>
		SearchFileInfo ^File;
		/// <summary>
		/// Number of files to search (unindexed search only)
		/// </summary>
		UInt32 FilesToSearch;
		/// <summary>
		/// Number of files searched so far(unindexed search only)
		/// </summary>
		UInt32 FilesSearched;

		/// <summary>
		/// Total hit count for all documents retrieved
		/// </summary>
		UInt32 TotalHitCount;

		/// <summary>
		/// Total number of documents retrieved
		/// </summary>
		UInt32 TotalFileCount;
		};

		/// <summary>
		/// Interface for SearchJob.StatusHandler.
		/// </summary>
		/// <remarks>
		/// Use SearchJob.StatusHandler to request a callback each time a document is found,
		/// and to monitor the progress of a search.  Use ISearchStatusHandler2 to obtain
		/// the OnProgressUpdate callback in addition to the other callbacks that
		/// ISearchStatusHandler supports.
		/// </remarks>
	public interface class ISearchStatusHandler
	{
			/// <summary>
			/// Called each time a document is found
			/// </summary>
		void OnFound(SearchResultsItem ^item) = 0;
			//M:OnFound(dtSearch.Engine.SearchResultsItem)

			/// <summary>
			/// Called when an indexed is searched
			/// </summary>
		void OnSearchingIndex(String ^index) = 0;
			//M:OnSearchingFile(System.String)

			/// <summary>
			/// Called when a file is searched in an unindexed search.
			/// </summary>
		void OnSearchingFile(String ^filename) = 0;
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
		/// Interface for SearchJob.StatusHandler, with additional OnProgressUpdate callback.
		/// </summary>
		/// <remarks>
		/// Use SearchJob.StatusHandler to request a callback each time a document is found,
		/// and to monitor the progress of a search.  Use ISearchStatusHandler2 to obtain
		/// the OnProgressUpdate callback in addition to the other callbacks that
		/// ISearchStatusHandler supports.
		/// </remarks>
	public interface class ISearchStatusHandler2 : public ISearchStatusHandler
	{
			/// <summary>
			/// Called during a search to provide status updates
			/// </summary>
			/// <returns>
			/// The SearchProgressInfo provides information about the current status of the search.
			/// </returns>
		void OnProgressUpdate(SearchProgressInfo^ info) = 0;
	};

	/// <summary>
	/// Use to search indexes or to search without an index.
	/// </summary>
	/// <remarks>
	/// * Search criteria *
	/// There are three components to the search request that can be
	/// included in a SearchJob:
	/// 
	/// (1) Request: The search request, which can be in the
	/// &quot;boolean&quot;, &quot;all words&quot;, or &quot;anywords&quot;
	/// syntax, depending on the value of SearchFlags.
	/// 
	/// (2) BooleanConditions: A string in the boolean syntax
	/// expressing any conditions that must be satisfied in addition
	/// to whatever is in Request. This provides a way to add boolean
	/// \conditions (such as field criteria) to a user-supplied
	/// query, which may be in the all words or any words syntax.
	/// 
	/// (3) FileConditions: A string expressing conditions such as
	/// filename matches, file size matches, etc., that documents
	/// must satisfy.
	/// 
	/// For information on the syntax for these values, see:
	/// 
	/// <see cref="Search Requests" text="Search Requests Overview" />
	/// * Search results *
	/// After a search, to obtain search results, use
	/// SearchJob.Results to create a SearchResults object with the
	/// documents that were found in the search.
	/// * Monitoring and cancelling searches *
	/// To receive callbacks during a search with the name of each
	/// document as it is found, create an object that implements the
	/// ISearchStatusHandler interface and attach it to the
	/// SearchJob's StatusHandler.
	/// 
	/// To implement a &quot;Cancel&quot; button, start the search
	/// using ExecuteInThread and then use AbortThread to cancel the
	/// search.
	/// 
	/// In server applications, to limit the amount of time and
	/// resources a search can consume, use TimeoutSeconds,
	/// MaxFilesToRetrieve, and AutoStopLimit. For more information,
	/// see:
	/// 
	/// <see cref="Limiting Searches" />
	/// * Search filters *
	/// A search filter represents a set of documents that can be
	/// returned in a search. To limit a search to the documents in a
	/// search filter, set up a SearchFilter object with the filter
	/// to use, and call SearchJob.SetFilter to attach the filter to
	/// the SearchJob. Example:
	/// <code>
	/// using (dtSearch.Engine.SearchFilter filter = new SearchFilter())
	/// {
	///     int iIndex = filter.AddIndex(m_indexPath);
	///     // Select the items to include in the filter.
	///     int ct = filter.SelectItemsBySearch(iIndex, sampleRequest, true);
	///     searchJob.SetFilter(filter);
	///     // Now when searchJob is executed, only documents identified by searchFilter can be
	///     // retrieved.
	///     ...
	/// </code>
	/// To generate a search filter based on the results of a search,
	/// set WantResultsAsFilter to true. After the search, every
	/// document retrieved in the search will be selected in the
	/// \output search filter, ResultsAsFilter. Note:
	/// MaxFilesToRetrieve does not affect outputSearchFilter, which
	/// will include <i>every</i>item retrieved in the search.
	/// However, if AutoStopLimit causes the search to terminate
	/// before it is complete, then only items found before the
	/// search stopped will be selected in the SearchFilter.
	/// 
	/// For more information, see: <see cref="Limiting searches with SearchFilters" />
	/// * IDisposable *
	/// When using the dtSearch Engine API, it is essential to use
	/// the IDisposable pattern with all API objects except
	/// JobErrorInfo. Otherwise, the .NET garbage collector may take
	/// a long time to free memory used by potentially large objects
	/// such as SearchResults, resulting in memory depletion in
	/// \long-running applications. In C# code, use a &quot;using&quot;
	/// clause with dtSearch Engine API objects, and in other code
	/// call Dispose() when you are done with an object.
	/// </remarks>
	/// <example>
	/// <code>
	/// dtSearch.Engine.SearchJob searchJob = new dtSearch.Engine.SearchJob();
	/// searchJob.Request = searchRequest.Text;
	/// searchJob.IndexesToSearch.Add(indexPath.Text);
	/// searchJob.MaxFilesToRetrieve = 100;
	/// searchJob.SearchFlags = SearchFlags.dtsSearchDelayDocInfo;
	/// searchJob.Execute();
	/// if (searchJob.Errors.Count &gt; 0)
	///     // ... display error messages ...
	/// else {
	///     dtSearch.Engine.SearchResults results;
	///     results = searchJob.Results;
	///     // ... display search results ...
	///     }
	/// </code>
	/// 
	/// </example>                                                                             
	public ref class SearchJob : public NetJobBase {

	public:

		SearchJob ();
		~SearchJob ();

		/// <summary>
		/// What to search for.
		/// </summary>
		property String^ Request;

		/// <summary>
		/// The full path of each index to search.
		/// </summary>
		System::Collections::Specialized::StringCollection ^IndexesToSearch;

		/// <summary>
		/// Filename filters to select documents to search in an unindexed search.
		/// </summary>
		System::Collections::Specialized::StringCollection ^IncludeFilters;
		/// <summary>
		/// Filename filters to exclude documents from the search in an unindexed search.
		/// </summary>
		System::Collections::Specialized::StringCollection ^ExcludeFilters;

		/// <summary>
		/// Additional boolean conditions that a document must meet to be retrieved in a search.
		/// </summary>
		property String^ BooleanConditions;

		/// <summary>
		/// File conditions that a document must meet to be retrieved in a search.
		/// </summary>
		property String^ FileConditions;

		/// <summary>
		/// Specify a relevance weight for hits that occur in listed
		/// fields (example: Subject:10,HtmlTitle:20)
		/// </summary>
		/// <remarks>
		/// FieldWeights is a list of field names, with a weight
		/// indicated after each field name, like this:
		/// <code>
		///     Subject:10,HtmlTitle:20,Description:15
		/// </code>
		/// Hits that occur in one of these fields will count more
		/// heavily than other hits, according to the weight you specify.
		/// For more information, see &quot;<see cref="Relevance" />&quot;
		/// in the &quot;Overviews&quot; section of the API
		/// documentation.
		///
		///
		/// </remarks>
		property String^ FieldWeights;


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
		System::Collections::Specialized::StringCollection ^FoldersToSearch;

		/// <summary>
		/// Set to a non-zero value to make the search automatically stop when this many documents were found.
		/// </summary>
		/// <remarks>
		/// AutoStopLimit causes a search to halt automatically when a specified number of documents have been found. This provides a way to limit the resources consumed by searches that retrieve a very large number of documents. When the number of files retrieved reaches the autoStopLimit value, the search will immediately halt, and the code dtsErSearchLimitReached will be returned through the error handler for the search job. If the AutoStopLimit is 0, the search will not halt.
		/// </remarks>
		property int AutoStopLimit;

		/// <summary>
		/// If non-zero, the engine will match words that are close to but not identical to a search term.
		/// </summary>
		/// <remarks>
		/// Fuzziness can range from 1 to 10.  The higher the fuzziness value, the more differences
		/// are tolerated when matching words, and the closer to the front of the word these differences
		/// can occur.
		/// </remarks>
		property int Fuzziness;
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
		property int MaxFilesToRetrieve;

		/// <summary>
		/// Flags controlling various search options.
		/// </summary>
		property dtSearch::Engine::SearchFlags SearchFlags;

		/// <summary>
		/// Flags controlling various search options.
		/// </summary>
		property dtSearch::Engine::UnindexedSearchFlags UnindexedSearchFlags;

		/// <summary>
		///  Set to a non-zero value to force the search to halt after a specified time.
		/// </summary>
		property int TimeoutSeconds;

		/// <summary>
		/// After a search, contains the number of hits in all files retrieved.
		/// </summary>
		property int HitCount;

		/// <summary>
		/// After a search, contains the number of files retrieved (this is the
		/// total number of documents matched, regardless of the MaxFilesToRetrieve setting).
		/// </summary>
		property int FileCount;

		/// <summary>
		/// Returns the results of the search.
		/// </summary>
		property SearchResults ^Results;

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
		property SearchFilter^ ResultsAsFilter;

		/// <summary>
		/// Object to receive callback notifications when documents are found.
		/// </summary>
		property ISearchStatusHandler^ StatusHandler;

		/// <summary>
		/// Checks whether a SearchJob started using ExecuteInThread is done.
		/// </summary>
		bool IsThreadDone ();
			//M:IsThreadDone

		/// <summary>
		/// Apply a search filter to the search that can be used to exclude or permit specified documents to be returned in search results.
		/// </summary>
		void SetFilter (SearchFilter^ newFilter);
			//M:SetFilter(dtSearch.Engine.SearchFilter)

		/// <summary>
		/// Use an IndexCache for this SearchJob
		/// </summary>
		void SetIndexCache(IndexCache^ aCache);

		/// <summary>
		/// Executes the search and returns when the search is complete.
		/// </summary>
		virtual bool Execute () override;
			//M:Execute

		/// <summary>
		/// Starts a separate thread to execute the SearchJob and returns immediately.
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
		property bool WantResultsAsFilter;
	protected:


	protected:
		IndexCache ^m_pObjIndexCache;
		dtsSearchFilter* m_searchFilter;
		dtsIndexCache *m_pIndexCache;
		bool m_fNeedResults;
		bool m_resultsReady;

		void makeJob(NetSearchJob& sj);

		virtual void DisposeUnmanagedItems() override;
		virtual void DisposeManagedItems() override;

		void freeFilter() {
			if (m_searchFilter) {
				delete m_searchFilter;
				m_searchFilter = 0;
				}
			}
	};
}
}
