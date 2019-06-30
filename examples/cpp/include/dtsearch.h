/*

dtSearch C++ API

Copyright 1995-2016 dtSearch Corp.  All Rights Reserved

*/

#ifndef DTSEARCH_H
    #define DTSEARCH_H

// Structure alignment:
// Win32: Single-byte
// Win64: 8 byte
#if defined(__BORLANDC__)
    #pragma option -a1
#elif defined(UNDER_CE)
    #pragma pack(push, dtSearchHeader)
    #pragma pack(4)
#elif defined(_MSC_VER)
    #pragma pack(push, dtSearchHeader)
	#if defined(_WIN64)
		#pragma pack(8)
    #elif defined(WIN32)
	    #pragma pack(1)
    #endif
#endif

#if defined(_MSC_VER)
    #include <stdlib.h>
#endif

#define DTPROC

#if !defined(WIN32)
    #define __int64 long long int
#endif
#ifdef WIN32
#if (_MSC_VER < 1300)
	#define intptr_t  int
#endif
#endif

// begin-internal
typedef void *dtsJobOptionsHandle;
// end-internal

/*
	Summary:
	Handle to a search results object

	Remarks:
	Use the dtsSearchResults class to access search results through a dtsSearchResultsHandle
*/
typedef void *dtsSearchResultsHandle;

/* Summary:
   Handle to a string

   Remarks:
   dtsStringHandles are used to return variable-length
   string values from certain dtSearch Engine API functions.  To obtain the text
   of the string,
   1. Use dtssStringGetLength to get the length of the string,
   2. Allocate a buffer large enough for the string, plus a terminating null,
   3. Call dtssStringGetText to obtain the text of the string, and
   4. Call dtssStringDelete to free the memory that was allocated for the string.
 */
typedef void *dtsStringHandle;

/* Summary:
   Handle to an array of 32-bit integers

   Remarks:
   dtsIntArrayHandles are used to return variable-length
   integer arrays from certain dtSearch Engine API functions.  To obtain the contents
   of the array:
   1. Use dtssIntArrayGetLength to get the length of the array,
   2. Allocate a buffer large enough for the array,
   3. Call dtssIntArrayGetItems to obtain the contents of the array, and
   4. Call dtssIntArrayDelete to free the memory that was allocated for the string.
 */
typedef void *dtsIntArrayHandle;

/*
	Summary:
	Handle to a search filter

	Remarks:
	Use the dtsSearchFilter class to access search filters through a dtsSearchFilterHandle.
*/
typedef void *dtsSearchFilterHandle;

/*
	Summary:
	Handle to an index cache

	Remarks:
	Use the dtsIndexCache (dtsIndexCache.h) class to create an index cache, and use the indexCacheHandle
	member of dtsSearchJob to associate it with a search job
*/
typedef void *dtsIndexCacheHandle;

/* Summary
   Flags to control the sorting of search results. */
enum SortFlags {
 dtsSortUndefined   =0,
 dtsSortNone        =1,
 // Sort in ascending order.
 dtsSortAscending   =2,					
 // Sort in descending order.
 dtsSortDescending  =0,					
 // Sort by filename.
 dtsSortByName      =4,					
 // Sort by modified date.
 dtsSortByDate      =8,					
// Summary
// Sort by hit count or relevance, depending on search type.  
// Deprecated: Use dtsSortByHits or dtsSortByRelevanceScore instead. 
 dtsSortByHits      =0x0010,				
 // Sort by file size.
 dtsSortBySize      =0x0020,				
 // Sort by user-defined field.
 dtsSortByField     =0x0040,				
 // Sort by user-defined field (deprecated, use dtsSortByField).
 dtsSortByUserField =0x0040,				
 // Sort by index retrieved from.
 dtsSortByIndex     =0x0080,				
 // Sort by file type.
 dtsSortByType      =0x0100,				
 // Sort by file title.
 dtsSortByTitle     =0x0200,				
 // Sort by the path of the file.
 dtsSortByLocation  =0x0400,				
 // Sort by modification time, ignoring the date.
 dtsSortByTime      =0x0800,				
 // Sort by caller-specified sort key (use setSortKey for each item to specify the key).
 dtsSortBySortKey   =0x1000,			
 // Make string comparisons in the sort case-insensitive.	
 dtsSortCaseInsensitive     =0x010000,	
 // Summary
 // Interpret sort values as numbers instead of text.  
 // This would cause "20" to be considered greater than "9". The sort key will be a signed, 32-bit integer.
 dtsSortNumeric             =0x020000,	
 // When sorting by filename, use the PDF Title as the filename for PDF files.
 dtsSortPdfUseTitleAsName   =0x040000,	
 // When sorting by filename, use the HTML Title as the filename for HTML files.
 dtsSortHtmlUseTitleAsName  =0x080000,	
 // Sort by the floating point numeric value of a field instead of its string value.
 dtsSortFloatNumeric        =0x100000,	
 // Summary
 // Remove some leading punctuation or white space from sort value before sorting. Also removes "re:", "fw:", and "fwd:".
 dtsSortCleanText           =0x200000,	
 // Sort by number of hits.
 dtsSortByHitCount			=0x400000, 
 // Sort by relevance score.
 dtsSortByRelevanceScore		=0x800000, 
 // Sort by path and filename.
 dtsSortByFullName             =0x1000000, 
 // In WordListBuilder only, sort by word.
 dtsSortByWord				   =0x2000000,
 // In WordListBuilder only, sort by document count. 
 dtsSortByDocCount			   =0x4000000 
	};

// begin-internal
#define dtsSearchAnyWords dtsSearchTypeAnyWords
#define dtsSearchAllWords dtsSearchTypeAllWords
// end-internal

/* Flags to control searches */
enum SearchFlags {
	/* Enable fuzzy searching for all words in the search request. */
  dtsSearchFuzzy             =0x0001,
  // Enable phonic searching for all words in the search request
  dtsSearchPhonic            =0x0002,
  // Enable stemming for all words in the search request
  dtsSearchStemming          =0x0004,
  // Equivalent to dtsSearchTypeAnyWords | dtsSearchAutoTermWeight | dtsSearchPositionalScoring.
  dtsSearchNatural           =0x0008,
  /* Summary
   Enable synonym expansion. See <link Synonym search settings>.
   Use one or more of the flags dtsSearchUserSynonyms, dtsSearchWordNetSynonyms, or dtsSearchWordNetRelated
   to specify the type of synonym expansion to perform.
    */
  dtsSearchSynonyms          =0x0010 ,
  /* Summary
     Enable synonym expansion using user-defined synonyms. See <link Synonym search settings>. 
     This flag must be used in combination with dtsSearchSynonyms to enable synonym searching.
     */
  dtsSearchUserSynonyms      =0x0020 ,
  /* Summary 
   Enable synonym expansion using WordNet related words. See <link Synonym search settings>. 
   This flag must be used in combination with dtsSearchSynonyms to enable synonym searching.

   */
  dtsSearchWordNetSynonyms   =0x0040 ,
  /* Summary
  	 Enable synonym expansion using WordNet synonyms. See <link Synonym search settings>. 
     This flag must be used in combination with dtsSearchSynonyms to enable synonym searching.

  */
  dtsSearchWordNetRelated    =0x0080 ,
  // Combines all synonym flags
  dtsSearchAllSynonyms       =0x00f0,
  /* Summary 
    Request a list of the words that were matched in each document. See <link dtsSearchWantHitsByWord>
    in the Overviews section.    
    This flag must be used in combination with dtsSearchSynonyms to enable synonym searching.
                                                                          */
  dtsSearchWantHitsByWord    =0x0100,
  /* In dtsWordListBuilder, include field names in the output. */
  dtsWlbListFields           =0x0100,
  dtsSearchReserved          =0x0200 ,
  /* Deprecated. Use dtsSearchWantHitDetails instead. */
  dtsSearchWantHitsByWord2   =0x0400 ,
  /* Summary 
  	 Adds word offsets of individual hits to the hits by word report. See <link dtsSearchWantHitsByWord>
     in the Overviews section.                                                                           */
  dtsSearchWantHitsArray     =0x0800 ,

  /* Summary 
     Optimize search by waiting until document records are requested through a search
     results object before reading them from the index. See <link dtsSearchDelayDocInfo>
     in the Overviews section.                                                           */
  dtsSearchDelayDocInfo      =0x1000,

  /* Summary 
    Enable synonym expansion using an external thesaurus. See <link Synonym search settings>. */
  dtsSearchExternalSynonyms  =0x2000,

  dtsSearchRegularExpression =0x4000,

  /* Summary
    Request detailed information about the words that generated each hit associated
     with an item in search results. See <link dtsSearchWantHitDetails> in the
     Overviews section.                                                              */
  dtsSearchWantHitDetails     =0x8000,

  /* Apply the automatic term weighting to each term in the request. See <link Relevance>
     in the Overviews section.                                                            */
  dtsSearchAutoTermWeight    =0x10000,

  /* Summary
     Find all of the words in the search request. See <link AllWords and AnyWords, "All words" and "Any words" Searches>
     in the Overviews section.                                                                                           */
  dtsSearchTypeAllWords       	=0x20000,

  /* Summary
     Find any of the words in the search request. 
     See <link AllWords and AnyWords, "All words" and "Any words" Searches>
     in the Overviews section.                                                                                           */
  dtsSearchTypeAnyWords       	=0x40000,

  /* Summary
     When selecting the documents to return (up to the MaxFilesToRetrieve 
     limit in the SearchJob), select the most recent documents rather than 
     the best-matching documents. This flag cannot be used with dtsSearchDelayDocInfo, 
     because the document date for each document is needed.
     */
  dtsSearchSelectMostRecent  =0x080000,

  /* Summary 
    Rank documents higher when hits are closer to the top of the document and when
     hits are located close to each other within a document. This improves relevancy
     ranking for "all words" and "any words" searches. See <link Relevance> in the
     Overviews section.                                                              */
  dtsSearchPositionalScoring	=0x100000,

  /* Summary
     Apply Language Analyzer to search request, if this application uses an external
     language analyzer (see <link Language Analyzer API>)                            */
  dtsSearchApplyLanguageAnalyzer	=0x200000,

 /* Summary
     Use Language Analyzer to generate synonyms from terms in search request,
     if this application uses an external
     language analyzer (see <link Language Analyzer API>)                            */
  dtsSearchLanguageAnalyzerSynonyms	=0x400000,
  /* Summary
     When SearchJob.WantResultsAsFilter is true, only output search results 
     information to the output search filter and do not create a SearchResults 
     object or call the OnFound callback function when documents are found.
     This can make generation of a SearchFilter significantly faster.
     */
  dtsSearchFastSearchFilterOnly = 0x800000
};

/* Values for indexing flags in dtsIndexJob
*/
enum IndexingFlags {
 // Reindex a document even if the modification date and size are unchanged.
 dtsAlwaysAdd      				=0x0001,
 // Summary
 // Check for sufficient disk space before indexing.  In the C++ API only, this flag will cause a
 // dtsAskDiskFullOverride message to be sent through the error handler callback (dtsErrorHandler.pNotifyFn)
 // giving the calling application the option to cancel the update.
 dtsCheckDiskSpace 				=0x0002,
 // Can be used in indexingFlags instead of the createFlags
 dtsIndexCreateCaseSensitive    =0x0004,
 // Create an access-sensitive index.
 dtsIndexCreateAccentSensitive	=0x0008,
 // Use relative rather than absolute paths in storing document locations.
 dtsIndexCreateRelativePaths		=0x0010,
 /* Summary
    Resume an earlier index update that did not complete. (Version 7 indexes only.) 
    */

 dtsIndexResumeUpdate			=0x0020,
 /* Summary
    Compress and store the text of documents in the index, for use in generating
    Search Reports and highlighting hits. (This flag must be set when an index is
    created.)  See <link Caching documents>.   */
 dtsIndexCacheText				 = 0x0040,
 /* Summary 
     Compress and store documents in the index, for use in generating Search Reports
    and highlighting hits. (This flag must be set when an index is created.)        
    See <link Caching documents>.   */
 dtsIndexCacheOriginalFile		 = 0x0080,
 // Obsolete -- version 6 indexes are no longer supported
 dtsIndexCreateVersion6   		= 0x00100,
 /* Summary
  When text caching is enabled, do not cache any fields that were provided through the data source API.
  See <link Caching documents>.   */
 dtsIndexCacheTextWithoutFields = 0x00200,
  /*
     Summary
     Preserve existing <link Document Ids, document ids> following a compression of an
     index or a merge of two or more indexes.
     Remarks
     Each document in an index is assigned a unique integer identifier called a
     document id or "DocId". The first document added to an index has the DocId 1, and
     subsequent documents will have sequentially numbered DocIds 2, 3, 4, and so
     forth. When a document is reindexed, its DocId is "cancelled" and a new DocId is
     assigned.

     When an index is compressed, all DocIds in the index are renumbered to remove the
     cancelled DocIds unless the dtsIndexKeepExistingDocIds flag is set in IndexJob.

     When an index is merged into another index, DocIds in the target index are never
     changed. The documents merged into the target index will all be assigned new,
     sequentially-numbered DocIds, unless (a) the dtsIndexKeepExistingDocIds flag is
     set in IndexJob and (b) the indexes have non-overlapping ranges of doc ids.

     If the dtsIndexKeepExistingDocIds is set in a merge and one or more indexes have
     overlapping doc ids, the error dtsErConflictingDocIdRanges will be returned and
     the merge will halt.

     When an index is created, you can specify a starting DocId other than 1 using
     IndexJob.StartingDocId. This makes it possible to ensure that indexes have
     non-overlapping ranges of DocIds so DocIds can be preserved after the indexes are
     merged.

                                                                                       */
 dtsIndexKeepExistingDocIds = 0x00400,

 // begin-internal
 // Create an index using the version 7 index format (obsolete -- version 7 indexes are the default)
 dtsIndexCreateVersion7   = 0x10000000,
 // Reserved for internal use
 dtsIndexTestDataSource   = 0x80000000
 // end-internal
};

/* Values for dtsOptions.binaryFiles (C++), Options.BinaryFiles (.NET), and Options.setBinaryFiles (Java). 
   See <link Filtering Options>.
*/
enum BinaryFilesSettings {
	/*  Summary
		Filter text from binary files using the character array in binaryFilterTextChars to determine which characters are text.
	    This option is not recommended.
	    Use dtsoFilterBinaryUnicode instead for more effective text extraction from binary data.
	 */
	 dtsoFilterBinary        =1,   
	 /* Summary
	    Index all contents of binary files as single-byte text.  
	    This option is not recommended.
	    Use dtsoFilterBinaryUnicode instead for more effective text extraction from binary data.
	 */
	 dtsoIndexBinary         =2,	
	 /* Do not index binary files
	 */
	 dtsoIndexSkipBinary     =3,	
	 /*  Summary
	     Filter text from binary files using a text extraction algorithm that 
	     scans for sequences of single-byte, UTF-8, or Unicode text in the input. 
	     This option is recommended for working with forensic data, particularly 
	     when searching for non-English text. 
	  */
	 dtsoFilterBinaryUnicode =4,

	 /* Summary
	    Index binary files disregarding all content within the file.  Only the filename
	    and any fields supplied externally to the file, such as in DocFields in the DataSource
	    API, will be indexed. 
	    */
	 dtsoIndexBinaryNoContent = 5	
	 };

/* Summary
   Values for dtsOptions.hyphens
   Remarks
   See <link Hyphens> in the <link Options, Options Overview> section for
   information on the effect of each value.                               */
enum HyphenSettings {
	 dtsoHyphenAsIgnore      =1, // index "first-class" as "firstclass"
	 dtsoHyphenAsHyphen      =2, // index "first-class" as "first-class"
	 dtsoHyphenAsSpace       =3, // index "first-class" as "first" and "class"
	 dtsoHyphenAll           =4  // index "first-class" all three ways
 };

// Flags for dtsConvertJob2 (C++) and FileConverter (.NET, Java)
enum ConvertFlags {
	// begin-internal
 dtsConvertViaHtml            = 0x0010,  // Internal use only
 dtsConvertReserved2		  = 0x8000,  // internal use only
 dtsConvertReserved3          = 0x10000, // internal use only
 dtsConvertReserved4 =          0x100000, // internal use only

 	// end-internal

	// Obsolete
 dtsConvertInputIsHtml        = 0x0001,

 // Summary
 //  Do not insert BeforeHit and AfterHit marks for a hit that cannot be displayed in HTML because the text is not visible. 
 // If this flag is not set, a pair of beforeHit/afterHit tags will be added before the next visible text in the file.
 dtsConvertSkipHiddenHits     = 0x0002,

 /* Summary
    Remove JavaScript from HTML files when adding hit highlight markings. JavaScript
    in HTML files can cause errors if displayed outside of the expected context of
    the script.                                                                      */
 dtsConvertRemoveScripts      = 0x0004,

 // Obsolete
 dtsConvertInputIsNotHtml     = 0x0008,

 /* Summary
    Extract a document either from an index or from a container

    Remarks
    (1) When a document is cached in the index, extract it without performing any
    conversion. Must be combined with dtsConvertGetFromCache, and the OutputFile must
    be set to the name of the file to write the result to (output cannot be directed
    to memory).                                                                       
    (2) When a document is stored in a container file format such as a PST archive,
    extract it without performing any conversion. OutputFile must
    be set to the name of the file to write the result to (output cannot be directed
    to memory).                                                                      
    */
 dtsConvertExtractOnly = 0x0020,

 /* Summary
    Add hit highlight markings to XML data while retaining the XML format. Both the
    \input and output formats must be XML. See <link Highlighting hits in XML files>
    in the dtSearch Engine Programmer's Reference (dtSearchApiRef.chm) for
    information on this flag.                                                        */
 dtsConvertXmlToXml			= 0x0040,

 // Just detect the file type of the document and return it in detectedTypeId, without generating
 // any output.
 dtsConvertJustDetectType	= 0x0080,

 /* Summary
    Use CSS styles instead of inline formatting to format HTML output. This affects
    content such as email headers, document properties, attachment filenames, and
    \other content that does not have its own formatting from the document. See <link Conversion output formatting>
    in the dtSearch Engine Programmer's Reference (dtSearchApiRef.chm) for
    information on this flag.                                                                                       */
dtsConvertUseStyles = 0x0100,

 // Read input file from cached document stored in the index.
 dtsConvertGetFromCache		= 0x2000,

// Summary
// If the input is a container (such as a ZIP file), recursively unpack each item in the
// container, convert it, and append it to the output.  dtsConvertInlineContainer
// is ignored if the input file is not a container, so it has no effect on the processing of .doc, .xls, etc. files.
dtsConvertInlineContainer	= 0x4000,

// Summary
//
// Automatically update hit offsets if this document was indexed by an earlier version
// of dtSearch or was changed since it was last indexed.  When using this flag,
// SetInputItem() must be used to transfer information about the search from
// SearchResults.  Additionally, the SearchResults must be generated from a search
// and not by calling UrlDecodeItem, because dtSearch needs information from the
// original search to ensure that highlighting information is correct.
dtsConvertAutoUpdateSearch = 0x20000,

 // include BOM in UTF-8 output
dtsConvertIncludeBOM = 0x40000,

/* Summary
   Highlight each search term using different highlighting attributes. See <link Multihighlighting, Highlighting each term using different attributes>
   in the dtSearch Engine Programmer's Reference (dtSearchApiRef.chm) for
   information on this flag.                                                                                                                           */
dtsConvertMultiHighlight = 0x80000

};

// Flags for dtsSearchReportJob (C++) and SearchReportJob (Java, .NET)
enum ReportFlags {
 // Count context by paragraphs.
 dtsReportByParagraph  = 0x0001,

 // Count context by words.
 dtsReportByWord       = 0x0002,

 // Include all items in the search report.
 dtsReportIncludeAll   = 0x0004,

 // Include full text of each selected file in the report.
 dtsReportWholeFile    = 0x0008,

 // Count context by words, and include exactly the requested amount of context.
 dtsReportByWordExact  = 0x0010,

 // Use cached version of document stored in the index, if present.
 dtsReportGetFromCache = 0x0020,

 // Store the search report for each item in search results.
 dtsReportStoreInResults = 0x0040,

 /* Summary
    Limit the size of context blocks when hits are contiguous. Using this flag
    prevents large context blocks from being extracted when many hits occur close
    together. For example, if a report is generated with 5 words of context around
    each hit, a single isolated hit would result in 11 words in a context block.
    However, if there were 100 hits clustered together, a much larger context block
    would be generated.
 */
 dtsReportLimitContiguousContext = 0x0080,

 // Include a block of text from the top of the file in the output.
 dtsReportIncludeFileStart = 0x0100,

 };

// Values for FileConverter.OutputFormat and SearchReportJob.OutputFormat
enum OutputFormat {
	// HTML
	itHTML = 226,
	// ANSI text
	itAnsi = 202,
	// RTF
	itRTF = 212,
	itUTF = 238,
	// XML
	itXML = 234,
	// Unicode encoded as UTF-8 (if output is directed to a string, the string will be UCS-16)
	itUTF8 = 238,
	// HTML without any formatting, for generating a synopsis that is HTML-encoded but that does not include formatting such as font settings, paragraph breaks, etc.
	itUnformattedHTML = 256,
	// Output format for FileConverter that organizes document content, metadata, and attachments into a standard XML format
    itContentAsXml		= 108

	} ;

// Error codes are returned after a job completes.  In the C++ API, use the dtsErrorInfo attached to a job
// to access error codes.  In .NET, use the Errors property of the job.  In Java, use getErrors().
enum ErrorCodes {
  // No error
  dtsErOK = 0,
	/* Unable to access index  */
  dtsErAccIndex           =1,
 /* If IndexingFlags.dtsCheckDiskSpace was set in an IndexJob,
  and if it appears that disk space is insufficient, then dtsAskDiskFullOverride will be
  sent to the error handler; the calling program should always allow the update to be
  cancelled in this case (the default action).  */
  dtsAskDiskFullOverride  =2,
  /* Unable to open temporary file.   */
  dtsErOpenTemp           =3,
  // Obsolete; no longer used.
  dtsInfNoFilesToIndex    =4,
  /* Index built by incompatible version of dtSearch */
  dtsErIxWrongVersion     =5,
  // Obsolete; no longer used.
  dtsErIxInterrupted      =6,
  /* Unable to create or access directory */
  dtsErAccDirectory       =7,
  /* Private directory is already locked */
  dtsErWorkDirFail        =8,
  /* Unable to access input file */
  dtsErAccFile            =9,
   /* File format error reading input file */
  dtsErFileCorrupt        =10,
  /* Index is full */
  dtsErIndexFull          =11,
  // Obsolete; no longer used.
  dtsErPdfLzwNotLicensed  =12,
  /* Operation timed out (trigged by timeoutSeconds) */
  dtsErTimeout            =13,
  /* Unable to create index */
  dtsErCreateFailed       =14,
  /* Unable to commit changes to index */
  dtsErCommitFailed       =15,
  /* The document was not found in the index (returned when a document listed to be removed from an index
     is not found in the index) */
  dtsErFileNotFound		=16,
  /* The document could not be accessed due to encryption */
  dtsErFileEncrypted 	  = 17,
  /* Incorrectly configured API structure (for example, invalid dtSearchJob) */
  dtsErApiError = 18,
  /* Exception thrown by data source */
  dtsErDataSourceException = 19,
  /* Filename is blank */
  dtsErBlankFilename = 20,
  /* File contains no data */
  dtsErFileEmpty = 21,
  /* A handle passed to a dtSearch API function was invalid */
  dtsErInvalidHandle = 22,
  /* In an index merge, the flag dtsIndexKeepExistingDocIds was set and one or more indexes had overlapping doc id ranges */
  dtsErConflictingDocIdRanges = 23,
  /* Search halted due to low memory available */
  dtsErLowMemory         =112,
  /* Syntax error in search request */
  dtsErBadRequest        =116,
  // Search request is blank
  dtsErSearchRequestEmpty = 117,
  /* Search found more files than the autoStopLimit for the search job */
  dtsErSearchLimitReached =120,
  /* No files retrieved in search */
  dtsErNoFilesRetrieved  =122,
  /* Search request was longer than dtsMaxSearchRequestLen */
  dtsErRequestTooLong    =135,
  /* Too many words retrieved in index */
  dtsErMaxWords          =137,
  /* Unable to write output file */
  dtsErWrFile            =143,
  /* Disk full -- indexing halted */
  dtsErDiskFull          =159,
  /* Index is corrupt */
  dtsErIxCorrupt         =162,
  /* Access to the index was denied */
  dtsErAccessDenied      =177,
  /* Unable to connect to data source during indexing */
  dtsErConnectFailed     =201,
  /* Unable to access cached document in index */
  dtsErAccCachedDoc      =202,
  /* Index update could not complete due to an error accessing the index */
  dtsErIndexingError     =203,
  /* An out-of-memory error occurred */
  dtsErOutOfMemory       =204,
  /* An unexpected exception occurred */
  dtsErUnknownException = 205,

  // begin-internal
  dtsErContainerItemCorrupt = 206,
  dtsErContainerItemEncrypted = 207,
  dtsErContainerItemZB = 208
  // end-internal

};

/* Flags for dtssConvertPath2 */
enum ConvertPathFlags {
	// Convert a local path to a virtual path
 dtsCpMakeVirtual          = 0x0001,
 // Convert a virtual path to a local path
 dtsCpMakeLocal            = 0x0002,

 // Convert a mapped path to a UNC path
 dtsCpMakeUNC              = 0x0008,

 // Use the Metabase to update the table of local-to-virtual mappings that the
 // dtSearch engine uses to perform these conversions
 dtsCpRefreshMappings      =  0x0010,

 // Store the updated table of mappings in the registry.
 dtsCpStoreInRegistry      = 0x0020,

 // Read the latest table of mappings from the registry.
 dtsCpReadFromRegistry     = 0x0040
};

/*
	Summary
    Values for Options.UnicodeFilterFlags (.NET) or dtsOptions.unicodeFilterFlags (C++) or Options.setUnicodeFilterFlags(java).

    Remarks
    UnicodeFilterFlags control the behavior of the Unicode Filtering algorithm when it is used to filter
    text from binary data.  See <link Filtering Options>.
 */
enum UnicodeFilterFlags {
	// Extracting blocks as HTML has no effect on the text that is extracted, but it adds additional information in HTML comments to each extracted block. The HTML comments identify the starting byte offset and encoding of each piece of text extracted from a file.
  dtsoUfExtractAsHtml				 = 0x0001,
  /* Summary
     Overlapping blocks prevents text that crosses a block boundary from being missed 
     in the filtering process. With overlapping enabled, each block extends 256 
     characters past the start of the previous block.
  */
  dtsoUfOverlapBlocks				 = 0x0002,
  /* Summary
     Automatically insert a word break in long sequences of letters. 
     A word break will be inserted when the word length reaches Options.MaxWordLength.
   */
  dtsoUfAutoWordBreakByLength		 = 0x0004,
  /* Summary
     Automatically insert a word break when a capital letter appears following lower-case letters.
     Example:  WordBreak would be indexed as Word Break.
   */
  dtsoUfAutoWordBreakByCase		 = 0x0008,
  // Automatically insert a word break when a digit follows letters.
  dtsoUfAutoWordBreakOnDigit		 = 0x0010,
  /* When a word break is automatically inserted due to dtsoUfAutoWordBreakByLength, 
     overlap the two words generated by the word break.  */
  dtsoUfAutoWordBreakOverlapWords   = 0x0020,
  /* When a document cannot be indexed due to file corruption or encryption, 
     apply the filtering algorithm to extract text from the file. */
  dtsoUfFilterFailedDocs			 = 0x0040,
  // Ignore file format information and apply Unicode Filtering to all documents.
  dtsoUfFilterAllDocs				 = 0x0080
};

