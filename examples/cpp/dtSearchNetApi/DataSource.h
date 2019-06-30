#pragma once

#include "stdafx.h"
#include "Macros.h"
#include "Utils.h"

using namespace System;
using namespace System::Collections;
using namespace System::Runtime::InteropServices;
using namespace System::Threading;

#pragma managed

namespace dtSearch {
namespace Engine
{
		/// <summary>
		/// Interface for the dataSourceToIndex member of IndexJob, for indexing non-file data sources such as databases.
		/// </summary>
		/// <remarks>
		/// <para>
		/// An IndexJob provides two ways specify the text to index: by files (the FoldersToIndex, Include Filters,
		/// and ExcludeFilters properties) and by
		/// data source (the DataSourceToIndex property). Most commonly, the text exists in disk files, in which case
		/// you would specify the files to be indexed using folder names and include and exclude filters.
		/// </para>
		///<para>
		/// In some situations, however, the text to be indexed may not be readily available as disk files.
		/// For example, the text may exist as rows in a remote SQL database or in Microsoft Exchange
		/// message stores.
		/// To supply this text to the dtSearch indexing engine, you can create an object that accesses the
		/// text and then attach the object to an <see cref="T:dtSearch.Engine.IndexJob">IndexJob</see>
		/// as the DataSourceToIndex property.
		/// </para>
		/// <para>
		/// The dtSearch Engine will call the GetNextDoc method of your DataSource implementation
		/// to obtain documents to index.   On each call, dtSearch will use the properties supplied (DocName,
		/// DocModifiedDate, DocFields, DocBytes, etc.) to set up a document object to index.
		/// </para>
		/// <para>
		/// On each call to GetNextDoc, the DocTypeId, DocId, and DocWordCount properties will be filled
		/// in with the results of the previous document indexed.   This enables the calling application
		/// to know the file type and document id assigned to each document after it has been indexed.
		/// (The document id is a unique integer identifying each document in an index, and can be used
		/// in <see cref="T:dtSearch.Engine.SearchFilter">SearchFilter</see> objects to limit searches to a subset of the documents in the index.)
		/// </para>
		/// <para>
		/// If the IndexingFlags.dtsAlwaysAdd flag is not set in the IndexJob, the DocName and DocModifiedDate
		/// will be used to determine whether the document is already in the index with the same date, and, if so, the
		/// document will not be reindexed.  In this case, the DocTypeId, DocId, and DocWordCount properties
		/// will be set to the values assigned when the document was originally indexed.
		/// </para>
		/// <para>
		/// The DocFields property lets you add meta-data to the document text.  Fields can be searchable
		/// or non-searchable, and can be designated as "stored" so they will be returned as document properties
		/// in search results (for example, to store a row id for easy access after a search).
		/// Field names can also include nesting, so instead of just "Author" or "Subject" you could use
		/// "Meta/Author" and "Meta/Subject".
		/// </para>
		/// <para>
		/// For sample code demonstrating the DataSource API, see:
		/// C:\Program Files\dtSearch Developer\examples\cs\ado_demo (C# sample) and
		/// C:\Program Files\dtSearch Developer\examples\vb.net\ado_demo (VB.NET sample)
		/// </para>
		/// <para>
		/// Note:  The IncludeFilters and ExcludeFilters in IndexJob do not apply to content returned
		/// from a data source.
		/// </para>
		/// </remarks>
		/// <seealso cref="T:dtSearch.Engine.IndexJob"/>
	public __gc class DataSource
	{
	public:
			/// <summary>
			/// Initialize the data source so the next GetNextDoc call will return the first document.
			/// </summary>
			/// <returns>
			/// true if the data source was initialized successfully, or false if the data could not be accessed.
			/// </returns>
		virtual bool Rewind () = 0;
			//M:Rewind

			/// <summary>
			/// Get the next document from the data source.
			/// </summary>
			/// <returns>
			/// true if a document was obtained, or false if there are no more documents to process.
			/// </returns>
		virtual bool GetNextDoc () = 0;
			//M: GetNextDoc

			/// <summary>
			/// The DocName is the name of the document, as you want it to appear in search results.
			/// </summary>
		IMPLEMENT_STRING (DocName);
			/// <summary>
			/// If True, DocName will be interpreted as the name of a file to be indexed, and dtSearch will index the contents of the file along with any data provided in DocText and DocFields.
			/// </summary>
		IMPLEMENT_BOOLEAN (DocIsFile);
			/// <summary>
			/// The DocDisplayName is a user-friendly version of the filename, which the dtSearch end-user product displays in search results.
			/// </summary>
		IMPLEMENT_STRING (DocDisplayName);
			/// <summary>
			/// The date that the document was last modified.
			/// </summary>
		IMPLEMENT_TYPE (DateTime, DocModifiedDate);

			/// <summary>
			/// The date that the document was originally created.
			/// </summary>
		IMPLEMENT_TYPE (DateTime, DocCreatedDate);

			/// <summary>
			/// In DocText, supply the text you want the dtSearch Engine to index.
			/// </summary>
		IMPLEMENT_STRING (DocText);
			/// <summary>
			/// In DocFields, supply any fielded data you want the dtSearch Engine to index.
			/// </summary>
			/// <remarks>
			/// <para>DocFields is a string containing any number of field-value pairs.  Each field name
			/// and each value must be followed by a TAB character (\t or Chr(9)).
			/// </para>
			/// <para>
			/// Add a * in front of a field name to prevent the field name from being indexed as
			/// part of the field. Example: "*SampleField\t This is the text of SampleField\t".
			/// </para>
			/// <para>
			/// Add two * in front of a field name to designate the field as a hidden stored field.
			/// Example: "**SampleField\t This is the text of SampleField". Hidden stored fields
			/// are automatically stored in the index as document properties, and any text in a hidden
			/// stored field is not searchable.   After a search, the values of any hidden stored
			/// fields will be returned in SearchResultsItem.UserFields for each item.
			/// </para>
			/// <para>
			/// Field names can include nesting, using / to separate field name elements.  Example:
			///	"Meta/Subject\t This is the subject\tMeta/Author\t This is the author"
			///	In this example, you could search across both fields by searching for "Meta contains
			/// (something)", or you could search for "Author contains (something)", or you could
			/// search for "Meta/Author contains (something)" to distinguish this Author field from
			/// any other Author fields that might be present in the document.
			/// </para>
 			/// </remarks>
		IMPLEMENT_STRING (DocFields);

			/// <summary>
			/// Each time GetNextDoc() is called, DocId will contain the doc id of the previous document.
			/// </summary>
		IMPLEMENT_INT(DocId);
			/// <summary>
			/// Each time GetNextDoc() is called, DocWordCount will contain the number of words in the previous document.
			/// </summary>
		IMPLEMENT_INT(DocWordCount);
			/// <summary>
			/// Each time GetNextDoc() is called, DocTypeId will return an integer identifying the file type of the previous document.
			/// </summary>
		IMPLEMENT_INT(DocTypeId);
			/// <summary>
			/// Each time GetNextDoc() is called, WasDocError will be true if there was an error processing the
			//  previous document (such as a file parsing error)
			/// </summary>
		IMPLEMENT_BOOLEAN(WasDocError);
			/// <summary>
			/// If WasDocError is true, DocError will contain a string providing details on the nature of the error.
			/// </summary>
		IMPLEMENT_STRING(DocError);
			/// <summary>
			/// Set HaveDocBytes to true if there is binary document data to process in DocBytes
			/// </summary>
		IMPLEMENT_BOOLEAN (HaveDocBytes);
			/// <summary>
			/// Use DocBytes to provide an array of bytes for dtSearch to use as the binary contents of this document.
			/// </summary>
			/// <remarks>
			///<para>
			/// dtSearch treats the contents of DocBytes as if it were a file found on disk, so it can be
			/// any binary document format that dtSearch supports, such as a PDF file, DOC file, XLS file,
			/// etc.   The contents of the document in DocBytes, along with any properties such as
			/// Office Summary Information fields, are combined with the field-value pairs in DocFields
			/// and indexed as a single document.
			///</para>
			///<para>
			/// Set HaveDocBytes to true if there is a document in DocBytes.
			///</para>
			/// </remarks>
		__property byte get_DocBytes() __gc[] {
			//P:DocBytes
			return m_DocBytes;
			}
		__property void set_DocBytes(byte docBytes __gc[]) {
			m_DocBytes = docBytes;
			}

	protected:
		FIELD_STRING (DocName);
		FIELD_BOOLEAN (DocIsFile);
		FIELD_STRING (DocDisplayName);
		FIELD_STRING (DocText);
		FIELD_STRING (DocFields);
		FIELD_BOOLEAN (HaveDocBytes);

		FIELD_INT(DocId);
		FIELD_INT(DocWordCount);
		FIELD_INT(DocTypeId);
		FIELD_BOOLEAN(WasDocError);
		FIELD_STRING(DocError);

		DateTime m_DocModifiedDate;
		DateTime m_DocCreatedDate;
		byte m_DocBytes __gc[];
	};


}
}

