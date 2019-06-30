#pragma once

#include "stdafx.h"
#include "Macros.h"
#include "JobErrorInfo.h"
#include "SearchResults.h"
#include <ApiConvertJob.h>

using namespace System;
using namespace System::Collections;
using namespace System::Runtime::InteropServices;
using namespace System::Threading;


// ------------------ Managed (.NET) code -----------------------
#pragma managed

namespace dtSearch
{
namespace Engine
{

	/// <summary>
	/// Converts files to HTML, RTF, or text, optionally marking hits with
	/// caller-supplied tags.
	/// </summary>
	/// <remarks>
	/// <para>
	/// Use SetInputItem to set up most of the properties of FileConverter from an item in SearchResults.
	/// SetInputItem will set the InputFile, InputTypeId, InputDocId, Hits, AlphabetLocation, and IndexRetrievedFrom.
	/// </para>
	/// <para>The document data to convert can consist of one binary document file, such as a Word document,
	/// and any number of field-value pairs in InputFields.  InputText can be used to provide additional text
	/// to include in the converted output.</para>
	/// <para>The binary document file to convert can be provided either as a disk file, in which InputFile is the name
	/// of the file to convert, or in a memory buffer in InputBytes.  InputText and InputFields are assumed to contain
	/// nothing but plain text (not HTML or RTF, which must be provided through InputFile or InputBytes).</para>
	/// <para> InputFile must be an accessible disk file.  UNC paths will work, provided that the network resource
	/// can be accessed, but HTTP paths will not.  To convert data accessed by HTTP, download the data to a memory buffer
	/// and supply it in InputBytes.</para>
	/// <para>If the original data was indexed using the DataSource indexing API, then to highlight hits
	/// set InputBytes, InputFields, and InputText to the same values that were returned from the data source
	/// as DocBytes, DocFields, and DocText when the document was indexed.</para>
	/// <para>The BeforeHit and AfterHit markers are inserted before and after each hit word. The BeforeHit and AfterHit markers can contain hypertext links. To facilitate creation of hit navigation markers, the strings "%%ThisHit%%", "%%NextHit%%", and "%%PrevHit%%" will be replaced with ordinals representing the current hit, the next hit, and the previous hit in the document.
	/// </para>
	/// </remarks>
	public __gc class FileConverter : public DisposableObject
	{
	public:

		FileConverter ();
		~FileConverter ();

			/// <summary>
			/// SetInputItem provides a quick way to set up a FileConvertJob with a particular item from a SearchResults list.
			/// </summary>
			/// <param name="results">The SearchResults containing the document to convert.</param>
			/// <param name="whichItem">The zero-based index of the document in results</param>
		void SetInputItem (SearchResults* results, Int32 whichItem);
			//M:SetInputItem(dtSearch.Engine.SearchResults,System.Int32)

			/// <summary>
			/// Performs the conversion.
			/// </summary>
		void Execute ();
			//M:Execute

			/// <summary>
			/// If true, output will be stored in an in-memory string variable rather than a disk file. (OutputFile will be ignored.) After the Execute method is done, the output will be in the OutputString property.
			/// </summary>
		IMPLEMENT_BOOLEAN (OutputToString);

			/// <summary>
			///  Flags that control the conversion.
			/// </summary>
		IMPLEMENT_TYPE (dtSearch::Engine::ConvertFlags, Flags);

			/// <summary>
			/// The doc id of the document being converted.  This is used when the document is being
			/// extracted from the index rather than from InputBytes or InputText.  SetInputItem() will
			/// set this based on information in SearchResults.
			/// </summary>
		IMPLEMENT_INT (InputDocId);

			/// <summary>
			/// The file type of the input document when it was indexed.  SetInputItem() will
			/// set this based on information in SearchResults.
			/// </summary>
		IMPLEMENT_INT (InputTypeId);

			/// <summary>
			/// By default, a FileConverter converts the input file to HTML. Other supported options are:  itRTF,
			/// itUTF8 (Unicode text), itAnsi, and itXML (for XML input data only).
			/// </summary>
		IMPLEMENT_TYPE (dtSearch::Engine::OutputFormats, OutputFormat);

			/// <summary>
			/// When output is directed to an in-memory string, you may wish to limit the maximum amount of memory used. To do this, set OutputStringMaxSize to the maximum size you want to allow.
			/// </summary>
		IMPLEMENT_INT (OutputStringMaxSize);

			/// <summary>
			/// Set TimeoutSeconds to the maximum amount of time you want to permit.  The default is 0, in which case
			/// Execute will not timeout.
			/// </summary>
			/// <remarks>
			/// When this time is exceeded,
			/// Execute will halt leaving incomplete results in the OutputFile or OutputString.
			/// If TimeoutSeconds is 0 (the default), no time limit will be set. After a timeout has occured, the Errors property will contain an message code dtsErTimeout.
			/// </remarks>
		IMPLEMENT_INT (TimeoutSeconds);

			/// <summary>
			/// The location of the dtSearch alphabet file to use when highlighting hits.  SetInputItem() will
			/// set this based on information in SearchResults.
			/// </summary>
			/// <remarks>
			/// The alphabet file determines how dtSearch counts words, so it is important that the same alphabet file used to index or search a file also be used to highlight hits. For more information on how hit highlighting works, see Highlighting Hits in the online help. To ensure that the same alphabet used to index a file is used to highlight hits in that file, set the alphabetLocation to the folder where the index is located. The alphabet definition will be stored in this folder (in a file named INDEX_A.IX). If no alphabetLocation is specified, dtSearch will use the default alphabet.
			/// </remarks>
		IMPLEMENT_STRING (AlphabetLocation);

			/// <summary>
			/// If an array of hit offsets has been provided in Hits, then the BeforeHit and AfterHit strings will be used to mark each hit in the document in the converted output
			/// </summary>
		IMPLEMENT_STRING (AfterHit);

			/// <summary>
			/// For HTML output, an HREF for a BASE tag to be inserted in the header.
			/// </summary>
		IMPLEMENT_STRING (BaseHRef);

			/// <summary>
			/// If an array of hit offsets has been provided in Hits, then the BeforeHit and AfterHit strings will be used to mark each hit in the document in the converted output
			/// </summary>
		IMPLEMENT_STRING (BeforeHit);

			/// <summary>
			/// The Footer will be appended to the conversion output and can use tags in the output format, such as HTML tags in a document converted to HTML.
			/// </summary>
		IMPLEMENT_STRING (Footer);

			/// <summary>
			/// The Header will appear at the top of the conversion output and can use tags in the output format, such as HTML tags in a document converted to HTML.
			/// </summary>
		IMPLEMENT_STRING (Header);

			/// <summary>
			/// Use HtmlHead to supply HTML data to appear inside the HEAD section of the output.
			/// </summary>
		IMPLEMENT_STRING (HtmlHead);
			/// <summary>
			/// The index in which the document was found.  SetInputItem will set this based on information in SearchResults.
			/// </summary>
		IMPLEMENT_STRING (IndexRetrievedFrom);

			/// <summary>
			/// If the document was indexed using a DataSource object, supply the same fields in InputFields that the DataSource returned for this document in the DocFields property.
			/// </summary>
		IMPLEMENT_STRING (InputFields);

			/// <summary>
			/// Name of the file to convert.  This can be a local disk file or a UNC path, but not an HTTP file.
			/// </summary>
		IMPLEMENT_STRING (InputFile);

			/// <summary>
			/// If the document was indexed using a DataSource object, supply the same text in InputText that the DataSource returned for this document in the DocText property.
			/// </summary>
		IMPLEMENT_STRING (InputText);

			/// <summary>
			/// Use InputBytes to provide a document in a memory buffer rather than as a disk file.
			/// </summary>
		__property byte get_InputBytes() __gc[] {
			//P:InputBytes
			return m_DocBytes;
			}
		__property void set_InputBytes(byte docBytes __gc[]) {
			m_DocBytes = docBytes;
			}

			/// <summary>
			/// Word offsets of the hits to highlight using the BeforeHit and AfterHit marks.
			/// </summary>
		__property int get_Hits() __gc[] {
			//P:Hits
			return m_Hits;
			}
		__property void set_Hits(int hits __gc[]) {
			m_Hits = hits;
			}

			/// <summary>
			/// Name of the converted file to create.
			/// </summary>
		IMPLEMENT_STRING (OutputFile);

			/// <summary>
			/// If OutputToString is true, output will be stored in OutputString rather than in a disk file.
			/// </summary>
		IMPLEMENT_STRING (OutputString);

			/// <summary>
			/// A JobErrorInfo containing information about any errors that occurred during conversion.
			/// </summary>
		IMPLEMENT_TYPE_GET (JobErrorInfo*, Errors);

		/// <summary>
		/// File type of input document detected by dtSearch file parsers.
		/// </summary>
		IMPLEMENT_TYPE_GET (dtSearch::Engine::TypeId, DetectedTypeId);

	private:

		FIELD_BOOLEAN (OutputToString);

		FIELD_TYPE (dtSearch::Engine::ConvertFlags, Flags);
		FIELD_INT (InputDocId);
		FIELD_INT (InputTypeId);
		FIELD_TYPE (dtSearch::Engine::OutputFormats, OutputFormat);
		FIELD_INT (OutputStringMaxSize);
		FIELD_INT (TimeoutSeconds);

		FIELD_STRING (AlphabetLocation);
		FIELD_STRING (AfterHit);
		FIELD_STRING (BaseHRef);
		FIELD_STRING (BeforeHit);
		FIELD_STRING (Footer);
		FIELD_STRING (Header);
		FIELD_INTARR (Hits);
		FIELD_STRING (HtmlHead);
		FIELD_STRING (IndexRetrievedFrom);
		FIELD_STRING (InputFields);
		FIELD_STRING (InputFile);
		FIELD_STRING (InputText);
		FIELD_STRING (OutputFile);
		FIELD_STRING (OutputString);

		FIELD_TYPE_GET (JobErrorInfo*, Errors);
		FIELD_TYPE (dtSearch::Engine::TypeId, DetectedTypeId);
	private:
		CInternalApiConvertJob *m_convertJob;
		void makeJob();
		void freeJob();
		byte m_DocBytes __gc[];

		void ExecuteFromInputStream(CInternalApiConvertJob& job);

	protected:
		void DisposeUnmanagedItems();

	};
}
}