/* Summary
   Control indexing of meta-data associated with documents
   Remarks
   FieldFlags provide options to control the indexing of meta-data associated with
   documents. When highlighting hits, it is important to make sure that FieldFlags
   has the same options that were used when a document was indexed. Otherwise, hit
   highlighting may be incorrect due to differences in the words found in each
   document.

   By default, dtSearch will index fields in documents such as the Summary
   Information fields in Word files and META tags in HTML files. FieldFlags can be
   used to suppress some or all of this metadata.

   dtSearch will also add a "Filename" field to the end of each document, with the
   full path and filename of the document, so words in the document name will be
   searchable like other text. To suppress this completely, use
   dtsoFfSkipFilenameField. To include only the name of the document (not the path),
   use dtsoSkipFilenameFieldPath.

   The dtsoFfHtmlShow* flags can be used to make normally hidden HTML elements, such
   as styles or links, visible and searchable. For each category of element that is
   enabled, a section will be added to the end of the HTML file listing the items in
   that category. For example, if dtsoFfHtmlShowComments is set, then each HTML file
   will have a list of the embedded comments after the body of the HTML.
   * API *
   C++: dtsOptions.fieldFlags

   Java: Options.setFieldFlags()

   .NET: Options.FieldFlags

   COM: Options.FieldFlags                                                           */
enum FieldFlags {
	// Do not generate a field named Filename containing the name of the file.
  dtsoFfSkipFilenameField         = 0x0001,
  // Do not index or search document summary fields
  dtsoFfSkipDocumentProperties    = 0x0002,
  // Make HTML links searchable
  dtsoFfHtmlShowLinks             = 0x0004,
  // Make HTML IMG src= attribute searchable
  dtsoFfHtmlShowImgSrc            = 0x0008,
  // Make HTML Comments searchable
  dtsoFfHtmlShowComments          = 0x0010,
  // Make HTML Scripts searchable
  dtsoFfHtmlShowScripts           = 0x0020,
  // Make HTML style sheets searchable
  dtsoFfHtmlShowStylesheets       = 0x0040,
  // Make HTML meta tags searchable and visible, appended to the body of the HTML file
  dtsoFfHtmlShowMetatags          = 0x0080,
  // Make content inside NOFRAMES tags searchable and visible, appended to the body of the HTML file
  dtsoFfHtmlShowNoframesContent   = 0x0100,

  // All of the dtsoFfHtmlShow* flags
  dtsoFfHtmlShowHiddenContent     = 0x01fc,
  // Suppress generation of HtmlTitle, HtmlH1, etc. fields
  dtsoFfHtmlNoHeaderFields        = 0x0200,
  // Skip non-text streams in Office documents
  dtsoFfOfficeSkipHiddenContent	= 0x0400,
  // In XML, make field names not searchable
  dtsoFfXmlHideFieldNames			= 0x0800,
  // Make NTFS file properties searchable
  dtsoFfShowNtfsProperties		= 0x1000,
  // Do not index attributes in XML files
  dtsoFfXmlSkipAttributes			= 0x2000,

  // Include only the filename (not the path) in the Filename field generated at the end of each document.
  dtsoFfSkipFilenameFieldPath		= 0x4000,

  /* Summary
     Skip attachments in PDF files.  If a PDF file has attachments, those attachments can be in any file format,
     so Adobe Reader cannot be used to highlight hits because it can only highlight hits in PDF data.
     Therefore, a PDF file with attachments must be hit-highlighted through file conversion like other
     document formats.  Skipping PDF attachments enables PDF files with attachments to be hit-highlighted
     using Adobe Reader.
  */
  dtsoFfPdfSkipAttachments = 0x8000,

  // Skip HTML INPUT tag "value" attributes
  dtsoFfHtmlSkipInputValues = 0x10000,

  // Skip HTML IMG tag "alt" attributes,
  dtsoFfHtmlSkipImageAlt = 0x20000,

  // Add file type field indicating the file format of the document (ex: "Microsoft Word")
  dtsoFfIncludeFileTypeField = 0x40000,

  // Add numeric type id field with the type id indicating the file format of the document
  dtsoFfIncludeFileTypeIdField = 0x80000,

  // Summary:
  // Suppress fields passed through the DataSource API through DataSource.DocFields or FileConverter.InputFields.
  // Remarks:
  // This flag is provides a way to prevent cached searchable fields from appearing in FileConverter output when
  // documents are indexed with caching of original files enabled.  
  dtsoFfSkipDataSourceFields = 0x100000,

  // Summary:
  // Suppress display of headers in emails.
  // Remarks:
  // This flag prevents MIME headers from appearing in emails when converted using FileConverter.
  // It does not affect display of the message properties (Sender, Recipient, Subject, etc.) at the
  // top of each message.
  //
  // To suppress display of message properties, use dtsoFfSkipEmailProperties
  dtsoFfSkipEmailHeaders = 0x200000,

  /* Summary
     Index the names of files in ZIP and RAR archives
     Remarks
     This option provides a way to search on the list of files in a ZIP or RAR
     archive, even if the individual files may be inaccessible due to encryption. When
     dtSearch indexes a ZIP or RAR archive, in addition to the files actually present
     in the ZIP or RAR archive, it will also make a list of all of the files in the
     archive and index it with the name ArchiveFileList.html.

     The original file is not modified but the ArchiveFileList.html file is searchable
     as if it were part of the ZIP or RAR file. The file consists of a list of the
     names of the files inside the archive.

     For example, suppose you index a ZIP file that contains "secret1.doc",
     "secret2.doc", and "secret3.doc", all stored in the ZIP file with encryption. The
     contents of the three files will not be searchable because of the encryption. However,
     a search on "secret1" will find the word "secret1" in ArchiveFileList.html in the
     ZIP file.

     RAR files can be encrypted in a way that makes even the filenames impossible to
     see without a password. In this case, the names will not be searchable.                */
  dtsoFfIndexArchiveFileLists = 0x400000,

  // Summary
  // Include a caption "Document Properties" on the table of document properties in Word, Excel, and PowerPoint 2003 documents
  // Remarks:
  // This caption was removed in dtSearch 7.70 by default (a side effect was to make "Document" and "Properties" 
  // searchable in all documents).  Use this flag to restore if required for backward compatibility.
  //
  dtsoFfIncludeDocumentPropertiesCaption = 0x800000,

  // Summary
  // Display properties of image files embedded in documents
  // Remarks:
  // When images are indexed as individual files, their properties are always indexed.
  // Set this flag to also index properties of images that are embedded in other documents.
  dtsoFfShowImageProperties = 0x1000000,

  // Summary
  // Suppress display of email properties (subject, sender, recipient, etc.)
  // Remarks:
  // This flag suppresses all email properties such as sender, subject, recipient, etc.
  // in Outlook (*.msg) and MIME (*.eml) messages.
  // To suppress MIME headers, use dtsoFfSkipEmailHeaders, which can be combined with this flag.
  dtsoFfSkipEmailProperties = 0x2000000,

};

// Values for Options.TextFlags (.NET), Options.setTextFlags() (Java), and dtsOptions.textFlags (C++)
enum TextFlags {
	/* Summary
	   By default, dtSearch indexes numbers both as text and as numeric values, which is necessary
	   for numeric range searching. Use this flag to suppress indexing of numeric values in applications
	   that do not require numeric range searching. This setting can reduce the size of the index by about 20%.
     */
	dtsoTfSkipNumericValues			= 0x0001,

	/* Summary
	   Suppress automatic generation of xfirstword and xlastword.
	   By default, xfirstword is defined to be the first word in each document, and xlastword
	   is defined to be the last word in each document.  These words are generated when an index
	   is created, so this flag must be set during indexing to suppress xlastword and xfirstword.
	 */
	dtsoTfSkipXFirstAndLast         = 0x0002,

	// Summary
	// Automatically recognize dates, email addresses, and credit card numbers in text as it is indexed.
	// See <link Recognition of Dates, Email Addresses, and Credit Card Numbers>.
	dtsoTfRecognizeDates = 0x0004,

	// Summary
	// Presume DD/MM/YY format for dates (default is MM/DD/YY).
	// See <link Recognition of Dates, Email Addresses, and Credit Card Numbers>.
	dtsoTfRecognizeDatesPresumeDMY = 0x0008,
	// Summary
	// Presume YY/MM/DD format for dates (default is MM/DD/YY).
	// See <link Recognition of Dates, Email Addresses, and Credit Card Numbers>.
	dtsoTfRecognizeDatesPresumeYMD = 0x0010,
	/* Summary
	   Automatically insert a word break around characters in the Chinese, Japanese, and Korean Unicode ranges.
	   This makes it possible to search text in documents that do not contain word breaks.
	   Like the hyphenation setting, this setting is kept in the alphabet for an index and so will only
	   change when an index is created.
	   See  <link Alphabet Settings>.
	*/
	dtsoTfAutoBreakCJK = 0x0020,
// begin-internal	
	dtsoTfReserved1 = 0x40,
	dtsoTfReserved2 = 0x80,
	dtsoTfReserved3 = 0x100,
	dtsoTfReserved4 = 0x200,
	/* Summary
	   Disable any internal file parsers that are currently classified as "beta" because they were
	   recently added and have not yet been tested.  This flag will have no effect on release builds
	   of the dtSearch Engine and only provides a way to use beta builds without allowing newly-added
	   file parsers to run.
	 */
	dtsoTfDisableBetaParsers	    = 0x400000,

	// Re-enable internal file parsers that were disabled by dtsoTfDisableBetaParsers.
	dtsoTfEnableBetaParsers			= 0x800000
// end-internal

};

// Values for dtsSearchJob.unindexedSearchFlags
enum UnindexedSearchFlags {
	// Case-sensitive search
	dtsUSearchCaseSensitive      = 0x0001,
	// Do not search binary files
	dtsUSearchSkipBinaryFiles    = 0x0002,
	// Accent-sensitive search
	dtsUSearchAccentSensitive    = 0x0004
	};

/* Summary
 * Flags for use with dtssDebugLogEx (C++) or SetDebugLogging (.NET)
 *
 * Remarks
 * dtsCrashLog is for use only in .NET, Java, and COM.  In C++, use dtssInstallCrashDiagnostics instead.
 *
 */
 enum DebugLogFlags {
	// Each line in the log will have include the time, in hundredths of a second, since the start of execution.
  dtsLogTime      =1,
   /* Summary
      The log will be committed to disk after each line is written. 
      This slows execution considerably but ensures that the log will survive a system crash.
    */
  dtsLogCommit    =2,
  	// Log data is appended to the file if it already exists.
  dtsLogAppend    =4,
  dtsLogMemory    =8,
  dtsLogToDbwin   =16,
  dtsLogWarnOnFailure =32,
  dtsLogTruncate	=64,      // Truncate the currently-open log
  dtsCrashLog      = 0x1000,   // Generate a stack trace in the logFile if an unhandled exception occurs (Windows only).
  dtsLogReserved1 = 0x10000
};

/*
 *  Used in the output.listingFlags member of a dtsSearchJob,
 *  these flags specify the format for a listing file created
 *  following an search.
 */
 enum ListingFlags {
	dtsListHitCount  = 0x0001, /* Include number of hits for each file */
	dtsListHits      = 0x0002,
	dtsListHeader    = 0x0400, /* Include header with search request, etc. */
	dtsListAppend    = 0x0800, /* Append to the file created */
	dtsListNoDisk    = 0x1000 /* Do not write search results to disk */
};

/*
 *  Values used in the Flags field for a file parser in filetype.xml.
 *  In filetype.xml, the numerical value must be used, not the constant name.
 */
enum FileTypeFlags {
 	dtsFtOverride    = 0x0001,
 	dtsFtDisable     = 0x0002,  // Disable a file parser
 	dtsFtEnable      = 0x0004
};

// Preserve backward compatibility with old name
#define dtsMessageCode MessageCode

/* Summary
   MessageCodes are sent to callback functions during an index or search to update
   the caller on the status of the job.
   * API *
   * C++ *
   Sent through dtsJobBase.pReportCallBack. See dtsMessage for documentation on
   structures passed to the callback.
   * .NET *
   Sent through IIndexStatusHandler and ISearchStatusHandler
   * Java *
   Sent through IIndexStatusHandler                                                */
enum MessageCode {
    dtsnFirstStatusMessage = 1000,

        /* Asks the calling application to give the user a chance to abort (i.e., check if
           the user has pressed a "cancel" button)                        */
    dtsnCheckForAbort = 1001,

// begin-internal
        /* Obsolete: Asks the calling application to update the screen display */
    dtsnRequestDraw = 1002,
// end-internal

        /* Last message sent during execution of an index or search job.  */
	dtsnJobClose = 1003,

    dtsnConvertPercentDone = 1004,

    /*
     * Status notification messages (searching), prefix = dtsnSearch
     */
    dtsnFirstSearchStatusMessage = 2000,

        /* Sent at the beginning of a search */
    dtsnSearchBegin,

        /* Sent at the end of a search
         */
    dtsnSearchDone,
        /* Sent during a search to indicate what is being searched. */
    dtsnSearchWhere,
        /* Sent when a file is retrieved in a search.  */
    dtsnSearchFound,
        /* Sent during a search periodically to update elapsed time display. */
    dtsnSearchUpdateTime,
	/* Sent during an unindexed search when the input document is encrypted. */
    dtsnSearchFileEncrypted,
	// Sent during an unindexed search when the input document is unreadable due to corruption.
    dtsnSearchFileCorrupt,
	// Sent during an unindexed search when the input document has been searched.

    dtsnSearchFileDone,

    dtsnLastSearchStatusMessage = 2999,

    dtsnFirstIndexStatusMessage = 3000,

        /* Sent at the start of an indexing job. */
    dtsnIndexBegin = 3001,
        /* Sent at the end of an indexing job. */
    dtsnIndexDone,

        /* Sent when an index is being created. */
    dtsnIndexCreate,

        /* Sent as dtSearch is checking files to see if they need to be indexed.
                                                                                 */
    dtsnIndexCheckingFiles,
        // As dtSearch checks the list of files to be indexed, this message will be sent
        // updating the amount of text to be indexed.
    dtsnIndexToAddUpdate,

        // Obsolete
    dtsnIndexAdded,

        /* Sent when dtSearch is starting to index a file. */
    dtsnIndexStartingFile,

        /* Sent when starting to index a file, periodically as the file is indexed, and when
           the file is done.
                                                                                             */
    dtsnIndexFileProgress,
        /* Sent when a file has been completely read. */
    dtsnIndexFileDone,

        /* Sent when an attempt to open a file fails. */
    dtsnIndexFileOpenFail,

        /* Sent when a file is not indexed because (1) it seems to be binary, and (2)
           indexing of binary files is turned off.
                                                                                      */
    dtsnIndexFileBinary,
        /* Sent to update the status of an index update during the "merging words" step. */
    dtsnIndexMergeProgress,

        /* Sent to update compression progress. */
    dtsnIndexCompressProgress,

        /* Sent when a file is not indexed because it is encrypted. */
    dtsnIndexFileEncrypted,
        /* Sent when words are being written to disk for merging. */
    dtsnIndexStoringWords,

	// Sent when an index update begins
    dtsnIndexStartingUpdate,

		/* Indicates that a file was indexed but that part of the file could not be accessed due to encryption. */
    dtsnIndexFilePartiallyEncrypted,

		/* Indicates that a file was indexed but that part of the file could not be accessed due to data corruption. */
    dtsnIndexFilePartiallyCorrupt,

		// Sent when an automatic index commit is starting (automatic commits are triggered by autoCommitIntervalMB).
    dtsnAutoCommitBegin,

    	// Sent when an automatic index commit is complete (automatic commits are triggered by autoCommitIntervalMB).
    dtsnAutoCommitDone,

    // Sent when a file is removed from the index because it appears to have been deleted
    dtsnIndexDeletedFileRemoved,

    // Sent when a file is removed from the index because it was listed in the IndexJob to be removed
    dtsnIndexListedFileRemoved,

    // Sent when a file is NOT removed from the index, although it was listed in the IndexJob to be removed,
    // because it was not found in the index
    dtsnIndexListedFileNotRemoved,

    // Sent when a folder could not be accessed to scan for documents to index
    dtsnIndexFolderInaccessible,

    dtsnLastIndexStatusMessage = 3999,

		/* Sent during a merge */
    dtsnIndexMergeJobProgress,

		/* Sent during verification of an index. */
	dtsnIndexVerifyProgress,
	// begin-internal
    /*  Diagnostic messages (internal use only) */
    dtsnFirstDiagnostic = 9000,
    dtsnSortingStart,
    dtsnSortingEnd,
    dtsnWritingMergeStart,
    dtsnWritingMergeEnd,
    dtsnMergeFileSize,
    dtsnMergeToXMS,
    dtsnSorterSize,
    dtsnMergeFreeSpace,
    dtsnMergeTrackCount,
    dtsnMergeBegin,
    dtsnMergeEnd,
    dtsnInterimCommitBegin, // deprecated, use dtsnAutoCommitBegin
    dtsnInterimCommitDone,  // deprecated, use dtsnAutoCommitEnd
    dtsnIndexAboutToCommit,
    dtsnLastDiagnostic,

    dtsLastMessage = 9999
	// end-internal
};

enum UrlEncodeItemFlags {
	dtsUrlEncodeAsSearch = 0x40000000
	};

/* Summary
   Flags to control the extraction of attachments and images using FileConverter
   (.NET, Java) or dtsFileConvertJob2 (C++).
   Remarks
   When highlighting hits, flags should be consistent with the behavior of the
   indexer or hit highlighting will be inconsistent.

   dtsExoExtractAttachments should not be used when highlighting hits because in
   some cases text is added to the conversion output to provide a location for the
   link to the extracted attachment.

   dtsExoDoNotConvertAttachments should not be used when highlighting hits because
   the indexer does convert attachments to text.

   dtsExoExtractImages can be used when highlighting hits.                         */
enum ExtractionOptionsFlags {
 /* Extract embedded images in the input and insert a reference to the extracted
    image in the conversion output                                               */
 dtsExoExtractImages = 0x0001,

 /* Extract embedded attachments in the input and insert a reference to the extracted
    attachment in the conversion output (dtsExoExtractAttachments should not be used
    when highlighting hits because in some cases text is added to the conversion
    \output to provide a location for the link to the extracted attachment).          */
 dtsExoExtractAttachments = 0x0002,

 /* Omit attachments from the conversion output (dtsExoDoNotConvertAttachments should
    not be used when highlighting hits because the indexer does convert attachments
    to text).                                                                         */
 dtsExoDoNotConvertAttachments = 0x0004,

 // Limit the filename extensions for extracted images and attachments to those listed
 // in allowedExtensions.
 dtsExoLimitExtensions = 0x0008,

// If true, on NTFS file systems, all extracted images and attachments will be marked
// as originating in the "Internet" Zone.  This enables various safety mechanisms in
// Microsoft Windows and Microsoft Office products to guard against opening unsafe
// attachments when you try to launch marked files.  
// To mark the files, dtSearch adds an alternate stream to the output file named 
// Zone.Identifier with the content "[ZoneTransfer]\r\nZoneId=3\r\n"
 dtsExoMarkAsInternetZone = 0x0010,

 dtsExoReserved1 = 0x0020

 };

/* For responding to queries about the abort status of a job */

#define dtsAbortImmediate   -2  // Terminate job immediately without saving changes
#define dtsAbort            -1  // Terminate job
#define dtsContinue          0  // Continue job

// These values apply only to the dtsnSearchFound notification
#define dtsVetoSearchResultsItem			-3  // Prevent item from being included in search results
#define dtsChangedItemScore				    -4  // The value of the score property in the dtsSearchNotificationItem was changed

#define maxRequestLen       dtsMaxSearchRequestLen

// Maximum size of a search request in bytes
const int dtsMaxSearchRequestLen = 70000;

// Maximum allowed value for maxWordsToRetrieve in dtsSearchJob and dtsOptions
const int dtsMaxWordsToRetrieveLimit = 0x80000;

// Maximum allowed value for maxStoredFieldSize
const int dtsMaxStoredFieldSize = 0x2000;

// begin-internal
/* Macros to classify the message codes */
#ifndef __UNIX__
#define dtsIsCommandMessage(c)      (c < dtsnFirstStatusMessage)
#define dtsIsSearchStatusMessage(c) ((c > dtsnFirstSearchStatusMessage) && \
                                     (c < dtsnLastSearchStatusMessage))
#define dtsIsIndexStatusMessage(c)  ((c > dtsnFirstIndexStatusMessage) && \
                                     (c < dtsnLastIndexStatusMessage))
#define dtsIsDiagnostic(c)          ((c > dtsnFirstDiagnostic) && \
                                     (c < dtsnLastDiagnostic))
#endif

// end-internal

/* Summary
   Structure passed to dtsJobBase.pReportCallBack to implement status notification
   callbacks.
   Remarks
   The meaning of the arguments depends on the "command" value, which is documented
   in the MessageCode enum.

   For dtsnSearch* messages, msg.ptrParam points to a dtsSearchProgressInfo with
   information on the status of the search.

   For dtsnIndex* messages, msg.ptrParam points to a dtsIndexProgressInfo with
   information on the status of the index update.

   Other values in the dtsMessage are still supported for backwards compatibility
   and are used in the dtsfc.cpp source code for DSearchJob and DIndexJob, but these
   values are deprecated.
   Returns
   pReportCallBack must return one of the following values:
   <table>
   Value                      Meaning
   -------------------------  -------------------------------------------------------
   dtsContinue                Continue to execute the job
   dtsAbort                   Cancel the job
   dtsAbortImmediate          Cancel the job without saving changes to the index
   dtsVetoSearchResultsItem   In response to dtsnSearchFound only, prevent this item
                               from being included in search results.
   </table>
   During index updates, dtsAbort allows the current document to be completed and
   the information indexed so far to be committed to the index. dtsAbortImmediate
   cancels the index update and rolls back the index status to the last committed
   update.

   Additional callbacks may occur after returning dtsAbort or dtsAbortImmediate.      */
struct dtsMessage {
	// First integer parameter of the message
	intptr_t paramA;
	// Second integer parameter of the message
	intptr_t paramB;
    // String parameter of the message
    const char *strParam;
	// Pointer parameter of the message
	void *ptrParam;
    // Return value to pass back from the callback function
    long result;
    // MessageCode value specifying the reason for the callback
    long command;

    dtsMessage() {  clear(); }

    dtsMessage(MessageCode cmd, long a = 0, long b = 0) {
        clear(); command = cmd; paramA = a; paramB = b; };
    dtsMessage(MessageCode cmd, const char *s, long a = 0, long b = 0) {
        clear(); command = cmd; strParam = s; paramA = a; paramB = b; }

    void clear() { command = (MessageCode) 0;
        paramA = paramB = 0; strParam = 0; ptrParam = 0; result = 0; };
    long ret(long v) { result = v; return v; };
    };

/* Summary
   Used with dtsIndexJob and dtsSearchJob to record errors that occur during
   indexing or searching.
   Remarks
   dtsErrorInfo accumulates messages in a buffer and has member functions to provide
   access to those messages. Additionally, it has a callback function you can use to
   \obtain notifications each time a message is added to the dtsErrorInfo.
   dtsErrorInfo provides a more thread-friendly way to report errors than the global
   callback functions in the dtsInitInfo structure passed to dtssDoInit.

   For each message, the following information is recorded: (1) a numeric code,
   which identifies the message in a language-independent way; (2) the text of the
   message; and (3) up to two string arguments for the message. For example, if an
   index cannot be accessed in a search, the numeric code will be dtsErAccIndex, the
   text of the message will be "Unable to access index," the first argument will be
   the path to the index, and the second argument will be the reason the index could
   not be accessed.

   To use dtsErrorInfo,

     1. Declare a dtsErrorInfo struct with the buffer size you want. (The
        constructor lets you specify the maximum number of messages and the total size of
        the string buffer.)
     2. Attach the dtsErrorInfo to the dtsSearchJob or dtsIndexJob as its
        errorHandler member.
     3. If you want to receive notifications when an error occurs, use the
        dtsErrorInfo's pNotifyFn member to designate the function to be called.
     4. Execute the dtsSearchJob or dtsIndexJob.
     5. After the job is complete, use the member functions described below to
        access information on the error conditions that occurred during the search.
     6. The dtsErrorInfo destructor will automatically delete the memory buffers
        allocated for the dtsErrorInfo. For information on error codes generated by the
        dtSearch Engine, see "Error Codes."

   The data members of dtsErrorInfo (messageText, messageCode, etc.) should not be
   accessed directly. Instead, use the dtsErrorInfo constructor to initialize them
   and then use getCount(), getMessage(), etc. to access them.                            */

struct dtsErrorInfo {
		// Initialize to sizeof(dtsErrorInfo).
        long structSize;
        // Use the pUserData pointer to store instance data that you want to access in the callback function in pNotifyFn.
        void *pUserData;

        long *messageCode;
        char *buf;
        const char **messageText;
        const char **messageArg1;
        const char **messageArg2;
        long bufSize;
        long maxMessages;
        long messageCount;
        long textSize;
        long fOverflowed;
        int (*pNotifyFn)(struct dtsErrorInfo *pHandler);
        long reserved[8];
#ifdef __cplusplus
		// Constructor for dtsErrorInfo, which will allocate fixed-length buffers
		// large enough to hold, by default, up to 20 messages with up to 4096 bytes.
		// The dtsErrorInfo destructor will automatically deallocate these buffers.
        dtsErrorInfo(int aMaxMessages = 100, int aBufSize = 8192);
        ~dtsErrorInfo();
        // Removes all messages from a dtsErrorInfo but does not deallocate the buffers.
		void clear();
		// Copy all data from another dtsErrorInfo
        void copy(const dtsErrorInfo& other);
        // Attaches a callback notification function to the dtsErrorInfo. Each time an error occurs during searching or indexing, the function provided will be called with a pointer to the dtsErrorInfo as the only parameter. Instance data can be accessed through the pUserData member of the dtsErrorInfo.
		void setNotify(int (*aNotifyFn)(dtsErrorInfo *pHandler));
		// Returns the number of messages logged in the dtsErrorInfo.
        long getCount() const;
        // Adds a message to the dtsErrorInfo
        int handleMessage(long code, const char *msg, const char *arg1 = 0,
            const char *arg2 = 0);
        // Returns the text of a message
        const char *getMessage(long iMessage = -1) const;
        // Returns the numeric code of a message (see Error Codes for the values of these codes)
        long getMessageCode(long iMessage = -1) const;
        const char *getMessageArg1(long iMessage = -1) const;
        const char *getMessageArg2(long iMessage = -1) const;
        // Returns true if some messages could not be stored in the dtsErrorInfo because the buffers were too small.
		int overflowed() const;
        bool haveMessage(long code) const;
        const char *storeString(const char *s);
        // Transfer messages to another dtsErrorInfo
        void forwardTo(dtsErrorInfo& other) const;
        // Return text meaning for a numeric error code
        static const char *getMessageForCode(int code);
#endif
    };

// Contains a file modification or creation time.
// File dates are always reported as UTC time.
struct dtsFileDate {
	// Year (4-digit)
    unsigned short year;
    // Month (1-12)
    char month;
    // Day (1-31)
    char day;
    // Hour (0-23)
    char hour;
    // Minute (0-59)
    char minute;
    // Second (0-59)
    char second;
#ifdef __cplusplus

