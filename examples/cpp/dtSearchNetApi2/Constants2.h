#pragma once

namespace dtSearch {

namespace Engine {



    ///    Values for dtsOptions.binaryFiles (C++), Options.BinaryFiles (.NET), and Options.setBinaryFiles (Java). 
    ///    See <link Filtering Options>.
    ///  
public enum class BinaryFilesSettings {

    ///     Summary
    /// 		Filter text from binary files using the character array in binaryFilterTextChars to determine which characters are text.
    /// 	    This option is not recommended.
    /// 	    Use dtsoFilterBinaryUnicode instead for more effective text extraction from binary data.
    /// 	 
    dtsoFilterBinary = 1,

    ///    Summary
    /// 	    Index all contents of binary files as single-byte text.  
    /// 	    This option is not recommended.
    /// 	    Use dtsoFilterBinaryUnicode instead for more effective text extraction from binary data.
    /// 	 
    dtsoIndexBinary = 2,

    ///    Do not index binary files
    /// 	 
    dtsoIndexSkipBinary = 3,

    ///     Summary
    /// 	     Filter text from binary files using a text extraction algorithm that 
    /// 	     scans for sequences of single-byte, UTF-8, or Unicode text in the input. 
    /// 	     This option is recommended for working with forensic data, particularly 
    /// 	     when searching for non-English text. 
    /// 	  
    dtsoFilterBinaryUnicode = 4,

    ///    Summary
    /// 	    Index binary files disregarding all content within the file.  Only the filename
    /// 	    and any fields supplied externally to the file, such as in DocFields in the DataSource
    /// 	    API, will be indexed. 
    /// 	    
    dtsoIndexBinaryNoContent = 5
};

    ///  Flags for dtsConvertJob2 (C++) and FileConverter (.NET, Java)
[System::Flags]
public enum class ConvertFlags {

    ///  Obsolete
    dtsConvertInputIsHtml = 0x0001,

    ///  Summary
    ///   Do not insert BeforeHit and AfterHit marks for a hit that cannot be displayed in HTML because the text is not visible. 
    ///  If this flag is not set, a pair of beforeHit/afterHit tags will be added before the next visible text in the file.
    dtsConvertSkipHiddenHits = 0x0002,

    ///    Summary
    ///     Remove JavaScript from HTML files when adding hit highlight markings. JavaScript
    ///     in HTML files can cause errors if displayed outside of the expected context of
    ///     the script.                                                                      
    dtsConvertRemoveScripts = 0x0004,

    ///  Obsolete
    dtsConvertInputIsNotHtml = 0x0008,

    ///    Summary
    ///     Extract a document either from an index or from a container
    ///  
    ///     Remarks
    ///     (1) When a document is cached in the index, extract it without performing any
    ///     conversion. Must be combined with dtsConvertGetFromCache, and the OutputFile must
    ///     be set to the name of the file to write the result to (output cannot be directed
    ///     to memory).                                                                       
    ///     (2) When a document is stored in a container file format such as a PST archive,
    ///     extract it without performing any conversion. OutputFile must
    ///     be set to the name of the file to write the result to (output cannot be directed
    ///     to memory).                                                                      
    ///     
    dtsConvertExtractOnly = 0x0020,

    ///    Summary
    ///     Add hit highlight markings to XML data while retaining the XML format. Both the
    ///     \input and output formats must be XML. See <link Highlighting hits in XML files>
    ///     in the dtSearch Engine Programmer's Reference (dtSearchApiRef.chm) for
    ///     information on this flag.                                                        
    dtsConvertXmlToXml = 0x0040,

    ///  Just detect the file type of the document and return it in detectedTypeId, without generating
    ///  any output.
    dtsConvertJustDetectType = 0x0080,

    ///    Summary
    ///     Use CSS styles instead of inline formatting to format HTML output. This affects
    ///     content such as email headers, document properties, attachment filenames, and
    ///     \other content that does not have its own formatting from the document. See <link Conversion output formatting>
    ///     in the dtSearch Engine Programmer's Reference (dtSearchApiRef.chm) for
    ///     information on this flag.                                                                                       
    dtsConvertUseStyles = 0x0100,

    ///  Read input file from cached document stored in the index.
    dtsConvertGetFromCache = 0x2000,

    ///  Summary
    ///  If the input is a container (such as a ZIP file), recursively unpack each item in the
    ///  container, convert it, and append it to the output.  dtsConvertInlineContainer
    ///  is ignored if the input file is not a container, so it has no effect on the processing of .doc, .xls, etc. files.
    dtsConvertInlineContainer = 0x4000,

    ///  Summary
    ///  
    ///  Automatically update hit offsets if this document was indexed by an earlier version
    ///  of dtSearch or was changed since it was last indexed.  When using this flag,
    ///  SetInputItem() must be used to transfer information about the search from
    ///  SearchResults.  Additionally, the SearchResults must be generated from a search
    ///  and not by calling UrlDecodeItem, because dtSearch needs information from the
    ///  original search to ensure that highlighting information is correct.
    dtsConvertAutoUpdateSearch = 0x20000,

    ///  include BOM in UTF-8 output
    dtsConvertIncludeBOM = 0x40000,

    ///    Summary
    ///    Highlight each search term using different highlighting attributes. See <link Multihighlighting, Highlighting each term using different attributes>
    ///    in the dtSearch Engine Programmer's Reference (dtSearchApiRef.chm) for
    ///    information on this flag.                                                                                                                           
    dtsConvertMultiHighlight = 0x80000
};

    ///    Summary
    ///    Flags to control the extraction of attachments and images using FileConverter
    ///    (.NET, Java) or dtsFileConvertJob2 (C++).
    ///    Remarks
    ///    When highlighting hits, flags should be consistent with the behavior of the
    ///    indexer or hit highlighting will be inconsistent.
    ///  
    ///    dtsExoExtractAttachments should not be used when highlighting hits because in
    ///    some cases text is added to the conversion output to provide a location for the
    ///    link to the extracted attachment.
    ///  
    ///    dtsExoDoNotConvertAttachments should not be used when highlighting hits because
    ///    the indexer does convert attachments to text.
    ///  
    ///    dtsExoExtractImages can be used when highlighting hits.                         
[System::Flags]
public enum class ExtractionOptionsFlags {

    ///    Extract embedded images in the input and insert a reference to the extracted
    ///     image in the conversion output                                               
    dtsExoExtractImages = 0x0001,

    ///    Extract embedded attachments in the input and insert a reference to the extracted
    ///     attachment in the conversion output (dtsExoExtractAttachments should not be used
    ///     when highlighting hits because in some cases text is added to the conversion
    ///     \output to provide a location for the link to the extracted attachment).          
    dtsExoExtractAttachments = 0x0002,

    ///    Omit attachments from the conversion output (dtsExoDoNotConvertAttachments should
    ///     not be used when highlighting hits because the indexer does convert attachments
    ///     to text).                                                                         
    dtsExoDoNotConvertAttachments = 0x0004,

    ///  Limit the filename extensions for extracted images and attachments to those listed
    ///  in allowedExtensions.
    dtsExoLimitExtensions = 0x0008,

