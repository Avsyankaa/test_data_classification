#pragma once

#include "stdafx.h"
#include "Macros.h"
#include "JobErrorInfo.h"
#include "SearchResults.h"
#include  <ApiConvertJob.h>

using namespace System;
using namespace System::Collections;
using namespace System::Runtime::InteropServices;
using namespace System::Threading;
using namespace System::IO;

// ------------------ Managed (.NET) code -----------------------
#pragma managed

namespace dtSearch
{
namespace Engine
{
	
	
/// <summary>
/// ExtractionOptions (attached to a FileConvertJob) specifies
/// how embedded images and attachments should be handled.
/// </summary>
/// <remarks>
/// During conversion, embedded images and attachments can be
/// extracted to a folder, with links to the extracted images and
/// attachments inserted in the output. Example:
/// <code lang="c#">
///     dtSearch.Engine.FileConverter conv = new FileConverter();
///     conv.OutputToString = true;
///     conv.InputFile = &quot;c:\\\\docs\\\\sample.doc&quot;;
///     conv.OutputFormat = OutputFormats.itHTML;
/// 
///     dtSearch.Engine.ExtractionOptions extractionOptions = new dtSearch.Engine.ExtractionOptions();
///     // This prevents attachments from being created with executable extensions
///     extractionOptions.AllowedExtensions = &quot;jpg jpeg pdf doc xls ppt zip&quot;;
///     // Attachments with disallowed extensions will have &quot;.data&quot; appended to the output filename
///     extractionOptions.DefaultExtension = &quot;data&quot;;
///     extractionOptions.OutputLocation = &quot;c:\\\\output&quot;;
///     extractionOptions.Flags = dtSearch.Engine.ExtractionOptionsFlags.dtsExoExtractAttachments |
///        dtSearch.Engine.ExtractionOptionsFlags.dtsExoExtractImages |
///        dtSearch.Engine.ExtractionOptionsFlags.dtsExoLimitExtensions;
///     extractionOptions.FilenamePrefix = &quot;tmp_&quot;;
///     extractionOptions.OutputReference = extractionOptions.OutputLocation;
///     extractionOptions.UnnamedAttachmentLinkText = &quot;[Attachment]&quot;;
///     conv.ExtractionOptions = extractionOptions;
///     conv.Execute();
/// </code>
/// When highlighting hits, flags should be consistent with the
/// behavior of the indexer or hit highlighting will be
/// inconsistent.
/// 
/// dtsExoExtractAttachments should not be used when highlighting
/// hits because in some cases text is added to the conversion
/// \output to provide a location for the link to the extracted
/// attachment.
/// 
/// dtsExoDoNotConvertAttachments should not be used when
/// highlighting hits because the indexer does convert
/// attachments to text.
/// 
/// dtsExoExtractImages can be used when highlighting hits.
/// </remarks>                                                                                               
                                                                                    
public ref class ExtractionOptions {
	public:
    ExtractionOptions()
    {   Flags = (ExtractionOptionsFlags) 0;
    }

	/// ExtractionOptionsFlags value.
	property dtSearch::Engine::ExtractionOptionsFlags Flags;

	/// Output folder for images and attachments extracted from the input file
	property String^ OutputLocation;

	/// <summary>
	/// URL of the OutputLocation for links in the conversion output.
	/// </summary>
	/// <remarks>
	/// When the output format for the conversion is HTML or XML, the
	/// location of each extracted image or attachment will be stored
	/// as a link. While OutputLocation must be a folder name,
	/// OutputReference should be the equivalent location expressed
	/// as a URL, so it will work when used in the links in the
	/// conversion output.
	/// 
	/// For example, on a web server OutputLocation might be
	/// c:\\inetpub\\user1\\temp and OutputReference would be the
	/// virtual path to this folder, /user1/temp.
	/// </remarks>                                                   
	property String^ OutputReference;

	/// <summary>
	/// Name of text file to create with the names of all images and
	/// attachments extracted to the OutputLocation.
	/// </summary>
	/// <remarks>
	/// Each attachment or image file extracted will be listed in the
	/// \file, with one name per line. Filenames will be stored in
	/// UTF-8 format. 
	/// </remarks>                                                   
	property String^ OutputFileListName;

	/// <summary>
	/// Filename prefix for extracted files stored in outputLocation
	/// </summary>
	/// <remarks>
	/// For example, if sample.jpg is extracted, and if
	/// FilenamePrefix is &quot;tmp_&quot;, then the name of the file
	/// saved will be &quot;tmp_sample.jpg&quot;. 
	/// </remarks>                                                   
	property String^ FilenamePrefix;

	/// <summary>
	/// Text to use for link to an attachment that has no name
	/// </summary>
	/// <remarks>
	/// If an attachment has no name attribute in the input document,
	/// the value in UnnamedAttachmentLinkText will be used as the
	/// visible text for the HTML link in the generated output. 
	/// </remarks>                                                   
	property String^ UnnamedAttachmentLinkText;

	/// <summary>
	/// If the dtsExoLimitExtensions flag is set, image and
	/// attachment filename extensions will be limited to the values
	/// listed in AllowedExtensions.
	/// </summary>
	/// <remarks>
	/// The AllowedExtensions option provides a way to guard against
	/// execution of malware that may be embedded in documents. When
	/// an attachment or image is extracted, unless the filename
	/// extension is one of the listed extensions in
	/// AllowedExtension, the filename will have the DefaultExtension
	/// appended to it.
	/// 
	/// For example, if &quot;exe&quot; is not included in the list
	/// \of AllowedExtensions, and if the DefaultExtension is
	/// &quot;.data&quot;, then an attachment named &quot;sample.exe&quot;
	/// will be saved as &quot;sample.exe.data&quot;.
	/// 
	/// Separate extensions with spaces. Do not include the &quot;.&quot;
	/// in each extension. */
	/// </remarks>                                                        
	property String^ AllowedExtensions;


	/// <summary>
	/// Extension to add to images and attachments when the
	/// dtsExoLimitExtensions flags is set and an image or attachment
	/// has an extension that is not listed in AllowedExtensions.
	/// </summary>
	/// <remarks>
	/// The AllowedExtensions list controls which attachments and
	/// images can be extracted with the original filename extension
	/// preserved. Attachments and images with extensions that are
	/// not in the list will have the DefaultExtension appended to
	/// the output filename, effectively disabling the original
	/// extension.
	/// 
	/// Do not include the &quot;.&quot; in the extension. 
	/// </remarks>                                                   
	property String^ DefaultExtension;

	};

	

