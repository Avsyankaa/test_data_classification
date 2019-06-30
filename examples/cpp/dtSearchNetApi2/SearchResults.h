#pragma once

#include "stdafx.h"
#include "Macros.h"
#include "JobErrorInfo.h"


using namespace System;
using namespace System::Collections;
using namespace System::Runtime::InteropServices;
using namespace System::Threading;

// ------------------ Unmanaged code -----------------------
#pragma unmanaged

// ------------------ Managed (.NET) code -----------------------
#pragma managed

namespace dtSearch { namespace Engine
{
	ref class SearchReportJob;
	ref class SearchResultsItem;

/// <summary>
/// List of documents retrieved in a search.
/// </summary>
/// <remarks>
/// After a search, SearchJob.Results will contain a
/// SearchResults listing the documents found in the search.
/// 
/// To access information about each document, call GetNthDoc()
/// to select the document in the list, then use CurrentItem to
/// access its properties.
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
/// </remarks>                                                     
	public ref class SearchResults : public DisposableObject
	{
	public:

		SearchResults();
		SearchResults (DSearchResults* results, bool bAttach);
		~SearchResults();


	/// <summary>
	/// Add a document to SearchResults.
	/// </summary>
	/// <param name="indexPath">Path of the index that contains this document.</param>
	/// <param name="docId">Document id of the document in the index.</param>
	/// <param name="hits">Word offset of the hits in this document.</param>
	/// <remarks>
	///
	/// </remarks>
		void AddDoc (String^ indexPath, long docId, array<int>^ hits);
			//M:AddDoc(System.String,System.Int32,System.Int32[])


	/// <summary>
	/// Add documents to SearchResults.
	/// </summary>
	/// <param name="indexPath">Path of the index that contains the
	///                         documents.</param>
	/// <param name="docIds">Document ids of the documents to
	///                      add.</param>
	/// <remarks>
	/// When adding more than one document to SearchResults, AddDocs
	/// is faster than AddDoc because the index is only opened once.
	/// 
	/// SearchResults.AddDocs can be used to convert a SearchFilter
	/// to a SearchResults object containing the same documents. First
	/// call SearchFilter.GetItems() to get an array the of document
	/// ids in the SearchFilter, and then call
	/// SearchResults.AddDocs() to add the document ids to a
	/// SearchResults object. Example:
	/// <code>
	/// private SearchResults ConvertFilterToResults(SearchFilter filter, int iIndex)
	///         {
	///             int[] docIds = filter.GetItems(iIndex);
	///             SearchResults results = new SearchResults();
	///             results.AddDocs(filter.getIndexPath(iIndex), docIds);
	///             return results;
	///         }
	/// </code>
	/// 
	/// </remarks>                                                                   
		void AddDocs (String^ indexPath, array<long>^ docIds);


	/// <summary>
	///	Set the sort key for the currently-selected document.
	/// </summary>
	/// <remarks>
	/// SetSortKey provides a way to assign a string to each item in search results
	/// to use as a sort key.   After each item has been assigned a sort key,
	/// call Sort(SortFlags.dtsSortBySortKey) to sort SearchResults by the assigned
	/// keys.
	/// </remarks>
		void SetSortKey (String^ newKey);
			//M:SetSortKey(System.String)



	/// <summary>
	///	Fills a SearchResults object with data from an XML stream generated by SerializeAsXml
	/// </summary>
		void SerializeFromXml (String^ val);
			//M:SerializeFromXml(System.String)

	/// <summary>
	/// Generates a stream of XML containing data in the search results list.
	/// </summary>
		String^ SerializeAsXml ();
			//M:SerializeAsXml


	/// <summary>
	/// UrlDecodeItem converts a string created by UrlEncodeItem back into a search results item.
	/// </summary>
	/// <remarks>
	/// UrlEncodeItem can be used to create HREFs to use in HTML search results.  When the user
	/// clicks the link, the URL-encoded item will be sent as part of the Query String.
	/// A server-based application that receives the URL-encoded item can then use UrlDecodeItem
	/// to convert the data in the HREF back into a SearchResults object.  This eliminates the need
	/// to keep the SearchResults object in memory between requests, making stateless operation
	/// possible on web servers.
	/// </remarks>
		void UrlDecodeItem (String^ val);
			//M:UrlDecodeItem(System.String)


	/// <summary>
	/// Creates a url-encoded string describing the currently-selected item in search results.
	/// </summary>
	/// <remarks>
	/// UrlEncodeItem can be used to create HREFs to use in HTML search results.  When the user
	/// clicks the link, the URL-encoded item will be sent as part of the Query String.
	/// A server-based application that receives the URL-encoded item can then use UrlDecodeItem
	/// to convert the data in the HREF back into a SearchResults object.  This eliminates the need
	/// to keep the SearchResults object in memory between requests, making stateless operation
	/// possible on web servers.
	/// </remarks>
		String^ UrlEncodeItem ();
			//M:UrlEncodeItem


	/// <summary>
	/// Generates xml data used to highlight hits in Adobe Reader through a web browser interface.
	/// </summary>
	/// <remarks>
	/// See <i>Indexing and Searching Adobe Acrobat Files</i> in the dtSearch Engine help file for more information.
	/// </remarks>
		String^ MakePdfWebHighlightFile ();
			//M:MakePdfWebHighlightFile


	/// <summary>
	/// Deprecated -- use MakePdfWebHighlightFile
	/// </summary>
		void MakePdfHighlightFile ();
			//M:MakePdfHighlightFile


	/// <summary>
	/// Creates a SearchReportJob that can be used to generate segments of context around each hit in one or
	/// more documents.
	/// </summary>
		SearchReportJob^ NewSearchReportJob ();
			//M:NewSearchReportJob


		/// <summary>
		/// Sorts the search results according to the value in flags, with an optional user-defined field name.
		/// </summary>
		void Sort (SortFlags flags, String^ fieldName);
			//M:Sort(dtSearch.Engine.SortFlags,System.String)

		/// <summary>
		/// Select a document in search results.
		/// </summary>
		/// <param name="whichDoc">Integer between zero and Count-1 specifying the document to select.</param>
		/// <remarks>
		/// After selecting a document, use SearchResults.CurrentItem to access the properties of the document.
		/// </remarks>
		bool GetNthDoc (long iDoc);
			//M:GetNthDoc(System.Int32)

		/// <summary>
		/// Access a named property of the selected SearchResults item.
		/// </summary>
		/// <remarks>
		/// <para>Use GetDocDetails to access all of the properties.</para>
		/// <para>The GetDocDetails and GetDocDetailItem methods are provided for backward compatibility
		/// with applications created using the old COM interface, and are deprecated.</para>
		/// <para>CurrentItem provides an easier and more efficient way to access document properties.</para>
		/// </remarks>
		property String^ DocDetailItem[String^ ] {
			//M:DocDetailItem(System.String)
			String^ get(String^ name) {
				DStringProperty toGet;
				toGet.set(name);
				DString result;
				m_results->getDocInfoItem(toGet, result);

				return result.allocNetString();
			}
		}

		/// <summary>
		/// Access all named properties of the selected SearchResults item.
		/// </summary>
		/// <remarks>
		/// <para>Use GetDocDetails to access all of the properties.</para>
		/// <para>The GetDocDetails and GetDocDetailItem methods are provided for backward compatibility
		/// with applications created using the old COM interface, and are deprecated.</para>
		/// <para>CurrentItem provides an easier and more efficient way to access document properties.</para>
		/// </remarks>
		property String^ DocDetails {
			//P:DocDetails
			String^ get() {
				return m_results->getDocInfoString().allocNetString();
			}
		}

		/// <summary>
		/// Calculate byte offset of each hit (for plain text files only)
		/// </summary>
		/// <remarks>
		/// The easiest way to highlight hits in files retrieved with the
		/// dtSearch Engine is to use a FileConverter to convert the
		/// retrieved file to an HTML document with the hits highlighted.
		/// For plain text files, the dtSearch Engine provides an
		/// alternative method. Instead of highlighting the hits
		/// directly, the Engine can provide a map of the hits in each
		/// \file. The map contains, for each hit, the byte offset in the
		/// \file, the word offset, the page number, and the paragraph
		/// number.
		/// 
		/// The map is expressed as a string consisting of a series of
		/// entries delimited with the &quot;;&quot; character. The
		/// entries are formatted as follows:
		/// 
		/// <code>
		///   wordOffset1 byteOffset1 page1 paragraph1;
		///   wordOffset2 byteOffset2 page2 paragraph2;
		/// </code>
		/// GetHitByteOffsets produces meaningful results <i>only</i> for
		/// plain text files. For other file types, including HTML or
		/// RTF, GetHitByteOffsets will not work. 
		/// </remarks>                                                   
		String^ GetHitByteOffsets ();
			//M:GetHitByteOffsets



		/// <summary>
		/// Total number of hits in all documents
		/// </summary>
	property long TotalHitCount {
			//P:TotalHitCount
		long get() {
				return m_results->getTotalHits();
		}
	}
		/// <summary>
		/// The search request that generated this list of results
		/// </summary>
	property String^ Request {
		//P:Request
		String^ get() {
			return m_results->Request.allocNetString();
		}
	}

		/// <summary>
		/// Word offsets of hits in the currently-selected document
		/// </summary>
		property array<int>^ DocHits {;
			//P:DocHits
		array<int>^ get() {
				return Converters::MakeNetArray(m_results->GetDocHits (), m_results->DocHitCount);
					}
		}

		/// <summary>
		/// Number of hits in the currently-selected document
		/// </summary>
		property long DocHitCount {
			//P:DocHitCount
			long get() {
				return m_results->DocHitCount;
			}
		}

		/// <summary>
		/// Name of the currently-selected document
		/// </summary>
		/// <remarks>
		/// Same as SearchResultsItem.Filename
		/// </remarks>
		property String^ DocName {
			//P:DocName
			String^ get() {
				return m_results->DocName.allocNetString();
			}
		}
		/// <summary>
		/// Number of documents in the list
		/// </summary>
		property long Count {
			//P:Count
			long get() {
				return m_results->getCount();
			}
		}

		/// <summary>
		/// Returns properties of the currently-selected item
		/// </summary>
		property SearchResultsItem ^CurrentItem {
			//P:CurrentItem
			SearchResultsItem^ get() {
				if (m_currentItem == nullptr)
					m_currentItem = Converters::MakeResultsItem(&m_results->DocInfo);
				return m_currentItem;
			}

		}

		/// <summary>
		/// For internal use.
		/// </summary>
			//M:getImplementation
		DSearchResults* getImplementation () {
			return m_results;
			}

		// Using this to attach search results avoids the need to
		// make a copy when detaching from a DSearchJob
		void setImplementation(DSearchResults *res) {
			deleteResults();
			m_results = res;
			}

		/// <summary>
		/// Remove all items from the search results list.
		/// </summary>
		void Empty()
			//M:Empty

			{
			setImplementation(new DSearchResults);
			}
		
		/// <summary>	
		/// Create a SearchResults object containing only those items that are present in two other SearchResults objects			
		/// </summary>
		void IntersectionOf(SearchResults ^a, SearchResults ^b);
		
		/// <summary>
		/// Combines search results with another search results object, eliminating duplicate items.
		/// </summary>
		void UnionWith(SearchResults ^other);
					
	protected:
		DSearchResults* m_results;
		SearchResultsItem ^m_currentItem;
		virtual void DisposeUnmanagedItems() override;
	private:
		void deleteResults() {
			if (m_results) {
				DSearchResults *toDelete = m_results;
				m_results = 0;
				delete toDelete;
				}
			}

	};
}
}