    ///  If true, on NTFS file systems, all extracted images and attachments will be marked
    ///  as originating in the "Internet" Zone.  This enables various safety mechanisms in
    ///  Microsoft Windows and Microsoft Office products to guard against opening unsafe
    ///  attachments when you try to launch marked files.  
    ///  To mark the files, dtSearch adds an alternate stream to the output file named 
    ///  Zone.Identifier with the content "[ZoneTransfer]\r\nZoneId=3\r\n"
    dtsExoMarkAsInternetZone = 0x0010,

    dtsExoReserved1 = 0x0020
};

    ///  Error codes are returned after a job completes.  In the C++ API, use the dtsErrorInfo attached to a job
    ///  to access error codes.  In .NET, use the Errors property of the job.  In Java, use getErrors().
public enum class ErrorCodes {

    ///  No error
    dtsErOK = 0,

    ///  Unable to access index  
    dtsErAccIndex = 1,

    ///    If IndexingFlags.dtsCheckDiskSpace was set in an IndexJob,
    ///   and if it appears that disk space is insufficient, then dtsAskDiskFullOverride will be
    ///   sent to the error handler; the calling program should always allow the update to be
    ///   cancelled in this case (the default action).  
    dtsAskDiskFullOverride = 2,

    ///  Unable to open temporary file.   
    dtsErOpenTemp = 3,

    ///  Obsolete; no longer used.
    dtsInfNoFilesToIndex = 4,

    ///  Index built by incompatible version of dtSearch 
    dtsErIxWrongVersion = 5,

    ///  Obsolete; no longer used.
    dtsErIxInterrupted = 6,

    ///  Unable to create or access directory 
    dtsErAccDirectory = 7,

    ///  Private directory is already locked 
    dtsErWorkDirFail = 8,

    ///  Unable to access input file 
    dtsErAccFile = 9,

    ///  File format error reading input file 
    dtsErFileCorrupt = 10,

    ///  Index is full 
    dtsErIndexFull = 11,

    ///  Obsolete; no longer used.
    dtsErPdfLzwNotLicensed = 12,

    ///  Operation timed out (trigged by timeoutSeconds) 
    dtsErTimeout = 13,

    ///  Unable to create index 
    dtsErCreateFailed = 14,

    ///  Unable to commit changes to index 
    dtsErCommitFailed = 15,

    ///    The document was not found in the index (returned when a document listed to be removed from an index
    ///      is not found in the index) 
    dtsErFileNotFound = 16,

    ///  The document could not be accessed due to encryption 
    dtsErFileEncrypted = 17,

    ///  Incorrectly configured API structure (for example, invalid dtSearchJob) 
    dtsErApiError = 18,

    ///  Exception thrown by data source 
    dtsErDataSourceException = 19,

    ///  Filename is blank 
    dtsErBlankFilename = 20,

    ///  File contains no data 
    dtsErFileEmpty = 21,

    ///  A handle passed to a dtSearch API function was invalid 
    dtsErInvalidHandle = 22,

    ///  In an index merge, the flag dtsIndexKeepExistingDocIds was set and one or more indexes had overlapping doc id ranges 
    dtsErConflictingDocIdRanges = 23,

    ///  Search halted due to low memory available 
    dtsErLowMemory = 112,

    ///  Syntax error in search request 
    dtsErBadRequest = 116,

    ///  Search request is blank
    dtsErSearchRequestEmpty = 117,

    ///  Search found more files than the autoStopLimit for the search job 
    dtsErSearchLimitReached = 120,

    ///  No files retrieved in search 
    dtsErNoFilesRetrieved = 122,

    ///  Search request was longer than dtsMaxSearchRequestLen 
    dtsErRequestTooLong = 135,

    ///  Too many words retrieved in index 
    dtsErMaxWords = 137,

    ///  Unable to write output file 
    dtsErWrFile = 143,

    ///  Disk full -- indexing halted 
    dtsErDiskFull = 159,

    ///  Index is corrupt 
    dtsErIxCorrupt = 162,

    ///  Access to the index was denied 
    dtsErAccessDenied = 177,

    ///  Unable to connect to data source during indexing 
    dtsErConnectFailed = 201,

    ///  Unable to access cached document in index 
    dtsErAccCachedDoc = 202,

    ///  Index update could not complete due to an error accessing the index 
    dtsErIndexingError = 203,

    ///  An out-of-memory error occurred 
    dtsErOutOfMemory = 204,

    ///  An unexpected exception occurred 
    dtsErUnknownException = 205
};

    ///    Summary
    ///    Control indexing of meta-data associated with documents
    ///    Remarks
    ///    FieldFlags provide options to control the indexing of meta-data associated with
    ///    documents. When highlighting hits, it is important to make sure that FieldFlags
    ///    has the same options that were used when a document was indexed. Otherwise, hit
    ///    highlighting may be incorrect due to differences in the words found in each
    ///    document.
    ///  
    ///    By default, dtSearch will index fields in documents such as the Summary
    ///    Information fields in Word files and META tags in HTML files. FieldFlags can be
    ///    used to suppress some or all of this metadata.
    ///  
    ///    dtSearch will also add a "Filename" field to the end of each document, with the
    ///    full path and filename of the document, so words in the document name will be
    ///    searchable like other text. To suppress this completely, use
    ///    dtsoFfSkipFilenameField. To include only the name of the document (not the path),
    ///    use dtsoSkipFilenameFieldPath.
    ///  
    ///    The dtsoFfHtmlShow* flags can be used to make normally hidden HTML elements, such
    ///    as styles or links, visible and searchable. For each category of element that is
    ///    enabled, a section will be added to the end of the HTML file listing the items in
    ///    that category. For example, if dtsoFfHtmlShowComments is set, then each HTML file
    ///    will have a list of the embedded comments after the body of the HTML.
    ///    * API *
    ///    C++: dtsOptions.fieldFlags
    ///  
    ///    Java: Options.setFieldFlags()
    ///  
    ///    .NET: Options.FieldFlags
    ///  
    ///    COM: Options.FieldFlags                                                           
[System::Flags]
public enum class FieldFlags {

    ///  Do not generate a field named Filename containing the name of the file.
    dtsoFfSkipFilenameField = 0x0001,

    ///  Do not index or search document summary fields
    dtsoFfSkipDocumentProperties = 0x0002,

    ///  Make HTML links searchable
    dtsoFfHtmlShowLinks = 0x0004,

    ///  Make HTML IMG src= attribute searchable
    dtsoFfHtmlShowImgSrc = 0x0008,

    ///  Make HTML Comments searchable
    dtsoFfHtmlShowComments = 0x0010,

    ///  Make HTML Scripts searchable
    dtsoFfHtmlShowScripts = 0x0020,

    ///  Make HTML style sheets searchable
    dtsoFfHtmlShowStylesheets = 0x0040,

    ///  Make HTML meta tags searchable and visible, appended to the body of the HTML file
    dtsoFfHtmlShowMetatags = 0x0080,

    ///  Make content inside NOFRAMES tags searchable and visible, appended to the body of the HTML file
    dtsoFfHtmlShowNoframesContent = 0x0100,

    ///  All of the dtsoFfHtmlShow* flags
    dtsoFfHtmlShowHiddenContent = 0x01fc,

    ///  Suppress generation of HtmlTitle, HtmlH1, etc. fields
    dtsoFfHtmlNoHeaderFields = 0x0200,

    ///  Skip non-text streams in Office documents
    dtsoFfOfficeSkipHiddenContent = 0x0400,

    ///  In XML, make field names not searchable
    dtsoFfXmlHideFieldNames = 0x0800,

