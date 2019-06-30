#pragma once

#include "stdafx.h"
#include "Macros.h"

using namespace System;
using namespace System::Collections;
using namespace System::Runtime::InteropServices;
using namespace System::Threading;

// ------------------ Managed (.NET) code -----------------------
#pragma managed

namespace dtSearch { namespace Engine
{
		/// <summary>
		/// Filters a search to allow only selected documents from an
		/// index to be returned.
		/// </summary>
		/// <remarks>
		/// The SearchFilter object provides a way to designate which
		/// documents can be returned by a search. It is useful in
		/// situations where a text search using must be combined with a
		/// search of a database. The database search is done first, and
		/// then the results of the database search are used to limit the
		/// dtSearch search.
		/// 
		/// <b>Document Ids</b>
		/// 
		/// Search filters do not use names to identify documents because
		/// a filter may specify thousands, or hundreds of thousands, of
		/// documents, and a table of filenames would take too much
		/// memory and would take too long to check. Instead, each
		/// document is identified by (a) the index it belongs to, and
		/// (b) the document's DocId, a unique integer that is assigned
		/// to each document in an index. The docId for a document can be
		/// \obtained by searching for the document by name, and then
		/// examining the document's properties in search results. It can
		/// also be obtained during indexing by using the DataSource
		/// abstract class as the base for your data source
		/// implementation.
		/// 
		/// A docId that is selected may be returned in search results. A
		/// document that is not selected will not be returned in search
		/// results, even if it otherwise satisfies the search request.
		/// 
		/// If the criteria for the SearchFilter can be expressed as one
		/// \or more search requests, you can use SelectItemsBySearch to
		/// select documents in the SearchFilter.
		/// 
		/// <b>Indexes and Index identifiers</b>
		/// 
		/// A search filter can cover any number of indexes. To add an
		/// index to a search filter, call addIndex() with the full path
		/// to the index. The path must be expressed exactly as it will
		/// be expressed in the search job. The AddIndex() method returns
		/// an integer that is used to identify that index when selecting
		/// and de-selecting documents for the filter. (This makes the
		/// selection and de-selection functions, which may be called
		/// thousands of times, more effiicent.)
		/// 
		/// <b>Implementation</b>
		/// 
		/// A search filter is implemented in the dtSearch Engine using a
		/// table of bit vectors, one for each index in the filter. Each
		/// bit vector has one bit for each document in its index. For
		/// \example, a search filter for a single index with 1,000,000
		/// documents would have 1,000,000 bits, or 125 kilobytes of
		/// data.
		/// * IDisposable *
		/// When using the dtSearch Engine API, it is essential to use
		/// the IDisposable pattern with all API objects except
		/// JobErrorInfo. Otherwise, the .NET garbage collector may take
		/// a long time to free memory used by potentially large objects
		/// such as SearchResults, resulting in memory depletion in
		/// \long-running applications. In C# code, use a &quot;using&quot;
		/// clause with dtSearch Engine API objects, and in other code
		/// call Dispose() when you are done with an object.
		/// 
		/// 
		/// * See Also: *
		/// <see cref="Limiting searches with SearchFilters" /> in
		/// dtSearchApiRef.chm. 
		/// </remarks>                                                     
	public ref class SearchFilter : public DisposableObject
	{
	public:

		SearchFilter();

		SearchFilter(dtsSearchFilter* filter, bool bShouldCopy);
		~SearchFilter();

			/// <summary>
			/// Add an index to the filter, returning an integer that can be
			/// used to identify the index in the selection functions.
			/// </summary>
			/// <param name="indexPath">Full path to the index to add to the
			///                         filter.</param>
			/// <returns>
			/// Integer to be used as whichIndex in the Select methods.
			/// 
			/// If the index is already present in the search filter, the
			/// index is not added a second time, and the identifier for the
			/// existing instance of the index is returned. Therefore,
			/// addIndex() can be used to get the identifier for an index
			/// that has previously been added to the SearchFilter.
			/// </returns>                                                  
		long AddIndex (String^ indexPath);

			/// <summary>
			///  Select all of the documents in the index.
			/// </summary>
			/// <param name="whichIndex">Integer identifying the index in this SearchFilter (obtained from AddIndex)</param>
		void SelectAll (long whichIndex);

			/// <summary>
			/// Select no documents in the index.
			/// </summary>
			/// <param name="whichIndex">Integer identifying the index in this SearchFilter (obtained from AddIndex)</param>
		void SelectNone (long whichIndex);

			/// <summary>
			///  Set the selection state of a range of document ids, from firstItem to lastItem, to the selection state indicated by fSelected.
			/// </summary>
			/// <param name="whichIndex">Integer identifying the index in this SearchFilter (obtained from AddIndex)</param>
			/// <param name="firstItem">Document id of the first document in the range to select.</param>
			/// <param name="lastItem">Document id of the last document in the range to select.</param>
			/// <param name="isSelected">Selection state to assign to the documents in the range.</param>
		void SelectItems (long whichIndex, long firstItem, long lastItem, bool isSelected);
			
			/// <summary>
			///  Set the selection state of an array of document ids to the selection state indicated by fSelected.
			/// </summary>
			/// <param name="whichIndex">Integer identifying the index in this SearchFilter (obtained from AddIndex)</param>
			/// <param name="docIds">Array of document ids to select.</param>
			/// <param name="isSelected">Selection state to assign to the documents in the array.</param>
		void SelectItems (long whichIndex, array<int> ^docIds, bool isSelected);

			

			/// <summary>
			/// Set the selection state of all documents in an index that
			/// match search request.
			/// </summary>
			/// <param name="whichIndex">Integer identifying the index in
			///                          this SearchFilter (obtained from
			///                          AddIndex)</param>
			/// <param name="searchRequest">Search request to select
			///                             documents</param>
			/// <param name="isSelected">Selection state to assign to
			///                          documents </param>
			/// 
			/// <remarks>
			/// You can also use SearchJob.WantResultsAsFilter to create a
			/// SearchFilter from the results of a search. Using
			/// SearchJob.WantResultsAsFilter is more flexible than
			/// SelectItemsBySearch because you can use any of the properties
			/// \of the SearchJob (search flags, FileConditions, etc.) rather
			/// than just a simple search request.
			/// 
			/// 
			/// </remarks>                                                   
		long SelectItemsBySearch (long whichIndex, String^ searchRequest, bool isSelected);

			/// <summary>
			/// Combine this filter with another filter in a logical "AND" operation.
			/// </summary>
		void And (SearchFilter^ other);

			/// <summary>
			/// Combine this filter with another filter in a logical "AND NOT" operation.
			/// </summary>
			/// <param name="other">SearchFilter to be combined with this filter (the other SearchFilter is not modified)</param>
		void AndNot (SearchFilter ^other);

			/// <summary>
			/// Combine this filter with another filter in a logical "OR" operation.
			/// </summary>
			/// <param name="other">SearchFilter to be combined with this filter (the other SearchFilter is not modified)</param>
		void Or (SearchFilter^ other);

			/// <summary>
			/// Compare two SearchFilters for equality.
			/// </summary>
			/// <param name="other">SearchFilter to be compared</param>
			///
			/// <returns>
			/// \Returns true if the two SearchFilters contain the same
			/// indexes and the same selections for each index.
			/// </returns>
		bool Equals (SearchFilter^ other);

			/// <summary>
			/// Read the search filter from a disk file.
			/// </summary>
			/// <param name="filename">Full path to the file to read.</param>
		bool Read (String^ filename);

			/// <summary>
			/// Save the search filter to a disk file.
			/// </summary>
			/// <param name="filename">Full path to the file to write.</param>
		bool Write (String^ filename);

			/// <summary>
			/// Read a series of search filters from disk files.
			/// </summary>
			/// <remarks>
			/// List the paths of the search filters to read, separated by
			/// spaces, with the paths in quotation marks, like this:
			/// <code>
			/// &quot;c:\\folder\\a.dat&quot; &quot;c:\\folder\\b.dat&quot;
			/// </code>
			/// The filters will be combined with &quot;OR&quot;, so a
			/// document selected by any of the listed filters will be
			/// selected by the result of the ReadMultiple operation. 
			/// </remarks>                                                 
		long ReadMultiple (String^ filtersToRead);

		/// <summary>
		/// Returns the number of indexes in the SearchFilter.
		/// </summary>
		property int IndexCount {
			int get() { return (m_searchFilter? m_searchFilter->getIndexCount() : 0); }
			}

		/// <summary>
		/// Returns an array of integers containing the document ids of the documents selected in the filter.
		/// </summary>
		array<long>^ GetItems(long whichIndex);

		// For internal use only 
		array<long>^ getIndexSelections (long whichIndex);
		String^ getIndexPath (long whichIndex);
		// For internal use only 
		dtsSearchFilter* getImplementation ();
		void freeFilter() {
			if (m_searchFilter) {
				delete m_searchFilter;
				m_searchFilter = 0;
				}
			}
	protected:
		virtual void DisposeUnmanagedItems() override;

	protected:
		dtsSearchFilter* m_searchFilter;
	};
}
}
