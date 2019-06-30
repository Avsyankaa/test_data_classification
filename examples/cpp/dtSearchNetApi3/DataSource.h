#pragma once

#include "stdafx.h"
#include "Macros.h"
#include "Utils.h"
#include <ApiIndexJob.h>

using namespace System;
using namespace System::Collections;
using namespace System::Runtime::InteropServices;
using namespace System::Threading;
using namespace System::IO;




#pragma managed

namespace dtSearch {
namespace Engine
{
		/// <summary>
		/// Interface for the dataSourceToIndex member of IndexJob, for
		/// indexing non-file data sources such as databases.
		/// </summary>
		/// <remarks>
		/// * Overview *
		/// An IndexJob provides two ways specify the text to index: by
		/// files (the FoldersToIndex, Include Filters, and
		/// ExcludeFilters properties) and by data source (the
		/// DataSourceToIndex property). Most commonly, the text exists
		/// in disk files, in which case you would specify the files to
		/// be indexed using folder names and include and exclude
		/// filters.
		/// 
		/// In some situations, however, the text to be indexed may not
		/// be readily available as disk files. For example, the text may
		/// exist as rows in a remote SQL database or in Microsoft
		/// Exchange message stores. To supply this text to the dtSearch
		/// indexing engine, you can create an object that accesses the
		/// text and then attach the object to an IndexJob as the
		/// DataSourceToIndex property.
		/// * GetNextDoc *
		/// The dtSearch Engine will call the GetNextDoc method of your
		/// DataSource implementation to obtain documents to index. On
		/// each call, dtSearch will use the properties supplied
		/// (DocName, DocModifiedDate, DocFields, DocBytes, etc.) to set
		/// up a document object to index.
		/// 
		/// On each call to GetNextDoc, the DocTypeId, DocId, and
		/// DocWordCount properties will be filled in with the results of
		/// the previous document indexed. This enables the calling
		/// application to know the file type and document id assigned to
		/// each document after it has been indexed. (The document id is
		/// a unique integer identifying each document in an index, and
		/// can be used in SearchFilter objects to limit searches to a
		/// subset of the documents in the index.)
		/// 
		/// If the IndexingFlags.dtsAlwaysAdd flag is not set in the
		/// IndexJob, the DocName and DocModifiedDate will be used to
		/// determine whether the document is already in the index with
		/// the same date, and, if so, the document will not be
		/// reindexed. In this case, the DocTypeId, DocId, and
		/// DocWordCount properties will be set to the values assigned
		/// when the document was originally indexed.
		/// 
		/// \Note: The IncludeFilters and ExcludeFilters in IndexJob do
		/// not apply to content returned from a data source.
		/// * Fields *
		/// The DocFields property lets you add meta-data to the document
		/// text. Fields can be searchable or non-searchable, and can be
		/// designated as &quot;stored&quot; so they will be returned as
		/// document properties in search results (for example, to store
		/// a row id for easy access after a search). Field names can
		/// also include nesting, so instead of just &quot;Author&quot;
		/// \or &quot;Subject&quot; you could use &quot;Meta/Author&quot;
		/// and &quot;Meta/Subject&quot;.
		/// * Examples *
		/// For sample code demonstrating the DataSource API, see:
		/// 
		/// <c>C:\\Program Files\\dtSearch
		/// Developer\\examples\\cs2\\ado_demo (C# sample)</c> and
		/// 
		/// <c>C:\\Program Files\\dtSearch
		/// Developer\\examples\\vb.net2\\ado_demo (VB.NET sample)</c>
		/// * See also *
		/// <see cref="Indexing Databases" /> in dtSearchApiRef.chm
		/// </remarks>                                                   
	public ref class DataSource abstract
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
		property String^ DocName;

			/// <summary>
			/// If True, DocName will be interpreted as the name of a file to be indexed, and dtSearch will index the contents of the file along with any data provided in DocText and DocFields.
			/// </summary>
			///<para>
			/// The DataSource interface provides three ways to return a binary document: DocStream (any .NET
			/// Stream object), DocBytes (a byte array), and DocIsFile (a disk file).
			/// Only one of these can be used for each document.
			///</para>
			
		property bool DocIsFile;

			/// <summary>
			/// The DocDisplayName is a user-friendly version of the filename, which the dtSearch end-user product displays in search results.
			/// </summary>
		property String^ DocDisplayName;

			/// <summary>
			/// The date that the document was last modified.
			/// </summary>
		property DateTime^ DocModifiedDate;

			/// <summary>
			/// The date that the document was originally created.
			/// </summary>
		property DateTime^ DocCreatedDate;

			/// <summary>
			/// In DocText, supply the text you want the dtSearch Engine to index.
			/// </summary>
		property String^ DocText;