    ///  Make NTFS file properties searchable
    dtsoFfShowNtfsProperties = 0x1000,

    ///  Do not index attributes in XML files
    dtsoFfXmlSkipAttributes = 0x2000,

    ///  Include only the filename (not the path) in the Filename field generated at the end of each document.
    dtsoFfSkipFilenameFieldPath = 0x4000,

    ///    Summary
    ///      Skip attachments in PDF files.  If a PDF file has attachments, those attachments can be in any file format,
    ///      so Adobe Reader cannot be used to highlight hits because it can only highlight hits in PDF data.
    ///      Therefore, a PDF file with attachments must be hit-highlighted through file conversion like other
    ///      document formats.  Skipping PDF attachments enables PDF files with attachments to be hit-highlighted
    ///      using Adobe Reader.
    ///   
    dtsoFfPdfSkipAttachments = 0x8000,

    ///  Skip HTML INPUT tag "value" attributes
    dtsoFfHtmlSkipInputValues = 0x10000,

    ///  Skip HTML IMG tag "alt" attributes,
    dtsoFfHtmlSkipImageAlt = 0x20000,

    ///  Add file type field indicating the file format of the document (ex: "Microsoft Word")
    dtsoFfIncludeFileTypeField = 0x40000,

    ///  Add numeric type id field with the type id indicating the file format of the document
    dtsoFfIncludeFileTypeIdField = 0x80000,

    ///  Summary:
    ///  Suppress fields passed through the DataSource API through DataSource.DocFields or FileConverter.InputFields.
    ///  Remarks:
    ///  This flag is provides a way to prevent cached searchable fields from appearing in FileConverter output when
    ///  documents are indexed with caching of original files enabled.  
    dtsoFfSkipDataSourceFields = 0x100000,

    ///  Summary:
    ///  Suppress display of headers in emails.
    ///  Remarks:
    ///  This flag prevents MIME headers from appearing in emails when converted using FileConverter.
    ///  It does not affect display of the message properties (Sender, Recipient, Subject, etc.) at the
    ///  top of each message.
    ///  
    ///  To suppress display of message properties, use dtsoFfSkipEmailProperties
    dtsoFfSkipEmailHeaders = 0x200000,

    ///    Summary
    ///      Index the names of files in ZIP and RAR archives
    ///      Remarks
    ///      This option provides a way to search on the list of files in a ZIP or RAR
    ///      archive, even if the individual files may be inaccessible due to encryption. When
    ///      dtSearch indexes a ZIP or RAR archive, in addition to the files actually present
    ///      in the ZIP or RAR archive, it will also make a list of all of the files in the
    ///      archive and index it with the name ArchiveFileList.html.
    ///  
    ///      The original file is not modified but the ArchiveFileList.html file is searchable
    ///      as if it were part of the ZIP or RAR file. The file consists of a list of the
    ///      names of the files inside the archive.
    ///  
    ///      For example, suppose you index a ZIP file that contains "secret1.doc",
    ///      "secret2.doc", and "secret3.doc", all stored in the ZIP file with encryption. The
    ///      contents of the three files will not be searchable because of the encryption. However,
    ///      a search on "secret1" will find the word "secret1" in ArchiveFileList.html in the
    ///      ZIP file.
    ///  
    ///      RAR files can be encrypted in a way that makes even the filenames impossible to
    ///      see without a password. In this case, the names will not be searchable.                
    dtsoFfIndexArchiveFileLists = 0x400000,

    ///  Summary
    ///  Include a caption "Document Properties" on the table of document properties in Word, Excel, and PowerPoint 2003 documents
    ///  Remarks:
    ///  This caption was removed in dtSearch 7.70 by default (a side effect was to make "Document" and "Properties" 
    ///  searchable in all documents).  Use this flag to restore if required for backward compatibility.
    ///  
    dtsoFfIncludeDocumentPropertiesCaption = 0x800000,

    ///  Summary
    ///  Display properties of image files embedded in documents
    ///  Remarks:
    ///  When images are indexed as individual files, their properties are always indexed.
    ///  Set this flag to also index properties of images that are embedded in other documents.
    dtsoFfShowImageProperties = 0x1000000,

    ///  Summary
    ///  Suppress display of email properties (subject, sender, recipient, etc.)
    ///  Remarks:
    ///  This flag suppresses all email properties such as sender, subject, recipient, etc.
    ///  in Outlook (*.msg) and MIME (*.eml) messages.
    ///  To suppress MIME headers, use dtsoFfSkipEmailHeaders, which can be combined with this flag.
    dtsoFfSkipEmailProperties = 0x2000000
};

    ///  Values for dtsFileInfo and dtsIndexProgressInfo.fileFlags (C++) and IndexFileInfo (.NET, Java)
[System::Flags]
public enum class FileInfoFlags {

    ///  The document was successfully indexed.
    fiDocumentOK = 0x0000,

    ///  The document is encrypted.
    fiEncrypted = 0x0001,

    ///  The document contains no text.
    fiEmpty = 0x0002,

    ///  The document is a container.
    fiContainer = 0x0004,

    ///  The document has an unrecognized file format.
    fiBinary = 0x0008,

    ///  The document has a recognized file format but is corrupt.
    fiCorrupt = 0x0010,

    ///  A portion of the document is encrypted.
    fiPartiallyEncrypted = 0x0020,

    ///  A portion of the document is corrupt.
    fiPartiallyCorrupt = 0x0040,

    ///  There was insufficient memory to process the document.
    fiOutOfMemory = 0x0080,

    ///  The document could not be opened due to a file open failure.
    fiOpenFailed = 0x0100,

    ///  The document is in an unrecognized format and is being indexed using the binary filtering algorithm.
    fiFilteredBinary = 0x0200,

    ///  The document contains images without text, other than metadata and annotations 
    fiImageOnly = 0x0400
};

    ///    Values for indexing flags in dtsIndexJob
    ///  
[System::Flags]
public enum class IndexingFlags {

    ///  Reindex a document even if the modification date and size are unchanged.
    dtsAlwaysAdd = 0x0001,

    ///  Summary
    ///  Check for sufficient disk space before indexing.  In the C++ API only, this flag will cause a
    ///  dtsAskDiskFullOverride message to be sent through the error handler callback (dtsErrorHandler.pNotifyFn)
    ///  giving the calling application the option to cancel the update.
    dtsCheckDiskSpace = 0x0002,

    ///  Can be used in indexingFlags instead of the createFlags
    dtsIndexCreateCaseSensitive = 0x0004,

    ///  Create an access-sensitive index.
    dtsIndexCreateAccentSensitive = 0x0008,

    ///  Use relative rather than absolute paths in storing document locations.
    dtsIndexCreateRelativePaths = 0x0010,

    dtsIndexResumeUpdate = 0x0020,

    ///    Summary
    ///     Compress and store the text of documents in the index, for use in generating
    ///     Search Reports and highlighting hits. (This flag must be set when an index is
    ///     created.)  See <link Caching documents>.   
    dtsIndexCacheText = 0x0040,

    ///    Summary 
    ///      Compress and store documents in the index, for use in generating Search Reports
    ///     and highlighting hits. (This flag must be set when an index is created.)        
    ///     See <link Caching documents>.   
    dtsIndexCacheOriginalFile = 0x0080,

    ///  Obsolete -- version 6 indexes are no longer supported
    dtsIndexCreateVersion6 = 0x00100,