    dtsFileDate() {
        clear();
        }
    bool isValidTime() const {
		return (hour >= 0) && (hour < 24) && (minute >= 0) && (minute < 60) && (second >= 0) && (second < 60);
		}
	bool isValidDate() const {
		return (year > 1000) && (year < 4000) && (month >= 1) && (month <= 13) && (day >= 1) && (day <= 31);
		}
	bool isValid() const {
		return isValidTime() && isValidDate();
		}
	void setYMD(int y, int m, int d) {
		year = (unsigned short) y;
		month = (char) m;
		day = (char) d;
		}
	void setHMS(int h, int m, int s) {
		hour = (char) h;
		minute = (char) m;
		second = (char) s;
		}
    // Copy a date
    dtsFileDate& copy(const dtsFileDate& other) {
        year = other.year;
        month = other.month;
        day = other.day;
        hour = other.hour;
        minute = other.minute;
        second = other.second;
        return *this;
        }
    // Set all values in a date to zero
    void clear() {
        year = 0; month = 0; day = 0; hour = 0; minute = 0; second = 0;
        }
    // True if all values in two dates are equal
    bool operator==(const dtsFileDate& other) const;
    // True if two dates are not identical
    bool operator!=(const dtsFileDate& other) const;
    // True if this date is after the other date
    bool operator>(const dtsFileDate& other) const;
    // True if this date is before the other date
    bool operator<(const dtsFileDate& other) const {
		return other > *this;
		}
	bool operator>=(const dtsFileDate& other) const {
		return (*this > other) || (*this == other);
		}
	bool operator<=(const dtsFileDate& other) const {
		return (*this < other) || (*this == other);
		}
#endif
    };

// begin-internal
struct dtsFileMapEntry {
    long wordCount;
    long filePtr;
    long paragraph;
    long page;
    char data[16];
#ifdef __cplusplus
    dtsFileMapEntry();
    dtsFileMapEntry(const dtsFileMapEntry& other);
    int operator==(const dtsFileMapEntry& other) const;
    dtsFileMapEntry& operator=(const dtsFileMapEntry& other);
    void clear();
#endif
    };

// For backwards compatibility
#define dtsSearchNotificationItem dtsSearchResultsItem
// end-internal

/* Summary
   Properties of a single retrieved document from a search.
   Remarks
   There are two ways to obtain a dtsSearchResultsItem:

   (1) Call dtsSearchResults.getDocInfo(), or (2) In a search callback function
   during a search.
   * Search callbacks *
   The dtSearch Engine will pass a pointer to a dtsSearchResultsItem to the
   pReportCallBack function that you supply in a dtsSearchJob to receive detailed
   status information during a search. The pointer is passed in the pFoundItem member of
   the dtsSearchResultsItem, which is passed in the dtsMessage structure passed to
   in the ptrParam. (The paramA member is the
   number of hits in the retrieved document and the strParam member is the name of
   the retrieved document.)

   If the dtsSearchDelayDocInfo SearchFlag was set, most of the data in the
   dtsSearchResultsItem will be blank in callbacks, because the document information
   record has not yet been retrieved. Only information generated by the search
   (document id, hit count, hitsByWord, etc.) will be present.                       */
struct dtsSearchResultsItem {
    long structSize;
    long parserId;
    // Name of the retrieved file.
    const char *filename;
    // Either a blank string, or the name of the retrieved file in a form that is more suited to user display than the filename
    const char *displayName;
    // Text from the beginning of the document
    const char *title;
    // The modification date of the document
    struct dtsFileDate modified;
    // count of items in hits list (See phraseCount below for an alternative count.)
    long hitCount;
    // Number of words in the document when it was indexed.
    long wordCount;
    // Size of the document, in bytes, when it was indexed (superseded by size64)
    long size;
    // The score for each document is either the hit count or the weighted score, if automatic or variable term weighting was used in the search.
    long score;
    // A null-delimited list of field names and field values containing each field in this document that was listed in the storedFields list in the dtsIndexJob used to index the document
    const char *userFields;
    // A long integer that uniquely identifies this document in the index
    long docId;
    // If more than one index was searched, this will identify the index that the document was retrieved from. The iIndex member is an index into the indexesToSearch list in the dtsSearchJob.
    long iIndex;
// begin-internal
    long fileMapEntryCount;
    struct dtsFileMapEntry *fileMapTable;
// end-internal
	/* A null-delimited string set containing the number of times each word or
	phrase in a search request was found. The string set consists of a series of
	entries with the number of hits, the word or phrase, and, if the search
	involved a field name, the name of the field. Phrases are quoted. To obtain
	hitsByWord, set the dtsSearchWantHitsByWord flag in the dtsSearchJob's
	searchFlags member. */
    const char *hitsByWord;
    // File type of the document when it was indexed (see dtsImportType for a list of type ids)
    long typeId;

	// deprecated, use hitDetails
    const char *hitsByWord2;
    // Word offsets of the hits in the document
    long *hits;
    // Location of the document
    const char *location;
    // Filename of the document with path removed
    const char *shortName;
    // Build number of dtSearch Engine version that indexed this file
    long indexedBy;
    // Path of the index this document was found in
    const char *indexRetrievedFrom;
    /* Snippet of text showing hits in context, if a search report was generated from the search results
     containing this item, and if the dtsReportStoreInResults flag was set in the dtsSearchReportJob   */
    const char *synopsis;
    /* A null-delimited string set containing details on the hits generated by
	each word match in a search request. See the documentation on the
	dtsSearchWantHitDetails Flag for more information. */
    const char *hitDetails;
    /* The scorePercent for each document is the score expressed as a
	percentage, relative to the highest-scoring document in the search results list.
	*/
    long scorePercent;
    // after URL-encode/decode, hitCount might be less than originalHitCount
    long originalHitCount;
    /* Number of phrases matching the search request.  By default, dtSearch
	counts each word in search results as a hit.  When the
	dtsSearchWantHitsByWord flag is set in the dtsSearchJob's searchFlags
	member, in addition to creating the hitsByWord table (see above), dtSearch
	will also calculate the hit count with each phrase counted as a single hit.
	The hitCount is still reported as the number of words matching the search
	request. */
    long phraseCount;
    // begin-internal
    long ssk;
    long parserDataPos;
    long parserDataLen;
    void *parserData;
    // end-internal
    // File created date
    struct dtsFileDate created;
	// begin-internal
	__int64 parserDataPos64;
	__int64 cachedTextPos64;
	__int64 cachedOriginalPos64;
    // end-internal
    // Size of the document, in bytes, when it was indexed
	__int64 size64;
	// begin-internal
	__int64 reserved4;
	__int64 reserved5;
	const char *authInfo;
#ifdef __cplusplus
    dtsSearchResultsItem();
    void copy(const dtsSearchResultsItem& other);
    void clear();
    void getFileMapEntryForWord(long w, dtsFileMapEntry& dest);
	// end-internal
	// Get user-friendly name to display for this file, taking into account
	// whether the PDF and HTML titles should be used in place of filenames
    const char *getDisplayName(int fPdfUseTitleAsName, int fHtmlUseTitleAsName) const;
    const char *getTitle(int fPdfUseTitleAsName, int fHtmlUseTitleAsName) const;
    const char *getTypeName();
    static const char *getTypeName(int typeId);
    // Returns true if this document was cached in the index when the index was created.
    bool isCached() {
		return (cachedTextPos64 > 0) || (cachedOriginalPos64 > 0);
		}
	void setSize(__int64 s) {
		size64 = s;
		if (s > 0x7fffffff)
			size = 0;
		else
			size = (long) s;
		}
	__int64 getSize() {
		if (size64)
			return size64;
		return size;
		}
#endif
    };

/* Summary
   Describes the current status of the indexer

   * API *
   C++: Returned through dtsIndexProgressInfo.step

   .NET: Returned through IndexProgressInfo and IndexJob.StatusIndexingStep

   Java: Returned through IndexProgressInfo                                 */
enum IndexingStep {
		ixStepNone = 0, 			// Not started
		ixStepBegin = 1, 			// Start of indexing job
		ixStepCreatingIndex = 2, 	// An index is being created
		ixStepCheckingFiles = 3,	// Searching for files to be added to the index or checking files to see if they have already been indexed
		ixStepReadingFiles = 4, 	// Parsing files into words and sorting the words
		ixStepStoringWords = 5, 	// Storing word information to be added to the index
		ixStepMerging = 6, 			// Adding words to the index
		ixStepCompressing = 7,		// Compressing the index
    	ixStepDone = 8, 			// Indexing complete
    	ixStepVerifyingIndex = 9, 	// Checking for index corruption
    	ixStepMergingIndexes = 10,  // Merging two or more indexes into a single index
    	ixStepRemovingDeletedFiles = 11, // Checking all files in the index to see if they have been deleted
    	ixStepCommittingChanges = 12	// Committing index update
    	};

// Preserve compatibility with old name for this enum
#define dtsInputType TypeId

/* Summary
   \File type identifiers
   Remarks
   Because some older file parsers are still supported for backward compatibility,
   in a few cases there may be more than one TypeId for a file format.             
   Not all file formats listed are supported for content extraction or indexing.
   For a current list of supported file types, see: http://support.dtsearch.com/faq/dts0103.htm
   */
enum TypeId {
    it_None             = 0,
// begin-internal
    it_Any              = 1,
    it_SearchResults    = 100,
    it_SearchReport     = 101,
    it_Directory        = 102,
    it_FileList         = 103,
// end-internal
    it_WordList       	= 104, // List of words in UTF-8 format, with the word ordinal in front of each word

// begin-internal
    it_DiagnosticInfo 	= 105,
    it_FPML             = 106,
    it_CTWB				= 107,
// end-internal
    it_ContentAsXml		= 108,  // Output format for FileConverter that organizes document content, metadata, and attachments into a standard XML format
	it_WordListNoOrdinal =109, 

    it_Ami              = 201,	// Ami Pro
    it_Ansi             = 202,	// Ansi text file
    it_Ascii            = 203,	// Ascii (DOS) text file
    it_Binary           = 204,	// Binary file (unrecognized format)
    it_CompoundDoc      = 205,	// OLE Compound Document (or "DocFile")
    it_DBF              = 206,	// XBase database file
    it_FilteredBinary   = 207,	// Filtered binary file
    it_HyperText        = 208,	// Obsolete
    it_MS_Word          = 209,	// Obsolete
    it_MS_Works         = 210,	// Microsoft Works word processor
    it_Multimate        = 211,	// Multimate (any version)
    it_RTF              = 212,	// Microsoft Rich Text Format
    it_WS_2000          = 213,  // Wordstar 2000
    it_WS_5             = 214,  // WordStar version 5 or 6
    it_WinWrite         = 215,	// Windows Write
    it_WordForDos		= 215,	// Word for DOS (same as Windows Write, it_WinWrite)
    it_WordForWin       = 216,	// Obsolete
    it_WordForWin6      = 217,  // Microsoft Word 6.0
    it_WordPerfect42    = 218,	// WordPerfect 4.2
    it_WordPerfect5     = 219,	// WordPerfect 5
    it_WordPerfect6     = 220,	// WordPerfect 6
    it_WordStar         = 221,  // WordStar through version 4
    it_XyWrite          = 222,	// XyWrite
    it_ZIP              = 223,	// ZIP archive
    it_Properties       = 224,  // PropertySet stream in a Compound Document
    it_Excel5           = 225,  // Excel versions 5 and 7
    it_HTML             = 226,	// HTML
    it_PDF              = 227,	// PDF
    it_WordForWin97     = 228,	// Word For Windows 97, 2000, XP, or 2003
    it_Excel97          = 229,	// Excel 97, 2000, XP, or 2003
    it_PowerPoint       = 230,	// PowerPoint 97-2003
    it_EncryptedHtml    = 231,  // Obsolete
    it_DatabaseRecord   = 232,  // Record in a database file (such as XBase or Access)
    it_SegmentedText    = 233,  // Text segmented using File Segmentation Rules
    it_XML              = 234,  // XML
    it_WordPerfectEmbedded = 235,  // WordPerfect document embedded in another file
    it_Unicode          = 236,  // UCS-16 text
    it_EudoraMessage    = 237,  // Message in a Eudora message store
    it_Utf8             = 238,	// UTF-8 text
    it_DjVu             = 239,  
    it_OutlookExpressMessage = 240, // Message in an Outlook Express message store
    it_MimeMessage = 241,		// dtSearch 6.40 and earlier file parser for .eml files
    it_SingleByteText = 242,    // Single-byte text, encoding automatically detected
    it_MBoxArchive    = 243,    // Email archive conforming to the MBOX standard (dtSearch versions 7.50 and earlier)
    it_FilteredBinaryUnicode = 244, // Binary file filtered using Unicode Filtering
    it_ZIP_zlib = 245,				// ZIP file parsed using zlib
	it_OutlookMsgFile = 246,  		// Microsoft Outlook .MSG file
	it_TreepadHjtFile = 247,        // TreePad file (HJT format in TreePad 6 and earlier)
	it_PfsProfessionalWrite = 248,  // PFS Professional Write file
    it_FilteredBinaryUnicodeStream = 249, // Binary file filtered using Unicode Filtering, not split into segments
    it_DocFile = 250,				// Compound document (new parser)
    it_Media = 251,   				// Music or video file
    it_NonTextData = 252, 			// Data file with no text to index
    it_EML = 253,   				// Mime stream handled as a single document
    it_MicrosoftWord = 254,			// Microsoft Word 95 - 2003 (dtSearch versions 6.5 and later)
    it_Utf8F = 255,
    it_UnformattedHTML = 256,		// Output format only, for generating a synopsis that is HTML-encoded but that does not include formatting such as font settings, paragraph breaks, etc.
    it_DatabaseRecord2 = 257,       // Database record (rendered as HTML)
    it_Excel2003Xml = 258,			// Microsoft Excel 2003 XML format
    it_Word2003Xml = 259,			// Microsoft Word 2003 XML format
    it_OpenOfficeDocument = 260, // OpenOffice versions 1, 2, and 3 documents, spreadsheets, and presentations (*.sxc, *.sxd, *.sxi, *.sxw, *.sxg, *.stc, *.sti, *.stw, *.stm, *.odt, *.ott, *.odg, *.otg, *.odp, *.otp, *.ods, *.ots, *.odf) (includes OASIS Open Document Format for Office Applications)
    it_CSV = 261,				   // Comma-separated values file
	it_MicrosoftAccess = 262,	   // Microsoft Access database
	it_XBase = 263,				   // XBase database
	it_OutlookExpressMessageStore = 264,  // Outlook Express dbx archive (versions 7.67 and earlier)
	it_IFilter = 265,              // File type processed using installed IFilter
	it_Gzip = 266,				// Archive compressed with gzip
	it_Excel4 = 267,			// Excel version 4 
	it_Word12 = 268,			// Word 2007
	it_Excel12 = 269,			// Excel 2007
	it_PowerPoint12 = 270,		// PowerPoint 2007
	it_TNEF = 271,				// Transport-neutral encapsulation format
	it_XPS = 272,               // XML Paper Specification (Metro)
	it_WMF = 273,       // Windows Metafile Format (Win16)
	it_EMF = 274, 		  // Windows Metafile Format (Win32)
	it_TAR = 275,			// TAR archive
    it_Excel2           = 277,  // Excel Version 2
    it_Excel3			= 278,	// Excel version 3 
    it_MBoxArchive2    = 279,    // Email archive conforming to the MBOX standard (dtSearch versions 7.51 and later)
    it_Lotus123		   = 280,	// Lotus 123 spreadsheet
	it_MsWorksWps4	   = 281,    // Microsoft Works WPS versions 4 and 5
	it_MsWorksWps6 	   = 282,	 // Microsoft Works WPS versions 6, 7, 8, and 9
	it_QuattroPro 	   = 283,	 // Quattro Pro 9 and newer
	it_QuattroPro8	   = 284,	 // Quattro Pro 8 and older
	it_OutlookMsgAsContainer = 285,  // Outlook .MSG file processed as a container
    it_MimeContainer = 286, 	// MIME-encoded message, processed as a container
    it_PdfWithAttachments = 287,  // PDF file with attachments
    it_MicrosoftAccess2 = 288, // Microsoft Access (parsed directly, not via ODBC or the Jet Engine)
    it_FlashSWF = 289,			// Flash SWF
    it_RAR = 290, 				// RAR archive
    it_NoContent = 291,  		// File indexed with all content ignored (see dtsoIndexBinaryNoContent)
    it_Excel12xlsb = 292,  		// Excel 2007 XLSB format
    it_XfaForm = 293,  			// XFA form 
    it_MifFile = 294,  			// Framemaker MIF file
	it_WordForWindows2 = 295, 	// Word for Windows 2
	it_ASF = 296,				// ASF file	
	it_MP3 = 297,				// MP3 file
	it_QuickTime = 298,			// QuickTime file
	it_MP4 = 299,				// MP4 file
	it_M4A = 300,				// M4A file
	it_MPG = 301,				// MPEG file
	it_JPEG = 302,				// JPEG file
	it_TIFF = 303,				// TIFF file
	it_MDI = 304,				// MDI image file
	it_GIF = 305,				// GIF image file
	it_PNG = 306,				// PNG image file
	it_CAB = 308,				// CAB archive
	it_dtSearchIndex = 309,		// dtSearch index file
	it_HtmlHelp = 310,			// HTML Help CHM file
	it_TrueTypeFont = 311,		// TrueType TTF file
	it_WAV = 312,				// WAV sound file
	it_BMP = 313,				// BMP image file
	it_AVI = 314,				// AVI file
	it_Visio = 315,				// Visio file
	it_Ichitaro = 316,			// Ichitaro word processor file (versions 8 through 2011)
	it_SolidWorks = 317,		// SolidWorks file
	it_Unigraphics = 318,		// Unigraphics file (docfile format)
	it_DWG = 319,				// DWG CAD file
	it_DWF = 320,				// DWF CAD file
	it_DXF = 321,				// DXF CAD file
	it_VisioXml = 322,			// Visio XML file
	it_WordForWindows1 = 323, // Word for Windows 1
	it_OutlookPst = 324,		// Outlook PST message store
	it_MicrosoftOfficeThemeData = 325,	// Microsoft Office .thmx file with theme data
	it_MIDI = 326,			// MIDI file
	it_OutlookExpressMessageStore2 = 327,  // Outlook Express dbx archive 
	it_PowerPoint4 = 328,  // PowerPoint 4
	it_PowerPoint95 = 329, // PowerPoint 95
	it_PowerPoint3 = 330,  // PowerPoint 3
	it_Ichitaro5 = 331,		// Ichitaro versions 5, 6, 7
	it_OleDataMso = 332,   // oledata.mso file
	it_CsvAsReport = 333,  // CSV file parsed as report (like a spreadsheet) instead of a database
	it_CsvAsDocument = 334,  // CSV file parsed as a single file listing all records
	it_MicrosoftAccessAsDocument = 335, // Access database parsed as a single file listing all records
	it_XBaseAsDocument = 336, // XBase file parsed as a single file listing all records
	it_OneNote2003 = 337,  // not supported 
	it_OneNote2007 = 338,  // OneNote 2007
	it_OneNote2010 = 339,  // OneNote 2010, 2013, and 2016
	it_JpegXR = 340,   // Windows Media Photo/HDPhoto/*.wdp
	it_WindowsExecutable = 341, // Windows .exe or .dll
	it_ElfExecutable = 342,  // ELF format executable
	it_ICalendar = 343,  // ICalendar (*.ics) file
	it_MicrosoftPublisher = 344, // Microsoft Publisher file
	it_MicrosoftPhotoEditor = 345,  
	it_PhotoshopImage = 346,  // Photoshop Image (*.psd)
	it_Cals = 347,  // CALS metadata format described in MIL-STD-1840C
	it_iWork2009 = 348, // IWork 2009
	it_iWork2009Pages = 349, // IWork 2009 Pages document
	it_iWork2009Numbers = 350, // IWork 2009 Numbers spreadsheet
	it_iWork2009Keynote = 351, // IWork 2009 Keynote presentation
	it_Visio2013 = 352,  // Visio 2013 document
	it_SASF = 353, // SASF call center audio file
	it_Unigraphics2 = 354, // Unigraphics file (#UGC format)
	it_OneNoteOnline = 355,  // OneNote variant generated by Microsoft online services
	it_OpenOfficeTextDocument = 356,
	it_OpenOfficeSpreadsheetDocument = 357,
	it_OpenOfficePresentationDocument = 358,
// begin-internal
    it_LastInternal     = 1002
// end-internal
    };

// begin-internal
#define it_Xml it_XML

// end-internal

// Values for dtsFileInfo and dtsIndexProgressInfo.fileFlags (C++) and IndexFileInfo (.NET, Java)
enum FileInfoFlags {
	// The document was successfully indexed.
	fiDocumentOK = 0x0000,
	// The document is encrypted.
	fiEncrypted = 0x0001,
	// The document contains no text.
	fiEmpty     = 0x0002,
	// The document is a container.
	fiContainer = 0x0004,
	// The document has an unrecognized file format.
	fiBinary    = 0x0008,
	// The document has a recognized file format but is corrupt.
	fiCorrupt = 0x0010,
	// A portion of the document is encrypted.
	fiPartiallyEncrypted = 0x0020,
	// A portion of the document is corrupt.
	fiPartiallyCorrupt = 0x0040,
	// There was insufficient memory to process the document.
	fiOutOfMemory = 0x0080,
	// The document could not be opened due to a file open failure.
	fiOpenFailed = 0x0100,
	// The document is in an unrecognized format and is being indexed using the binary filtering algorithm.
	fiFilteredBinary = 0x0200,
	// The document contains images without text, other than metadata and annotations 
	fiImageOnly = 0x0400,
	// begin-internal
	fiReservedzb = 0x8000
	// end-internal

	};

// Values for WordListBuilder.SetFlags
enum WordListBuilderFlags {
	/*
	Enable caching of field values in a WordListBuilder for faster ListFieldValues calls 
	*/
	dtsWordListEnableFieldValuesCache = 1
	};

/* Summary
   Contains information about a file being indexed.
   Remarks
   Passed in indexing status notifications embedded in a dtsIndexProgressInfo. */
struct dtsIndexFileInfo {
    const char  *name;				// Name of the document
    const char  *location;			// Directory where the document is located
    const char  *type;  			// Name of the file type of the document (WordPerfect, Word for Windows, etc.)
    const char  *openFailMessage; 	// If the file could not be accessed, openFailMessage will point to a string describing the reason for the failure
    long size;						// Size of the document, in bytes (superseded by size64)
    long bytesRead;   				// Number of bytes of the file processed so far.  (Deprecated -- use size64 * percentDone / 100)
    long wordCount;					// Number of words extracted from the file, so far
    short percentDone;				// Percentage of the file that has been processed
    unsigned short typeId;			// TypeId identifying the file type of this document
    long docId;						// The document id assigned to this document in the index
    const char *displayName;		// User-friendly name to display for this document
    const char *fullname;  			// Full path and filename
	__int64 size64;                 // Size of the document, in bytes

	__int64 getSize() {
		if (size64)
			return size64;
		else
			return size;
		}
	void setSize(__int64 s) {
		size64 = s;
		if (s > 0x7fffffff)
			size = 0;
		else
			size = (long) s;
		}
    };

// Information about a file being searched in an unindexed search.
struct dtsSearchFileInfo {
	long structSize;
	// Name of the file
	const char *name;
	// File type of the file (Word, PDF, etc.)
	const char *type;
	// If the file could not be accessed, a message providing information about the reason for the failure
	const char *openFailMessage;
	// Size of the file
	__int64 size;
	// TypeId value specifying the file format of the document
	long typeId;
	// FileInfoFlags value specifying whether the file was encrypted, corrupt, etc.
	long fileInfoFlags;
	// Number of words in the file
	long wordCount;
	#ifdef __cplusplus
	dtsSearchFileInfo();
	#endif
	};

// Reported through the status notification callback in a dtsSearchJob, and the OnProgressUpdate virtual function
// callback in DSearchJob
struct dtsSearchProgressInfo {
	long structSize;
	// A MessageCode identifying the reason for this update
	long updateType;
	// Information about the file being searched (unindexed searches only)
	dtsSearchFileInfo *currentFile;
	// Number of disk files to be searched (unindexed searches only)
	long filesToSearch;
	// Number of files searched so far (unindexed searches only)
	long filesSearched;
	// Total hits count for all documents retrieved
	long totalHitCount;
	// Total number of documents retrieved
	long totalFileCount;
	// The path of the index being searched (indexed searches only)
	const char *currentIndexPath;
	// Information on the item found, if this is a dtsnSearchFound notification
	dtsSearchResultsItem *pFoundItem;

	// begin-internal
    intptr_t reserved[5];
	#ifdef __cplusplus
	dtsSearchProgressInfo();
	void copy(const dtsSearchProgressInfo& other);
	#endif
	// end-internal
	};

// Reported through the status notification callback in a dtsIndexJob
struct dtsIndexProgressInfo {
	// A MessageCode identifying the reason for this update
    long updateType;
    long step;  /* IndexingStep enum */
    // Name of the index being updated
    const char  *indexName;
    // Location of the index being updated
    const char  *indexPath;
    // Drive where the index is located
    const char  *driveName;
	// The percentage of the files that have been indexed
    long percentDone;
    // Number bytes to index.  Deprecated: Use bytesToIndexKB or bytesToIndex64
    long bytesToIndex;
    // Number of files to index
    long filesToIndex;
	// Number of files in the selected folders to check to see if they have been indexed
    long filesToCheck;
    // Number of files in the selected folders that have been checked to see if they need to be indexed
    long filesChecked;

	// Kilobytes of document data that have been read so far
    long docBytesReadKB;
    // Bytes of document data that have been read so far.  Deprecated: Use docBytesReadKB
    long bytesRead;
    // Number of files read so far
    long filesRead;
    // Number of documents read so far (a single file may include more than one document)
    long docsRead;

    /* Current file info */
    struct dtsIndexFileInfo file;

    /* Merge status info */
    long docBytesMerged;      /* bytes of docs that have been fully processed */
    long currMergePercent;    /* How far along is the current merge process */

    // Number of documents in the index
    long docsInIndex;
    // Number of unique words in the index
    long wordsInIndex;

	// Seconds since the index job started
    long elapsedSeconds;
    // Deprecated -- use diskAvail64
    long diskAvail;           

	// Number of files not indexed because they were determined to be binary. (Handling of binary files is controlled by dtssSetOptions.)
    long binaryCount;
    // Number of files not indexed because they could not be accessed
    long openFailures;
	// If step is ixsCompressing, the percentage of the index that has been compressed
    long compressPercentDone;
    // Number of files not indexed because they were encrypted.
    long encryptedCount;
	// Deprecated -- use diskAvail64
    long diskAvailKB;
    // Number of deleted documents removed from the index
    long docsRemovedFromIndex;
    // Total size of the files to be indexed, in kilobytes
    long bytesToIndexKB;
    // Total size of the files indexed so far, in kilobytes
    long bytesReadKB;
    long docBytesMergedKB;
    // dtsAbort, dtsContinue, or dtsAbortImmediate, depending on whether this index job was cancelled
    long cancelFlag;
    // Percentage of the maximum possible size of this index
    long indexPercentFull;
    // Estimated number of seconds remaining in the index job
    long estRemaining;
    // Number of field names in the index
    long fieldCount;
    // Total size of the files to be indexed
	__int64 bytesToIndex64;
    // Total size of the files indexed so far
    __int64 bytesRead64;
	// Total amount of document data that have been read so far.
	__int64 docBytesRead64;
	// Number of documents that were partially encrypted
	long partiallyEncryptedCount;
	// Number of documents that were partially corrupt
	long partiallyCorruptCount;