	/// <summary>
	/// Converts files to HTML, RTF, or text, optionally marking hits
	/// with caller-supplied tags.
	/// </summary>
	/// <remarks>
	/// Use SetInputItem to set up most of the properties of
	/// FileConverter from an item in SearchResults. SetInputItem
	/// will set the InputFile, InputTypeId, InputDocId, Hits,
	/// AlphabetLocation, and IndexRetrievedFrom. If the index was
	/// built with caching of documents, SetInputItem will also set
	/// up FileConverter to retrieved the cached version of the
	/// document from the index.
	/// 
	/// The document data to convert can consist of one binary
	/// document file, such as a Word document, and any number of
	/// field-value pairs in InputFields. InputText can be used to
	/// provide additional text to include in the converted output.
	/// 
	/// The binary document file to convert can be provided either as
	/// a disk file, in which InputFile is the name of the file to
	/// convert, or in a memory buffer in InputBytes. InputText and
	/// InputFields are assumed to contain nothing but plain text
	/// (not HTML or RTF, which must be provided through InputFile or
	/// InputBytes).
	/// 
	/// InputFile must be an accessible disk file. UNC paths will
	/// work, provided that the network resource can be accessed, but
	/// HTTP paths will not. To convert data accessed by HTTP,
	/// download the data to a memory buffer and supply it in
	/// InputBytes.
	/// 
	/// If the original data was indexed using the DataSource
	/// indexing API, then to highlight hits set InputBytes,
	/// InputFields, and InputText to the same values that were
	/// returned from the data source as DocBytes, DocFields, and
	/// DocText when the document was indexed.
	/// 
	/// The BeforeHit and AfterHit markers are inserted before and
	/// after each hit word. The BeforeHit and AfterHit markers can
	/// contain hypertext links. To facilitate creation of hit
	/// \navigation markers, the strings &quot;%%ThisHit%%&quot;,
	/// &quot;%%NextHit%%&quot;, and &quot;%%PrevHit%%&quot; will be
	/// replaced with ordinals representing the current hit, the next
	/// hit, and the previous hit in the document.
	/// * IDisposable *
	/// When using the dtSearch Engine API, it is essential to use
	/// the IDisposable pattern with all API objects except
	/// JobErrorInfo. Otherwise, the .NET garbage collector may take
	/// a long time to free memory used by potentially large objects
	/// such as SearchResults, resulting in memory depletion in
	/// \long-running applications. In C# code, use a &quot;using&quot;
	/// clause with dtSearch Engine API objects, and in other code
	/// call Dispose() when you are done with an object.
	/// * See also *
	/// <see cref="Highlighting Hits" />
	/// 
	/// <see cref="Caching documents" />
	/// </remarks>                                                     
	public ref class FileConverter : public DisposableObject
	{
	public:

		FileConverter ();
		~FileConverter ();

			/// <summary>
			/// SetInputItem provides a quick way to set up a FileConvertJob with a particular item from a SearchResults list.
			/// </summary>
			/// <param name="results">The SearchResults containing the document to convert.</param>
			/// <param name="whichItem">The zero-based index of the document in results</param>
		void SetInputItem (SearchResults^ results, Int32 whichItem);
			//M:SetInputItem(dtSearch.Engine.SearchResults,System.Int32)

			/// <summary>
			/// Performs the conversion.
			/// </summary>
		void Execute ();
			//M:Execute

			/// <summary>
			/// If true, output will be stored in an in-memory string variable rather than a disk file. (OutputFile will be ignored.) After the Execute method is done, the output will be in the OutputString property.
			/// </summary>
		property bool OutputToString;

			/// <summary>
			///  Flags that control the conversion.
			/// </summary>
		property dtSearch::Engine::ConvertFlags Flags;

			/// <summary>
			/// The doc id of the document being converted.  This is used when the document is being
			/// extracted from the index rather than from InputBytes or InputText.  SetInputItem() will
			/// set this based on information in SearchResults.
			/// </summary>
		property int InputDocId;

			/// <summary>
			/// The file type of the input document when it was indexed.  SetInputItem() will
			/// set this based on information in SearchResults.
			/// </summary>
		property int InputTypeId;

			/// <summary>
			/// By default, a FileConverter converts the input file to HTML. Other supported options are:  itRTF,
			/// itUTF8 (Unicode text), itAnsi, and itXML (for XML input data only).
			/// </summary>
		property dtSearch::Engine::OutputFormats OutputFormat;

			/// <summary>
			/// When output is directed to an in-memory string, you may wish to limit the maximum amount of memory used. To do this, set OutputStringMaxSize to the maximum size you want to allow.
			/// </summary>
		property int  OutputStringMaxSize;

			/// <summary>
			/// Set TimeoutSeconds to the maximum amount of time you want to permit.  The default is 0, in which case
			/// Execute will not timeout.
			/// </summary>
			/// <remarks>
			/// When this time is exceeded,
			/// Execute will halt leaving incomplete results in the OutputFile or OutputString.
			/// If TimeoutSeconds is 0 (the default), no time limit will be set. After a timeout has occured, the Errors property will contain an message code dtsErTimeout.
			/// </remarks>
		property int TimeoutSeconds;

			/// <summary>
			/// The location of the dtSearch alphabet file to use when highlighting hits.  SetInputItem() will
			/// set this based on information in SearchResults.
			/// </summary>
			/// <remarks>
			/// The alphabet file determines how dtSearch counts words, so it is important that the same alphabet file used to index or search a file also be used to highlight hits. For more information on how hit highlighting works, see Highlighting Hits in the online help. To ensure that the same alphabet used to index a file is used to highlight hits in that file, set the alphabetLocation to the folder where the index is located. The alphabet definition will be stored in this folder (in a file named INDEX_A.IX). If no alphabetLocation is specified, dtSearch will use the default alphabet.
			/// </remarks>
		property String^ AlphabetLocation;

			/// <summary>
			/// If an array of hit offsets has been provided in Hits, then the BeforeHit and AfterHit strings will be used to mark each hit in the document in the converted output
			/// </summary>
		property String^ AfterHit;

			/// <summary>
			/// For HTML output, an HREF for a BASE tag to be inserted in the header.
			/// </summary>
		property String^ BaseHRef;

			/// <summary>
			/// If an array of hit offsets has been provided in Hits, then the BeforeHit and AfterHit strings will be used to mark each hit in the document in the converted output
			/// </summary>
		property String^ BeforeHit;

			/// <summary>
			/// For HTML output, a DocType tag such as &lt;!DOCTYPE html&gt;to
			/// go before the first tag in the output.
			/// </summary>                                                    
		property String^ DocTypeTag;

			/// <summary>
			/// The Footer will be appended to the conversion output and can use tags in the output format, such as HTML tags in a document converted to HTML.
			/// </summary>
		property String^ Footer;

			/// <summary>
			/// The Header will appear at the top of the conversion output and can use tags in the output format, such as HTML tags in a document converted to HTML.
			/// </summary>
		property String^ Header;

			/// <summary>
			/// Use HtmlHead to supply HTML data to appear inside the HEAD section of the output.
			/// </summary>
		property String^ HtmlHead;
			/// <summary>
			/// The index in which the document was found.  SetInputItem will set this based on information in SearchResults.
			/// </summary>
		property String^ IndexRetrievedFrom;

			/// <summary>
			/// If the document was indexed using a DataSource object, supply the same fields in InputFields that the DataSource returned for this document in the DocFields property.
			/// </summary>
		property String^ InputFields;

			/// <summary>
			/// Name of the file to convert.  This can be a local disk file or a UNC path, but not an HTTP file.
			/// </summary>
		property String^ InputFile;

			/// <summary>
			/// If the document was indexed using a DataSource object, supply the same text in InputText that the DataSource returned for this document in the DocText property.
			/// </summary>
		property String^ InputText;

			/// <summary>
			/// Use InputBytes to provide a document in a memory buffer rather than as a disk file.
			/// </summary>
		property array<byte>^ InputBytes;

			/// <summary>
			/// Use DocStream to provide access to binary document data for this document.
			/// </summary>
			/// <remarks>
			///<para>
			/// dtSearch treats the contents of DocStream as if it were a file found on disk, so it can be
			/// any binary document format that dtSearch supports, such as a PDF file, DOC file, XLS file,
			/// etc.
			///</para>
			///<para>
			/// DocStream can be any .NET class derived from System.IO.Stream, as long as the class
			/// supports Read() and Seek() and returns true from both CanRead and CanSeek.
			/// </para>
			///<para>
			/// The FileConverter object provides three ways to return a binary document: InputStream (any .NET
			/// Stream object), InputBytes (a byte array), and InputFile (a disk file).
			/// Only one of these can be used for each document.
			///</para>
			/// </remarks>
		property System::IO::Stream^ InputStream;

			/// <summary>
			/// Word offsets of the hits to highlight using the BeforeHit and AfterHit marks.
			/// </summary>
		property array<int>^ Hits;

			/// <summary>
			/// Information returned in SearchResultsItem.HitsByWord
			/// </summary>
		property array<String ^> ^HitsByWord;


			/// <summary>
			/// Name of the converted file to create.
			/// </summary>
		property String^ OutputFile;

			/// <summary>
			/// If OutputToString is true, output will be stored in OutputString rather than in a disk file.
			/// </summary>
		property String^ OutputString;

			/// <summary>
			/// A JobErrorInfo containing information about any errors that occurred during conversion.
			/// </summary>
		property JobErrorInfo^ Errors;

		/// <summary>
		/// File type of input document detected by dtSearch file parsers.
		/// </summary>
		property dtSearch::Engine::TypeId DetectedTypeId;
		
		/// Options for extraction of embedded images and attachments 
		property dtSearch::Engine::ExtractionOptions^ ExtractionOptions;

	private:
		DFileConvertJob *m_convertJob;
		void makeJob();
		void freeJob();

		void ExecuteFromInputStream(DFileConvertJob& job);

	protected:
		virtual void DisposeUnmanagedItems() override;

	};
}
}