    ///    Summary
    ///   When text caching is enabled, do not cache any fields that were provided through the data source API.
    ///   See <link Caching documents>.   
    dtsIndexCacheTextWithoutFields = 0x00200,

    ///  
    ///      Summary
    ///      Preserve existing <link Document Ids, document ids> following a compression of an
    ///      index or a merge of two or more indexes.
    ///      Remarks
    ///      Each document in an index is assigned a unique integer identifier called a
    ///      document id or "DocId". The first document added to an index has the DocId 1, and
    ///      subsequent documents will have sequentially numbered DocIds 2, 3, 4, and so
    ///      forth. When a document is reindexed, its DocId is "cancelled" and a new DocId is
    ///      assigned.
    ///  
    ///      When an index is compressed, all DocIds in the index are renumbered to remove the
    ///      cancelled DocIds unless the dtsIndexKeepExistingDocIds flag is set in IndexJob.
    ///  
    ///      When an index is merged into another index, DocIds in the target index are never
    ///      changed. The documents merged into the target index will all be assigned new,
    ///      sequentially-numbered DocIds, unless (a) the dtsIndexKeepExistingDocIds flag is
    ///      set in IndexJob and (b) the indexes have non-overlapping ranges of doc ids.
    ///  
    ///      If the dtsIndexKeepExistingDocIds is set in a merge and one or more indexes have
    ///      overlapping doc ids, the error dtsErConflictingDocIdRanges will be returned and
    ///      the merge will halt.
    ///  
    ///      When an index is created, you can specify a starting DocId other than 1 using
    ///      IndexJob.StartingDocId. This makes it possible to ensure that indexes have
    ///      non-overlapping ranges of DocIds so DocIds can be preserved after the indexes are
    ///      merged.
    ///  
    ///                                                                                        
    dtsIndexKeepExistingDocIds = 0x00400
};

    ///    Summary
    ///    Describes the current status of the indexer
    ///  
    ///    * API *
    ///    C++: Returned through dtsIndexProgressInfo.step
    ///  
    ///    .NET: Returned through IndexProgressInfo and IndexJob.StatusIndexingStep
    ///  
    ///    Java: Returned through IndexProgressInfo                                 
public enum class IndexingStep {

    ///  Not started
    ixStepNone = 0,

    ///  Start of indexing job
    ixStepBegin = 1,

    ///  An index is being created
    ixStepCreatingIndex = 2,

    ///  Searching for files to be added to the index or checking files to see if they have already been indexed
    ixStepCheckingFiles = 3,

    ///  Parsing files into words and sorting the words
    ixStepReadingFiles = 4,

    ///  Storing word information to be added to the index
    ixStepStoringWords = 5,

    ///  Adding words to the index
    ixStepMerging = 6,

    ///  Compressing the index
    ixStepCompressing = 7,

    ///  Indexing complete
    ixStepDone = 8,

    ///  Checking for index corruption
    ixStepVerifyingIndex = 9,

    ///  Merging two or more indexes into a single index
    ixStepMergingIndexes = 10,

    ///  Checking all files in the index to see if they have been deleted
    ixStepRemovingDeletedFiles = 11,

    ///  Committing index update
    ixStepCommittingChanges = 12
};

    ///  Flags used in dtsListIndexJob (C++) and ListIndexJob (.NET, Java)
[System::Flags]
public enum class ListIndexFlags {

    ///  List words in the index
    dtsListIndexWords = 0x00000001,

    ///  List files in the index
    dtsListIndexFiles = 0x00000002,

    ///  List fields in the index
    dtsListIndexFields = 0x00000004,

    ///  For lists of words, include the number of times each word occurs in the index
    dtsListIndexIncludeCount = 0x00000008,

    ///  Return the list using the outputString
    dtsListIndexReturnString = 0x00000010,

    ///  For lists of words, include the field where each word is found
    dtsListIndexIncludeField = 0x00000100,

    ///  Delimit items in list with tabs instead of spaces
    dtsListTabDelimit = 0x200,

    ///  In a list of files, include the doc id of each document
    dtsListIndexIncludeDocId = 0x400,

    ///  In a list of words, include the number of documents each word occurs in
    dtsListIndexIncludeDocCount = 0x800,

    ///  In a list of documents, include document properties
    dtsListIndexIncludeDocInfoAsXml = 0x1000,

    ///  Skip noise words when listing words in an index
    dtsListIndexSkipNoiseWords = 0x2000,

    dtsListIndexReserved2 = 0x20000000,

    dtsListIndexReserved1 = 0x40000000
};

    ///    Summary
    ///    MessageCodes are sent to callback functions during an index or search to update
    ///    the caller on the status of the job.
    ///    * API *
    ///    * C++ *
    ///    Sent through dtsJobBase.pReportCallBack. See dtsMessage for documentation on
    ///    structures passed to the callback.
    ///    * .NET *
    ///    Sent through IIndexStatusHandler and ISearchStatusHandler
    ///    * Java *
    ///    Sent through IIndexStatusHandler                                                
public enum class MessageCode {

    dtsnFirstStatusMessage = 1000,

    ///    Asks the calling application to give the user a chance to abort (i.e., check if
    ///            the user has pressed a "cancel" button)                        
    dtsnCheckForAbort = 1001,

    ///  Last message sent during execution of an index or search job.  
    dtsnJobClose = 1003,

    dtsnConvertPercentDone = 1004,

    ///  
    ///      * Status notification messages (searching), prefix = dtsnSearch
    ///      
    dtsnFirstSearchStatusMessage = 2000,

    ///  Sent at the beginning of a search 
    dtsnSearchBegin = 2001,

    ///    Sent at the end of a search
    ///          
    dtsnSearchDone = 2002,

    ///  Sent during a search to indicate what is being searched. 
    dtsnSearchWhere = 2003,

    ///  Sent when a file is retrieved in a search.  
    dtsnSearchFound = 2004,

    ///  Sent during a search periodically to update elapsed time display. 
    dtsnSearchUpdateTime = 2005,

    ///  Sent during an unindexed search when the input document is encrypted. 
    dtsnSearchFileEncrypted = 2006,

    ///  Sent during an unindexed search when the input document is unreadable due to corruption.
    dtsnSearchFileCorrupt = 2007,

    dtsnSearchFileDone = 2008,

    dtsnLastSearchStatusMessage = 2999,

    dtsnFirstIndexStatusMessage = 3000,

    ///  Sent at the start of an indexing job. 
    dtsnIndexBegin = 3001,

    ///  Sent at the end of an indexing job. 
    dtsnIndexDone = 3002,

    ///  Sent when an index is being created. 
    dtsnIndexCreate = 3003,

    ///    Sent as dtSearch is checking files to see if they need to be indexed.
    ///                                                                                  
    dtsnIndexCheckingFiles = 3004,

    ///  As dtSearch checks the list of files to be indexed, this message will be sent
    ///  updating the amount of text to be indexed.
    dtsnIndexToAddUpdate = 3005,

    ///  Obsolete
    dtsnIndexAdded = 3006,

    ///  Sent when dtSearch is starting to index a file. 
    dtsnIndexStartingFile = 3007,

    ///    Sent when starting to index a file, periodically as the file is indexed, and when
    ///            the file is done.
    ///                                                                                              
    dtsnIndexFileProgress = 3008,

    ///  Sent when a file has been completely read. 
    dtsnIndexFileDone = 3009,