	// FileInfoFlags value with the result of the last file indexed
	long fileInfoFlags;

// begin-internal
	long structSize;
    long reserved[3];
// end-internal
	// Number of folders listed in the IndexJob that could not be accessed to scan for documents to index
	long inaccessibleFolderCount;
	// Modified date of the last file indexed
	dtsFileDate fileModifiedDate;
	// Stored fields from the document that was indexed (on dtsnIndexFileDone notification only)
	// This is a series of field-value pairs passed as a null-delimited string set (a series of string values delimited by null characters,
	// and terminated by a double-null).
	const char *userFields;
	// Amount of disk space available, or -1 if the amount of free disk space could not be determined
	__int64 diskAvail64;
// begin-internal
	void *reserved2[5];
#ifdef __cplusplus
    dtsIndexProgressInfo();
    long getPercentDone() {
		if (step == ixStepCompressing)
			return compressPercentDone;
		else
			return percentDone;
		}
	void copyNumbers(const dtsIndexProgressInfo& other);
#endif
// end-internal
    };

/*
 *   Base class for dtsSearchJob, dtsIndexJob, dtsFileConvertJob, etc.
 */
struct dtsJobBase {
    long structSize;
    /* Pointer to a function to be called with status report information during the job.
       If you do not want status report information, set pReportCallBack to NULL. See
       dtsMessage for more information on handling these callbacks.                      */
    int  (*pReportCallBack)(void  *pReportData, dtsMessage& message);
    // User data that will be passed to the status report callback function
    void  *pReportData;
    // When the job is completed, cancelFlag will be set to dtsAbort if the job was cancelled.
    long cancelFlag;
    // Pointer to a dtsErrorInfo to store any error messages generated during the job. If you do not want error information, set the errorHandler to NULL
    dtsErrorInfo *errorHandler;
    // If you want the job to automatically halt after a specified amount of time, set the timeoutSeconds to a positive value. If the job times out, the errorHandler will contain a message with the code dtsErTimeout
    long timeoutSeconds;
// begin-internal
    dtsJobOptionsHandle jobOptions;
    long reserved[2];
    void reportError(int code, const char *msg = 0, const char *arg1 = 0, const char *arg2 = 0);
// end-internal
    };

#define IndexNameLen    40
#define FileNameLen     512

/* Summary
   Used to define a set of files to search or index
   Remarks
   fileListName provides a way to simply pass a list of files to search or index.

   Alternatively, the calling program can ask dtSearch to list the files in one
   directory (the dir member) or in a list of directories (the dirListName member),
   using the supplied include and exclude filters.

   To specify a directory and all subdirectories, add a \<+\> symbol at the end,
   like this: C:DOCS\<+\>                                                           */
struct dtsFileSpec {
    /* Filename filters to apply to files in the directories
       selected. Must point to a null-delimited string set,
       terminated by a double-null. If not used, initialize to 0.

       Filename filters containing a slash (/ or \\) are matched
       against the full path and filename. Filename filters without
       a slash (/ or \\) are matched against only the filename.     */
    const char  *includeFilters;
    /* Filename filters used to exclude files in selected
       directories. Must point to a null-delimited string set,
       terminated by a double-null. If not used, initialize to 0.

       Filename filters containing a slash (/ or \\) are matched
       against the full path and filename. Filename filters without
       a slash (/ or \\) are matched against only the filename.     */
    const char  *excludeFilters;

    /* Directory to scan using the filters.  Deprecated: Use dirList. */
    char dir[FileNameLen];

    /* Name of file containing list of directories to scan using the filters */
    char dirListName[FileNameLen];

    /* Summary
       Name of file containing list of files.
       Remarks
       The file should be a plain text file, in UTF-8 or Unicode text format, with one
       filename per line.

       The includeFilters and excludeFilters will not apply to listed files.           */
    char fileListName[FileNameLen];

    /* Null-delimited string set containing list of directories to scan using the filters */
    const char  *dirList;

// begin-internal
    /* Reserved for internal use.  Must be initialized to 0 */
    const char  *fileConditions;
    long reserved[4];
// end-internal
#ifdef __cplusplus
	// Constructor
    dtsFileSpec();
    // Clear all data
    void clear();
    // Copy data from another dtsFileSpec (shallow copy)
    dtsFileSpec& copy(const dtsFileSpec& other);
#endif
    };

struct dtsDataSource;

/* Summary
   Used to modify words, or create additional words, as text is indexed.
   Remarks
   The dtsOnIndexWordInfo structure is used with the pOnIndexWordFn callback in
   dtsIndexJob to provide a way to modify the words in a document as they are being
   indexed. Some possible uses of this callback include: (1) Customizing the
   character handling for certain characters; (2) Excluding certain words from being
   indexed; or (3) adding variations on a word.

   \Example (from the textdemo sample application):

   <code>
   // Demonstrates how the OnIndexWord callback function can be used to
   // modify text being indexed. In this case, each word is indexed along with an
   // inverted version of the word.
   void OnIndexWord_Inverter(void *pData, dtsOnIndexWordInfo* pInfo)
   {   // First time we are called for a word, just request another callback
       if (pInfo-\>iWhichCallback == 0) {
           pInfo-\>nAdditionalWords = 1;
           return;
           }

       // Second time we are called for a word, return the inverted version
       if (pInfo-\>iWhichCallback == 1) {
           int l = strlen(pInfo-\>originalWord);
           if (l \>= sizeof pInfo-\>wordToIndex)
               l = sizeof pInfo-\>wordToIndex-1;
           for (int i = 0; i \< l; ++i)
               pInfo-\>wordToIndex[i] = pInfo-\>originalWord[l-i-1];
           pInfo-\>wordToIndex[l] = '0';
           }
   }
   </code>                                                                           */

struct dtsOnIndexWordInfo {
    dtsOnIndexWordInfo();
    long structSize;
    // The original text of the word being indexed.  Read-only.
    const char *originalWord;
    // The word offset (one-based) of the word being indexed.  Read-only.
    long positionInFile;

    /* The first time the callback is called for a word, iWhichCallback will be
	zero.  If nAdditionalWords is returned to request additional callbacks,
	iWhichCallback will be incremented once for each callback */
    long iWhichCallback;
    /* The callback function can set fShouldIndex to false to prevent a word
	from being indexed.  The dtSearch Indexer will skip the word as if it was a
	noise word */
    long fShouldIndex;
    /* If you want to provide additional variations on the word, in the same
	word position in the file, set nAdditionalWords to the number of variations
	to be added.  The callback function will be called once for each variation.
	On each callback, the function should fill in an alternative form of the
	word in wordToIndex. */
    long nAdditionalWords;
    /* wordToIndex can be used on the first callback to modify the text of the
	word to be indexed, and on additional callbacks it can be used to provide
	alternative forms of a word (alternative spellings, synonyms, equivalents in
	other languages, etc.) to be indexed in the same word position. */
    char wordToIndex[512];
	// begin-internal
    long reserved[4];
    // end-internal
    };

// Specifies whether to create, add documents to, compress, or remove items from an index in a dtsIndexJob.
struct dtsIndexJobAction {
	// Create an index in indexPath.
	short create;
	// Add documents to an index. The documents to be added are specified in toAdd.
	short add;
	// Compress the index. Compressing an index removes obsolete information from the index
	short compress;
	// Remove the files listed in toRemoveList from the index
	short remove;
	// Check that each file in the index still exists on disk. Remove from the index any files that no longer exist.
	short removeDeleted;
	// Set one or more elements to TRUE to define the tasks to be performed.  See "Building and Maintaining Indexes" for more information on indexing actions.
	};

// In a dtsIndexJob, specifies whether the index to create should be case-sensitive, accent-sensitive, or use relative paths
struct dtsIndexJobCreateFlags{
	// Index will treat words with different capitalization as different words. (apple and Apple would be two different words.)
	short caseSensitive;
	// Index will preserve accents when indexing words.
	short accentSensitive;
	// Use relative rather than absolute paths in storing document locations
	short relativePaths;
	// Obsolete
	short addToLibrary;
	};

/* Summary
   Describes an indexing task to be performed.
   Remarks
   Call dtssDoIndexJob to execute a dtsIndexJob. */

#ifdef __cplusplus
struct dtsIndexJob : public dtsJobBase {
	// begin-internal
#else
struct dtsIndexJob {
    long structSize;
    int  (*pReportCallBack)(void  *pReportData,
        struct dtsMessage  *message);
    void  *pReportData;
    long cancelFlag;
    struct dtsErrorInfo *errorHandler;
    long timeoutSeconds;
    long reserved[3];
#endif
	// end-internal
	// Name of the index to create. An index name is not necessary to access an index (the index path is always sufficient).
    char indexName[IndexNameLen];
    // Directory where the index is located. The index will consist of a series of files named index*.ix
    char indexPath[FileNameLen];

    /* Obsolete.  Concurrent access is always supported. */
    short allowConcurrentAccess;

	// Specifies whether to create, add documents to, compress, or remove items from the index.
	dtsIndexJobAction action;
	// Specifies whether the index to create should be case-sensitive, accent-sensitive, or use relative paths
	dtsIndexJobCreateFlags createFlags;

    /* Specifies files to be added to the index if action.add is true */
    struct dtsFileSpec toAdd;

    /* Name of file containing a list of files to be deleted from the index */
    char toRemoveList[FileNameLen];

	//
	// Folder to use instead of the index folder for temporary files during indexing.
	//
	// During indexing, the dtSearch Engine may need to create temporary files to store
	// word lists that are too large to fit into memory.  By default, these files will be
	// placed in the index folder.  Use this setting to specify a different location
	// for the word lists.  The indexer will automatically delete the word list files
	// when the index update completes.
    const char  *tempFileDir;

// begin-internal
    /* Obsolete */
    long maxTempFileSize;
// end-internal

	/* Pointer to a dtsDataSource. Initialize to NULL if you are indexing files
	selected with the toAdd member. The dataSourceToIndex member is useful if
	you need to index data that is not readily available as a disk file, such as
	records from a SQL database */
    struct dtsDataSource *dataSourceToIndex;

    /* Summary
       Used to specify that the text in certain fields should be collected, stored in
       the index, and returned in searches.
       Remarks
       To specify the fields to be stored, set storedFields to point to a null-delimited
       list of field names, terminated with a double-null. The field names in the list
       can contain wildcards (* and ?). A set containing a single entry "*" would match
       all fields, causing the text of every field to be stored in the index.

       The maximum amount of text that can be stored in each stored field is controlled
       by dtsOptions.maxStoredFieldSize.

       For more information, see <link Retrieving Fields in Search Results>              */
    const char *storedFields;

	// Flags controlling the indexing process.  See IndexingFlags enumeration */
    long indexingFlags;

	// Maximum size of a document to cache in the index, if caching is enabled for this index
    long maxTextToCache;

	// Automatically commit index changes to disk after this many megabytes have been indexed
    long autoCommitIntervalMB;

    /* Summary
       MaxMemToUseMB controls the size of the memory buffers that dtSearch can use to
       sort words during indexing.
       Remarks
       If MaxMemToUseMB is zero, dtSearch will decide the amount of memory to use based
       \on the estimated amount of text to be indexed and the amount of system memory
       available.

       If possible, dtSearch will use memory for all sorting operations; otherwise, some
       disk-based buffers will be used. For large updates, some disk-based sort buffers
       are always necessary and there is little benefit to MaxMemToUseMB values above
       512 (32-bit) or 2048 (64-bit).

       MaxMemToUseMB does not affect other memory that may be used during indexing for
       \other purposes such as parsing document formats.                                 */
    long maxMemToUseMB;

	// The pOnIndexWordFn can be used to provide a callback to modify the text of words as they are being indexed.  See dtsOnIndexWordInfo for more information.
    void  (*pOnIndexWordFn)(void *pOnIndexWordData,
        struct dtsOnIndexWordInfo *info);
	// Data to pass to pOnIndexWordFn
    void *pOnIndexWordData;

	/* Summary
	   If non-zero, the first doc id to assign to documents in this index.
	   Remarks
	   By default, the dtSearch indexer assigns each document a numerical id starting
	   with 1 in each index. These ids are used in SearchFilters and in other places in
	   the dtSearch Engine API to identify documents within an index. When an index is
	   created, startingDocId can be used to specify that doc ids for the index should
	   start with a value other than 1. This can be useful in situations where it is
	   important that doc ids not change when indexes are merged.

	   Each DocId value in an index requires one bit in a SearchFilter, so a
	   SearchFilter for an index with DocIds ranging from 1 to 1,000,000 would require
	   125,000 bytes of memory. Note that this means there is a performance cost to
	   using unnecessarily high values for startingDocId.
	   See Also
	   <link Document Ids>                                                              */
	long startingDocId;

// begin-internal
    long reserved_i[4];
// end-internal
    /* Summary
       Used to specify that the text in certain fields should be stored in the index in
       a way that permits unique field values to be enumerated quickly.
       Remarks
       When a field is listed in enumerableFields, all of the values of that field are
       stored in the index in a way that permits the values to be enumerated using
       WordListBuilder.ListFieldValues. To specify the fields, set enumerableFields to
       point to a null-delimited list of field names, terminated with a double-null. The
       field names in the list can contain wildcards (* and ?). A set containing a
       single entry "*" would match all fields.

       The maximum amount of text that can be stored in each stored field is the lesser
       \of the maxStoredFieldSize option setting or 128 characters.

       All enumerable fields are also automatically designated as stored fields (see
       storedFields).
       See Also
       <link dtsWordListBuilder::listFieldValues@char *@char *@long, dtsWordListBuilder::listFieldValues Method>

       <link Retrieving Fields in Search Results>
                                                                                                                 */
    const char *enumerableFields;

#ifdef __cplusplus
    dtsIndexJob();
    // Clear all data in a dtsIndexJob
    void clear();
    // Copy another dtsIndexJob (shallow copy)
    dtsIndexJob& copy(const dtsIndexJob& other);
#endif
    };

// begin-internal

/*  OBSOLETE:  use dtsSearchResults instead
 *   dtsRetrievedFileTable is used to get search results, sorted
 *   by relevance, without writing a search results file or listing
 *   to disk.  It consists of a table of dtsRetrievedFiles,
 *   each of which holds information about a single retrieved file.
 *
 *   Because a fixed-length memory buffer is used for this purpose,
 *   the amount of information returned is limited.  The number of
 *   files to be returned is limited to the size of the table, and
 *   the number of hits returned for each file is limited to the
 *   number that will fit in the hits[] member of each file's
 *   dtsRetrievedFile struct.
 *
 */

#define dtsRfMaxHits 126

struct dtsRetrievedFile {
    struct dtsFileDate modified;
    long size;
    long wordCount;
    long hitCount;
    long score;
    long hitsStored;
    long hits[dtsRfMaxHits];
    long docId;
    short iIndex;
    short reserved;
    char filename[256];
    };

struct dtsRetrievedFileTable {
    struct dtsRetrievedFile *fileTable;
    long  *sortOrder;
    long maxCount;
    long fileCount;
#ifdef __cplusplus
    dtsRetrievedFileTable(long aMaxCount);
    ~dtsRetrievedFileTable();
    void copy(const dtsRetrievedFileTable& other);
    void allocTable(long aMaxSize);
    void clear();
    dtsRetrievedFile  *getInOrder(int i);
#endif
    };
// end-internal

 // Describes action to perform in a dtsSearchJob
 struct dtsSearchJobAction {
	 short searchFiles;      /* unindexed search */
	 short searchIndexes;	 /* indexed search */
 	};

/* Summary
   Define a search to be performed.
   Remarks
   A dtsSearchJob is used in the dtssDoSearchJob function call to define a search to
   be performed
   * Search criteria *
   There are three components to the <link Search Requests Overview, search request>
   that can be included in a dtsSearchJob:

   (1) request2: The search request, which can be in the "<link Boolean Search Requests, boolean>",
   "all words", or "anywords" syntax, depending on the value of searchFlags2.

   (2) booleanConditions: A string in the boolean syntax expressing any conditions
   that must be satisfied in addition to whatever is in request2. This provides a
   way to add boolean conditions (such as field criteria) to a user-supplied query,
   which may be in the all words or any words syntax.

   (3) fileConditions2: A string expressing conditions such as filename matches,
   \file size matches, etc., that documents must satisfy. See <link File Conditions, FileConditions>
   for the syntax.
   * Search results *
   Search results are obtained using a search results object. To set up a search
   job, declare a dtsSearchResults object to hold the results and then attach its
   handle to dtsSearchResults.resultsHandle. After the search, you can use the
   dtsSearchResults object to access the results of the search.
   Example
   <code>
           dtsSearchResults searchResults;
           dtsSearchJob searchJob;
           // ... set up searchJob ...
           searchJob.resultsHandle = searchResults.getHandle();
           \short errorFlag;
           dtssDoSearchJob(searchJob, errorFlag);
           // .. now searchResults contains the results of the search
   </code>
   * Search filters *
   A search filter represents a set of documents that can be returned in a search.
   To limit a search to the documents in a search filter, set up a dtsSearchFilter
   \object with the filter to use, and set dtsSearchResults.searchFilterHandle to
   the handled returned by dtsSearchFilter.getHandle(). Example:

   <code>
           dtsSearchFilter searchFilter;
           // ... read searchFilter from disk, or generate it by searching, or set allowed values
           // directly using searchFilter.selectItems() ...
           dtsSearchJob searchJob;
           // ... set up searchJob ...
           searchJob.searchFilterHandle = searchFilter.getHandle();
           // Now when searchJob is executed, only documents identified by searchFilter can be
           // retrieved.
   </code>

   To generate a search filter based on the results of a search, set the
   \outputSearchFilterHandle to the handle of a dtsSearchFilter. Every document
   retrieved in the search will be selected in the output search filter. Note:
   maxFilesToRetrieve does not affect outputSearchFilter, which will include *every*
   item retrieved in the search. However, if autoStopLimit causes the search to
   terminate before it is complete, then only items found before the search stopped
   will be selected in the dtsSearchFilter.                                                          */

#ifdef __cplusplus
struct dtsSearchJob : public dtsJobBase {
	// begin-internal
#else
struct dtsSearchJob {
    long structSize;
    int  (*pReportCallBack)
    void  *pReportData;
    long cancelFlag;
    struct dtsErrorInfo *errorHandler;
    long timeoutSeconds;
    long reserved[3];
#endif
	// end-internal

	// Deprecated: use request2
    char request[256];
    // Deprecated; use fileConditions2
    char fileConditions[256];

	// begin-internal
	// Obsolete, and ignored
    short sortType;
#ifdef __cplusplus
    int  (*getSortType)(dtsSearchJob& searchJob);
#else
    int  (*getSortType)(struct dtsSearchJob *searchJob);
#endif
    // end-internal
    // Type of search to perform.  Set action.searchIndexes=true for an indexed search,
    // and action.searchFiles=true for an unindexed search.
    struct dtsSearchJobAction action;

	// Identifies files to search in an *unindexed search only*
    struct dtsFileSpec filesToSearch;

	// Null-delimited string set identifying indexes to search.
	// For each index, supply the full path to the index folder.
    const char *indexesToSearch;

	// begin-internal
	// Obsolete -- use resultsHandle to obtain search results
    struct {
		/* dtSearch Search Results file to create -- for internal use only*/
        char resultsName[FileNameLen];
        /* If set to a filename, each file retrieved in the search will be
		written to this file. If the dtsSearchDelayDocInfo flag is set and a
		dtsSearchResults object is provided in the resultsHandle, the list will
		be built from the best-matching files after the search is done, sorted
		by relevance, and will contain at most maxFilesToRetrieve items.
		Otherwise, the list will include all files retrieved in the search, in
		the order they were found. */
        char listingName[FileNameLen];  /* Plain utf-8 text file */
        // Determines the contents of a search results listing created in output.listingName.
        // See the ListingFlags enum.
        long listingFlags;
        } output;
	// end-internal

	// Following a search, contains the number of files retrieved
    long totalFiles;
    // Following a search, contains the number of hits retrieved in all files.
    long totalHits;

	// begin-internal
    void  *pUserData;  // not used
    // end-internal

	/* A null-terminated string containing the search request. See dtSearch
	User's Manual for information about search requests. The string can be up to
	32,000 characters in length. */
    const char *request2;

	/* Handle of a dtsSearchResults to store the results of this search. To obtain a handle,
	declare a dtsSearchResults object and call its getHandle() method.  After the search, you can
	use the dtsSearchResults object to access the results of the search.

	Example:
	<code>
		dtsSearchResults searchResults;
		dtsSearchJob searchJob;
		// ... set up searchJob ...
		searchJob.resultsHandle = searchResults.getHandle();
		short errorFlag;
		dtssDoSearchJob(searchJob, errorFlag);
		// .. now searchResults contains the results of the search
	</code>
	*/
    void *resultsHandle;
    /* In an indexed search, filename, date, or size criteria that a file must meet to be retrieved.
    If both fileConditions and a request are provided, both must be matched. */
    const char *fileConditions2;
    /* In an unindexed search, the contents of the dtsDataSource will be
	searched along with any files specified by filesToSearch. */
    dtsDataSource *dataSourceToSearch;

    /* A per-search override to the dtsOptions.maxWordsToRetrieve setting */
    long maxWordsToRetrieve;

	// Obsolete
    struct dtsRetrievedFileTable *resultsBuffer;

	// Obsolete -- use maxFilesToRetrieve2;
    short maxFilesToRetrieve;

	/* UnindexedSearchFlags values controlling an unindexed search */
    short unindexedSearchFlags;

	// Obsolete -- use searchFlags2
	short searchFlags;

	// If non-zero, the engine will match words that are close to but not identical to a search term.
	// Fuzziness can range from 1 to 10. The higher the fuzziness value, the more differences are
	// tolerated when matching words, and the closer to the front of the word these differences can occur.
    short fuzziness;

	/* Set to a non-zero value to make the search automatically stop when this
	many documents were found.

	AutoStopLimit causes a search to halt automatically when a specified number
	of documents have been found. This provides a way to limit the resources
	consumed by searches that retrieve a very large number of documents. When
	the number of files retrieved reaches the autoStopLimit value, the search
	will immediately halt, and the code dtsErSearchLimitReached will be returned
	through the error handler for the search job. If the AutoStopLimit is 0, the
	search will not halt.
	*/
    long autoStopLimit;

    /* SearchFlags values controlling various search options */
    long searchFlags2;

    /* Handle to a dtsSearchFilter to use to limit the documents that can be retrieved in this search.
    */
    dtsSearchFilterHandle searchFilterHandle;
    // Additional boolean conditions that a document must meet to be retrieved in a search.
    const char *booleanConditions;
    /* \ \
       Summary
       Limit the maximum size of search results to a specified number of files.
       Remarks
       When maxFilesToRetrieve2 is non-zero, it controls the maximum size of the search
       results (dtsSearchResults) that can be returned after a search. The most relevant
       documents from all matching documents in the index will be included in search
       results.

       Regardless of the maxFilesToRetrieve2 setting, totalFiles will return the a count
       that includes <i>all</i> matching documents. maxFilesToRetrieve2 limits the
       number of items saved in the returned dtsSearchResults, but it does not stop the
       search before all matching documents have been identified.

       For example, if you set maxFilesToRetrieve=10 and autoStopLimit=5000, then search
       results will contain the 10 most relevant documents from the first 5000 found.
       Documents after the first 5000 found will not be considered, because
       autoStopLimit=5000 will force the search to halt after 5000 matching documents
       are found.

       If the dtsSearchSelectMostRecent flag is set in searchFlags2, the most <i>recent</i>
       documents will be returned instead of the most relevant.                             */
    long maxFilesToRetrieve2;
    /* A search filter to hold the results of the search (possibly in addition
	to resultsHandle).  If outputSearchFilterHandle is the handle of a valid
	dtsSearchFilter object, then every document retrieved in the search will
	also be selected in the search filter */
    dtsSearchFilterHandle outputSearchFilterHandle;

	/* Summary
	   Specify a relevance weight for hits that occur in listed fields (example:
	   Subject:10,HtmlTitle:20)
	   Remarks
	   FieldWeights is a list of field names, with a weight indicated after each field
	   name, like this:
	   <code>
	       Subject:10,HtmlTitle:20,Description:15
	   </code>
	   Hits that occur in one of these fields will count more heavily than other hits,
	   according to the weight you specify. For more information, see "<link Relevance>"
	   in the "Overviews" section of the API documentation.                              */
	const char *fieldWeights;
// begin-internal
    long reserved2[3];

// end-internal
    // Optional handle to a dtsIndexCache to use for faster high-volume searching
    dtsIndexCacheHandle indexCacheHandle;
// begin-internal
	const char *pReserved;
	const char *pReserved2;
// end-internal    
#ifdef __cplusplus
    dtsSearchJob();
    // Clear all data
    void clear();
    long getMaxFilesToRetrieve() {
        if (maxFilesToRetrieve2)
            return maxFilesToRetrieve2;
        else
            return maxFilesToRetrieve;
        }
    // Copy another dtsSearchJob (shallow copy)
    dtsSearchJob& copy(const dtsSearchJob& other);
#endif

    };

// begin-internal

/* Status flags returned by the DLL following initialization (in dllFlags) */
#define dtsDllLimitedSearching  0x1L

/* DLL iniitalization error codes */
#define dtsInitFailMissingFile 0x1L
#define dtsInitFailBadPrivateDir  0x2L

/* Return values for the question-asking functions in dtsInitInfo */
#define AnswerNo            0
#define AnswerYes           1
#define AnswerYesToAll      2
#define AnswerNoToAll       -2

/* Obsolete
   Older dtSearch Engine versions had to be initialized at startup and used a
   global table of function pointers to handle message callbacks.

   dtsOptions can be used to setup all of the option settings in dtsInitInfo,
   so there is no reason to use dtsInitInfo anymore.
*/
struct dtsInitInfo {
    /* Size of the structure, in bytes */
    long structSize;

    /* Obsolete - Code that enables LZW decoding of PDF files */
    long lzwEnableCode;

    /* Directory to use for user setup files and temporary files.
     * Each user must have its own private, writable directory.   */
    char privateDir[FileNameLen];

    /* Directory where dtSearch files are kept.  This directory
     * can be a shared network directory with read-only access.  */
    char homeDir[FileNameLen];

    /* Functions to call to display error messages or ask questions */
    void (*pShowErrorFn)(void  *pData, const char  *msg);
    int  (*pAskYesNoFn)(void  *pData, const char  *msg);
    void (*pShowInfoFn)(void  *pData, const char  *msg);
    void (*pBeepFn)(void  *pData);
    void  *pData;

    long reserved2;

    /* Display a question the answer to which may be yes, yes to all,
     * no, no to all   */
    int (*pAskYesNoAllFn)(void  *pData, const char  *msg);

    /* Initialization error information */
    long errorCode;