			/// <summary>
			/// In DocFields, supply any fielded data you want the dtSearch
			/// Engine to index.
			/// </summary>
			/// <remarks>
			/// DocFields is a string containing any number of field-value
			/// pairs. Each field name and each value must be followed by a
			/// TAB character (\\t or Chr(9)).
			/// 
			/// Add a * in front of a field name to prevent the field name
			/// from being indexed as part of the field. Example:
			/// &quot;*SampleField\\t This is the text of SampleField\\t&quot;.
			/// 
			/// Add two * in front of a field name to designate the field as
			/// a hidden stored field.
			/// </remarks>
			/// <example>
			/// &quot;**SampleField\\t This is the text of SampleField&quot;.
			/// Hidden stored fields are automatically stored in the index as
			/// document properties, and any text in a hidden stored field is
			/// not searchable. After a search, the values of any hidden
			/// stored fields will be returned in
			/// SearchResultsItem.UserFields for each item.
			/// 
			/// Field names can include nesting, using / to separate field
			/// name elements. Example: &quot;Meta/Subject\\t This is the
			/// subject\\tMeta/Authort This is the author&quot; In this
			/// \example, you could search across both fields by searching
			/// for &quot;Meta contains (something)&quot;, or you could
			/// search for &quot;Author contains (something)&quot;, or you
			/// could search for &quot;Meta/Author contains (something)&quot;
			/// to distinguish this Author field from any other Author fields
			/// that might be present in the document.
			/// 
			/// If you are indexing documents, metadata in those documents
			/// will also be indexed as fields, so you may want to name your
			/// DocFields items to avoid clashes with common document
			/// metadata fields such as Title, Subject, Author, etc.
			/// </example>                                                     
		property String^ DocFields;

			/// <summary>
			/// Each time GetNextDoc() is called, DocId will contain the doc id of the previous document.
			/// </summary>
		property int DocId;
			/// <summary>
			/// Each time GetNextDoc() is called, DocWordCount will contain the number of words in the previous document.
			/// </summary>
		property int DocWordCount;
			/// <summary>
			/// Each time GetNextDoc() is called, DocTypeId will return an integer identifying the file type of the previous document.
			/// </summary>
		property int DocTypeId;
			/// <summary>
			/// Each time GetNextDoc() is called, WasDocError will be true if there was an error processing the
			/// previous document (such as a file parsing error)
			/// </summary>
		property bool WasDocError;
			/// <summary>
			/// If WasDocError is true, DocError will contain a string providing details on the nature of the error.
			/// </summary>
		property String^ DocError;

			/// <summary>
			/// Use DocStream to provide access to binary document data for this document in the data source.
			/// </summary>
			/// <remarks>
			///<para>
			/// dtSearch treats the contents of DocStream as if it were a file found on disk, so it can be
			/// any binary document format that dtSearch supports, such as a PDF file, DOC file, XLS file,
			/// etc.   The contents of the document in DocStream, along with any properties such as
			/// Office Summary Information fields, are combined with the field-value pairs in DocFields
			/// and indexed as a single document.
			///</para>
			///<para>
			/// DocStream can be any .NET class derived from System.IO.Stream, as long as the class
			/// supports Read() and Seek() and returns true from both CanRead and CanSeek.
			/// </para>
			///<para>
			/// The DataSource interface provides three ways to return a binary document: DocStream (any .NET
			/// Stream object), DocBytes (a byte array), and DocIsFile (a disk file).
			/// Only one of these can be used for each document.
			///</para>
			/// </remarks>
		property System::IO::Stream^ DocStream;
		
			/// <summary>
			/// Set HaveDocBytes to true if there is binary document data to process in DocBytes
			/// </summary>
		property bool HaveDocBytes;
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
			///<para>
			/// The DataSource interface provides three ways to return a binary document: DocStream (any .NET
			/// Stream object), DocBytes (a byte array), and DocIsFile (a disk file).
			/// Only one of these can be used for each document.
			///</para>
			/// </remarks>
		property array<byte> ^DocBytes;

	};

class CNetInputStream : public DInputStreamBase {
	public:
		CNetInputStream();
		~CNetInputStream() {
			Buffer = 0;
			m_pStream = 0;
			}
		void attach(gcroot<Stream ^> s);
        virtual void seek(DFilePosition where);
        virtual DFilePosition tell();
        virtual long read(void *dest, long bytes);
	protected:
		gcroot<Stream ^>  m_pStream;
		gcroot< array<byte> ^> Buffer;
		void logException(const char *msg, Exception^ ex);
	};

class CNetDataSource : public DDataSourceBase {
    public:
        CNetDataSource();
        ~CNetDataSource() {
			m_pDS = 0;
			}
        void attach(gcroot<DataSource^> ds);
        virtual int getNextDoc(dtsInputStream& dest);
        virtual int rewind();
        bool getErrors(DString& dest) {
			if (m_fDataSourceError)
				dest << dataSourceError;
			return m_fDataSourceError;
			}
    protected:
        bool m_fDone;
        bool m_fUseDocBytes;
        bool m_fDataSourceError;
        DString dataSourceError;
	    InternalApiIndexJob::CInputStreamMaker inputStreamMaker;
	    
		gcroot<DataSource ^> m_pDS;
		CNetInputStream netInputStream;		
        int getNextDocImp(dtsInputStream& dest);
        void reportError(const char *s, const char *source) {
			dataSourceError = source;
			dataSourceError << "( thrown by data source from " << source << ")";
			debugLog(dataSourceError);
			m_fDataSourceError = true;
			}
		void reportError(System::String^ str, const char *source) {
			dataSourceError.set(str);
			dataSourceError << " (thrown by data source from " << source << ")";
			debugLog(dataSourceError);
			m_fDataSourceError = true;
			}
		static void ConvertDate(dtsFileDate& dest, DateTime ^source);
    };


}
}