    ///  Sent when an attempt to open a file fails. 
    dtsnIndexFileOpenFail = 3010,

    ///    Sent when a file is not indexed because (1) it seems to be binary, and (2)
    ///            indexing of binary files is turned off.
    ///                                                                                       
    dtsnIndexFileBinary = 3011,

    ///  Sent to update the status of an index update during the "merging words" step. 
    dtsnIndexMergeProgress = 3012,

    ///  Sent to update compression progress. 
    dtsnIndexCompressProgress = 3013,

    ///  Sent when a file is not indexed because it is encrypted. 
    dtsnIndexFileEncrypted = 3014,

    ///  Sent when words are being written to disk for merging. 
    dtsnIndexStoringWords = 3015,

    ///  Sent when an index update begins
    dtsnIndexStartingUpdate = 3016,

    ///  Indicates that a file was indexed but that part of the file could not be accessed due to encryption. 
    dtsnIndexFilePartiallyEncrypted = 3017,

    ///  Indicates that a file was indexed but that part of the file could not be accessed due to data corruption. 
    dtsnIndexFilePartiallyCorrupt = 3018,

    ///  Sent when an automatic index commit is starting (automatic commits are triggered by autoCommitIntervalMB).
    dtsnAutoCommitBegin = 3019,

    ///  Sent when an automatic index commit is complete (automatic commits are triggered by autoCommitIntervalMB).
    dtsnAutoCommitDone = 3020,

    ///  Sent when a file is removed from the index because it appears to have been deleted
    dtsnIndexDeletedFileRemoved = 3021,

    ///  Sent when a file is removed from the index because it was listed in the IndexJob to be removed
    dtsnIndexListedFileRemoved = 3022,

    ///  Sent when a file is NOT removed from the index, although it was listed in the IndexJob to be removed,
    ///  because it was not found in the index
    dtsnIndexListedFileNotRemoved = 3023,

    ///  Sent when a folder could not be accessed to scan for documents to index
    dtsnIndexFolderInaccessible = 3024,

    dtsnLastIndexStatusMessage = 3999,

    ///  Sent during a merge 
    dtsnIndexMergeJobProgress = 4000,

    ///  Sent during verification of an index. 
    dtsnIndexVerifyProgress = 4001
};

    ///  Flags for dtsSearchReportJob (C++) and SearchReportJob (Java, .NET)
[System::Flags]
public enum class ReportFlags {

    ///  Count context by paragraphs.
    dtsReportByParagraph = 0x0001,

    ///  Count context by words.
    dtsReportByWord = 0x0002,

    ///  Include all items in the search report.
    dtsReportIncludeAll = 0x0004,

    ///  Include full text of each selected file in the report.
    dtsReportWholeFile = 0x0008,

    ///  Count context by words, and include exactly the requested amount of context.
    dtsReportByWordExact = 0x0010,

    ///  Use cached version of document stored in the index, if present.
    dtsReportGetFromCache = 0x0020,

    ///  Store the search report for each item in search results.
    dtsReportStoreInResults = 0x0040,

    ///    Summary
    ///     Limit the size of context blocks when hits are contiguous. Using this flag
    ///     prevents large context blocks from being extracted when many hits occur close
    ///     together. For example, if a report is generated with 5 words of context around
    ///     each hit, a single isolated hit would result in 11 words in a context block.
    ///     However, if there were 100 hits clustered together, a much larger context block
    ///     would be generated.
    ///  
    dtsReportLimitContiguousContext = 0x0080,

    ///  Include a block of text from the top of the file in the output.
    dtsReportIncludeFileStart = 0x0100
};

    ///  Flags to control searches 
[System::Flags]
public enum class SearchFlags {

    ///  Enable fuzzy searching for all words in the search request. 
    dtsSearchFuzzy = 0x0001,

    ///  Enable phonic searching for all words in the search request
    dtsSearchPhonic = 0x0002,

    ///  Enable stemming for all words in the search request
    dtsSearchStemming = 0x0004,

    ///  Equivalent to dtsSearchTypeAnyWords | dtsSearchAutoTermWeight | dtsSearchPositionalScoring.
    dtsSearchNatural = 0x0008,

    ///    Summary
    ///    Enable synonym expansion. See <link Synonym search settings>.
    ///    Use one or more of the flags dtsSearchUserSynonyms, dtsSearchWordNetSynonyms, or dtsSearchWordNetRelated
    ///    to specify the type of synonym expansion to perform.
    ///     
    dtsSearchSynonyms = 0x0010,

    ///    Summary
    ///      Enable synonym expansion using user-defined synonyms. See <link Synonym search settings>. 
    ///      This flag must be used in combination with dtsSearchSynonyms to enable synonym searching.
    ///      
    dtsSearchUserSynonyms = 0x0020,

    ///    Summary 
    ///    Enable synonym expansion using WordNet related words. See <link Synonym search settings>. 
    ///    This flag must be used in combination with dtsSearchSynonyms to enable synonym searching.
    ///  
    ///    
    dtsSearchWordNetSynonyms = 0x0040,

    ///    Summary
    ///   	 Enable synonym expansion using WordNet synonyms. See <link Synonym search settings>. 
    ///      This flag must be used in combination with dtsSearchSynonyms to enable synonym searching.
    ///  
    ///   
    dtsSearchWordNetRelated = 0x0080,

    ///  Combines all synonym flags
    dtsSearchAllSynonyms = 0x00f0,

    ///    Summary 
    ///     Request a list of the words that were matched in each document. See <link dtsSearchWantHitsByWord>
    ///     in the Overviews section.    
    ///     This flag must be used in combination with dtsSearchSynonyms to enable synonym searching.
    ///                                                                           
    dtsSearchWantHitsByWord = 0x0100,

    ///  In dtsWordListBuilder, include field names in the output. 
    dtsWlbListFields = 0x0100,

    dtsSearchReserved = 0x0200,

    ///  Deprecated. Use dtsSearchWantHitDetails instead. 
    dtsSearchWantHitsByWord2 = 0x0400,

    ///    Summary 
    ///   	 Adds word offsets of individual hits to the hits by word report. See <link dtsSearchWantHitsByWord>
    ///      in the Overviews section.                                                                           
    dtsSearchWantHitsArray = 0x0800,

    ///    Summary 
    ///      Optimize search by waiting until document records are requested through a search
    ///      results object before reading them from the index. See <link dtsSearchDelayDocInfo>
    ///      in the Overviews section.                                                           
    dtsSearchDelayDocInfo = 0x1000,

    ///    Summary 
    ///     Enable synonym expansion using an external thesaurus. See <link Synonym search settings>. 
    dtsSearchExternalSynonyms = 0x2000,

    dtsSearchRegularExpression = 0x4000,

    ///    Summary
    ///     Request detailed information about the words that generated each hit associated
    ///      with an item in search results. See <link dtsSearchWantHitDetails> in the
    ///      Overviews section.                                                              
    dtsSearchWantHitDetails = 0x8000,

    ///    Apply the automatic term weighting to each term in the request. See <link Relevance>
    ///      in the Overviews section.                                                            
    dtsSearchAutoTermWeight = 0x10000,

    ///    Summary
    ///      Find all of the words in the search request. See <link AllWords and AnyWords, "All words" and "Any words" Searches>
    ///      in the Overviews section.                                                                                           
    dtsSearchTypeAllWords = 0x20000,