    /* Reserved for internal use */
    const char  *errorText;
    void (*pTraceFn)(void  *pData, int msgType, const char  *msg);
    dtsErrorInfo *errorHandler;
    long reserved[3];
    const char *licenseKey;
#ifdef __cplusplus
    dtsInitInfo();
#endif
    };

struct dtsIndexInfoInt;

// end-internal

// Indicates whether the index is case-sensitive, accent-sensitive, or uses relative paths
struct dtsIndexInfoFlags {
	// true if this is an accent-sensitive index
	char accentSensitive;
	// true if this is a case-sensitive index
	char caseSensitive;
	// true if this index uses relative paths to store document locations
	char relativePaths;
	};

/* Holds information about an index supplied by dtssGetIndexInfo. */
struct dtsIndexInfo {
	// Initialize to sizeof(dtsIndexInfo).
    long structSize;
    // Number of documents in the index
    long docCount;
    // Number of unique words in the index
    long wordCount;
    // Number of directories indexed
    long dirCount;
    /* Measures the extent to which an index is fragmented by being built in
	multiple indexing jobs. Compressing an index eliminates fragmentation.
	Fragmentation of an index increases the size of the index and slows
	searching, but the effect is generally not noticeable unless the
	fragmentation is severe. Indexing documents in very small batches can
	produce a severely fragmented index, but otherwise index fragmentation can
	be disregarded. */
    long fragmentation;
    /* When a document is indexed, modified, then indexed again, information
	 about the document in the index is marked as obsolete and new information
	 about the document is appended to the index. obsoleteCount is the number of
	 documents for which obsolete information is stored in the index.
	 Compressing an index removes all obsolete information. */
    long obsoleteCount;
    // The name of the index that was supplied when the index was created.
    char name[IndexNameLen];
    // When index was created
    struct dtsFileDate created;
    // When index was last updated
    struct dtsFileDate updated;
    // When index was last created
    struct dtsFileDate compressed;

    // Indicates whether the index is case-sensitive, accent-sensitive, or uses relative paths
    dtsIndexInfoFlags flags;

    // Size of the index, in bytes.  Deprecated: use indexSize64 instead.
    long indexSize;

	// begin-internal
    char creator[32];
    // end-internal

    // Size of the index, in bytes
    __int64 indexSize64;
    /* Indicates how much of the maximum capacity of the index has been used  (This is a percentage of the maximum index capacity and does not consider the amount of disk space actually available.
    */
    long percentFull;
    /* Number of unique words in the index */
    __int64 wordCount64;

	/* Index structure version.  Values currently supported:

		600 - dtSearch 6.x index format.
		700 - dtSearch 7 index format (2004 beta versions)
		701 - dtSearch 7 index format
	*/
    long structureVersion;
    /* If true, an index update was halted before it completed, and the indexer saved information that
    can be used to resume the index update */
    long canResumeUpdate;
    /* Flags indicating the index type (see IndexingFlags) */
    long indexFlags;
    /* Number of fields in the index */
    long fieldCount;
    // First document id assigned in this index
    long startingDocId;
    // Last document id assigned in this index
    long lastDocId;

	// The total size of all documents added to this index (including documents indexed multiple times)
	__int64 totalDataSize;
	// begin-internal
    float fragmentationRatio;
    long reserved[3];
    dtsIndexInfoInt *pIndexInfoInt;
    char reserved3[256];
#ifdef __cplusplus
    dtsIndexInfo();
    void clear();
    void copy(const dtsIndexInfo& other);
#endif
	// end-internal
    };

/*  Struct passed to dtssSetOptions and dtssGetOptions to control indexing
 *  options.  To change one or more options, first call dtssGetOptions to
 *  fill the struct with the existing values, change values as needed,
 *  and then call dtssSetOptions.
 */
struct dtsViewerInfo;
struct dtsLanguageAnalyzerInterface;

/* Summary
   Sets dtSearch Engine indexing and searching option settings.
   Remarks
   Deprecated -- use dtsOptions2 instead of dtsOptions.

   To change option settings,
     1. Call dtssGetOptions to get the option settings currently in effect
     2. Change the values in dtsOptions as needed, and
     3. Call dtssSetOptions to apply the changes
   Option settings are not persisted anywhere so changes must be made each time a
   new program instance starts.

   Option settings apply to the current thread and any threads created after the
   current thread. Therefore, each thread can have its own settings.              */

struct dtsOptions {
    long structSize;
    // BinaryFilesSettings value specifying the treatment of binary files.
    long binaryFiles;
    // Define characters considered to be text if binaryFiles is set ot dtsoFilterBinary
    char binaryFilterTextChars[256];
    // HyphenSettings value specifying the treatment of hyphens in text
    long hyphens;
    /* Name of dtSearch alphabet file to use when parsing text into words.

    	Remarks:
    	The	alphabet file determines which characters are letters and what the rules are
	for handling capitalization and punctuation. If blank, dtSearch will use
		a default alphabet.

	The alphabet file only controls the indexing of characters in the range
	32-127. Unicode character properties control the treatment of other
	characters. */
    char alphabetFile[FileNameLen];
    // If false, any word that begins with a digit will not be indexed.
    long indexNumbers;
    /* List of noise words to skip during indexing (default: "noise.dat") A
	noise word is a word such as the or if that is so common that it is not
	useful in searches. To save time, noise words are not indexed and are
	ignored in index searches.

	When an index is created, dtSearch copies the list of words from noise.dat
	into the index directory and also builds the word list into other index
	files. After an index is created, subsequent changes to the noise word list
	will not affect indexing for that index */
    char noiseWordFile[FileNameLen];
    /* Stemming rules for stemming searches (default: "stemming.dat")

	The stemming.dat file uses a plain text format and includes comments in the file that describe the file format.
	*/
    char stemmingRulesFile[FileNameLen];

    /* Maximum number of words that can be matched in a search. This can be any
	value from 16 to 256k. The default is 64k. If a search matches more unique
	words than the maxWordsToRetrieve limit, the error code dtsErMaxWords (137)
	will be returned.  */
    long maxWordsToRetrieve;
    /* Maximum size of a single <link Retrieving Fields in Search Results, stored field>.
       Stored fields are field data collected during indexing that is returned in search
       results.                                                                           */
    long maxStoredFieldSize;
    /* Use this option to change the number of characters stored as the "title" property
       \of each document, up to a maximum of 512. By default, the dtSearch Engine
       collects the first 80 characters of text from a file for the title associated
       with each document.                                                               */
    long titleSize;
    /* Comma-separated list of tags to ignore when indexing XML
        Remarks:
	If your XML data includes HTML tags like P, B, I, etc., use XmlIgnoreTags to prevent these tags from affecting the
	field structure of XML. Example:
	<code>
		options.XmlIgnoreTags = "b,p,i,u,h1,h2,h3,br,hr"
	</code>
	You can also use wildcards to exclude tags, like this:
	<code>
		options.XmlIgnoreTags = "h?,id*"
	</code>
	*/
    char xmlIgnoreTags[512];
    // Words longer than the maxWordLength will be truncated when indexing. The default maxWordLength is 32. The maximum value is 128.
    long maxWordLength;
    /* File segmentation rules, used to split up long text files into logical
	subdocuments during indexing.

    The SegmentationRulesFile is an XML file. To create the file, start dtSearch
	Desktop, click Options > Preferences > File Segmentation Rules and use the
	dialog box to set up the rules. The XML file will be saved as fileseg.xml in
	your dtSearch UserData folder. See File Segmentation Rules in the dtSearch
	Desktop help for information on this setting.
	*/
    char segmentationRulesFile[FileNameLen];

    /* Name of the file containing rules for extraction of field data from text
	files based on markers in the next

	The TextFieldsFile is an XML file. To create the file, start dtSearch
	Desktop, click Options > Preferences > Text Fields and use the dialog box to
	set up the text field definitions. The XML file will be saved as fields.xml
	in your dtSearch UserData folder. See Define Text Fields in the dtSearch
	Desktop help for information on this setting. */
    char textFieldsFile[FileNameLen];

    /* User-defined synonym sets. The UserThesaurusFile is an XML
       \file. To create the file, start dtSearch Desktop, click <b>Options
       \> Preferences \> User Thesaurus</b> and use the dialog box
       to set up the synonym definitions. The XML file will be saved
       as thesaur.xml in your dtSearch UserData folder. See User
       Thesaurus in the dtSearch Desktop help for information on
       this setting.                                                       */
    char userThesaurusFile[FileNameLen];

    /* Set to true to force all configuration files to be re-read. If the
	contents of a configuration file such as TextFieldsFile changes, but the
	filename is not changed, set UpdateFiles=true to indicate that dtSearch
	should discard any internally-cached copies of configuration files and
	re-read them from disk. */
    long updateFiles;

    /* Obsolete.  This was used in older versions to enable LZW decompression in
	file parsers, which is now always enabled. */
    long lzwEnableCode;

    /* Summary
       Directory where the dtSearch Engine and support files are located.
       Remarks
       This may be a shared, read-only network directory, and is usually the folder
       where the dtSearch Engine is located.

       The dtSearch Engine automatically loads external file parser DLLs from the
       viewers subdirectory of the HomeDir directory. If no Home directory is supplied,
       the dtSearch Engine will attempt to determine the location of its home directory
       using Windows system calls. Often this will work, but it is better to supply a
       specific Home directory.

       For more information, see <link Home and Private Directories, The HomeDir\, PrivateDir\, and TempFileDir option settings>. */
    char homeDir[FileNameLen];

	/* Summary
	   A directory that the dtSearch Engine can use to store per-user settings and
	   temporary files.
	   Remarks
	   PrivateDir, if non-blank, must be unique for each concurrent user of the dtSearch
	   Engine.

	   For more information, see <link Home and Private Directories, The HomeDir\, PrivateDir\, and TempFileDir option settings>.
	                                                                                                                              */
    char privateDir[FileNameLen];

    /*
    Summary:

    Use to replace the default connectors used in search requests.

    Remarks:

	The BooleanConnectors option setting lets you change the connectors that
	dtSearch uses in boolean searches (and, or, not, etc.)

	The default connectors are: AND, OR, NOT, W/, CONTAINS, and TO.

	To replace the default connectors with alternative connectors (i.e., in
	French) set BooleanConnectors to a series of items each of which consists of
	a connector string and the equivalent English connector, in this format:
	<code>
	{new connector=connector name}
	</code>

	For example:

	<code>
	{et=and} {ou=or} {sauf=not} {m/=w/} {contient=contains} {and=and} {or=or}
	{not=not} {contains=contains} {w/=w/} {to=to}
	</code>

	The standard English connectors are included as well as the French
	connectors in this example. If BooleanConnectors is not blank, the
	connectors it defines will replace the defaults, so dtSearch will not
	recognize and, or, not, etc. To restore the default English connectors, add
	an item for each connector to the list, as in the above example. It is
	possible to have more than one way of expressing each type of connector so,
	for example, "and" or "et" both are equivalent to the default "AND"
	connector.

	Text in connectors must consist entirely of searchable letters. Punctuation
	characters that are not searchable letters cannot be used as part of a
	connector. The only exception to this rule is the w/N syntax for proximity
	searches, which allows w/ to be redefined as any single letter (a-z only)
	followed by a /.

    */
    char booleanConnectors[512];

    /* Name of the file containing a table of filename patterns for file formats
	that dtSearch cannot detect automatically, such as older versions of
	WordStar.

	The FileTypeTableFile is an XML file. To create the file, start dtSearch
	Desktop, click Options > Preferences > File Types and use the dialog box to
	set up the file type definitions. The XML file will be saved as filetype.xml
	in your dtSearch UserData folder. See File Types in the dtSearch Desktop
	help for information on this setting.
	*/
    char fileTypeTableFile[FileNameLen];
	// Flags that control text-processing options.  See the TextFlags enum for values.
    long textFlags;
    // Maximum depth of nested fields (value must be between 1 and 32)
    long maxFieldNesting;

		/* Size of files that are always processed using the unicode filtering algorithm, in megabytes.

		   Files larger than autoFilterSizeMB are assumed to be non-document files
		   such as forensically recovered disk images or slack space, and are indexed using the
		   unicode filtering algorithm.  If zero, the unicode filtering algorithm is never applied
		   based on file size.
       See Also
       <link Filtering Options>                                                          */
		long autoFilterSizeMB;
// begin-internal
    long reservedInt2;
	long otherFlags;
    char macroFile[FileNameLen];
// end-internal
	// Character that indicates that a search term is a macro (default "@")
    char macroChar;
    // Character that enables fuzzy searching for a search term (default "%")
    char fuzzyChar;
    // Character that enables phonic searching for a search term (default "#").
    // The regular expression mark ("##") is a doubling of the phonicChar.
    char phonicChar;
    // Character that enables stemming for a search term.
    // The expression used for range searching (default "~~") a doubling of the stemmingChar.
    char stemmingChar;
    // Character that enables synonym searching for a search term (default "&")
    char synonymChar;
    /* Character used to indicate term weighting (example: apple:5)
       The prefix used to add field name in front of a word in an
       \xfilter expression is a doubling of the weightChar (default
       "::"). For example, if you change the WeightChar to !, then
       an xfilter expression with a field would look like this:
       <code>
       \xfilter(word "FieldName!!Something")
       </code>                                                      */
    char weightChar;
    // Wildcard character that matches a single digit (default "=")
    char matchDigitChar;
	// Character to insert between multiple instances of a stored field in a single document
    char storedFieldDelimiterChar;

	/* FieldFlags values that control indexing of metadata.  */
    long fieldFlags;
    /* Summary
       Indicates Unicode ranges that are of interest when filtering.
       Remarks
       If unicodeFilterRanges is set to 1 and 8, then the filtering algorithm will look
       for characters from U+0100-U+01FF and U+0800-U+08FF

       This is used to help the filtering algorithm to distinguish text from non-text
       data. It is only used as a hint in the algorithm, so if the text extraction
       algorithm detects text in another language with a sufficient level of confidence,
       it will return that text even if the language was not selected.

       In the C++ API, a 256-byte array is used to specify the ranges, with each byte
       set to a nonzero value to indicate that the corresponding range should be
       included.
       See Also
       <link Filtering Options>                                                          */
    char unicodeFilterRanges[256];
    /* Summary
       Specifies how each input file is divided into blocks before being filtered.
       See Also
       <link Filtering Options>                                                    */
    long unicodeFilterBlockSize;
    /* Summary
       UnicodeFilterFlags values controlling the behavior of the Unicode filtering
       algorithm.

       See Also
       <link Filtering Options>                                                    */
    long unicodeFilterFlags;
// begin-internal
    dtsViewerInfo *viewers;
// end-internal
	/* Summary
	   Minimum length of a run of text when applying the Unicode Filtering algorithm.
	   Remarks
	   This option specifies how many text characters must occur consecutively for a
	   block of text to be included. At the default value, 6, a series of 5 text
	   characters surrounded by non-text data would be filtered out.
	   See Also
	   <link Filtering Options>                                                       */
    long unicodeFilterMinTextSize;
	// Pointer to language analyzer to use for word breaking
    dtsLanguageAnalyzerInterface *pAnalyzer;

    /* Summary
       Amount of overlap when automatically breaking words when applying the Unicode
       Filtering algorithm.
       Remarks
       Unicode Filtering can automatically break long runs of letters into words each
       time more than Options.MaxWordLength consecutive letters are found. By default, a
       word break is inserted and the next word starts with the following character. Set
       UnicodeFilterWordOverlapAmount and also set the dtsoUfAutoWordBreakOverlapWords
       flag in UnicodeFilterFlags to start the next word before the end of the previous
       word.

       For example, suppose the maximum word length is set to 8, and the following run
       \of letters is found: aaaaahiddenaaaaa. By default, this would be indexed as
       aaaaahid and denaaaa, which means that a search for *hidden* would not find it.
       With a word overlap of 4, this would be indexed as: aaaaahid, ahiddena, denaaaaa
       which would allow the embedded word "hidden" to be found in a search for
       \*hidden*.
       See Also
       <link Filtering Options>                                                          */
    long unicodeFilterWordOverlapAmount;

	/* Summary
	   Directory to use for temporary files.
	   Remarks
	   Use tempFileDir to control the location of most temporary files that the
	   dtSearch Engine creates, such as temporary files used for large file 
	   conversion operations or extraction of large documents stored in containers.  

	   IndexJob.TempFileDir will still control
	   the location of temporary lists and sort buffers created during indexing.

	*/
    char tempFileDir[200];

// begin-internal
    char reservedText2[36];
// end-internal
#ifdef __cplusplus
    dtsOptions();
    // Copy another dtsOptions
    void copy(const dtsOptions& other);
    // Force all data in dtsOptions to valid values.  See dtsearch.cpp for the source code implementing this.
    void validate();
    // Compare options for equality
    bool equals(const dtsOptions& other) const;
#endif

    };

/* Summary
   Sets dtSearch Engine indexing and searching option settings.

   Remarks
   Use dtsOptionsReader to read option settings into a dtsOptions2 structure,
   change the values in dtsOptions2 as needed, and call dtssSetOptions2 
   to apply the changes.

   Option settings are not persisted anywhere so changes must be made each time a
   new program instance starts.

   Option settings apply to the current thread and any threads created after the
   current thread. Therefore, each thread can have its own settings.              */

struct dtsOptions2 {
    int structSize;
    // BinaryFilesSettings value specifying the treatment of binary files.
    int binaryFiles;
    // Define characters considered to be text if binaryFiles is set ot dtsoFilterBinary
    char binaryFilterTextChars[256];
    // HyphenSettings value specifying the treatment of hyphens in text
    int hyphens;
    /* Name of dtSearch alphabet file to use when parsing text into words.

    	Remarks:
    	The	alphabet file determines which characters are letters and what the rules are
	for handling capitalization and punctuation. If blank, dtSearch will use
		a default alphabet.

	The alphabet file only controls the indexing of characters in the range
	32-127. Unicode character properties control the treatment of other
	characters. */
    const char *alphabetFile;
    // If false, any word that begins with a digit will not be indexed.
    int indexNumbers;
    /* List of noise words to skip during indexing (default: "noise.dat") A
	noise word is a word such as the or if that is so common that it is not
	useful in searches. To save time, noise words are not indexed and are
	ignored in index searches.

	When an index is created, dtSearch copies the list of words from noise.dat
	into the index directory and also builds the word list into other index
	files. After an index is created, subsequent changes to the noise word list
	will not affect indexing for that index */
    const char *noiseWordFile;
    /* Stemming rules for stemming searches (default: "stemming.dat")

	The stemming.dat file uses a plain text format and includes comments in the file that describe the file format.
	*/
    const char *stemmingRulesFile;

    /* Maximum number of words that can be matched in a search. This can be any
	value from 16 to 256k. The default is 64k. If a search matches more unique
	words than the maxWordsToRetrieve limit, the error code dtsErMaxWords (137)
	will be returned.  */
    int maxWordsToRetrieve;
    /* Maximum size of a single <link Retrieving Fields in Search Results, stored field>.
       Stored fields are field data collected during indexing that is returned in search
       results.                                                                           */
    int maxStoredFieldSize;
    /* Use this option to change the number of characters stored as the "title" property
       \of each document, up to a maximum of 512. By default, the dtSearch Engine
       collects the first 80 characters of text from a file for the title associated
       with each document.                                                               */
    int titleSize;
    /* Comma-separated list of tags to ignore when indexing XML
        Remarks:
	If your XML data includes HTML tags like P, B, I, etc., use XmlIgnoreTags to prevent these tags from affecting the
	field structure of XML. Example:
	<code>
		options.XmlIgnoreTags = "b,p,i,u,h1,h2,h3,br,hr"
	</code>
	You can also use wildcards to exclude tags, like this:
	<code>
		options.XmlIgnoreTags = "h?,id*"
	</code>
	*/
    const char *xmlIgnoreTags;
    // Words longer than the maxWordLength will be truncated when indexing. The default maxWordLength is 32. The maximum value is 128.
    int maxWordLength;
    /* File segmentation rules, used to split up long text files into logical
	subdocuments during indexing.

    The SegmentationRulesFile is an XML file. To create the file, start dtSearch
	Desktop, click Options > Preferences > File Segmentation Rules and use the
	dialog box to set up the rules. The XML file will be saved as fileseg.xml in
	your dtSearch UserData folder. See File Segmentation Rules in the dtSearch
	Desktop help for information on this setting.
	*/
    const char *segmentationRulesFile;

    /* Name of the file containing rules for extraction of field data from text
	files based on markers in the next

	The TextFieldsFile is an XML file. To create the file, start dtSearch
	Desktop, click Options > Preferences > Text Fields and use the dialog box to
	set up the text field definitions. The XML file will be saved as fields.xml
	in your dtSearch UserData folder. See Define Text Fields in the dtSearch
	Desktop help for information on this setting. */
    const char *textFieldsFile;

    /* User-defined synonym sets. The UserThesaurusFile is an XML
       \file. To create the file, start dtSearch Desktop, click <b>Options
       \> Preferences \> User Thesaurus</b> and use the dialog box
       to set up the synonym definitions. The XML file will be saved
       as thesaur.xml in your dtSearch UserData folder. See User
       Thesaurus in the dtSearch Desktop help for information on
       this setting.                                                       */
    const char *userThesaurusFile;

    /* Set to true to force all configuration files to be re-read. If the
	contents of a configuration file such as TextFieldsFile changes, but the
	filename is not changed, set UpdateFiles=true to indicate that dtSearch
	should discard any internally-cached copies of configuration files and
	re-read them from disk. */
    int updateFiles;

    /* Summary
       Directory where the dtSearch Engine and support files are located.
       Remarks
       This may be a shared, read-only network directory, and is usually the folder
       where the dtSearch Engine is located.

       The dtSearch Engine automatically loads external file parser DLLs from the
       viewers subdirectory of the HomeDir directory. If no Home directory is supplied,
       the dtSearch Engine will attempt to determine the location of its home directory
       using Windows system calls. Often this will work, but it is better to supply a
       specific Home directory.

       For more information, see <link Home and Private Directories, The HomeDir\, PrivateDir\, and TempFileDir option settings>. */
    const char *homeDir;

	/* Summary
	   A directory that the dtSearch Engine can use to store per-user settings and
	   temporary files.
	   Remarks
	   PrivateDir, if non-blank, must be unique for each concurrent user of the dtSearch
	   Engine.

	   For more information, see <link Home and Private Directories, The HomeDir\, PrivateDir\, and TempFileDir option settings>.
	                                                                                                                              */
    const char *privateDir;

/* Summary
	   Directory to use for temporary files.
	   Remarks
	   Use tempFileDir to control the location of most temporary files that the
	   dtSearch Engine creates, such as temporary files used for large file 
	   conversion operations or extraction of large documents stored in containers.  

	   IndexJob.TempFileDir will still control
	   the location of temporary lists and sort buffers created during indexing.

	*/
    const char *tempFileDir;

    /*
    Summary:

    Use to replace the default connectors used in search requests.

    Remarks:

	The BooleanConnectors option setting lets you change the connectors that
	dtSearch uses in boolean searches (and, or, not, etc.)

	The default connectors are: AND, OR, NOT, W/, CONTAINS, and TO.

	To replace the default connectors with alternative connectors (i.e., in
	French) set BooleanConnectors to a series of items each of which consists of
	a connector string and the equivalent English connector, in this format:
	<code>
	{new connector=connector name}
	</code>

	For example:

	<code>
	{et=and} {ou=or} {sauf=not} {m/=w/} {contient=contains} {and=and} {or=or}
	{not=not} {contains=contains} {w/=w/} {to=to}
	</code>

	The standard English connectors are included as well as the French
	connectors in this example. If BooleanConnectors is not blank, the
	connectors it defines will replace the defaults, so dtSearch will not
	recognize and, or, not, etc. To restore the default English connectors, add
	an item for each connector to the list, as in the above example. It is
	possible to have more than one way of expressing each type of connector so,
	for example = "and" or "et" both are equivalent to the default "AND"
	connector.

	Text in connectors must consist entirely of searchable letters. Punctuation
	characters that are not searchable letters cannot be used as part of a
	connector. The only exception to this rule is the w/N syntax for proximity
	searches, which allows w/ to be redefined as any single letter (a-z only)
	followed by a /.

    */
    const char *booleanConnectors;

    /* Name of the file containing a table of filename patterns for file formats
	that dtSearch cannot detect automatically, such as older versions of
	WordStar.

	The FileTypeTableFile is an XML file. To create the file, start dtSearch
	Desktop, click Options > Preferences > File Types and use the dialog box to
	set up the file type definitions. The XML file will be saved as filetype.xml
	in your dtSearch UserData folder. See File Types in the dtSearch Desktop
	help for information on this setting.
	*/
    const char *fileTypeTableFile;
	// Flags that control text-processing options.  See the TextFlags enum for values.
    int textFlags;
    // Maximum depth of nested fields (value must be between 1 and 32)
    int maxFieldNesting;

		/* Size of files that are always processed using the unicode filtering algorithm, in megabytes.

		   Files larger than autoFilterSizeMB are assumed to be non-document files
		   such as forensically recovered disk images or slack space, and are indexed using the
		   unicode filtering algorithm.  If zero, the unicode filtering algorithm is never applied
		   based on file size.
       See Also
       <link Filtering Options>                                                          */
		int autoFilterSizeMB;
// begin-internal
    int otherFlags;
    const char *macroFile;
// end-internal
	// Character that indicates that a search term is a macro (default "@")
    char macroChar;
    // Character that enables fuzzy searching for a search term (default "%")
    char fuzzyChar;
    // Character that enables phonic searching for a search term (default "#").
    // The regular expression mark ("##") is a doubling of the phonicChar.
    char phonicChar;
    // Character that enables stemming for a search term.
    // The expression used for range searching (default "~~") a doubling of the stemmingChar.
    char stemmingChar;
    // Character that enables synonym searching for a search term (default "&")
    char synonymChar;
    /* Character used to indicate term weighting (example: apple:5)
       The prefix used to add field name in front of a word in an
       \xfilter expression is a doubling of the weightChar (default
       "::"). For example, if you change the WeightChar to !, then
       an xfilter expression with a field would look like this:
       <code>
       \xfilter(word "FieldName!!Something")
       </code>                                                      */
    char weightChar;
    // Wildcard character that matches a single digit (default "=")
    char matchDigitChar;
	// Character to insert between multiple instances of a stored field in a single document
    char storedFieldDelimiterChar;

	/* FieldFlags values that control indexing of metadata.  */
    int fieldFlags;
    /* Summary
       Indicates Unicode ranges that are of interest when filtering.
       Remarks
       If unicodeFilterRanges is set to 1 and 8, then the filtering algorithm will look
       for characters from U+0100-U+01FF and U+0800-U+08FF

       This is used to help the filtering algorithm to distinguish text from non-text
       data. It is only used as a hint in the algorithm, so if the text extraction
       algorithm detects text in another language with a sufficient level of confidence,
       it will return that text even if the language was not selected.

       In the C++ API, a 256-byte array is used to specify the ranges, with each byte
       set to a nonzero value to indicate that the corresponding range should be
       included.
       See Also
       <link Filtering Options>                                                          */
    char unicodeFilterRanges[256];
    /* Summary
       Specifies how each input file is divided into blocks before being filtered.
       See Also
       <link Filtering Options>                                                    */
    int unicodeFilterBlockSize;
    /* Summary
       UnicodeFilterFlags values controlling the behavior of the Unicode filtering
       algorithm.

       See Also
       <link Filtering Options>                                                    */
    int unicodeFilterFlags;
// begin-internal
    dtsViewerInfo *viewers;
// end-internal
	/* Summary
	   Minimum length of a run of text when applying the Unicode Filtering algorithm.
	   Remarks
	   This option specifies how many text characters must occur consecutively for a
	   block of text to be included. At the default value, 6, a series of 5 text
	   characters surrounded by non-text data would be filtered out.
	   See Also
	   <link Filtering Options>                                                       */
    int unicodeFilterMinTextSize;
	// Pointer to language analyzer to use for word breaking
    dtsLanguageAnalyzerInterface *pAnalyzer;

