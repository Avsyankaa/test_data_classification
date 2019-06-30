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
		/// Filters a search to allow only selected documents from an index to be returned.
		/// </summary>
		/// <remarks>
		///<para>
		/// The SearchFilter object provides a way to designate which documents can be returned by a search. It is useful in situations where a text search using must be combined with a search of a database. The database search is done first, and then the results of the database search are used to limit the dtSearch search.
		///</para>
		///<para>
		/// <b>Document Ids</b>
		///</para>
		///<para>
		/// Search filters do not use names to identify documents because a filter may specify thousands,
		/// or hundreds of thousands, of documents, and a table of filenames would take too much memory
		/// and would take too long to check. Instead, each document is identified by (a) the index it
		/// belongs to, and (b) the document's DocId, a unique integer that is assigned to each document
		/// in an index. The docId for a document can be obtained by searching for the document by name,
		/// and then examining the document's properties in search results. It can also be obtained during
		/// indexing by using the <see cref="T:dtSearch.Engine.DataSource">DataSource</see>
		/// abstract class as the base for your data source implementation.
		///</para>
		///<para>
		/// A docId that is selected may be returned in search results. A document that is not selected will not be returned in search results, even if it otherwise satisfies the search request.
		///</para>
		///<para>
		/// If the criteria for the SearchFilter can be expressed as one or more search requests, you can use SelectItemsBySearch to select documents in the SearchFilter.
		///</para>
		///<para>
		/// <b>Indexes and Index identifiers</b>
		///</para>
		///<para>
		/// A search filter can cover any number of indexes. To add an index to a search filter, call addIndex() with the full path to the index. The path must be expressed exactly as it will be expressed in the search job. The AddIndex() method returns an integer that is used to identify that index when selecting and de-selecting documents for the filter. (This makes the selection and de-selection functions, which may be called thousands of times, more effiicent.)
		///</para>
		///<para>
		///<b>Implementation</b>
		///</para>
		///<para>
		/// A search filter is implemented in the dtSearch Engine using a table of bit vectors, one for each index in the filter. Each bit vector has one bit for each document in its index. For example, a search filter for a single index with 1,000,000 documents would have 1,000,000 bits, or 125 kilobytes of data.
		///</para>
		/// </remarks>
	public __gc class SearchFilter : public DisposableObject
	{
	public:

		SearchFilter();
		SearchFilter(dtsSearchFilter* filter);
		~SearchFilter();

			/// <summary>
			/// Add an index to the filter, returning an integer that can be used to identify the index in the selection functions.
			/// </summary>
			/// <param name="indexPath">Full path to the index to add to the filter.</param>
			/// <returns>
			/// Integer to be used as whichIndex in the Select methods.
			/// </returns>
		long AddIndex (String* indexPath);
			//M:AddIndex(System.String)

			/// <summary>
			///  Select all of the documents in the index.
			/// </summary>
			/// <param name="whichIndex">Integer identifying the index in this SearchFilter (obtained from AddIndex)</param>
		void SelectAll (long whichIndex);
			//M:SelectAll(System.Int32)

			/// <summary>
			/// Select no documents in the index.
			/// </summary>
			/// <param name="whichIndex">Integer identifying the index in this SearchFilter (obtained from AddIndex)</param>
		void SelectNone (long whichIndex);
			//M:SelectNone(System.Int32)

			/// <summary>
			///  Set the selection state of a range of document ids, from firstItem to lastItem, to the selection state indicated by fSelected.
			/// </summary>
			/// <param name="whichIndex">Integer identifying the index in this SearchFilter (obtained from AddIndex)</param>
			/// <param name="firstItem">Document id of the first document in the range to select.</param>
			/// <param name="lastItem">Document id of the last document in the range to select.</param>
			/// <param name="isSelected">Selection state to assign to the documents in the range.</param>
		void SelectItems (long whichIndex, long firstItem, long lastItem, bool isSelected);
			//M:SelectItems(System.Int32,System.Int32,System.Int32,System.Boolean)

			/// <summary>
			/// Set the selection state of all documents in an index that match search request.
			/// </summary>
			/// <param name="whichIndex">Integer identifying the index in this SearchFilter (obtained from AddIndex)</param>
			/// <param name="searchRequest">Search request to select documents</param>
			/// <param name="isSelected">Selection state to assign to documents   </param>
			/// <note>You can also use SearchJob.WantResultsAsFilter to create a SearchFilter from the results of a search.  Using SearchJob.WantResultsAsFilter is more flexible than SelectItemsBySearch because you can use any of the properties of the SearchJob (search flags, FileConditions, etc.) rather than just a simple search request.</note>
		long SelectItemsBySearch (long whichIndex, String* searchRequest, bool isSelected);
			//M:SelectItemsBySearch(System.Int32,System.String,System.Boolean)

			/// <summary>
			/// Combine this filter with another filter in a logical "AND" operation.
			/// </summary>
		void And (SearchFilter* other);
			//M:And(dtSearch.Engine.SearchFilter)

			/// <summary>
			/// Combine this filter with another filter in a logical "AND NOT" operation.
			/// </summary>
			/// <param name="other">SearchFilter to be combined with this filter (the other SearchFilter is not modified)</param>
		void AndNot (SearchFilter *other);
			//M:AndNot(dtSearch.Engine.SearchFilter)

			/// <summary>
			/// Combine this filter with another filter in a logical "OR" operation.
			/// </summary>
			/// <param name="other">SearchFilter to be combined with this filter (the other SearchFilter is not modified)</param>
		void Or (SearchFilter* other);
			//M:Or(dtSearch.Engine.SearchFilter)

			/// <summary>
			/// Compare two SearchFilters for equality.
			/// </summary>
			/// <param name="other">SearchFilter to be compared</param>
		bool Equals (SearchFilter* other);
			//M:Equals(dtSearch.Engine.SearchFilter)

			/// <summary>
			/// Read the search filter from a disk file.
			/// </summary>
			/// <param name="filename">Full path to the file to read.</param>
		bool Read (String* filename);
			//M:Read(System.String)

			/// <summary>
			/// Save the search filter to a disk file.
			/// </summary>
			/// <param name="filename">Full path to the file to write.</param>
		bool Write (String* filename);
			//M:Write(System.String)

			/// <summary>
			/// Read a series of search filters from disk files.
			/// </summary>
		long ReadMultiple (String* filtersToRead);
			//M:ReadMultiple(System.String)

		__property int get_IndexCount()
	    {
			if (m_searchFilter)
				return m_searchFilter->getIndexCount();
			else
				return 0;
		}
		long getIndexSelections (long whichIndex) __gc [];
		String* getIndexPath (long whichIndex);
		dtsSearchFilter* getImplementation ();
		void freeFilter() {
			if (m_searchFilter) {
				delete m_searchFilter;
				m_searchFilter = 0;
				}
			}
	protected:
		FIELD_INT_GET (IndexCount);
		void DisposeUnmanagedItems();

	protected:
		dtsSearchFilter* m_searchFilter;
	};
}
}