    ///    Summary
    ///      Find any of the words in the search request. 
    ///      See <link AllWords and AnyWords, "All words" and "Any words" Searches>
    ///      in the Overviews section.                                                                                           
    dtsSearchTypeAnyWords = 0x40000,

    ///    Summary
    ///      When selecting the documents to return (up to the MaxFilesToRetrieve 
    ///      limit in the SearchJob), select the most recent documents rather than 
    ///      the best-matching documents. This flag cannot be used with dtsSearchDelayDocInfo, 
    ///      because the document date for each document is needed.
    ///      
    dtsSearchSelectMostRecent = 0x080000,

    ///    Summary 
    ///     Rank documents higher when hits are closer to the top of the document and when
    ///      hits are located close to each other within a document. This improves relevancy
    ///      ranking for "all words" and "any words" searches. See <link Relevance> in the
    ///      Overviews section.                                                              
    dtsSearchPositionalScoring = 0x100000,

    ///    Summary
    ///      Apply Language Analyzer to search request, if this application uses an external
    ///      language analyzer (see <link Language Analyzer API>)                            
    dtsSearchApplyLanguageAnalyzer = 0x200000,

    ///    Summary
    ///      Use Language Analyzer to generate synonyms from terms in search request,
    ///      if this application uses an external
    ///      language analyzer (see <link Language Analyzer API>)                            
    dtsSearchLanguageAnalyzerSynonyms = 0x400000,

    ///    Summary
    ///      When SearchJob.WantResultsAsFilter is true, only output search results 
    ///      information to the output search filter and do not create a SearchResults 
    ///      object or call the OnFound callback function when documents are found.
    ///      This can make generation of a SearchFilter significantly faster.
    ///      
    dtsSearchFastSearchFilterOnly = 0x800000
};

    ///    Summary
    ///    Flags to control the sorting of search results. 
[System::Flags]
public enum class SortFlags {

    dtsSortUndefined = 0,

    dtsSortNone = 1,

    ///  Sort in ascending order.
    dtsSortAscending = 2,

    ///  Sort in descending order.
    dtsSortDescending = 0,

    ///  Sort by filename.
    dtsSortByName = 4,

    ///  Sort by modified date.
    dtsSortByDate = 8,

    ///  Summary
    ///  Sort by hit count or relevance, depending on search type.  
    ///  Deprecated: Use dtsSortByHits or dtsSortByRelevanceScore instead. 
    dtsSortByHits = 0x0010,

    ///  Sort by file size.
    dtsSortBySize = 0x0020,

    ///  Sort by user-defined field.
    dtsSortByField = 0x0040,

    ///  Sort by user-defined field (deprecated, use dtsSortByField).
    dtsSortByUserField = 0x0040,

    ///  Sort by index retrieved from.
    dtsSortByIndex = 0x0080,

    ///  Sort by file type.
    dtsSortByType = 0x0100,

    ///  Sort by file title.
    dtsSortByTitle = 0x0200,

    ///  Sort by the path of the file.
    dtsSortByLocation = 0x0400,

    ///  Sort by modification time, ignoring the date.
    dtsSortByTime = 0x0800,

    ///  Sort by caller-specified sort key (use setSortKey for each item to specify the key).
    dtsSortBySortKey = 0x1000,

    ///  Make string comparisons in the sort case-insensitive.	
    dtsSortCaseInsensitive = 0x010000,

    ///  Summary
    ///  Interpret sort values as numbers instead of text.  
    ///  This would cause "20" to be considered greater than "9". The sort key will be a signed, 32-bit integer.
    dtsSortNumeric = 0x020000,

    ///  When sorting by filename, use the PDF Title as the filename for PDF files.
    dtsSortPdfUseTitleAsName = 0x040000,

    ///  When sorting by filename, use the HTML Title as the filename for HTML files.
    dtsSortHtmlUseTitleAsName = 0x080000,

    ///  Sort by the floating point numeric value of a field instead of its string value.
    dtsSortFloatNumeric = 0x100000,

    ///  Summary
    ///  Remove some leading punctuation or white space from sort value before sorting. Also removes "re:", "fw:", and "fwd:".
    dtsSortCleanText = 0x200000,

    ///  Sort by number of hits.
    dtsSortByHitCount = 0x400000,

    ///  Sort by relevance score.
    dtsSortByRelevanceScore = 0x800000,

    ///  Sort by path and filename.
    dtsSortByFullName = 0x1000000,

    ///  In WordListBuilder only, sort by word.
    dtsSortByWord = 0x2000000,

    ///  In WordListBuilder only, sort by document count. 
    dtsSortByDocCount = 0x4000000
};

    ///  Values for Options.TextFlags (.NET), Options.setTextFlags() (Java), and dtsOptions.textFlags (C++)
[System::Flags]
public enum class TextFlags {

    ///    Summary
    /// 	   By default, dtSearch indexes numbers both as text and as numeric values, which is necessary
    /// 	   for numeric range searching. Use this flag to suppress indexing of numeric values in applications
    /// 	   that do not require numeric range searching. This setting can reduce the size of the index by about 20%.
    ///      
    dtsoTfSkipNumericValues = 0x0001,

    ///    Summary
    /// 	   Suppress automatic generation of xfirstword and xlastword.
    /// 	   By default, xfirstword is defined to be the first word in each document, and xlastword
    /// 	   is defined to be the last word in each document.  These words are generated when an index
    /// 	   is created, so this flag must be set during indexing to suppress xlastword and xfirstword.
    /// 	 
    dtsoTfSkipXFirstAndLast = 0x0002,

    ///  Summary
    ///  Automatically recognize dates, email addresses, and credit card numbers in text as it is indexed.
    ///  See <link Recognition of Dates, Email Addresses, and Credit Card Numbers>.
    dtsoTfRecognizeDates = 0x0004,

    ///  Summary
    ///  Presume DD/MM/YY format for dates (default is MM/DD/YY).
    ///  See <link Recognition of Dates, Email Addresses, and Credit Card Numbers>.
    dtsoTfRecognizeDatesPresumeDMY = 0x0008,

    ///  Summary
    ///  Presume YY/MM/DD format for dates (default is MM/DD/YY).
    ///  See <link Recognition of Dates, Email Addresses, and Credit Card Numbers>.
    dtsoTfRecognizeDatesPresumeYMD = 0x0010,

    ///    Summary
    /// 	   Automatically insert a word break around characters in the Chinese, Japanese, and Korean Unicode ranges.
    /// 	   This makes it possible to search text in documents that do not contain word breaks.
    /// 	   Like the hyphenation setting, this setting is kept in the alphabet for an index and so will only
    /// 	   change when an index is created.
    /// 	   See  <link Alphabet Settings>.
    /// 	
    dtsoTfAutoBreakCJK = 0x0020
};

    ///    Summary
    ///    \File type identifiers
    ///    Remarks
    ///    Because some older file parsers are still supported for backward compatibility,
    ///    in a few cases there may be more than one TypeId for a file format.             
    ///    Not all file formats listed are supported for content extraction or indexing.
    ///    For a current list of supported file types, see: http://support.dtsearch.com/faq/dts0103.htm
    ///    
public enum class TypeId {

    it_None = 0,

    ///  List of words in UTF-8 format, with the word ordinal in front of each word
    it_WordList = 104,

    ///  Output format for FileConverter that organizes document content, metadata, and attachments into a standard XML format
    it_ContentAsXml = 108,