    /* Summary
       Amount of overlap when automatically breaking words when applying the Unicode
       Filtering algorithm.
       Remarks
       Unicode Filtering can automatically break long runs of letters into words each
       time more than Options.MaxWordLength consecutive letters are found. By default, a
       word break is inserted and the next word starts with the following character. Set
       UnicodeFilterWordOverlapAmount and also set the dtsoUfAutoWordBreakOverlapWords
       flag in UnicodeFilterFlags to start the next word before the end of the previous
       word.

       For example, suppose the maximum word length is set to 8, and the following run
       \of letters is found: aaaaahiddenaaaaa. By default, this would be indexed as
       aaaaahid and denaaaa, which means that a search for *hidden* would not find it.
       With a word overlap of 4, this would be indexed as: aaaaahid, ahiddena, denaaaaa
       which would allow the embedded word "hidden" to be found in a search for
       \*hidden*.
       See Also
       <link Filtering Options>                                                          */
    int unicodeFilterWordOverlapAmount;
// begin-internal
    char reserved[256];
// end-internal
#ifdef __cplusplus
    dtsOptions2();
    // Copy another dtsOptions2
    void copy(const dtsOptions2& other);
    // Force all data in dtsOptions2 to valid values.  See dtsearch.cpp for the source code implementing this.
    void validate();
    // Compare options for equality
    bool equals(const dtsOptions2& other) const;

	// Copy only the fixed-size values from a dtsOptions structure 
	void copyFixedSizeValues(const dtsOptions& source);

#endif
    };

// begin-internal
struct dtsInputStream;
// Backward compatability after name change
#define dtsInputSource dtsInputStream

/*  OBSOLETE:  use dtsFileConvertJob2
 *  Struct passed to dtssConvertFile to convert a file to a different format.
 *  If an array of hit offsets is included, these hits will be boldfaced
 *  in the output document.  Output can be either to a disk file or to
 *  a memory buffer.
 */
struct dtsFileConvertJob {
    long structSize;
    const char  *inputFile;
    const char  *outputFile;
    char  *outputBuf;
    long outputBufLen;
    long hitCount;
    long  *hits;
    long outputFormat;
    int (*pOutputFn)(void *pData, const char *text, int len);
    void *pData;
    const char *beforeHit;
    const char *afterHit;
    const char *header;
    const char *footer;
    struct dtsInputStream *inputSource;
    long flags;
    const char *htmlHead;
    const char *baseHref;
    struct dtsErrorInfo *errorHandler;
    const char *alphabetLocation;
    long reserved[10];
#ifdef __cplusplus
    dtsFileConvertJob();
#endif
    };
// end-internal

/* Summary
   dtsExtractionOptions (attached to a dtsFileConvertJob2) specifies how embedded
   images and attachments should be handled.
   Remarks
   During conversion, embedded images and attachments can be extracted to a folder,
   with links to the extracted images and attachments inserted in the output.

   When highlighting hits, flags should be consistent with the behavior of the
   indexer or hit highlighting will be inconsistent.

   dtsExoExtractAttachments should not be used when highlighting hits because in
   some cases text is added to the conversion output to provide a location for the
   link to the extracted attachment.

   dtsExoDoNotConvertAttachments should not be used when highlighting hits because
   the indexer does convert attachments to text.

   dtsExoExtractImages can be used when highlighting hits.                          */ 
struct dtsExtractionOptions {
	long structSize;
	// ExtractionOptionsFlags value.
	int flags;
	// Output folder for images and attachments extracted from the input file
	const char *outputLocation;
	/* Summary
	   URL of the outputLocation for links in the conversion output.
	   Remarks
	   When the output format for the conversion is HTML or XML, the location of each
	   extracted image or attachment will be stored as a link. While outputLocation must
	   be a folder name, outputReference should be the equivalent location expressed as
	   a URL, so it will work when used in the links in the conversion output.

	   For example, on a web server outputLocation might be c:\\inetpub\\user1\\temp and
	   \outputReference would be the virtual path to this folder, /user1/temp.           */
	const char *outputReference;
	/* Summary
	   Name of text file to create with the names of all images and attachments
	   extracted to the outputLocation.
	   Remarks
	   Each attachment or image file extracted will be listed in the file, with one name
	   per line. Filenames will be stored in UTF-8 format.                               */
	const char *outputFileListName;
	/* Summary
	   Filename prefix for extracted files stored in outputLocation
	   Remarks
	   For example, if sample.jpg is extracted, and if filenamePrefix is "tmp_", then
	   the name of the file saved will be "tmp_sample.jpg".                           */
	const char *filenamePrefix;
	/* Summary
	   Text to use for link to an attachment that has no name
	   Remarks
	   If an attachment has no name attribute in the input document, the value in
	   unnamedAttachmentLinkText will be used as the visible text for the HTML link in
	   the generated output.                                                           */
	const char *unnamedAttachmentLinkText;

	/* \ \ 
	   Summary
	   If the dtsExoLimitExtensions flag is set, image and attachment filename
	   extensions will be limited to the values listed in allowedExtensions.
	   Remarks
	   The allowedExtensions option provides a way to guard against execution of malware
	   that may be embedded in documents. When an attachment or image is extracted,
	   unless the filename extension is one of the listed extensions in
	   allowedExtensions, the filename will have the defaultExtension appended to it.

	   For example, if "exe" is not included in the list of allowedExtensions, and if
	   the defaultExtension is ".data", then an attachment named "sample.exe" will be
	   saved as "sample.exe.data". allowedExtensions is a null-delimited string set.

	   Do not include the "." in each extension.                                         */
	const char *allowedExtensions;
	/* Summary
	   Extension to add to images and attachments when the dtsExoLimitExtensions flag
	   is set and an image or attachment has an extension that is not listed in
	   allowedExtensions.
	   Remarks
	   The allowedExtensions list controls which attachments and images can be extracted
	   with the original filename extension preserved. Attachments and images with
	   extensions that are not in the list will have the defaultExtension appended to
	   the output filename, effectively disabling the original extension.

	   Do not include the "." in the extension.                                          */
	const char *defaultExtension;

	const char *reserved[4];
	#ifdef __cplusplus
	// Constructor
    dtsExtractionOptions();
    // Clear all data 
    void clear();
    // Copy another dtsExtractionOptions (shallow copy)
    void copy(const dtsExtractionOptions& other);

	#endif
	};	

/* Summary
   Describes how you want a dtSearch search report to be generated.
   Remarks
   dtsOutputInfo provides ways to control the format (HTML, RTF, plain text) and
   appearance of the output (file format, headers, footers, etc.) as well as the
   manner in which the output is provided (in memory, to a file, or through a
   callback function).

   The header, footer, beforeHit, and afterHit markings should be in the format of
   the output. For example, if the output format is HTML, then beforeHit and
   afterHit markings could be \<b\> and \</b\>.

   beforeHit, afterHit, header, and footer control the appearance of converted text.

   The header and footer strings are inserted before and after the body of the
   document.

   The beforeHit and afterHit markers are inserted before and after each hit word.
   The beforeHit and afterHit markers can contain hypertext links.

   To facilitate creation of hit navigation markers, the strings "%%ThisHit%%",
   "%%NextHit%%", and "%%PrevHit%%" will be replaced with ordinals representing the
   current hit, the next hit, and the previous hit in the document.

   For example, the following code makes the beforeHit and nextHit marks act as HTML
   "Previous Hit" and "Next Hit" buttons.
   <code>
       job.beforeHit =
                    "\<A NAME="hit%%ThisHit%%"\>"
                          "\<A HREF="#hit%%PrevHit%%"\>"
                    "\<IMG SRC="hit1.gif" ALIGN="middle" ALT="[Prev Hit]" BORDER=0\>";
       job.afterHit =
                       "\<A HREF="#hit%%NextHit%%"\>"
                       "\<IMG SRC="hit2.gif" ALIGN="middle" ALT="[Next Hit]" BORDER=0\>";
   </code>                                                                                */
struct dtsOutputInfo {
    long structSize;
    // Name of file to create, if the output is to be written to a file.
    const char *outputFile;
    // Buffer to store output.
    char  *outputBuf;
    // Size of the buffer pointed to by outputBuf.
    long outputBufLen;
    // Callback function to receive output.
    int (*pOutputFn)(void *pData, const char *text, int len);
    // pData parameter passed to the pOutputFn callback.
    void *pData;
    // OutputFormat identifing the format of the output -- itHTML, itRTF, itUtf8 (Unicode),  itUnformattedHTML, or itAnsi.
    long outputFormat;
	// Text inserted before each hit
    const char *beforeHit;
    // Text inserted after each hit
    const char *afterHit;
    // Text inserted at the start of the file
    const char *header;
    // Text inserted at the end of the file
    const char *footer;
    /* Tags to insert after the HEAD tag in HTML output */
    const char *htmlHead;
    // For HTML output, an HREF for a BASE tag to be inserted in the header.
    const char *baseHref;
    const char *reserved2;
    // For HTML output, a DocType tag to go before the first tag in the output
    const char *docTypeTag;
    long reserved[19];
#ifdef __cplusplus
	// Constructor
    dtsOutputInfo();
    // Clear all data from a dtsOutputInfo
    void clear();
    // Copy another dtsOutputInfo (shallow copy)
    void copy(const dtsOutputInfo& other);
#endif
    };

#ifdef __cplusplus
/* Summary
   Converts files to HTML, RTF, or text, optionally marking hits with
   caller-supplied tags.
   Remarks
   dtsFileConvertJob provides an easy way to display retrieved files in a web
   browser, web browser control, or other viewer with hits highlighted. If an array
   \of hit offsets is included, these hits will be highlighted using the beforeHit
   and afterHit markers in the output document. The header and footer markers will
   appear at the beginning and end of the output file. Output can go to a disk file
   (outputFile), a memory buffer (outputBuf) or a callback function (pOutputFn).
   \Input can come from a disk file (inputFile) or from a dtsInputStream
   (inputSource).
                                                                                    */

struct dtsFileConvertJob2 : public dtsJobBase {
	// begin-internal
#else
struct dtsFileConvertJob2 {
    long structSize;
    int (*pReportCallBack)(void  *pReportData, struct dtsMessage *message);
    void  *pReportData;
    long cancelFlag;
    struct dtsErrorInfo *errorHandler;
    long reserved[4];
#endif
	// end-internal

	// Specification for the output format and destination (file, memory buffer, or callback)
    struct dtsOutputInfo *output;
    /* Name of the input file to highlight. */
    const char  *inputFile;
    // A dtsInputStream that will return the document to be converted. (If an inputSource is provided, the inputFile will be ignored.)
    struct dtsInputStream *inputSource;
    // Number of hits to be highlighted
    long hitCount;
    // Word offsets of the hits to be highlighted (usually from dtsSearchResultsItem.hits)
    long  *hits;
    // ConvertFlags specifying how the conversion should be performed
    long flags;
    /* The location of the dtSearch alphabet file to use when highlighting hits.
	 The alphabet file determines how dtSearch counts words, so it is important
	 that the same alphabet file used to index or search a file also be used to
	 highlight hits. For more information on how hit highlighting works, see
	 Highlighting Hits in the online help. To ensure that the same alphabet used
	 to index a file is used to highlight hits in that file, set the
	 alphabetLocation to the folder where the index is located. The alphabet
	 definition will be stored in this folder (in a file named INDEX_A.IX). If
	 no alphabetLocation is specified, dtSearch will use the default alphabet.
	 */
    const char *alphabetLocation;
    /* Type id of the file parser to use to convert this file.  This can be used
	to specify, for example, that an HTML file should be processed as plain
	text.  If the document was retrieved in a search, the typeId should be set
	to the typeId returned from the search results object in the
	dtsSearchResultsItem.  This ensures that the same file parser used to index
	the document is also used to highlight hits.
	*/
    long typeId;
    /* <link Document Ids, Document id> of this document in the index it was found in.  */
    long docId;
    // Path of the index this document was found in
    const char *indexRetrievedFrom;
    // File type of input document detected by dtSearch file parsers.
    //
    // typeId vs. detectedTypeId:
    // typeId is provided as input to the conversion, and allows the caller to
    // specify which file parser to use with a document.
    // detectedTypeId is returned from the engine and indicates which file parser matched
    // the document.
    long detectedTypeId;

	// The build number of dtSearch that was used to index this file.
	// This is transferred from search results by SetInputItem.
	// It is used to improve backward compatibility when there are changes in file parsers,
	// to minimize mis-highlighting.
	long indexedBy;

	// Encoded version of search request that found this document
	// String generated by urlEncodeItem with dtsUrlEncodeAsSearch
	const char *encodedSearchItem;

	// Options for extraction of embedded images and attachments
	dtsExtractionOptions *extractionOptions;

	// A null-delimited string set containing the data returned by a search
	// when the dtsSearchWantHitsByWord and dtsSearchWantHitDetails search flags
	// are set.  This is only used when the flag dtsConvertMultiHighlight is
	// set to use different highlighting strings for each search term.
	const char *hitsByWord;

    // Optional handle to a dtsIndexCache to use when repeatedly 
	// using dtsConvertGetFromCache, to avoid extra index open/close operations.
    dtsIndexCacheHandle indexCacheHandle;

    void *reserved2[2];
#ifdef __cplusplus
	// Constructor
    dtsFileConvertJob2();
#endif
    };

struct dtsDataSourceDocExtractor;

/* Summary
   Describes a search report to be prepared from some or all of the items in a
   dtsSearchResults set.
   Remarks
   To generate a search report, call the makeReport() member of a dtsSearchResults
   \object.
   * Format *
   A search report lists the hits found in one or more documents, with each hit
   surrounded by a specified amount of context. Each block of context starts with a
   contextHeader and ends with the contextFooter. Contiguous or overlapping blocks
   \of context will be combined. The amount of context included in the report can be
   specified by words or by paragraphs.

   Each block of context is constructed as follows:
   <code>
       [ContextHeader]
       ...text... [BeforeHit] hit [AfterHit] ...text...
       [ContextFooter]
   </code>

   The report as a whole is constructed as follows:
   <code>
       [Header]
           [FileHeader]
               [ContextHeader]
               ...text... [BeforeHit] hit [AfterHit] ...text...
               [ContextFooter]
               [ContextHeader]
               ...text... [BeforeHit] hit [AfterHit] ...text...
               [ContextFooter]
               ... more blocks of context, if present
           [FileFooter]
           ... more files ...
       [Footer]
   </code>

   Use the following symbols to insert file information into the FileHeader and
   FileFooter:
   <table>
   Symbol               Meaning
   -------------------  -------------------------------------------------------------
   Filename             The name of the file (without path information). For PDF and
                         HTML files, this will be the Title.
   Location             The location of the file
   Fullname             The path and filename of the file.
   Size                 \File size in bytes
   SizeK                \File size in kilobytes
   Date                 Modification date of the file when indexed
   Hits                 Number of hits in the file
   Title                The first 80 characters of the file
   DocId                The docId of the file
   Type                 The file type (Microsoft Word, PDF, HTML, etc.)
   Ordinal              The 1-based ordinal of this item in the SearchResults from
                         which it was generated
   IndexRetrievedFrom   The index where the file was found
   </table>
   Use the following symbols to insert context information in the contextHeader,
   which appears in front of each block of context:
   <table>
   Symbol      Meaning
   ----------  --------------------------------------------------------------------
   Page        Page number where the hit occurs
   Paragraph   Paragraph number where the hit occurs (relative to the start of the
                page)
   Word        Word offset of the block of context from the beginning of the file.
   FirstHit    Word offset of the first hit in the block of context.
   </table>
   Use %% around each symbol, like this: %%FullName%%                                 */

struct dtsSearchReportJob : public dtsJobBase {
	//  Handle to the search results to generate this report from.
	//  Use dtsSearchResults.getHandle() to get the handle from a search results object.
    dtsSearchResultsHandle results;
    //  dtsOutputInfo that defines the format and destination of the report output.
    dtsOutputInfo *output;
    // ReportFlags specifying how the report should be generated
    long flags;
    /* Number of words or paragraphs of context to include surrounding each hit.
	If dtsReportByWordExact is used, exactly the requested number of words of
	context will be included. Otherwise, enough paragraphs of context will be
	included to provide at least the requested number of words.  Contiguous or
	overlapping blocks of context will be merged into a single block. */
    long amountOfContext;
    // Text to appear in front of each file in the search report.
    const char *fileHeader;
    // Text to appear in front of each block of context.
    const char *contextHeader;
    /* Summary
       Array of long integers identifying the items in search results to include.
       Remarks
       If the dtsReportIncludeAll flag is set, any values in selections are ignored.

       Each integer is a zero-based index into the SearchResults list.               */
    long *selections;
    // Number of items in the selections array.
    long selectCount;
    // Text to appear after each file in the search report
    const char *fileFooter;
    dtsDataSourceDocExtractor *docExtractor;
    // Number of blocks of context to include in the report.  If zero, all hits in each document will be included.
    long maxContextBlocks;
    /* Number of words to scan in each document looking for blocks of context to
	include in the report.   For example, if maxWordsToRead=5000, then the
	search report will only include hits that occur in the first 5000 words in
	each document.   This setting can be used to generate faster (but not
	necessarily complete) reports by limiting the search report to hits near the
	beginning of each document. */
    long maxWordsToRead;
    // Text to appear after each block of context
    const char *contextFooter;
    // Text to appear after the contextFooter, between two blocks of context
    const char *contextSeparator;
    long reserved[15];
    dtsSearchReportJob();
    // Copy another dtsSearchReportJob (shallow copy)
    void copy(const dtsSearchReportJob& other);
    };

// begin-internal
// Used in dtssDoRemoteSearch
struct dtsRemoteConnectInfo {
    const char  *url;
    char errorText[512];
    long errorCode;
    };

// dtsRemoteConnectInfo error codes
const int dtsInvalidURL =       101;
const int dtsInvalidPassword =  102;
const int dtsUnableToConnect =  103;
const int dtsServerError =      104;

// end-internal

// Values for dtsMergeStatusInfo.step
enum MergeStep {
	// Checking documents in the indexes to be merged
	dtsIndexMergeCheckingDocs = 1,
	// Merging document lists in the indexes
	dtsIndexMergeMergingDocs = 2,
	// Merging word lists in the indexes
	dtsIndexMergeMergingWords = 3
	};

/* Summary
   Status of a merge job in progress
   Remarks
   When an index is being merged, the dtSearch Engine will provide progress updates
   by passing a dtsMergeStatusInfo structure back to a callback function.           */
struct dtsMergeStatusInfo {
    long structSize;
    // The text of the document or word being merged
    const char *text;
    // Number of documents checked in the first step of a merge (checking the documents to see which documents should go in the target index).
    long docsChecked;
    // Number of documents that will be added to the target index.
    long docsToMerge;
    // During the second step of a merge, merging the document information, docsMerged is the number of documents merged so far.
    long docsMerged;
    // Number of words that will be added to the target index.
    long wordsToMerge;
    // During the third step of a merge, merging the words, wordsMerged is the number of words merged so far.
    long wordsMerged;
    // MergeStep indicating the state of the merge process
    long step;
    // Documents added to target index
    long docsAdded;
    // Documents skipped because they are already present in the target index
    long docsSkipped;
    /* Documents updated in the target index */
    long docsUpdated;
    // Percentage of the input indexes that have been merged
    long percentDone;
    long reserved[16];
#ifdef __cplusplus
    dtsMergeStatusInfo();
#endif
    };

/* Summary
   Passed to dtssMergeIndexes to merge two more indexes together.
   Remarks
   Merging combines two or more indexes into a single target index. The resulting
   index will contain any documents that were present in any of the input indexes. */
struct dtsIndexMergeJob : public dtsJobBase {
	// A null-delimited string set containing a list of the indexes to be merged (for each index, the full path to the index should be listed).
    const char *indexesToMerge;
    /* 0-based index that indicates which of the indexes in the indexesToMerge
	list should be the "target" of the merge (that is, the index that will
	receive the information from the other indexes). */
    long iTargetIndex;
    // Obsolete.  Concurrent access is always enabled for the target index, unless fClearTarget is true.
    long fAllowConcurrentAccess;
    /* Set to true if you want the target index to be deleted before the merge
	begins. If false, information from the other indexes will be appended to
	what is already in the index. */
    long fClearTarget;
    /* On return, result is 0 if the operation completed
       successfully and non-zero if an error occurred.   */
    long result;
    // IndexingFlags specifying how the merge should be performed.
    long mergeFlags;
    long reserved2[9];
    // Constructor
    dtsIndexMergeJob();
    };

// Values for dtsVerifyStatusInfo.step
enum VerifyStep {
	// Checking document records
	dtsIndexVerifyCheckingDocs = 1,
	// Checking words
	dtsIndexVerifyCheckingWords = 2,
	// Index is corrupt
	dtsIndexVerifyCorrupt = 3,
	// Done verifying index
	dtsIndexVerifyDone = 4
	};

/* Summary
   Status of an index verify job.
   Remarks
   When an index is being verified, the dtSearch Engine will provide progress
   updates by passing a dtsVerifyStatusInfo structure back to a callback function. */
struct dtsVerifyStatusInfo {
    long structSize;
    // The text of the document or word being verified
    const char *text;
    // Number of documents in the index to be checked
    long docsToVerify;
    // Number of documents checked so far.
    long docsVerified;
    // Number of words in the index to be verified
    long wordsToVerify;
    // Number of words verified so far
    long wordsVerified;
    // VerifyStep indicating the state of the verify process
    long step;
    // Percentage of the index that has been verified
    long percentDone;
    unsigned long kbytesInIndex;  // Number of bytes in documents in this index (in KB)
    unsigned long kwordsInIndex;  // Number of words in documents in this index (in thousands)
    long reserved[18];
    dtsVerifyStatusInfo();
    };

/* Summary
   dtsIndexVerifyJob is passed to dtssVerifyIndex, and is used to verify that an index is in good condition.
   Remarks
   To verify an index, dtSearch scans all structures in the index and performs many detailed checks to validate all data in the index for consistency.
   While the job is executing, dtsnIndexVerifyProgress messages will be sent through the pReportCallBack function
   with a dtsVerifyStatusInfo attached to the dtsMessage's ptrParam.

   */
struct dtsIndexVerifyJob :
	public dtsJobBase {
		dtsIndexVerifyJob();
		// Location of the index to Verify
		const char *indexPath;
		// On return, result is 0 if the operation completed successfully and -1 if an error occurred.
		long result;
		// begin-internal
		long flags;
		long reserved2[9];
		// end-internal
		};

// Information returned by dtssVersionEx, which returns the dtSearch Engine version
struct dtsVersionInfo {
	// Minor version
    long minorVersion;
    // Major version
    long majorVersion;
    // Build number
    long build;
    long reserved[10];
    };

// begin-internal
struct dtsHitLocation {
    long wordOffset;
    long byteOffset;
    long page;
    long paragraph;
#ifdef __cplusplus
    dtsHitLocation() :
        wordOffset(0), byteOffset(0),
        page(0), paragraph(0) { }
#endif
    };

#ifdef __cplusplus
struct dtsMapHitsJob : public dtsJobBase {
    dtsMapHitsJob(dtsSearchResultsItem& item);
    dtsMapHitsJob(long tableSize);
    ~dtsMapHitsJob();
#else
struct dtsMapHitsJob {
    long structSize;
    int (*pReportCallBack)(void  *pReportData, struct dtsMessage *message);
    void  *pReportData;
    long cancelFlag;
    long reserved[5];
#endif
    const char *filename;
    struct dtsHitLocation *table;
    long nTableEntries;
    };

// end-internal

typedef void * dtsThesaurusLookupHandle;
typedef dtsThesaurusLookupHandle (*dtsThesaurusStartLookupFunc)(const char *word);
typedef long (*dtsThesaurusGetNextWordFunc)(dtsThesaurusLookupHandle hLookup, char *dest, long maxLen);
typedef void (*dtsThesaurusEndLookupFunc)(dtsThesaurusLookupHandle hLookup);
typedef void (*dtsThesaurusGetNameFunc)(char *dest, long maxLen);

#ifndef DllExport
#   if defined(__BORLANDC__)
#       define DllExport _export 
#       define DllImport _import 
#   elif defined(_MSC_VER)
#		if defined(_WINRT_DLL)
#			define DllExport __declspec(dllexport) 
#			define DllImport __declspec(dllimport) 
#       elif defined(WIN32)
#           define DllExport __declspec(dllexport) 
#           define DllImport __declspec(dllimport) 
#       else
#           define DllExport _export
#           define DllImport _import
#       endif
#	else
#		define DllExport
#		define DllImport		
#	endif
#endif

#ifdef BUILD_DLL
#   define DLLFUNC DllExport
#elif defined(BUILD_DLL_USER)
#   define DLLFUNC DllImport
#else
#   define DLLFUNC
#endif

/*

Summary:
Used by dtssConvertPath2 to convert among virtual, local, and UNC paths

Remarks:

Internet Information Server stores its mapping between virtual and local paths
in the Metabase. Reading this information from the Metabase is time-consuming
and requires access rights that may not be available to a process running in the
web server context. Therefore, the dtSearch Engine caches this information in
the registry, under the key HKEY_LOCAL_MACHINE\Software\dtSearch
Corp.\dtSearch\Virtual Roots. By default, the conversion will use the cached table
values. Call ConvertPath with the dtsCpRefreshMappings + dtsCpStoreInRegistry
flags to update the cached information. (Running dtWebSetup.exe, the dtSearch
Web Setup program, will also update this information.)

If path conversion returns a blank string, it means that either (1) the folder
passed in does not have a virtual directory associated with it, or (2) the
registry tables have not been refreshed since the virtual directory was created.
*/

struct dtsConvertPathRequest {
    long structSize;
    // Pointer to a dtsErrorInfo to store any error messages generated during the job
    dtsErrorInfo *errorHandler;
    // ConvertPathFlags indicating the type of conversion to perform
    long flags;
    // Output buffer for the conversion
    char *dest;
    // Size of the output buffer for the conversiondtsLi
    long maxLen;
    // Input path to convert
    const char *path;
    /* ID of the web site on a web server. If there is only one web site on the
	server, use 1. The ASP value Request.ServerVariables("INSTANCE_ID") can be
	used if the web server has more than one web site. (The web site id is
	needed because each site has its own mapping between virtual and local
	paths.) */
    long siteId;
    // Registry key where virtual-to-local path mappings are stored.  Leave blank to
    // use the default value where dtSearch Web Setup and dtSearch Web store
    // the mappings.
    const char *registryKey;
    long reserved[6];
    };

#ifdef __cplusplus
/*
Summary:
Used to install a crash handler so a stack trace can be recorded in the event of an unhandled exception

Remarks:
The functions to install and remove the crash handler are:
	void dtssInstallCrashDiagnostics(dtsCrashHandlerInfo& info)
	void dtssRemoveCrashDiagnostics()

The stack trace file will list each item on the stack as an address.  A utility
included with the dtSearch Engine, StackTrace.exe, can be used to convert these
addresses to symbol names.  To use StackTrace.exe, you will need a .map file for
your program.   For example, if your program is MySearch.exe, you will need a
map file named MySearch.map.  The Visual C++ linker does not generate map files
by default so you will need to enable this option to get a map file.  Also, the
map file is only valid for the particular build for which it was generated.  If
you modify and rebuild the program, a new map file will be needed.  Once you
have a map file and the text file created by dtssInstallCrashDiagnostics, you
can get the symbols as follows:
<code>
	StackTrace MySearch.map Error.txt > ErrorWithSymbols.txt
</code>
If your program includes multiple modules, you can list the map files for each
module before the error file, like this:
<code>
	StackTrace MySearch.map MyAdditionalDll.map Error.txt > ErrorWithSymbols.txt
</code>

*/

struct dtsCrashHandlerInfo {
    long structSize;
    // An optional value to be passed to the pNotifyFn.
    void *pData;
    // An optional callback function to be called when a crash occurs.
    void (*pNotifyFn)(void *pData, const char *logName);
    // Information identifying the application, which will be written in the header of the log file.
    char appName[40];
    // Information identifying the application, which will be written in the header of the log file.
    char appVersion[40];
    // Deprecated: Use logName2
    char logName[256];
    // The name of the log file to create with the stack trace.
    const char *logName2;
    // Name of Windows MiniDump file to create
    const char *miniDumpName;
    long reserved[3];