    it_WordListNoOrdinal = 109,

    ///  Ami Pro
    it_Ami = 201,

    ///  Ansi text file
    it_Ansi = 202,

    ///  Ascii (DOS) text file
    it_Ascii = 203,

    ///  Binary file (unrecognized format)
    it_Binary = 204,

    ///  OLE Compound Document (or "DocFile")
    it_CompoundDoc = 205,

    ///  XBase database file
    it_DBF = 206,

    ///  Filtered binary file
    it_FilteredBinary = 207,

    ///  Obsolete
    it_HyperText = 208,

    ///  Obsolete
    it_MS_Word = 209,

    ///  Microsoft Works word processor
    it_MS_Works = 210,

    ///  Multimate (any version)
    it_Multimate = 211,

    ///  Microsoft Rich Text Format
    it_RTF = 212,

    ///  Wordstar 2000
    it_WS_2000 = 213,

    ///  WordStar version 5 or 6
    it_WS_5 = 214,

    ///  Windows Write
    it_WinWrite = 215,

    ///  Word for DOS (same as Windows Write, it_WinWrite)
    it_WordForDos = 215,

    ///  Obsolete
    it_WordForWin = 216,

    ///  Microsoft Word 6.0
    it_WordForWin6 = 217,

    ///  WordPerfect 4.2
    it_WordPerfect42 = 218,

    ///  WordPerfect 5
    it_WordPerfect5 = 219,

    ///  WordPerfect 6
    it_WordPerfect6 = 220,

    ///  WordStar through version 4
    it_WordStar = 221,

    ///  XyWrite
    it_XyWrite = 222,

    ///  ZIP archive
    it_ZIP = 223,

    ///  PropertySet stream in a Compound Document
    it_Properties = 224,

    ///  Excel versions 5 and 7
    it_Excel5 = 225,

    ///  HTML
    it_HTML = 226,

    ///  PDF
    it_PDF = 227,

    ///  Word For Windows 97, 2000, XP, or 2003
    it_WordForWin97 = 228,

    ///  Excel 97, 2000, XP, or 2003
    it_Excel97 = 229,

    ///  PowerPoint 97-2003
    it_PowerPoint = 230,

    ///  Obsolete
    it_EncryptedHtml = 231,

    ///  Record in a database file (such as XBase or Access)
    it_DatabaseRecord = 232,

    ///  Text segmented using File Segmentation Rules
    it_SegmentedText = 233,

    ///  XML
    it_XML = 234,

    ///  WordPerfect document embedded in another file
    it_WordPerfectEmbedded = 235,

    ///  UCS-16 text
    it_Unicode = 236,

    ///  Message in a Eudora message store
    it_EudoraMessage = 237,

    ///  UTF-8 text
    it_Utf8 = 238,

    it_DjVu = 239,

    ///  Message in an Outlook Express message store
    it_OutlookExpressMessage = 240,

    ///  dtSearch 6.40 and earlier file parser for .eml files
    it_MimeMessage = 241,

    ///  Single-byte text, encoding automatically detected
    it_SingleByteText = 242,

    ///  Email archive conforming to the MBOX standard (dtSearch versions 7.50 and earlier)
    it_MBoxArchive = 243,

    ///  Binary file filtered using Unicode Filtering
    it_FilteredBinaryUnicode = 244,

    ///  ZIP file parsed using zlib
    it_ZIP_zlib = 245,

    ///  Microsoft Outlook .MSG file
    it_OutlookMsgFile = 246,

    ///  TreePad file (HJT format in TreePad 6 and earlier)
    it_TreepadHjtFile = 247,

    ///  PFS Professional Write file
    it_PfsProfessionalWrite = 248,

    ///  Binary file filtered using Unicode Filtering, not split into segments
    it_FilteredBinaryUnicodeStream = 249,

    ///  Compound document (new parser)
    it_DocFile = 250,

    ///  Music or video file
    it_Media = 251,

    ///  Data file with no text to index
    it_NonTextData = 252,

    ///  Mime stream handled as a single document
    it_EML = 253,

    ///  Microsoft Word 95 - 2003 (dtSearch versions 6.5 and later)
    it_MicrosoftWord = 254,

    it_Utf8F = 255,

    ///  Output format only, for generating a synopsis that is HTML-encoded but that does not include formatting such as font settings, paragraph breaks, etc.
    it_UnformattedHTML = 256,

    ///  Database record (rendered as HTML)
    it_DatabaseRecord2 = 257,

    ///  Microsoft Excel 2003 XML format
    it_Excel2003Xml = 258,

    ///  Microsoft Word 2003 XML format
    it_Word2003Xml = 259,

    ///  OpenOffice versions 1, 2, and 3 documents, spreadsheets, and presentations (*.sxc, *.sxd, *.sxi, *.sxw, *.sxg, *.stc, *.sti, *.stw, *.stm, *.odt, *.ott, *.odg, *.otg, *.odp, *.otp, *.ods, *.ots, *.odf) (includes OASIS Open Document Format for Office Applications)
    it_OpenOfficeDocument = 260,

    ///  Comma-separated values file
    it_CSV = 261,

    ///  Microsoft Access database
    it_MicrosoftAccess = 262,

    ///  XBase database
    it_XBase = 263,

    ///  Outlook Express dbx archive (versions 7.67 and earlier)
    it_OutlookExpressMessageStore = 264,

    ///  File type processed using installed IFilter
    it_IFilter = 265,

    ///  Archive compressed with gzip
    it_Gzip = 266,

    ///  Excel version 4 
    it_Excel4 = 267,

    ///  Word 2007
    it_Word12 = 268,

    ///  Excel 2007
    it_Excel12 = 269,

    ///  PowerPoint 2007
    it_PowerPoint12 = 270,

    ///  Transport-neutral encapsulation format
    it_TNEF = 271,

    ///  XML Paper Specification (Metro)
    it_XPS = 272,

    ///  Windows Metafile Format (Win16)
    it_WMF = 273,

    ///  Windows Metafile Format (Win32)
    it_EMF = 274,

    ///  TAR archive
    it_TAR = 275,

    ///  Excel Version 2
    it_Excel2 = 277,

    ///  Excel version 3 
    it_Excel3 = 278,

    ///  Email archive conforming to the MBOX standard (dtSearch versions 7.51 and later)
    it_MBoxArchive2 = 279,

    ///  Lotus 123 spreadsheet
    it_Lotus123 = 280,

    ///  Microsoft Works WPS versions 4 and 5
    it_MsWorksWps4 = 281,

    ///  Microsoft Works WPS versions 6, 7, 8, and 9
    it_MsWorksWps6 = 282,

    ///  Quattro Pro 9 and newer
    it_QuattroPro = 283,

    ///  Quattro Pro 8 and older
    it_QuattroPro8 = 284,

    ///  Outlook .MSG file processed as a container
    it_OutlookMsgAsContainer = 285,

    ///  MIME-encoded message, processed as a container
    it_MimeContainer = 286,

    ///  PDF file with attachments
    it_PdfWithAttachments = 287,

    ///  Microsoft Access (parsed directly, not via ODBC or the Jet Engine)
    it_MicrosoftAccess2 = 288,

    ///  Flash SWF
    it_FlashSWF = 289,

    ///  RAR archive
    it_RAR = 290,