    dtsCrashHandlerInfo();
    void copy(const dtsCrashHandlerInfo& other);
    };

#endif

// Flags used in dtsListIndexJob (C++) and ListIndexJob (.NET, Java)
enum ListIndexFlags {
	// List words in the index
	dtsListIndexWords     = 0x00000001,
	// List files in the index
	dtsListIndexFiles     = 0x00000002,
	// List fields in the index
	dtsListIndexFields    = 0x00000004,
	// For lists of words, include the number of times each word occurs in the index
	dtsListIndexIncludeCount  = 0x00000008,
	// Return the list using the outputString
	dtsListIndexReturnString  = 0x00000010,
	// For lists of words, include the field where each word is found
	dtsListIndexIncludeField   = 0x00000100,
	// Delimit items in list with tabs instead of spaces
	dtsListTabDelimit = 0x200,
	// In a list of files, include the doc id of each document
	dtsListIndexIncludeDocId = 0x400,
	// In a list of words, include the number of documents each word occurs in
	dtsListIndexIncludeDocCount = 0x800,
	// In a list of documents, include document properties
	dtsListIndexIncludeDocInfoAsXml = 0x1000,
	// Skip noise words when listing words in an index
	dtsListIndexSkipNoiseWords = 0x2000,

	dtsListIndexReserved2 = 0x20000000,
	dtsListIndexReserved1 = 0x40000000
	};

// List the contents of an index
struct dtsListIndexJob : public dtsJobBase {
    dtsListIndexJob();
	// Maximum size of the output string to generate
    long outputStringMaxSize;
    // ListIndexFlags specifying the type of list to generate
    long listFlags;
    // SearchFlags specifying search features to be used in matching the toMatch expression, such as fuzziness, stemming, etc
    long searchFlags;
    // If the dtsSearchFuzzy flag is set in searchFlags, set the fuzziness value from 1 to 10 to specify the level of fuzzy searching to apply.
    long fuzziness;
    /* toMatch is an optional search expression specifying the text to match
	against items being listed. For example, to list all field names starting
	with "A", you would set listFlags to dtsListIndexFields, and set toMatch to
	"A*". */
    const char *toMatch;
    // Location of the index to list.
    const char *indexPath;
    /* Name of file to create (if the dtsListIndexReturnString flag is not set */
    const char *outputFile;
    /* If the dtsListIndexReturnString flag is set, the list will be returned
	through a dtsStringHandle. The dtsStringHandle must be released by the
	caller. */
    dtsStringHandle outputString;
    /* If true, the output was halted due to outputStringMaxSize before all items were listed */
    long fOutputStringWasTruncated;

    long reserved[10];
    };

#ifndef SUPPRESS_DTENGINE_IMPORTS

#define DTENGINE_IMPORTS

#ifdef __cplusplus
extern "C" {
	// Execute an index job
    // Parameters:
	// job: 	The index job to execute
	// resultFlag: 	0 on success, or -1 if an error occurred
    void DLLFUNC dtssDoIndexJob(dtsIndexJob& job, short& resultFlag);

    // Merge indexes
    // Parameters:
    // job: 	The merge job to execute
    void DLLFUNC dtssMergeIndexes(dtsIndexMergeJob& job);

    // Verify indexes
    // Parameters:
    // job:     The verify job to execute
    void DLLFUNC dtssVerifyIndex(dtsIndexVerifyJob& job);

    // Execute a search
    // job:     	 The search job to execute
    // Parameters:
    // resultFlag:   0 on success, or -1 if an error occurred
    void DLLFUNC dtssDoSearchJob(dtsSearchJob& job, short& resultFlag);

    // Get information about an index
    // Parameters:
    // indexPath: 	 Path of the index
    // info:         Structure to receive returned information about the index
    // resultFlag:	 0 on success, or -1 if an error occurred
    void DLLFUNC dtssGetIndexInfo(const char  *indexPath, dtsIndexInfo& info,
        short& resultFlag);

    // Get the dtSearch Engine version.  See dtssVersionEx for more detailed information.
    // Parameters:
    // majorVersion:  The major version number
    // minorVersion:  The minor version number
    void DLLFUNC dtssVersion(short& majorVersion, short& minorVersion);

    // Get the dtSearch Engine version
    // Parameters:
    // versionInfo:   Structure to receive
    void DLLFUNC dtssVersionEx(dtsVersionInfo& versionInfo);

// begin-internal
    void DLLFUNC dtssDoInit(dtsInitInfo& initInfo, short& result);
    void DLLFUNC dtssDoShutDown(short& result);
    void DLLFUNC dtssDoGetFileText(char  *toExtractName, char  *toCreateName,
        short& result);
    void DLLFUNC dtssDebugLog(const char  *fn);
// end-internal

	/*
	  Summary:
	  Enable debug logging

	  Remarks:
	  dtSearch Engine can generate a debug log that provides detailed information about internal processing of an API call.

	  Note:  Debug logging will not work if the process creating the log lacks
	  write access to the folder containing the log file.   For example, an ASP
	  application running inside IIS may not be permitted to write to some
	  folders.

	  Parameters:
	  logName:   Name of the log file to create
	  flags:     DebugLogFlags specifying the type of log to create
	 */
    void DLLFUNC dtssDebugLogEx(const char  *logName, long flags);
    void DLLFUNC dtssAddToLog(const char  *msg);

    // Change option settings
    // Parameters:
    // opts:  		 New option settings to apply
    // resultFlag:	 0 on success, or -1 if an error occurred
    void DLLFUNC dtssSetOptions(dtsOptions& opts, short& resultFlag);
    // Retrieve current option settings
    // Parameters:
    // opts:         Structure to receive option settings
    // resultFlag:	 0 on success, or -1 if an error occurred
    void DLLFUNC dtssGetOptions(dtsOptions& opts, short& result);

    /* Summary
       Retrieve current option settings
       Parameters

       Remarks
       Call dtssGetOptions2 first with a null buffer and pointer to an int to get the
       size of the variable-length data, then allocate the buffer and call again with a
       buffer large enough to hold the data. The dtsOptionsReader class provides a
       simple wrapper around this process. Example:
       <code lang="c++">
       dtsOptions2 options;
       int sizeNeeded = 0;
       dtssGetOptions2(options, 0, 0, &amp;sizeNeeded);
       char *dataBuffer = new char[sizeNeeded];
       if (dataBuffer)
            dtssGetOptions2(options, dataBuffer, sizeNeeded, &amp;sizeNeeded);
       </code>                                                                          */
	void DLLFUNC dtssGetOptions2(dtsOptions2& dest, char *dataBuf, int dataBufSize, int *pDataSize);

    /* Summary
       Change option settings
       Parameters
       opts :        New option settings to apply
       resultFlag :  0 on success, or \-1 if an error occurred */
    void DLLFUNC dtssSetOptions2(dtsOptions2& opts, short& resultFlag);

	// Delete an index
    // Parameters:
    // indexPath:    Path to the index to delete
    // resultFlag:	 0 on success, or -1 if an error occurred
    void DLLFUNC dtssDeleteIndex(char  *indexPath, short& result);

    void DLLFUNC dtssRunScript(char  *scriptName, short& result);
    void DLLFUNC dtssMapHitsInFile(dtsMapHitsJob& job, dtsSearchResultsItem *pItem, short& result);
    void DLLFUNC dtssConvertFile(dtsFileConvertJob& info, short& result);

    // Convert a file to HTML, RTF, or text, optionally highlighting hits
    // Parameters:
    // job:     	Specifies the conversion job to perform
    // resultFlag:	0 on success, or -1 if an error occurred
    void DLLFUNC dtssConvertFile2(dtsFileConvertJob2& job, short& result);

    // Convert Unicode to UTF-8
    // Parameters:
    // dest: 		Buffer to receive UTF-8 text
    // destLen:		Size of dest buffer
    // source:		Unicode text to convert
    // sourceLen:	Number of unicode characters to convert
    // flags:		Not used
    // Returns:		Length of the output string, in bytes
    // Remarks:
    // Each Unicode character may be translated to as many as three Utf-8 characters, so to avoid truncation the output buffer should be at least 3 * sourceLen + 1 bytes.
    long DLLFUNC dtssUtf8Encode(char *dest, long destLen, const wchar_t *source, long sourceLen, long flags);

    // Convert UTF-8 to Unicode
    // Parameters:
    // dest: 		Buffer to receive Unicode text
    // destLen:		Size of dest buffer, in characters
    // source:		UTF-8 text to convert
    // sourceLen:	Number of UTF-8 characters to convert
    // flags:		Not used
    // Returns:     Length of the output string, in characters
    long DLLFUNC dtssUtf8Decode(wchar_t *dest, long destLen, const char *source, long sourceLen, long flags);

    // Install diagnostic handler for unhandled exceptions
    void DLLFUNC dtssInstallCrashDiagnostics(dtsCrashHandlerInfo& info);
    // Remove diagnostic handler for unhandled exceptions (see dtsCrashHandlerInfo)
    void DLLFUNC dtssRemoveCrashDiagnostics();
	// Restore a previously-installed diagnostic handler that was overwritten by another
	// component (such as a call to LoadLibrary that loads another DLL in the process)
	void DLLFUNC dtssRestoreCrashDiagnostics();

// begin-internal

    dtsSearchResultsHandle DLLFUNC dtsSR_Alloc(long maxCount, long flags);
    void DLLFUNC dtsSR_Free(dtsSearchResultsHandle h);
    dtsSearchResultsHandle DLLFUNC dtsSR_Copy(dtsSearchResultsHandle h);
    long DLLFUNC dtsSR_GetDocInfo(dtsSearchResultsHandle h, long iItem, dtsSearchResultsItem& item);
    void DLLFUNC dtsSR_SetDocInfo(dtsSearchResultsHandle h, long iItem, const dtsSearchResultsItem& item);
    void DLLFUNC dtsSR_SetSortKey(dtsSearchResultsHandle h, long iItem, const char *key);
    long DLLFUNC dtsSR_GetDocInfoStrings(dtsSearchResultsHandle h, long iItem, char *dest, long maxLen);
    void DLLFUNC dtsSR_GetCounts(dtsSearchResultsHandle h, long *pInfoCount, long *pTotalFileCount, long *pTotalHitCount);
    void DLLFUNC dtsSR_GetSearchRequest(dtsSearchResultsHandle h, char *dest, int maxLen);
    void DLLFUNC dtsSR_Sort(dtsSearchResultsHandle h, long flags, const char *userFieldName);
    void DLLFUNC dtsSR_MakeReport(dtsSearchResultsHandle h, struct dtsSearchReportJob& job);
    void DLLFUNC dtsSR_MakePdfHighlightFile(dtsSearchResultsHandle h, long iItem, const char *outputFilename, const char *tempFilename);
    dtsStringHandle DLLFUNC dtsSR_MakePdfWebHighlightFile(dtsSearchResultsHandle h, long iItem, const char *tempFilename);
    void DLLFUNC dtsSR_ReadDocInfo(dtsSearchResultsHandle h);
    long DLLFUNC dtsSR_Serialize(dtsSearchResultsHandle h, const char *fn, int flags);
    dtsStringHandle DLLFUNC dtsSR_SerializeAsXml(dtsSearchResultsHandle h, int iItem);
    void DLLFUNC dtsSR_SerializeFromXml(dtsSearchResultsHandle h, const char *s);
    void DLLFUNC dtsSR_LogDocInfo(dtsSearchResultsHandle h, long iItem, dtsSearchResultsItem *pItem);
    void DLLFUNC dtsSR_UnionWith(dtsSearchResultsHandle h, dtsSearchResultsHandle h2);
    void DLLFUNC dtsSR_IntersectionOf(dtsSearchResultsHandle h, dtsSearchResultsHandle a, dtsSearchResultsHandle b);
    int  DLLFUNC dtsSR_UrlDecodeItem(dtsSearchResultsHandle h, const char *s);
    int  DLLFUNC dtsSR_UrlEncodeItem(dtsSearchResultsHandle h, long iItem, char *dest, int maxLen);
    dtsStringHandle  DLLFUNC dtsSR_UrlEncodeItem2(dtsSearchResultsHandle h, int iItem, int maxLen, int flags);
    void DLLFUNC dtsSR_GetSearchDate(dtsSearchResultsHandle h, dtsFileDate& date);
    int DLLFUNC dtsSR_AddItem(dtsSearchResultsHandle h, const char *indexRetrievedFrom, long docId, long hitCount, long *hits);
    int DLLFUNC dtsSR_AddItems(dtsSearchResultsHandle h, const char *indexRetrievedFrom, long count, long *docIds);
    long DLLFUNC dtsSR_GetSearchFlags(dtsSearchResultsHandle h, long *pFlags, int *pFuzziness);
	void DLLFUNC dtsSR_SetSelections(dtsSearchResultsHandle hResults, long count, long *selections);
	dtsIntArrayHandle DLLFUNC dtsSR_GetSelections(dtsSearchResultsHandle hResults);

    dtsJobOptionsHandle DLLFUNC dtsJO_Create(dtsOptions& options);
    void DLLFUNC dtsJO_Free(dtsJobOptionsHandle h);
    void DLLFUNC dtsJO_GetOptions(dtsJobOptionsHandle h, dtsOptions& options);

    void DLLFUNC dtssConvertPath(const char *p, char *d, int maxLen, long flags );
    // end-internal

	// Convert paths to local, virtual, or UNC (Windows only)
	// Parameters:
	// request:   Specifies the conversion to perform
    void DLLFUNC dtssConvertPath2(dtsConvertPathRequest& request);

	// Get the length of a string represented by a dtsStringHandle
	// Parameters:
	// hStr:	Handle to the string
	// Returns:	Length of the string
    long DLLFUNC dtssStringGetLength(dtsStringHandle hStr);

	/* Get the text of a string represented by a dtsStringHandle

	   Parameters:
	   hStr:	   Handle to the string
	   buf:		   Buffer to receive the text
	   bufferSize: Size of the buffer, in bytes
	   Returns:	   Length of the string

	   Remarks:
	   Use dtssStringGetLength() to get the length of the string, to make sure
	   the buffer is large enough.

	   dtsStringHandles are used to return variable-length
	   string values from certain dtSearch Engine API functions.  After retrieving
	   the text of the string, the caller must use dtssStringDelete to free the memory
	   that was allocated for the string.
	   */
    long DLLFUNC dtssStringGetText(dtsStringHandle hStr, char *buf, long bufferSize);
    /* Free the memory allocated for a string that was returned as a dtsStringHandle

	   Parameters:
	   hStr:	   Handle to the string

	   Remarks:
	   dtsStringHandles are used to return variable-length
	   string values from certain dtSearch Engine API functions.  After retrieving
	   the text of the string, the caller must use dtssStringDelete to free the memory
	   that was allocated for the string.
	   */

    void DLLFUNC dtssStringDelete(dtsStringHandle hStr);

	// Get the length of an array represented by a dtsIntArrayHandle
	// Parameters:
	// hArray:	Handle to the array
	// Returns:	Number of items in the array
    long DLLFUNC dtssIntArrayGetLength(dtsIntArrayHandle hArray);

	/* Get the contents of an integer array represented by a dtsStringHandle

	   Parameters:
	   hStr:	   Handle to the array
	   pDest:	   Buffer to receive the integer data
	   maxItems:   Size of the pDest buffer, in 32-bit integers
	   Returns:	   Length of the array.

	   Remarks:
	   dtsIntArrayHandles are used to return variable-length
	   integer arrays from certain dtSearch Engine API functions.  To obtain the contents
	   of the array:
	   1. Use dtssIntArrayGetLength to get the length of the array,
	   2. Allocate a buffer large enough for the array,
	   3. Call dtssIntArrayGetItems to obtain the contents of the array, and
	   4. Call dtssIntArrayDelete to free the memory that was allocated for the string.
	   */
    long DLLFUNC dtssIntArrayGetItems(dtsIntArrayHandle hArray, long *pDest, long maxItems);

	/* Get partial contents of an integer array represented by a dtsStringHandle

	   Parameters:
	   hStr:	   Handle to the array
	   nFirstItem: Zero-based first index of items to retrieve from array
	   nLastItem:  Zero-based last index of items to retrieve from array
	   pDest:	   Buffer to receive the integer data, which must have space for nLastItem-nFirstItem+1 integers

	   Returns:	   Number of integers copied to pDest, which may be less than the number requested if the array does not contain sufficient items.

	   Remarks:
	   dtsIntArrayHandles are used to return variable-length
	   integer arrays from certain dtSearch Engine API functions.  To obtain the contents
	   of the array:
	   1. Use dtssIntArrayGetLength to get the length of the array,
	   2. Allocate a buffer large enough for the array,
	   3. Call dtssIntArrayGetItemRange to obtain the contents of the array, and
	   4. Call dtssIntArrayDelete to free the memory that was allocated for the string.
	   */
    long DLLFUNC dtssIntArrayGetItemRange(dtsIntArrayHandle hArray, int nFirstItem, int nLastItem, long *pDest);

	/* Delete an integer array represented by a dtsStringHandle

	   Parameters:
	   hStr:	   Handle to the array

	   Remarks:
	   dtsIntArrayHandles are used to return variable-length
	   integer arrays from certain dtSearch Engine API functions.  To obtain the contents
	   of the array:
	   1. Use dtssIntArrayGetLength to get the length of the array,
	   2. Allocate a buffer large enough for the array,
	   3. Call dtssIntArrayGetItems to obtain the contents of the array, and
	   4. Call dtssIntArrayDelete to free the memory that was allocated for the string.
	   */
    void DLLFUNC dtssIntArrayDelete(dtsIntArrayHandle hArray);

	/* Summary
	   Generates a stream of XML containing synonym information for the specified word.
	   Parameters
	   word :   The word to look up in the thesaurus
	   flags :  SearchFlags specifying the type of synonym expansion to perform
	   Returns
	   XML data containing the results of the thesaurus lookup, returned as a
	   dtsStringHandle.
	   Remarks
	   The stream of XML is returned as a dtsStringHandle, which should be released once
	   the calling program has extracted the text. The flags are a combination of the
	   synonym-related SearchFlags used in dtsSearchJob.

	   Below is an example of the XML stream returned. Field order may vary and new
	   fields may be added in future versions, so calling programs should not depend on
	   the specific field layout in this example.

	   <code>
	   \<?xml version="1.0" encoding="UTF-8" ?\>
	   \<ThesaurusOutput\>
	        \<SynonymSet\>
	              \<Gloss\>example, illustration, instance: a single item that is representative of a type\</Gloss\>
	              \<RelationDescription\>Synonym\</RelationDescription\>
	              \<RelationName\>Synonym\</RelationName\>
	              \<RelationId\>19\</RelationId\>
	              \<Members\>example illustration instance representative\</Members\>
	              \<PartOfSpeech\>noun\</PartOfSpeech\>
	        \</SynonymSet\>
	        [Additional synonym sets omitted]
	   \</ThesaurusOutput\>
	   </code>                                                                                                       */
    dtsStringHandle DLLFUNC dtssGetSynonyms(const char *word, long flags);

    /* List words, fields, or documents in an index
       Parameters:
       		job:		Specifies the index to list, what to list, and where to direct the output
       		resultFlag  0 on success, or -1 if an error occurred
	*/
    void DLLFUNC dtssListIndex(dtsListIndexJob& job, short& resultFlag);

// begin-internal
    // Provided by dtwebcli.dll
    int DLLFUNC dtssDoRemoteSearch(dtsRemoteConnectInfo& connectInfo, dtsSearchJob& searchJob);

    dtsSearchFilterHandle DLLFUNC dtsSF_Copy(dtsSearchFilterHandle other);
    dtsSearchFilterHandle DLLFUNC dtsSF_Alloc();
    void DLLFUNC dtsSF_Free(dtsSearchFilterHandle hFilter);
    void DLLFUNC dtsSF_SelectNone(dtsSearchFilterHandle hFilter, int iIndex);
    void DLLFUNC dtsSF_SelectAll(dtsSearchFilterHandle hFilter, int iIndex);
    int DLLFUNC dtsSF_AddIndex(dtsSearchFilterHandle hFilter, const char *indexPath);
    int DLLFUNC dtsSF_AddIndexW(dtsSearchFilterHandle hFilter, const wchar_t *indexPath);
    void DLLFUNC dtsSF_SetSelections(dtsSearchFilterHandle hFilter, int iIndex, unsigned long *idList, unsigned long idCount,
		int fSelected);
    void DLLFUNC dtsSF_SelectItems(dtsSearchFilterHandle m_hFilter, int iIndex, unsigned long firstId, unsigned long lastId, int fSelected);
    dtsSearchFilterHandle DLLFUNC dtsSF_MakeFromSearchResults(dtsSearchResultsHandle hResults);
    int DLLFUNC dtsSF_Write(dtsSearchFilterHandle hFilter, const char *name);
    dtsSearchFilterHandle DLLFUNC dtsSF_Read(const char *name);
    void DLLFUNC dtsSF_Or(dtsSearchFilterHandle hFilterTarget, dtsSearchFilterHandle hFilterOther);
    int DLLFUNC dtsSF_Equals(dtsSearchFilterHandle hFilterTarget, dtsSearchFilterHandle hFilterOther);
    void DLLFUNC dtsSF_And(dtsSearchFilterHandle hFilterTarget, dtsSearchFilterHandle hFilterOther);
    void DLLFUNC dtsSF_AndNot(dtsSearchFilterHandle hFilterTarget, dtsSearchFilterHandle hFilterOther);
    void DLLFUNC dtsSF_SelectItemsBySearch(dtsSearchFilterHandle hFilter, int iIndex, dtsSearchJob& searchJob,
		int fSelected);
    dtsIntArrayHandle DLLFUNC dtsSF_GetItems(dtsSearchFilterHandle hFilter, int iIndex);
	dtsStringHandle DLLFUNC dtsSF_GetIndexPath(dtsSearchFilterHandle hFilter, int iIndex);
	int DLLFUNC dtsSF_GetIndexCount(dtsSearchFilterHandle hFilter);
	void DLLFUNC dtsSF_GetExpiredState(dtsSearchFilterHandle hFilter, bool *pObsolete, bool *pIncomplete,
		bool *pSucceeded);

// end-internal
   };

#else
// begin-internal
    void DLLFUNC dtssDoIndexJob(struct dtsIndexJob  *job, short  * result);
    void DLLFUNC dtssDoSearchJob(struct dtsSearchJob  *job, short  * result);
    void DLLFUNC dtssGetIndexInfo(char  *indexPath, struct dtsIndexInfo * info,
        short *result);
    void DLLFUNC dtssVersion(short * majorVersion, short * minorVersion);
    void DLLFUNC dtssVersionEx(struct dtsVersionInfo  *versionInfo);
    void DLLFUNC dtssDoInit(struct dtsInitInfo * initInfo, short * result);
    void DLLFUNC dtssDoShutDown(short * result);
    void DLLFUNC dtssDoGetFileText(char  *toExtractName, char  *toCreateName,
        short* result);
    void DLLFUNC dtssDebugLog(const char  *fn);
    void DLLFUNC dtssSetOptions(struct dtsOptions  * opts, short  * result);
    void DLLFUNC dtssGetOptions(struct dtsOptions * opts, short  * result);
    void DLLFUNC dtssDeleteIndex(char  *indexPath, short  * result);
    void DLLFUNC dtssRunScript(char  *scriptName, short  * result);
    void DLLFUNC dtssAddToLog(const char  *msg);
    void DLLFUNC dtssMapHitsInFile(struct dtsMapHitsJob  * job, struct dtsSearchResultsItem *pItem, short  * result);
    void DLLFUNC dtssConvertFile(struct dtsFileConvertJob * info, short  * result);
    void DLLFUNC dtssConvertFile2(struct dtsFileConvertJob2  * info, short  *result);
    long DLLFUNC dtssUtf8Encode(char *dest, long destLen, const unsigned short *source, long sourceLen, long flags);
    long DLLFUNC dtssUtf8Decode(unsigned short *dest, long destLen, const char *source, long sourceLen, long flags);

    /* reserved for internal use */
    dtsSearcherHandle DLLFUNC dtssCreateSearcher();
    void DLLFUNC dtssDeleteSearcher(dtsSearcherHandle handle);
    void DLLFUNC dtssGetIndexList(short  *count, char  *buf, long bufSize,
        long *bufSizeNeeded);
// end-internal
#endif

#ifdef __cplusplus

// begin-internal
// Flags for dtsSearchResults constructor
const long srWantFileMap = 0x0001;  // store hit map info in search results

// Flags for dtsSR_Serialize()
const long dtsSrSerializeWrite = 0x0001;
const long dtsSrSerializeWriteSelected = 0x0002;

// end-internal

/* Summary
   Provides access to the results of a search.
   Remarks
   A handle managed by the dtSearch Engine represents the search results, and
   dtsSearchResults calls dtSearch Engine API functions to obtain information using
   the handle and, in the destructor, to destroy the object represented by the
   handle.
   Example
   The following code sequence demonstrates the use of dtsSearchResults:
   <code>
       dtsSearchResults results;

       // Attach the dtsSearchResults object to the dtsSearchJob
       dtsSearchJob searchJob;
       searchJob.resultsHandle = results.getHandle();

       // Execute the search
       ... set up rest of searchJob ...
       \short errFlag;
       dtssDoSearchJob(searchJob, errFlag);
   </code>                                                                          */

class dtsSearchResults {
    public:
    	/* Constructor for a search results object
    	   Parameters:
    	   	aMaxCount:    Maximum number of items to store in the search results
    	   	aFlags:		  Reserved
    	*/
        dtsSearchResults(long aMaxCount = 256, long aFlags = 0)
            {
            init();
            flags = aFlags;
            maxCount = aMaxCount;
            handle = dtsSR_Alloc(maxCount, flags);
            fOwnsHandle = true;
            };
        // Create a copy of another search results object.  This creates a deep copy,
        // with its own handle.
        dtsSearchResults(const dtsSearchResults& other)
            {
            init();
            handle = dtsSR_Copy(other.handle);
            fOwnsHandle = true;
            }
        /* Wrap an existing search results handle in a dtsSearchResults object
           Parameters:
           	aHandle:        Handle to an existing search results object
           	bTakeOwnership	If true, the destructor will delete the search results object
        */
        dtsSearchResults(dtsSearchResultsHandle aHandle, int bTakeOwnership = false)
            {
            init();
            handle = aHandle;
            fOwnsHandle = bTakeOwnership;
            }
        // Copy another search results object.  This creates a deep copy,
        // with its own handle.
        dtsSearchResults& operator=(const dtsSearchResults& other) {
            freeHandle();
            handle = dtsSR_Copy(other.handle);
            fOwnsHandle = true;
            return *this;
            }
        // Free the search results object
        virtual ~dtsSearchResults() {
            freeHandle();
            }
        void allocHandle(int aMaxCount, int aFlags) {
			freeHandle();
            flags = aFlags;
            maxCount = aMaxCount;
            handle = dtsSR_Alloc(maxCount, flags);
            fOwnsHandle = true;
			}

        void init() {
            handle = 0;
            fOwnsHandle = 0;
            fileCount = 0;
            totalFiles = 0;
            totalHits = 0;
            fGotCounts = 0;
            flags = 0;
            maxCount = 0;
            searchFlags = 0;
            fuzziness =0;
            }

        void freeHandle() {
            if (fOwnsHandle)
                dtsSR_Free(handle);
            handle = 0;
            fOwnsHandle = 0;
            init();
            }

		/* Get one item from search results.
		   Parameters
		   iItem :  The item to get (a zero\-based index into the search results list).
		   item :   Struct to receive the item.                                         */
        long getDocInfo(long iItem, dtsSearchResultsItem& item) {
            return dtsSR_GetDocInfo(handle, iItem, item);
            }
        /* Change the values in a search results item
        	Parameters:
        		iItem:      The item to change (a zero-based index into the search results list)
        		item:		The values to assign to this item
        	Remarks:
        		setDocInfo can be used to change values a search results list.  For example, it
        		can be used to assign a different score to each item.  Call getDocInfo first to
        		get the data associated with the item.

        */
        void setDocInfo(long iItem, const dtsSearchResultsItem& item) {
            dtsSR_SetDocInfo(handle, iItem, item);
            }
        /* Specify the string value to use when sorting search results.

        Remarks:
        When sort() is called with the dtsSortBySortKey flag, the search results
		list will be sorted according the value set, for each item in search
		results, using setSortKey. SetSortKey is useful when the sort must be
		done based on a computed combination of more than one field.
		*/
        void setSortKey(long iItem, const char *key) {
            dtsSR_SetSortKey(handle, iItem, key);
            }
        /* Generates a null-delimited string set containing field information associated
           with a document.
           Parameters
           iItem :    The item to generate the string from (a zero\-based index into the
                      search results list).
           buf :      Buffer to receive the generated string.
           bufSize :  Size of the buffer.
           Returns
           Size of the string set generated. If the data is larger than bufSize, the size of
           the buffer needed is returned, and partial data is copied to buf
           Remarks
           The fields are generated from data in dtsSearchResultsItem. In C++ applications,
           it is more efficient to access the data directly using dtsSearchResultsItem.

           All built-in fields are prefixed with an underscore. Additionally, any fields in
           the userFields set will also be included.

           <table>
           Field                 \Description
           --------------------  --------------------------------------------------------------
           _filename             Path and filename of the document.
           _shortName            Name of the document, without the path.
           _location             Folder where the document is located.
           _displayName          If non-blank, an alternative name to display for the document
                                  (for example, the title of an HTML document).
           _title                The first 80 characters of the document.
           _score                In a natural language search, the weighted score for the
                                  document.
           _hitcount             Number of hits found for this document.
           _size                 Size of the document when indexed, in bytes.
           _wordcount            Number of words in the document when indexed.
           _date                 Modification date of the document when indexed, in local date
                                  format.
           _genericDate          Modification date of the document when indexed, in generic
                                  date format (M03/D25/Y1997).
           _createdDate          Created date of the document when indexed, in local date
                                  format.
           _genericCreatedDate   Created date of the document when indexed, in generic date
                                  format (M03/D25/Y1997).
           _time                 Modification time of the document in HH:MM:SS format.
           _iindex               The index that the document was retrieved from (an index into
                                  the list in the IndexesToSearch list in the DSearchJob).
           _docid                Document id of this document in the index.
           _typeId               TypeId specifying the file format of the document.
           _typeName             \File format of the document, as a string.
           _indexPath            Path of the index that the document was retrieved from
           _hitDetails           See dtsSearchWantHitDetails flag.
           _hitsByWord           See dtsSearchWantHitsByWord flag.
           _synopsis             Snippet of text showing hits in context (see ReportFlags flag
                                  dtsReportStoreInResults).
           </table>                                                                             */
        long getDocInfoStrings(long iItem, char *buf, ptrdiff_t bufSize) {
            return dtsSR_GetDocInfoStrings(handle, iItem, buf, (int) bufSize);
            }
        // Returns the number of files in the search results set.
        long getCount() {
            getCounts();
            return fileCount;
            }
        /* \Returns the date the search was executed. */
        void getSearchDate(dtsFileDate& date) {
            dtsSR_GetSearchDate(handle, date);
            }
        /* Summary
           \Returns the number of files retrieved in the search, which may be more than the
           number actually stored in search results
           Remarks
           If search results were generated by a search, TotalFiles will contain the number
           \of documents retrieved in the search, including documents that were not retained
           in the SearchResults object due to the MaxFilesToRetreive limit.

           If search results were generated in any other way, such as through the unionWith
           \or intersectionOf operations, then TotalFiles will be the number of documents
           actually stored in search results.                                                */
        long getTotalFiles() {
            getCounts();
            return totalFiles;
            }
        /* Summary
           \Returns the number of hits in all files retrieved in the search, which may be
           more than the number actually stored in search results
           Remarks
           If search results were generated by a search, TotalHits will contain the number
           hits in all documents retrieved in the search, including documents that were not
           retained in the SearchResults object due to the MaxFilesToRetreive limit.

           If search results were generated in any other way, such as through the unionWith
           \or intersectionOf operations, then TotalHits will be the number of hits in the
           documents actually stored in search results.                                     */
        long getTotalHits() {
            getCounts();
            return totalHits;
            }
        /* \Returns the SearchFlags that were used in the search that generated this set of
           results.                                                                         */
        long getSearchFlags() {
			getCounts();
			return searchFlags;
			}
		/* \Returns the fuzziness value used in the search that generated this set of
		   results.                                                                   */
		int getFuzziness() {
			getCounts();
			return fuzziness;
			}
		/* \Returns the search request in the search that generated this set of results. */
        void getSearchRequest(char *dest, ptrdiff_t maxLen) {
            dtsSR_GetSearchRequest(handle, dest, (int) maxLen);
            }
        // Sorts the search results set according to the SortFlags values in flags.
        // parameters:
        // flags:       Specifies the type of sort.  See SortFlags for values.
        // userFields:  If dtsSortByField is set in flags, the field in dtsSearchResultsItem.userFields to use as the sort key
        virtual void sort(long aflags, const char *userField) {
            dtsSR_Sort(handle, aflags, userField);
            }
        void sortByHits() {
            sort(dtsSortByHits, "");
            }
        void sortByDate(int fRecentFirst = 1) {
            sort((fRecentFirst? dtsSortByDate : dtsSortAscending | dtsSortByDate), "");
            }
        void sortByName() {
            sort(dtsSortByName | dtsSortAscending | dtsSortCaseInsensitive, "");
            }
        /* getHandle() returns the handle that the dtSearch Engine uses
		internally to identify a search results set. The dtsSearchResults
		constructor allocates this handle and the dtsSearchResults destructor
		releases it, in both cases through calls to the dtSearch Engine API. To
		attach a dtsSearchResults object to a search job, set the search job's
		resultsHandle member to the value returned by getHandle(). */
        dtsSearchResultsHandle getHandle() { return handle; }

        /* Generate a search report from search results. A search report shows each hit with
           a specified amount of context, and can be in HTML, RTF, or plain text.
           Parameters
           aJob :  The dtsSearchReportJob that specifies how the report should be generated  */
        void makeReport(dtsSearchReportJob& aJob) {
            dtsSR_MakeReport(handle, aJob);
            }

// begin-internal
        void makePdfHighlightFile(int iItem, const char *fn = 0, const char *tempFilename = 0) {
            dtsSR_MakePdfHighlightFile(handle, iItem, fn, tempFilename);
            }
// end-internal
        // Creates a string used to highlight hits in PDF files.
        dtsStringHandle makePdfWebHighlightFile(long iItem, const char *tempFilename = 0) {
            return dtsSR_MakePdfWebHighlightFile(handle, iItem, tempFilename);
            }
        /* Read all document information records for items in this search results list.

        Remarks:
        If the dtsSearchDelayDocInfo flag was used in a search, then the search results list will
        initially contain just the document ids, hit offsets, and score for each retrieved document.
        Document information records are not retrieved for a document until needed, either for a sort
        or to return data requested by getDocInfo.  Use readDocInfo to force all document records to be
        read for items in this search results list.
        */
        void readDocInfo() {
            dtsSR_ReadDocInfo(handle);
            }
        /* Read search results from an XML file.
           Parameters:
           		fn:     Name of the file to read
           Returns:
           		0 on success, or -1 if an error occurred
         */
        long read(const char *fn) {
            long v = dtsSR_Serialize(handle, fn, 0);
            getCounts(true);
            return v;
            }
        /* Save search results as an XML file.
           Parameters:
           		fn:     Name of the file to create
           Returns:
           		0 on success, or -1 if an error occurred
         */
        long write(const char *fn) {
            return dtsSR_Serialize(handle, fn, dtsSrSerializeWrite);
            }
        /* Save search results as an XML file, saving only the items selected using setSelections.
           Parameters:
           		fn:     Name of the file to create
           Returns:
           		0 on success, or -1 if an error occurred
         */
        long writeSelected(const char *fn) {
            return dtsSR_Serialize(handle, fn, dtsSrSerializeWrite | dtsSrSerializeWriteSelected);
            }
		// internal use only
        void logDocInfo(long iItem, dtsSearchResultsItem *pItem = 0) {
            dtsSR_LogDocInfo(handle, iItem, pItem);
            }
        // Combines search results with another search results object, eliminating duplicate items.
        void unionWith(dtsSearchResults& other) {
            dtsSR_UnionWith(handle, other.getHandle());
            getCounts(true);
            }
        // Inserts into the current search results object only those items that are present in both A and B.
        void intersectionOf(dtsSearchResults& A, dtsSearchResults& B) {
            dtsSR_IntersectionOf(handle, A.getHandle(), B.getHandle());
            getCounts(true);
            }
        /* urlEncode generates a string encoding the contents of the currently
		selected item in search results. The string can be used in a subsequent
		call to urlDecode to re-create the search results item.
		Parameters:
			iItem:    Zero-based index identifying the item to encode
			dest: 	  Buffer to receive the url-encoded item
			maxLen:   Size of the buffer (at least 1024 bytes is recommended)
		Remarks:
			If the buffer is not large enough to store all of the hit offsets, a partial
			list of hit offsets will be stored.
		*/
        int urlEncode(int iItem, char *dest, ptrdiff_t maxLen) {
            return dtsSR_UrlEncodeItem(handle, iItem, dest, (int) maxLen);
            }
        /* urlEncode generates a string encoding the contents of the currently
		selected item in search results. The string can be used in a subsequent
		call to urlDecode to re-create the search results item.
		Parameters:
			iItem:    Zero-based index identifying the item to encode
			maxLen:   Maximum length of string to return, or zero to indicate no limit
			flags:    UrlEncodeItemFlags value specifying options for the URL encoding
		Returns:
			A dtsStringHandle with the result of the URL encoding
		Remarks: 
		    The limit in maxLen is ignored if dtsUrlEncodeAsSearch is set in flags.
		*/
        dtsStringHandle urlEncode(int iItem, int maxLen, int aflags) {
            return dtsSR_UrlEncodeItem2(handle, iItem, maxLen,  aflags);
            }
        // urlEncode generates a string encoding the contents of the currently selected item in search results. The string can be used in a subsequent call to urlDecode to re-create the search results item.
        // Parameters:
        //   str:    String containing a search results item encoded with urlEncode
        int urlDecode(const char *str) {
            int v = dtsSR_UrlDecodeItem(handle, str);
            getCounts(true);
            return v;
            }
        /* serializeAsXml() encodes all of the data in a search results object
		as an XML stream and returns it using a dtsStringHandle. If iItem is -1,
		all of the items in search results are returned. If iItem is -2, only
		the XML describing the search results as a whole (the request, total
		hits, etc.) is returned. Otherwise, only the item indicated by iItem is
		returned. */
        dtsStringHandle serializeAsXml(int iItem = -1) {
            return dtsSR_SerializeAsXml(handle, iItem);
            }
		// serializeFromXml() extracts search results data from a stream generated by serializeAsXml.
        void serializeFromXml(const char *s) {
            dtsSR_SerializeFromXml(handle, s);
            fGotCounts = 0;
            }

        int good() {
            if (!handle)
                return 0;
            getCounts(true);
            return (fileCount >= 0);
            }
        /* Add a document to search results.
           Parameters
           indexRetrievedFrom :  The path to the index containing this document.
           docId :               The document id of this document (usually obtained from
                                 dtsSearchResultsItem.docId).
           hitCount :            Number of hits in the document.
           hits :                A vector of hitCount 32\-bit integers with the word offsets
                                 of hits.                                                    */
		int addItem(const char *indexRetrievedFrom, long docId, long hitCount, long *hits) {
			fGotCounts = false;
			return dtsSR_AddItem(handle, indexRetrievedFrom, docId, hitCount, hits);

			}
        /* Add multiple documents to search results.
           Parameters
           indexRetrievedFrom :  The path to the index containing the documents.
           count :               Number of document ids to add.
           docIds :              Document ids to add.                            */
		int addItems(const char *indexRetrievedFrom, long count, long *docIds) {
			fGotCounts = false;
			return dtsSR_AddItems(handle, indexRetrievedFrom, count, docIds);

			}
		// Set the selection state of the items in the search results list.
		void setSelections(long count, long *selections) {
			dtsSR_SetSelections(handle, count, selections);
			}
		/* Retrieve the selection state of the items in the search results list. */
		dtsIntArrayHandle getSelections() {
			return dtsSR_GetSelections(handle);
			}

    protected:
        dtsSearchResultsHandle handle;
        long fOwnsHandle;
        long fileCount;
        long totalFiles;
        long totalHits;
        long fGotCounts;
        long flags;
        long maxCount;
        long searchFlags;
        int fuzziness;
        long reserved[30];

        void getCounts(int fForceUpdate = false) {
            if (fForceUpdate || !fGotCounts) {
                dtsSR_GetCounts(handle, &fileCount, &totalFiles, &totalHits);
                dtsSR_GetSearchFlags(handle, &searchFlags, &fuzziness);
                }
            fGotCounts = 1;
            }

    };

class dtsJobOptions {
    public:
        dtsJobOptions() :
            handle(0)
        { }
        ~dtsJobOptions() {
            freeHandle();
            }
        dtsJobOptions(const dtsJobOptions& other) : handle(0) {
            *this = other;
            }
        dtsJobOptions& operator=(const dtsJobOptions& other) {
            dtsOptions options;
            other.getOptions(options);
            create(options);
            return *this;
            }
        void freeHandle() {
            if (handle) {
                dtsJO_Free(handle);
                handle = 0;
                }
            }
        dtsJobOptionsHandle getHandle() const {
            return handle;
            }
        void setupJob(dtsJobBase& aJob) const {
            aJob.jobOptions = getHandle();
            }
        void create(dtsOptions& options) {
            freeHandle();
            handle = dtsJO_Create(options);
            }
        void getOptions(dtsOptions& options) const {
            if (handle)
                dtsJO_GetOptions(handle, options);
            }
    protected:
        dtsJobOptionsHandle handle;
    };

/* Summary
   Used to efficiently limit the results of a search to a specified subset of one or
   more indexes.
   Remarks
   * Overview *
   The dtsSearchFilter class provides a way to designate which documents can be
   returned by a search. It is useful in situations where a text search using must
   be combined with a search of a database. The database search is done first, and
   then the results of the database search are used to limit the dtSearch search.

   To use a search filter to limit a search,
     1. Create the search filter, or read a previously-create search filter from a
        disk file.
     2. Attach the search filter to a dtsSearchJob's searchFilterHandle
   * Document Ids *
   Search filters do not use names to identify documents because a filter may
   specify thousands, or hundreds of thousands, of documents, and a table of
   filenames would take too much memory and would take too long to check. Instead,
   each document is identified by (a) the index it belongs to, and (b) the
   document's document id, or docId, a unique integer that is assigned to each
   document in an index. The docId for a document can be obtained by searching for
   the document by name, and then examining the document's properties in search
   results. In the C++ API, the docId is returned as dtsSearchResultsItem.docId.
   See: <link Document Ids>

   The first document added to an index has the DocId 1, and subsequent documents
   will have sequentially numbered DocIds 2, 3, 4, and so forth. When a document is
   reindexed, its docId is "cancelled" and a new docId is assigned. Compressing an
   index renumbers all of the docIds, so after an index has been compressed, a
   document's docId may change.

   A docId that is selected may be returned in search results. A document that is
   not selected will not be returned in search results, even if it otherwise
   satisfies the search request.

   If the criteria for the search filter can be expressed as one or more search
   requests, you can use selectItemsBySearch to select documents in the
   SearchFilter.
   * Indexes and Index identifiers *
   A search filter can cover any number of indexes. To add an index to a search
   filter, call addIndex() with the full path to the index. The path must be
   expressed exactly as it will be expressed in the search job. The addIndex()
   function returns an integer that is used to identify that index when selecting
   and de-selecting documents for the filter. (This makes the selection and
   de-selection functions, which may be called thousands of times, more effiicent.)
   * Implementation *
   A search filter is implemented in the dtSearch Engine using a table of bit
   vectors, one for each index in the filter. Each bit vector has one bit for each
   document in its index. For example, a search filter for a single index with
   1,000,000 documents would have 1,000,000 bits, or 125 kilobytes of data. The
   write function compresses the bit vector where possible, so a saved search filter
   may be smaller than the bit vectors that it contains.

   When a search filter is created, the dtSearch Engine returns a handle to it. The
   dtsSearchFilter class wraps this handle.
   * Example *
   The following code sequence demonstrates the use of dtsSearchFilter to limit a
   search:
   <code>
       dtsSearchJob searchJob;
        ... set up search job ...
         // Set up a filter that will only allow documents 1 through 10 in the index
        // to be returned.
        dtsSearchFilter filter;
        int iIndex = filter.addIndex("c:\\data\\index");
        filter.selectItems(iIndex, 1, 10, true);

        // Attach the search filter to the search job
        searchJob.searchFilterHandle = filter.getHandle();

        // Execute the search
       \short errFlag;
       dtssDoSearchJob(searchJob, errFlag);
   </code>                                                                           */
class dtsSearchFilter {
    public:
    	// Create an empty search filter
        dtsSearchFilter() {
            m_hFilter = dtsSF_Alloc();
            }
        // Free resources allocated for a search filter
        ~dtsSearchFilter() {
            freeHandle();
            }
        // Create a search filter that is a copy of another search filter
        dtsSearchFilter(const dtsSearchFilter& other) {
            m_hFilter = 0;
            copy(other);
            }
        // Copy another search filter
        dtsSearchFilter& operator=(const dtsSearchFilter& other) {
            copy(other);
            return *this;
            }
        // Clear all data from a search filter
        void clear() {
			freeHandle();
            m_hFilter = dtsSF_Alloc();
		}
		int good() const {
			return m_hFilter != 0;
			}
		/* Returns the handle for a search filter, which can be used to limit a search
		(set dtsSearchJob.searchFilterHandle) or to receive the results of a search (set
		dtsSearchJob.outputSearchFilterHandle).
		*/
        dtsSearchFilterHandle getHandle() const {
            return m_hFilter;
            }
		/*
			Returns the handle for the search filter and detaches it from this object,
			so the caller is responsible for deleting the search filter.
		*/
		dtsSearchFilterHandle detachHandle() {
			dtsSearchFilterHandle hRet = m_hFilter;
			m_hFilter = 0;
			return hRet;
			}
        // Copy a search fitler
        void copy(const dtsSearchFilter& other) {
            freeHandle();
            m_hFilter = dtsSF_Copy(other.m_hFilter);
            }

        // Copy a search fitler
        void copy(dtsSearchFilterHandle otherHandle) {
            freeHandle();
            m_hFilter = dtsSF_Copy(otherHandle);
            }

        /* Select no documents in the index. By default, no documents are
		selected when a search filter is created, so there is no need to call
		selectNone to initialize the filter. */
        void selectNone(int iIndex) {
            if (m_hFilter)
                dtsSF_SelectNone(m_hFilter, iIndex);
            }
        /* Select all of the documents in the index. */
        void selectAll(int iIndex) {
            if (m_hFilter)
                dtsSF_SelectAll(m_hFilter, iIndex);
            }
        /* Summary
           Add an index to the filter, returning an integer that can be used to identify the
           index in the selection functions.
           Parameters
           indexPath :  Path to the index, exactly as it is provided in
                        dtsSearchJob.indexesToSearch
           Returns
           Integer identifying the index in this search filter. If the index is already
           present in the search filter, the index is not added a second time, and the
           identifier for the existing instance of the index is returned. Therefore,
           addIndex() can be used to get the identifier for an existing index.
                                                                                             */
        int addIndex(const char *indexPath) {
            if (!m_hFilter)
                m_hFilter = dtsSF_Alloc();
            return dtsSF_AddIndex(m_hFilter, indexPath);
            }
		/*
		Set the selection state of a list of document ids to the value indicated by fSelected.

		Parameters:
			iIndex:     The index containing these documents (identifier returned by addIndex)
			idList:	    Pointer to a list of document ids
			idCount:    Number of ids in the idList
			fSelected:  Indicates whether the listed documents should be selected or not
		*/
        void setSelections(int iIndex, unsigned long *idList, unsigned long idCount, int fSelected) {
            if (m_hFilter)
                dtsSF_SetSelections(m_hFilter, iIndex, idList, idCount, fSelected);
            }
		/*
		Set the selection state of a range of document ids to the value indicated by fSelected.

		Parameters:
			iIndex:     The index containing these documents (identifier returned by addIndex)
			firstId:	First document to select
			lastId:     Last document to select
			fSelected:  Indicates whether the listed documents should be selected or not
		*/
        void selectItems(int iIndex, unsigned long firstId, unsigned long lastId, int fSelected) {
            if (m_hFilter)
                dtsSF_SelectItems(m_hFilter, iIndex, firstId, lastId, fSelected);
            }
		/* Set the selection state of a set of document ids based on the results
		of a search.  All documents retrieved by the search set up by searchJob
		will have their selection state changed to the state indicated by
		fSelected.

		*All* documents retrieved in the search are selected, regardless of the maxFilesToRetrieve
		value in the dtsSearchJob.

		Parameters:
			iIndex:     The index containing these documents (identifier returned by addIndex)
			searchJob:	The search to execute
			fSelected:  Indicates whether the listed documents should be selected or not
		*/
        void selectItemsBySearch(int iIndex, dtsSearchJob& searchJob, int fSelected) {
			if (m_hFilter)
				dtsSF_SelectItemsBySearch(m_hFilter, iIndex, searchJob, fSelected);
			}
		/* Select all document ids listed in search results (deprecated)

		   makeFrom() is deprecated because the search results may contain only some of the
		   matching documents (for example, a SearchResults object may contain only the best-matching
		   100 documents).  Instead, either using selectItemsBySearch or setting SearchJob.WantResultsAsFilter
		   is recommended, to ensure that the SearchFilter produced is complete.
		*/
        void makeFrom(dtsSearchResultsHandle hResults) {
            freeHandle();
            m_hFilter = dtsSF_MakeFromSearchResults(hResults);
            }
		/* Returns an array of integers containing the document ids of the documents selected in the filter.
		*/
        dtsIntArrayHandle getItems(int iIndex) const {
			return dtsSF_GetItems(m_hFilter, iIndex);
			}
		/* Returns the index path for an index in the filter */
		dtsStringHandle getIndexPath(int iIndex) const {
			return dtsSF_GetIndexPath(m_hFilter, iIndex);
			}
		/* Returns the number of indexes in the filter, or -1 if the handle is missing or invalid */
		int getIndexCount() const {
			return dtsSF_GetIndexCount(m_hFilter);
			}
		bool checkHandle() {
			return (getIndexCount() >= 0);
		}
		/* Read the search filter from a disk file. Any existing data
		in the search filter is replaced by the data read from the file.
		Parameters:
			name: 	File to read the filter from
		Returns:  0 on success, or non-zero if an error occurred.
		*/
        int read(const char *name) {
            freeHandle();
            m_hFilter = dtsSF_Read(name);
            if (m_hFilter == 0)
                return -1;
            else
                return 0;
            }
		/* Write the search filter to a disk file.
		Parameters:
			name: 	File to create
		Returns:  0 on success, or non-zero if an error occurred.
		*/
        int write(const char *name) {
            if (m_hFilter)
                return dtsSF_Write(m_hFilter, name);
            else
                return -1;
            }
		// Compare two search filters for equality.  Two search filters are equal if
		// they contain the same indexes and, for each index, the same document selections
		int equals(const dtsSearchFilter& other) {
			return dtsSF_Equals(m_hFilter, other.m_hFilter);
			}
		/* Select only the document ids that are also selected in another search filter */
        void andWith(const dtsSearchFilter& other) {
            dtsSF_And(m_hFilter, other.m_hFilter);
            }
 		/* Select any document ids that are also selected in another search filter */
        void orWith(const dtsSearchFilter& other) {
            dtsSF_Or(m_hFilter, other.m_hFilter);
            }
		/* Select only the document ids that are not selected in another search filter */
        void andNot(const dtsSearchFilter& other) {
            dtsSF_AndNot(m_hFilter, other.m_hFilter);
            }
       	/* Retrieves information about whether a search filter is out of date.

       	   If the index was compressed (without the dtsIndexKeepExistingDocIds flag) or created
       	   since the search was done, the search filter is obsolete because the doc ids in the
       	   search filter will not correspond to the doc ids in the index.

       	   If the index was updated since the search filter was created, it may be incomplete.
       	  */
        bool getExpiredState(bool *pObsolete, bool *pIncomplete) {
        	bool succeeded = false;
        	dtsSF_GetExpiredState(m_hFilter, pObsolete, pIncomplete, &succeeded);
        	return succeeded;
        	}
    protected:
        void freeHandle() {
            if (m_hFilter) {
				dtsSearchFilterHandle toDelete = m_hFilter;
				m_hFilter = 0;
                dtsSF_Free(toDelete);
                }
            }
        dtsSearchFilterHandle m_hFilter;
    };

/* Summary
   dtsOptionsReader is a simple class wrapper around dtsOptions2 to handle
   allocation of the data buffer used for variable-length values in dtsOptions2. */
class dtsOptionsReader {
	public:
		dtsOptionsReader() : dataBuffer(0), bufferSize(0) { }
		~dtsOptionsReader() { 
			freeBuffer(); 
			}
		// The option settings retrieved by dtsOptionsReader.get()
		dtsOptions2 options;
		// Retrieve the option settings currently in effect on this thread.
		// The settings will be stored in options.
		bool get() {
			int sizeNeeded = 0;
			dtssGetOptions2(options, 0, 0, &sizeNeeded);
			freeBuffer();
			dataBuffer = new char[sizeNeeded];
			if (!dataBuffer)
				return false;
			bufferSize = sizeNeeded;
			dtssGetOptions2(options, dataBuffer, bufferSize, &sizeNeeded);
			if (sizeNeeded > bufferSize)
				return false; // should never happpen
			return true;
			}
	private:
		// Buffer to receive variable-length data stored in options
		char *dataBuffer;
		// Size of the allocated buffer
		int bufferSize;
		void freeBuffer() {
			if (dataBuffer) {
				delete [] dataBuffer;
				dataBuffer = 0;
				bufferSize = 0;
			}
		}
	};

#endif

#endif // SUPPRESS_DTENGINE_IMPORTS

/* Restore alignment */
#if defined(__BORLANDC__)
    #pragma option -a.
#elif defined(_MSC_VER)
    #if defined(WIN32)
        #pragma pack(pop, dtSearchHeader)
    #else
        #pragma pack( )
    #endif
#endif

#endif