    ///  File indexed with all content ignored (see dtsoIndexBinaryNoContent)
    it_NoContent = 291,

    ///  Excel 2007 XLSB format
    it_Excel12xlsb = 292,

    ///  XFA form 
    it_XfaForm = 293,

    ///  Framemaker MIF file
    it_MifFile = 294,

    ///  Word for Windows 2
    it_WordForWindows2 = 295,

    ///  ASF file	
    it_ASF = 296,

    ///  MP3 file
    it_MP3 = 297,

    ///  QuickTime file
    it_QuickTime = 298,

    ///  MP4 file
    it_MP4 = 299,

    ///  M4A file
    it_M4A = 300,

    ///  MPEG file
    it_MPG = 301,

    ///  JPEG file
    it_JPEG = 302,

    ///  TIFF file
    it_TIFF = 303,

    ///  MDI image file
    it_MDI = 304,

    ///  GIF image file
    it_GIF = 305,

    ///  PNG image file
    it_PNG = 306,

    ///  CAB archive
    it_CAB = 308,

    ///  dtSearch index file
    it_dtSearchIndex = 309,

    ///  HTML Help CHM file
    it_HtmlHelp = 310,

    ///  TrueType TTF file
    it_TrueTypeFont = 311,

    ///  WAV sound file
    it_WAV = 312,

    ///  BMP image file
    it_BMP = 313,

    ///  AVI file
    it_AVI = 314,

    ///  Visio file
    it_Visio = 315,

    ///  Ichitaro word processor file (versions 8 through 2011)
    it_Ichitaro = 316,

    ///  SolidWorks file
    it_SolidWorks = 317,

    ///  Unigraphics file (docfile format)
    it_Unigraphics = 318,

    ///  DWG CAD file
    it_DWG = 319,

    ///  DWF CAD file
    it_DWF = 320,

    ///  DXF CAD file
    it_DXF = 321,

    ///  Visio XML file
    it_VisioXml = 322,

    ///  Word for Windows 1
    it_WordForWindows1 = 323,

    ///  Outlook PST message store
    it_OutlookPst = 324,

    ///  Microsoft Office .thmx file with theme data
    it_MicrosoftOfficeThemeData = 325,

    ///  MIDI file
    it_MIDI = 326,

    ///  Outlook Express dbx archive 
    it_OutlookExpressMessageStore2 = 327,

    ///  PowerPoint 4
    it_PowerPoint4 = 328,

    ///  PowerPoint 95
    it_PowerPoint95 = 329,

    ///  PowerPoint 3
    it_PowerPoint3 = 330,

    ///  Ichitaro versions 5, 6, 7
    it_Ichitaro5 = 331,

    ///  oledata.mso file
    it_OleDataMso = 332,

    ///  CSV file parsed as report (like a spreadsheet) instead of a database
    it_CsvAsReport = 333,

    ///  CSV file parsed as a single file listing all records
    it_CsvAsDocument = 334,

    ///  Access database parsed as a single file listing all records
    it_MicrosoftAccessAsDocument = 335,

    ///  XBase file parsed as a single file listing all records
    it_XBaseAsDocument = 336,

    ///  not supported 
    it_OneNote2003 = 337,

    ///  OneNote 2007
    it_OneNote2007 = 338,

    ///  OneNote 2010, 2013, and 2016
    it_OneNote2010 = 339,

    ///  Windows Media Photo/HDPhoto/*.wdp
    it_JpegXR = 340,

    ///  Windows .exe or .dll
    it_WindowsExecutable = 341,

    ///  ELF format executable
    it_ElfExecutable = 342,

    ///  ICalendar (*.ics) file
    it_ICalendar = 343,

    ///  Microsoft Publisher file
    it_MicrosoftPublisher = 344,

    it_MicrosoftPhotoEditor = 345,

    ///  Photoshop Image (*.psd)
    it_PhotoshopImage = 346,

    ///  CALS metadata format described in MIL-STD-1840C
    it_Cals = 347,

    ///  IWork 2009
    it_iWork2009 = 348,

    ///  IWork 2009 Pages document
    it_iWork2009Pages = 349,

    ///  IWork 2009 Numbers spreadsheet
    it_iWork2009Numbers = 350,

    ///  IWork 2009 Keynote presentation
    it_iWork2009Keynote = 351,

    ///  Visio 2013 document
    it_Visio2013 = 352,

    ///  SASF call center audio file
    it_SASF = 353,

    ///  Unigraphics file (#UGC format)
    it_Unigraphics2 = 354,

    ///  OneNote variant generated by Microsoft online services
    it_OneNoteOnline = 355
};

    ///  Values for WordListBuilder.SetFlags
[System::Flags]
public enum class WordListBuilderFlags {

    ///  
    /// 	Enable caching of field values in a WordListBuilder for faster ListFieldValues calls 
    /// 	
    dtsWordListEnableFieldValuesCache = 1
};

    ///  
    /// 	Summary
    ///     Values for Options.UnicodeFilterFlags (.NET) or dtsOptions.unicodeFilterFlags (C++) or Options.setUnicodeFilterFlags(java).
    ///  
    ///     Remarks
    ///     UnicodeFilterFlags control the behavior of the Unicode Filtering algorithm when it is used to filter
    ///     text from binary data.  See <link Filtering Options>.
    ///  
[System::Flags]
public enum class UnicodeFilterFlags {

    ///  Extracting blocks as HTML has no effect on the text that is extracted, but it adds additional information in HTML comments to each extracted block. The HTML comments identify the starting byte offset and encoding of each piece of text extracted from a file.
    dtsoUfExtractAsHtml = 0x0001,

    ///    Summary
    ///      Overlapping blocks prevents text that crosses a block boundary from being missed 
    ///      in the filtering process. With overlapping enabled, each block extends 256 
    ///      characters past the start of the previous block.
    ///   
    dtsoUfOverlapBlocks = 0x0002,

    ///    Summary
    ///      Automatically insert a word break in long sequences of letters. 
    ///      A word break will be inserted when the word length reaches Options.MaxWordLength.
    ///    
    dtsoUfAutoWordBreakByLength = 0x0004,

    ///    Summary
    ///      Automatically insert a word break when a capital letter appears following lower-case letters.
    ///      Example:  WordBreak would be indexed as Word Break.
    ///    
    dtsoUfAutoWordBreakByCase = 0x0008,

    ///  Automatically insert a word break when a digit follows letters.
    dtsoUfAutoWordBreakOnDigit = 0x0010,

    ///    When a word break is automatically inserted due to dtsoUfAutoWordBreakByLength, 
    ///      overlap the two words generated by the word break.  
    dtsoUfAutoWordBreakOverlapWords = 0x0020,

    ///    When a document cannot be indexed due to file corruption or encryption, 
    ///      apply the filtering algorithm to extract text from the file. 
    dtsoUfFilterFailedDocs = 0x0040,

    ///  Ignore file format information and apply Unicode Filtering to all documents.
    dtsoUfFilterAllDocs = 0x0080
};

    ///  Values for dtsSearchJob.unindexedSearchFlags
[System::Flags]
public enum class UnindexedSearchFlags {

    ///  Case-sensitive search
    dtsUSearchCaseSensitive = 0x0001,

    ///  Do not search binary files
    dtsUSearchSkipBinaryFiles = 0x0002,

    ///  Accent-sensitive search
    dtsUSearchAccentSensitive = 0x0004
};

	}
}


