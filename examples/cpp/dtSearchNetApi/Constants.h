#pragma once

namespace dtSearch {

namespace Engine {

		/// <summary>
		/// Values that CheckForAbort (IIndexStatusHandler or
		/// ISearchStatusHandler) can return
		/// to indicate whether a job should continue.
		/// </summary>
	public __value enum AbortValue {

		/// <summary>
		/// Continue the search or index update
		/// </summary>
		Continue = 0,

		/// <summary>
		/// Cancel the search or index update.  For index updates,
		/// documents indexed so far are saved, and the indexer
		/// will stop after completing the current document.
		/// </summary>
		Cancel = -1,

		/// <summary>
		/// For index updates only, cancels the update immediately,
		/// without saving any changes to the index.
		/// </summary>
		CancelImmediately = -2
	};


    	/// <summary>
   		/// Values for IndexJob.StatusIndexingStep
		/// </summary>
    public __value enum IndexingStep {

		/// <summary>
		/// No indexing in progress
		/// </summary>
        ixStepNone,

        /// <summary>
        /// Indexing started
        /// </summary>
        ixStepBegin,

        /// <summary>
        /// Creating index
        /// </summary>
        ixStepCreatingIndex,

        /// <summary>
        /// Checking files to see which files need to be reindexed
        /// </summary>
        ixStepCheckingFiles,
        /// <summary>
        /// Reading input files and extracting words
        /// </summary>
        ixStepReadingFiles,

        /// <summary>
        /// Storing word references in the index
        /// </summary>
        ixStepStoringWords,

        /// <summary>
        /// Merging words into the index
        /// </summary>
        ixStepMerging,

        /// <summary>
        /// Compressing the index to remove obsolete information
        /// </summary>
        ixStepCompressing,

        /// <summary>
        /// Done indexing
        /// </summary>
        ixStepDone,

        /// <summary>
        /// The index is being verified
        /// </summary>
        ixStepVerifyingIndex,

        /// <summary>
        /// Merging two or more indexes into a single index
        /// </summary>
        ixStepMergingIndexes
        };

    	/// <summary>
   		/// Values for SearchResults.Sort
		/// </summary>
    [System::Flags]
    public __value enum SortFlags {

		/// <summary>
		/// Sort in descending order.
		/// </summary>
        dtsSortDescending = 0,

        /// <summary>
        /// Sort in ascending order
        /// </summary>
        dtsSortAscending = 2,

        /// <summary>
        /// Sort by filename (without path)
        /// </summary>
        dtsSortByName = 4,

        /// <summary>
        /// Sort by modification date, including the time
        /// </summary>
        dtsSortByDate = 8,

        /// <summary>
        /// Sort by hit count or score, depending on whether the automatic term weighting was used in the search
        /// </summary>
        dtsSortByHits = 16,

        /// <summary>
        /// Sort by file size
        /// </summary>
        dtsSortBySize = 32,

        /// <summary>
        /// Sort by one of the UserFields.
        /// </summary>
        dtsSortByField = 64,

        /// <summary>
        /// Sort by the index the document was found in.
        /// </summary>
        dtsSortByIndex = 128,

        /// <summary>
        /// Sort by file type
        /// </summary>
        dtsSortByType = 256,

        /// <summary>
        /// Sort by the title string
        /// </summary>
        dtsSortByTitle = 512,

        /// <summary>
        /// Sort by the path of the file
        /// </summary>
        dtsSortByLocation = 1024,

        /// <summary>
        /// Sort by modification time, ignoring the date
        /// </summary>
        dtsSortByTime = 2048,

        /// <summary>
        /// Sort by caller-specified sort key (use SetSortKey for each item to specify the key)
        /// </summary>
        dtsSortBySortKey = 4096,

        /// <summary>
        /// Make string comparisons in the sort case-insensitive
        /// </summary>
        dtsSortCaseInsensitive = 0x10000L,

        /// <summary>
        /// Sort by the integral numeric value of a field instead of its string value.
        /// </summary>
        /// <remarks>
        /// This would cause "20" to be considered greater than "9". The sort key will be a signed, 32-bit integer.
        /// </remarks>
        dtsSortNumeric = 0x20000L,

        /// <summary>
        /// When sorting by filename, use the PDF Title as the filename for PDF files
        /// </summary>
        dtsSortPdfUseTitleAsName = 0x40000L,

        /// <summary>
        /// When sorting by filename, use the HTML Title as the filename for HTML files
        /// </summary>
        dtsSortHtmlUseTitleAsName = 0x80000L,

        /// <summary>
        /// Sort by the floating point numeric value of a field instead of its string value.
        /// </summary>
		dtsSortFloatNumeric = 0x100000L,

		/// <summary>
		/// Remove some leading punctuation or white space from sort value before sorting.  Also removes "re:", "fw:", and "fwd:".
		/// </summary>
        dtsSortCleanText = 0x200000L,

        /// <summary>
        /// Sort by number of hits
        /// </summary>
        dtsSortByHitCount = 0x400000L,

        /// <summary>
        /// Sort by relevance score
        /// </summary>
        dtsSortByRelevanceScore =    0x800000L,

        /// <summary>
        /// Sort by full path and filename
        /// </summary>
        dtsSortByFullName =			0x1000000L
        };

    	/// <summary>
    	/// Flags for Server.ConvertPath
    	/// </summary>
    [System::Flags]
    public __value enum ConvertPathFlags {

		/// <summary>
		/// Convert a local path to a virtual path
		/// </summary>
        dtsCpMakeVirtual = 1,

        /// <summary>
        /// Convert a virtual path to a local path
        /// </summary>
        dtsCpMakeLocal = 2,

        /// <summary>
        /// Convert a mapped path to a UNC path
        /// </summary>
        dtsCpMakeUNC = 0x0008,

        /// <summary>
        /// Use the Metabase to update the table of local-to-virtual mappings that the dtSearch engine uses to perform these conversions
        /// </summary>
        dtsCpRefreshMappings = 0x0010,

        /// <summary>
        /// Store the updated table of mappings in the registry.
        /// </summary>
        dtsCpStoreInRegistry = 0x0020,

        /// <summary>
        /// Read the latest table of mappings from the registry.
        /// </summary>
        dtsCpReadFromRegistry = 0x0040
        };

    	/// <summary>
    	///  Output file formats for conversion of documents and generation of search reports
    	/// </summary>
    public __value enum OutputFormats {

		/// <summary>
		/// HTML
		/// </summary>
        itHTML = 226,

        /// <summary>
        /// Ansi text
        /// </summary>
        itAnsi = 202,

        /// <summary>
        /// Microsoft Rich Text Format
        /// </summary>
        itRTF = 212,

        /// <summary>
        /// Unicode
        /// </summary>
        itUTF8 = 238,

		/// <summary>
		/// XML
		/// </summary>
        itXML = 234,

		/// <summary>
		/// HTML, with all formatting removed
		/// </summary>
        itUnformattedHTML = 256,

		/// <summary>
  		/// Output format for FileConverter that organizes document content, metadata, and attachments into a standard XML format        
		/// </summary>
        it_ContentAsXml		= 108
        

        };

    	/// <summary>
    	/// Error codes returned in a JobErrorInfo through the Errors property of SearchJob, IndexJob, etc.
    	/// </summary>
    public __value enum ErrorCodes {

		/// <summary>
		/// The dtSearch Engine could not access an index
		/// </summary>
        dtsErAccIndex = 1,

        /// <summary>
        /// A temporary file could not be opened
        /// </summary>
        dtsErOpenTemp = 3,

        /// <summary>
        /// An attempt was made to open an index created by an incompatible version of dtSearch.
        /// </summary>
        dtsErIxWrongVersion = 5,

        /// <summary>
        /// A directory (the home or private directory during initialization) could not be accessed.
        /// </summary>
        dtsErAccDirectory = 7,

        /// <summary>
        /// The private directory could not be locked.
        /// </summary>
        dtsErWorkDirFail = 8,

        /// <summary>
        /// A file could not be accessed, either because it was not present or because it was locked in a way that prevented the dtSearch Engine from accessing it.
        /// </summary>
        dtsErAccFile = 9,

        /// <summary>
        ///A file could not be accessed because it appears to be damaged, causing a file parsing error.
        /// </summary>
        dtsErFileCorrupt = 10,

        /// <summary>
        /// The index has reached the maximum size for an index.
        /// </summary>
        dtsErIndexFull = 11,

		/// <summary>
		/// Operation timed out (trigged by TimeoutSeconds).
		/// </summary>
        dtsErTimeout = 13,

        /// <summary>
        ///A file could not be accessed because it is encrypted
        /// </summary>
        dtsErFileEncrypted = 17,


        /// <summary>
        /// The search request provided to the dtSearch Engine contained one or more syntax errors.
        /// </summary>
        dtsErBadRequest = 116,

        /// <summary>
        /// The search request contained no text.
        /// </summary>
        dtsErSearchRequestEmpty = 117,

        /// <summary>
        /// The search was stopped due to the AutoStopLimit setting (too many documents were found).
        /// </summary>
        dtsErSearchLimitReached = 120,

        /// <summary>
        /// No files were retrieved in a search.
        /// </summary>
        dtsErNoFilesRetrieved = 122,

        /// <summary>
        /// The search request was longer than 32,000 bytes
        /// </summary>
        dtsErRequestTooLong = 135,

        /// <summary>
        /// A search request matched too many unique words in an index.
        /// </summary>
        /// <remarks>
        /// The dtSearch Engine limits the number of unique words (not hits) that can be retrieved in a search.
        /// A search for * or *a* would retrieve thousands of words, exceeding this limit.
        /// The default limit is 64k.
        /// To increase or decrease the limit, use Options.MaxWordsToRetrieve.
        /// </remarks>
        dtsErMaxWords = 137,

        /// <summary>
        /// There is not enough disk space to continue indexing
        /// </summary>
        dtsErDiskFull = 159,

        /// <summary>
		/// Index is corrupt
		/// </summary>
		dtsErIxCorrupt = 162,

        /// <summary>
        /// The index could not be locked for update due to an "Access Denied" error.
        /// This means that either (1) the folder's security settings do not permit write access for
        /// the user performing the update, or (2) another user or process was updating the index.
        /// </summary>
        dtsErAccessDenied = 177,

        /// <summary>
        /// Unable to connect to data source during indexing
        /// </summary>
	    dtsErConnectFailed     =201,

        /// <summary>
        /// Unable to access cached document in index
        /// </summary>
	    dtsErAccCachedDoc      =202,

        /// <summary>
        /// Index update could not complete due to an error accessing the index
        /// </summary>
        dtsErIndexingError     =203
        };



	/// <summary>
	/// Values for SearchJob.SearchFlags and WordListBuilder.ListMatchingWords
	/// </summary>
	/// <remarks>
	/// <para>
	/// Synonym searching: to enable synonym searching for all words in a search request,
	/// set both dtsSearchSynonyms and one of the other synonym flags such as dtsSearchUserSynonyms, to
	/// specify the type of synonym searching to perform.   If dtsSearchSynonyms is not set,
	/// then only words marked with the synonym searching character &amp; will be expanded.
	/// </para>
	/// </remarks>

    [System::Flags]
    public __value enum SearchFlags {

		///<summary>
		/// Enable fuzzy searching for all words in the search request
		///</summary>
		dtsSearchFuzzy =                0x0001,

		///<summary>
		/// Enable phonic searching for all words in the search request
		///</summary>
		dtsSearchPhonic =               0x0002,

		///<summary>
		/// Enable stemming for all words in the search request
		///</summary>
		dtsSearchStemming =             0x0004,

		///<summary>
		/// Equivalent to dtsSearchTypeAnyWords | dtsSearchAutoTermWeight.| dtsSearchPositionalScoring
		///</summary>
		dtsSearchNatural =              0x0008,

		///<summary>
		///Enable synonym expansion for all words in the search request.  (See synonym searching note above.)
		///</summary>
		dtsSearchSynonyms =             0x0010,

		///<summary>
		/// When expanding synonyms, use user thesaurus.  (See synonym searching note above.)
		///</summary>
		dtsSearchUserSynonyms =         0x0020,

		///<summary>
		/// When expanding synonyms, use WordNet thesaurus.  (See synonym searching note above.)
		///</summary>
		dtsSearchWordNetSynonyms =      0x0040,

		///<summary>
		///  When expanding synonyms, use related words from WordNet.  (See synonym searching note above.)
		///</summary>
		dtsSearchWordNetRelated =       0x0080,

		///<summary>
		/// Request a list of the words that were matched in each document in SearchResultsItem.HitsByWord
		///</summary>
		dtsSearchWantHitsByWord =       0x0100,

		///<summary>
		/// Include word offsets of hits in HitsByWord report.
		///</summary>
		dtsSearchWantHitsArray =        0x0800,

		///<summary>
		/// Optimize search by waiting until document records are requested through a search results object before reading them from the index.
		///</summary>
		dtsSearchDelayDocInfo =         0x1000,

		///<summary>
		/// Search for synonyms in an external thesaurus (see <i>External Thesaurus API</i>)
		///</summary>
		dtsSearchExternalSynonyms =     0x2000,

		///<summary>
		/// Interpret the search request as a regular expression
		///</summary>
		dtsSearchRegularExpression =   0x4000,

		///<summary>
		/// Request detailed information about the words that generated each hit associated with an item in search results.
		///</summary>
		dtsSearchWantHitDetails =       0x8000,

		///<summary>
		/// Apply the automatic term weighting to each term in the request.   Weighting of retrieved documents takes into account: the number of documents each word in your search request appears in (the more documents a word appears in, the less useful it is in distinguishing relevant from irrelevant documents); the number of times each word in the request appears in the documents; and the density of hits in each document. Noise words and search connectors like NOT and OR are ignored.
		///</summary>
		dtsSearchAutoTermWeight =       0x10000,

		///<summary>
		/// Find all of the words in the search request
		///</summary>
		dtsSearchTypeAllWords =         0x20000,

		///<summary>
		/// Find any of the words in the search request
		///</summary>
		dtsSearchTypeAnyWords =         0x40000,

		///<summary>
		/// When selecting the documents to return (up to the MaxFilesToRetrieve limit in the SearchJob), select the most recent documents rather than the best-matching documents.
		/// Cannot be used with dtsSearchDelayDocInfo, because the document date for each document is needed.
		///</summary>
		dtsSearchSelectMostRecent =     0x80000,

		///<summary>
		/// Rank documents higher when hits are closer to the top of the document and when hits are located close to each other within a document.  (This improves relevancy ranking for "all words" and "any words" searches.)
		///</summary>
		dtsSearchPositionalScoring =    0x100000,

	    /// <summary>
	    /// Apply Language Analyzer to search request, if this application uses an external language analyzer (see "Language Analyzer API")
	    /// </summary>
	  	dtsSearchApplyLanguageAnalyzer	=0x200000

        };

    	/// <summary>
    	/// Flags for FileConverter.Flags
    	/// </summary>
    [System::Flags]
    public __value enum ConvertFlags {
			/// <summary>
			/// Assume that the input file is HTML for purposes of deciding whether to do HTML-to-HTML highlighting.
			/// </summary>
        dtsConvertInputIsHtml        = 0x0001,
			/// <summary>
			/// Do not insert BeforeHit and AfterHit marks for a hit that cannot be displayed in HTML because the text
			/// is not visible. If this flag is not set, a pair of beforeHit/afterHit tags will be added before the
			/// next visible text in the file.
			/// </summary>
        dtsConvertSkipHiddenHits    = 0x0002,

			/// <summary>
			/// Remove JavaScript from HTML files when adding hit highlight markings.   JavaScript in HTML files can cause errors if displayed outside will have inthe expected context of the script.
			/// </summary>
        dtsConvertRemoveScripts     = 0x0004,

        	/// <summary>
        	/// Assume that the input file is not HTML for purposes of deciding whether to do HTML-to-HTML conversion. If the output format is HTML, this forces the dtSearch Engine to convert the file to simple text before adding hit highlight markings.
        	/// </summary>
        dtsConvertInputIsNotHtml    = 0x0008,

        /// <summary>
        ///  For internal use.
        /// </summary>
        dtsConvertViaHtml           = 0x0010,

        	/// <summary>
        	/// Add hit highlight markings to XML data while retaining the XML format. Set the OutputFormat to itXML.  See: Highlighting hits in XML files
        	/// </summary>
        dtsConvertXmlToXml			= 0x0040,

       	/// <summary>
        /// Just detect the file type of the document and return it in DetectedTypeId, without generating
		/// any output.
       	/// </summary>
 		dtsConvertJustDetectType	= 0x0080,


        	/// <summary>
        	/// Read input file from a cached document stored in the index.
        	/// </summary>
        dtsConvertGetFromCache		= 0x2000,
        
        /// <summary>
        /// If the input is a container (such as a ZIP file), recursively unpack each item in the 
		/// container, convert it, and append it to the output.
		/// </summary>
		dtsConvertInlineContainer	= 0x4000   

        };

		/// <summary>
		/// Flags for SearchReportJob.Flags
		/// </summary>
    [System::Flags]
    public __value enum ReportFlags {

		///<summary>
		/// Count context by paragraphs
		///</summary>
		dtsReportByParagraph  = 0x0001,

		///<summary>
		/// Count context by words
		///</summary>
		dtsReportByWord       = 0x0002,

		///<summary>
		/// Include all items in the search report
		///</summary>
		dtsReportIncludeAll   = 0x0004,

		///<summary>
		/// Include full text of each selected file in the report
		///</summary>
		dtsReportWholeFile    = 0x0008,

		///<summary>
		/// Count context by words, and include exactly the requested amount of context
		///</summary>
		dtsReportByWordExact  = 0x0010,

		///<summary>
		/// Use cached version of document stored in the index, if present
		///</summary>
		dtsReportGetFromCache = 0x0020,

		///<summary>
		/// Store the search report for each item in search results
		///</summary>
		dtsReportStoreInResults = 0x0040,

		/// <summary>
		/// Limit the size of context blocks when hits are contiguous.
		/// Using this flag prevents large context blocks from being extracted when
		/// many hits occur close together.  For example, if a report is generated
		/// with 5 words of context around each hit, a single isolated hit would
		/// result in 11 words in a context block.   However, if there were 100 hits
		/// clustered together, a much larger context block would be generated.
		/// </summary>
		 dtsReportLimitContiguousContext = 0x0080,

		 /// <summary>
		 /// Include a block of text from the top of the file in the output
		 /// </summary>
		 dtsReportIncludeFileStart = 0x0100

        };

		/// <summary>
		/// Flags for Server.SetDebugLogging
		/// </summary>
	[System::Flags]
    public __value enum DebugLogFlags {

		/// <summary>
		/// Each line in the log will include the time, in hundredths of a second, since the start of execution.
		/// </summary>
        dtsLogTime                  = 0x0001,

        /// <summary>
        /// The log will be committed to disk after each line is written. This slows execution considerably but ensures that the log will survive a system crash.
        /// </summary>
        dtsLogCommit                = 0x0002,

        /// <summary>
        /// Log data is appended to the file if it already exists.
        /// </summary>
        dtsLogAppend                = 0x0004,

        /// <summary>
        /// Generate a stack trace in the logFile if an unhandled exception occurs.
        /// </summary>
        /// <remarks>
        /// dtsCrashLog should not be combined with the other flags and the filename provided
        /// should be different from the filename used for logging.
        /// </remarks>
        dtsCrashLog                 = 0x1000
        };

    	/// <summary>
    	/// Flags for Options.FieldFlags
    	/// </summary>
    [System::Flags]

    /// <summary>
    /// Values for Options.FieldFlags
    /// </summary>
    /// <remarks>
    ///<para>
    /// FieldFlags provide options to control the indexing of meta-data associated with
    /// documents.  When highlighting hits, it is important to make sure that FieldFlags
    /// has the same options that were used when a document was indexed.  Otherwise,
    /// hit highlighting may be incorrect due to differences in the words found in each document.
    ///</para>
    ///<para>
    /// By default, dtSearch will index fields in documents such as the Summary Information
    /// fields in Word files and META tags in HTML files.   FieldFlags can be used to suppress
    /// some or all of this metadata.
    ///</para>
    ///<para>
    /// dtSearch will also add a "Filename" field to the end of each document, with the full path
    /// and filename of the document, so words in the document name will be searchable like other
    /// text.   To suppress this completely, use dtsoFfSkipFilenameField.  To include only
    /// the name of the document (not the path), use dtsoSkipFilenameFieldPath.
    ///</para>
    ///<para>
    /// The dtsoFfHtmlShow* flags can be used to make normally hidden HTML elements, such as
    /// styles or links, visible and searchable.   For each category of element that is enabled,
    /// a section will be added to the end of the HTML file listing the items in that category.
    /// For example, if dtsoFfHtmlShowComments is set, then each HTML file will have a list of
    /// the embedded comments after the body of the HTML.
    ///</para>
    /// </remarks>
    public __value enum  FieldFlags {

		///<summary>
		/// Do not generate a field named Filename containing the name of the file.
		///</summary>
		dtsoFfSkipFilenameField       =  0x0001,

		///<summary>
		///  Do not index or search document summary fields
		///</summary>
		dtsoFfSkipDocumentProperties  =  0x0002,

		///<summary>
		/// Make HTML links searchable
		///</summary>
		dtsoFfHtmlShowLinks           =  0x0004,

		///<summary>
		/// Make HTML IMG src= attribute searchable
		///</summary>
		dtsoFfHtmlShowImgSrc          =  0x0008,

		///<summary>
		/// Make HTML Comments searchable
		///</summary>
		dtsoFfHtmlShowComments        =  0x0010,

		///<summary>
		/// Make HTML Scripts searchable
		///</summary>
		dtsoFfHtmlShowScripts         =  0x0020,

		///<summary>
		/// Make HTML style sheets searchable
		///</summary>
		dtsoFfHtmlShowStylesheets     =  0x0040,

		///<summary>
		/// Make HTML meta tags searchable and visible, appended to the body of the HTML file
		///</summary>
		dtsoFfHtmlShowMetatags        =  0x0080,

		///<summary>
		/// Suppress automatic generation of the HtmlTitle field for the title and the HtmlH1,
		/// HtmlH2, etc. fields for header content in HTML files.
		///</summary>
		dtsoFfHtmlNoHeaderFields      =  0x0200,

		///<summary>
		/// Skip non-text streams in Office (Word, Excel, PowerPoint) documents.
		///</summary>
		dtsoFfOfficeSkipHiddenContent =  0x0400,

		///<summary>
		/// Do not index field names in XML files
		///</summary>
		dtsoFfXmlHideFieldNames       =  0x0800,

		///<summary>
		/// Make NTFS file properties searchable
		///</summary>
		dtsoFfShowNtfsProperties      =  0x1000,

		///<summary>
		/// Do not index attributes in XML files
		///</summary>
		dtsoFfXmlSkipAttributes		  =  0x2000,

		///<summary>
		///  Include only the filename (not the path) in the Filename field generated at the end of each document.
		///</summary>
		dtsoFfSkipFilenameFieldPath	  =  0x4000,

		/// <summary>
		/// Skip attachments in PDF files.
		/// </summary>
  		dtsoFfPdfSkipAttachments = 0x8000,
  				
		/// <summary>
  		/// Skip HTML INPUT tag "value" attributes
		/// </summary>
		dtsoFfHtmlSkipInputValues = 0x10000,
  
		/// <summary>
  		/// Skip HTML IMG tag "alt" attributes
		/// </summary>
  		dtsoFfHtmlSkipImageAlt = 0x20000

		
        };

		/// <summary>
		/// Values for Options.Hyphens
		/// </summary>
		/// <remarks>
		/// <para>
		/// The dtSearch Engine supports four options for the treatment of hyphens when indexing documents: spaces, searchable text, ignored, and "all three".
		/// </para><para>
		/// <i>For most applications, treating hyphens as spaces is the best option.</i> Hyphens are translated to spaces during indexing and during searches. For example, if you index "first-class mail" and search for "first class mail", "first-class-mail", or "first-class mail", you will find the phrase correctly.
		/// </para><para>
		/// <b>Effect on Indexes</b>
		/// </para><para>
		/// When an index is created, the hyphenation option currently in effect is stored in the index, and cannot be changed without re-creating that index. Therefore, the hyphenation option you select affects any indexes you create in the future, but it does not affect indexes that already exist.
		/// </para><para>
		/// When a user searches an index, the hyphenation option for that index applies to the user's search request.
		/// </para><para>
		///
		/// <b>How the options apply during indexing</b>
		///
		/// </para><para>
		/// During indexing, dtSearch extracts a stream of words from each document, and each word is assigned a number that represents that word's position in the file. The first word is assigned the position "1", the second word is assigned the position "2", and so forth. Consider a document that starts with the sentence, "I sent it by first-class mail". The following describes how the document would be treated under each of the hyphenation options:
		/// </para><para>
		/// 1.   Hyphens treated as spaces:
		/// I (1), sent (2), it (3), by (4), first (5), class (6), mail (7)
		/// </para><para>
		/// 2.   Hyphens treated as searchable characters:
		/// I (1), sent (2), it (3), by (4), first-class (5), mail (6)
		/// </para><para>
		/// 3.   Hyphens ignored:
		/// I (1), sent (2), it (3), by (4), firstclass (5), mail (6)
		/// </para><para>
		/// 4.   All three:
		/// I (1), sent (2), it (3), by (4), (5) first-class, (6) first-class, (5) first, (6) class, (5) firstclass
		/// (6) firstclass, (7) mail
		/// </para><para>
		///
		/// </para><para>
		/// <b>How the options apply during searching</b>
		///
		/// </para><para>
		/// During a search, dtSearch translates the search request according to the hyphenation option for the index being searched. For example, if you search for "first-class" in an index created with hyphens treated as spaces, the search request is translated into "first class".
		///
		/// </para><para>
		/// During a search of an index created with the "all three" option, the search request is not modified. For example, if you search for "first-class", dtSearch will not search for "firstclass" or "first class".
		///
		/// </para><para>
		/// <b>Effects of the "all three" option</b>
		/// </para><para>
		/// The "all three" option has one advantage over treating hyphens as spaces: it will return a document containing "first-class" in a search for "firstclass". Otherwise, it provides no benefit over treating hyphens as spaces, and it has some significant disadvantages:
		/// </para><para>
		/// 1.   The "all three" option generates many extra words during indexing. For each pair of words separated by a hyphen, six words are generated in the index.
		/// </para><para>
		/// 2.   It can produce unexpected results in searches involving longer phrases or words with multiple hyphens. With the "all three" option enabled, the sequence "a-b-c" would be indexed as: a (1), ab (1) a-b (1), ab (2), a-b (2), b (2), b-c (2), bc (2), b-c (3), bc (3), c (3). Thus, "a b c" would be found as would "a bc" or "ab c", but not "a-b-c" or "a-bc" or "ab-c". (To prevent the number of permutations from becoming excessive, dtSearch only permutes one hyphen at a time.)
		/// </para>
		/// </remarks>
    public __value enum  HyphenSettings {

		///<summary>
		/// index "first-class" as "firstclass"
		///</summary>
		dtsoHyphenAsIgnore           = 1,

		///<summary>
		/// index "first-class" as "first-class"
		///</summary>
		dtsoHyphenAsHyphen           = 2,

		///<summary>
		/// index "first-class" as "first" and "class"
		///</summary>
		dtsoHyphenAsSpace            = 3,

		///<summary>
		/// index "first-class" all three ways
		///</summary>
		dtsoHyphenAll                = 4
        };

		/// <summary>
		/// Values for Options.BinaryFiles
		/// </summary>
		/// <remarks>
		///<para>
		/// Use Options.BinaryFiles to specify whether dtSearch should index binary files as plain text, skip them entirely, or filter out only the text of binary files.
		///</para>
		/// <para>
		/// Binary files are files that dtSearch does not recognize as documents.   Examples of binary files include executable programs, fragments of documents recovered through an "undelete" process, or blocks of unallocated or recovered data obtained through computer forensics.  Content in these files may be stored in a variety of formats, such as plain text, Unicode text, or fragments of .DOC or .XLS files.  Many different fragments with different encodings may be present in the same binary file.  Indexing such a file as if it were a simple text file would miss most of the content.
		/// </para>
		/// <para>
		/// The dtSearch filtering algorithm scans a binary file for anything that looks like text using multiple encoding detection methods.  The algorithm can detect sequences of text with different encodings or formats in the same file, so it is much better able to extract content from recovered or corrupt data than a simple text scan.  Input files can be up to 2 Gb in size.
		/// </para>
		/// </remarks>
    public __value enum BinaryFilesSettings {

		///<summary>
		/// Filter text from binary files using the character array in binaryFilterTextChars to determine which characters are text.
		///</summary>
		dtsoFilterBinary        = 1,

		///<summary>
		/// Index all contents of binary files as single-byte text.
		///</summary>
		dtsoIndexBinary         = 2,

		///<summary>
		/// Do not index files binary files
		///</summary>
		dtsoIndexSkipBinary     = 3,

		///<summary>
		/// Filter text from binary files using a text extraction algorithm that scans for sequences of single-byte, UTF-8, or Unicode text in the input.  This option is recommended for working with forensic data, particularly when searching for non-English text.
		///</summary>
		dtsoFilterBinaryUnicode = 4
		} ;

		/// <summary>
		/// Values for Options.UnicodeFilterFlags
		/// </summary>
     [System::Flags]
   public __value enum UnicodeFilterFlags {

		///<summary>
		/// Extracting blocks as HTML has no effect on the text that is extracted, but it adds additional information in
		/// HTML comments to each extracted block.  The HTML comments identify the starting byte offset and encoding of
		/// each piece of text extracted from a file.
		///</summary>
		dtsoUfExtractAsHtml = 1,

		///<summary>
		/// Overlapping blocks prevents text that crosses a block boundary from being missed in the filtering process.  With overlapping enabled, each block extends 256 characters past the start of the previous block.
		///</summary>
		dtsoUfOverlapBlocks = 2,

		/// <summary>
		/// Automatically insert a word break in long sequences of letters.
		/// A word break will be inserted when the word length reaches
		/// Options.MaxWordLength.
		/// </summary>
		dtsoUfAutoWordBreakByLength	= 0x0004,

		/// <summary>
		/// Automatically insert a word break when a capital letter appears
		/// following lower-case letters.
		/// </summary>
		dtsoUfAutoWordBreakByCase = 0x0008,


		/// <summary>
		/// Automatically insert a word break when a digit follows letters.
		/// </summary>
		dtsoUfAutoWordBreakOnDigit = 0x0010,

		/// <summary>
		/// When a word break is automatically inserted due to dtsoUfAutoWordBreakByLength,
		/// overlap the two words generated by the word break.
		/// </summary>
		dtsoUfAutoWordBreakOverlapWords = 0x0020,

		/// <summary>
		/// When a document cannot be indexed due to file corruption or
		/// encryption, apply the filtering algorithm to extract text from the file.
		/// </summary>
		dtsoUfFilterFailedDocs = 0x0040,


		/// <summary>
		/// Ignore file format information and apply Unicode Filtering to
		/// all documents.
		/// </summary>
		dtsoUfFilterAllDocs = 0x0080

		} ;

		/// <summary>
		/// Values for IndexJob.Flags
		/// </summary>
    [System::Flags]
    public __value enum IndexingFlags {

		///<summary>
		/// Index every document specified in the IndexJob, even if the document is already in the index
		/// with the same modification date and size
		///</summary>
		dtsAlwaysAdd					= 0x0001,

		///<summary>
		/// Create a case-sensitive index.  Index will treat words with different capitalization as different words. (apple and Apple would be two different words.)
		///</summary>
		dtsIndexCreateCaseSensitive     = 0x0004,

		///<summary>
		/// Create an access-sensitive index.
		///</summary>
		dtsIndexCreateAccentSensitive	= 0x0008,

		///<summary>
		/// Use relative rather than absolute paths in storing document locations.
		///</summary>
		dtsIndexCreateRelativePaths		= 0x0010,

		///<summary>
		/// Resume an earlier index update that did not complete.  (Version 7 indexes only.)
		///</summary>
		dtsIndexResumeUpdate			= 0x0020,

		///<summary>
		/// Compress and store the text of documents in the index, for use in generating Search Reports
		/// and highlighting hits.  (Version 7 indexes only.)
		///</summary>
		dtsIndexCacheText				= 0x0040,

		///<summary>
		/// Compress and store documents in the index, for use in generating Search Reports
		/// and highlighting hits.  (Version 7 indexes only.)
		///</summary>
		dtsIndexCacheOriginalFile		= 0x0080,

		///<summary>
		/// When text caching is enabled, do not cache any fields that were provided through the data source API (in DocFields).
		///</summary>
		dtsIndexCacheTextWithoutFields = 0x00200,
		/// <summary>
		/// When compressing an index, do not remap document ids, so document ids will be unmodified in the
        /// index once compression is done.
        /// </summary>
        dtsIndexKeepExistingDocIds = 0x00400,

		///<summary>
		/// Create an index using the version 7 index format.  Version 7 indexes are created by default in
		/// versions after 7.0, so this flag is no longer needed.
		///</summary>
		dtsIndexCreateVersion7			= 0x10000000
		} ;

	/// <summary>
	/// Values for Options.TextFlags
	/// </summary>
    [System::Flags]
    public __value enum TextFlags {

		/// <summary>
		/// By default, dtSearch indexes numbers both as text and as numeric values, which is necessary for numeric range searching.  Use this flag to suppress indexing of numeric values in applications that do not require numeric range searching.   This setting can reduce the size of the index by about 20%.
		/// </summary>
		dtsoTfSkipNumericValues = 1,

		/// <summary>
		/// Suppress automatic generation of xfirstword and xlastword.
		/// By default, xfirstword is defined to be the first word in each document, and xlastword
		/// is defined to be the last word in each document.  These words are generated when an index
		/// is created, so this flag must be set during indexing to suppress xlastword and xfirstword.
		/// </summary>
		dtsoTfSkipXFirstAndLast         = 2,

		/// <summary>
 		/// Automatically recognize dates in text as it is indexed
 		/// </summary>
 		dtsoTfRecognizeDates = 4,

		/// <summary>
		/// Presume DD/MM/YY format for dates (default is MM/DD/YY)
		/// </summary>
		dtsoTfRecognizeDatesPresumeDMY = 8,
		/// <summary>
		/// Presume YY/MM/DD format for dates (default is MM/DD/YY)
		/// </summary>
		dtsoTfRecognizeDatesPresumeYMD = 16,

		/// <summary>
		/// Automatically insert a word break around characters in the Chinese, Japanese, and Korean Unicode ranges.
		/// This makes it possible to search text in documents that do not contain word breaks.
		/// Like the hyphenation setting, this setting is kept in the alphabet for an index and so will only
		/// change when an index is created.
		/// </summary>
		dtsoTfAutoBreakCJK = 0x0020  	

		} ;



	/// <summary>
	/// Values for IndexFileInfo.TypeId and SearchResultsItem.TypeId
	/// </summary>
	public __value enum TypeId {
    it_None             = 0,
    it_Ami              = 201,
    it_Ansi             = 202,
    it_Ascii            = 203,
    it_Binary           = 204,
    it_CompoundDoc      = 205,
    it_DBF              = 206,
    it_FilteredBinary   = 207,
    it_HyperText        = 208,
    it_MS_Word          = 209,
    it_MS_Works         = 210,
    it_Multimate        = 211,
    it_RTF              = 212,
    it_WS_2000          = 213,
    it_WS_5             = 214,
    it_WinWrite         = 215,
    it_WordForWin       = 216,
    it_WordForWin6      = 217,
    it_WordPerfect42    = 218,
    it_WordPerfect5     = 219,
    it_WordPerfect6     = 220,
    it_WordStar         = 221,
    it_XyWrite          = 222,
    it_ZIP              = 223,
    it_Properties       = 224,
    it_Excel5           = 225,
    it_HTML             = 226,
    it_PDF              = 227,
    it_WordForWin97     = 228,
    it_Excel97          = 229,
    it_PowerPoint       = 230,
    it_EncryptedHtml    = 231,
    it_DatabaseRecord   = 232,
    it_SegmentedText    = 233,
    it_XML              = 234,
    it_WordPerfectEmbedded = 235,
    it_Unicode          = 236,
    it_EudoraMessage    = 237,
    it_Utf8             = 238,
    it_DjVu             = 239,
    it_OutlookExpressMessage = 240,
    it_MimeMessage      = 241,
    it_SingleByteText = 242,
    it_MBoxArchive    = 243,
    it_FilteredBinaryUnicode = 244,
    it_ZIP_zlib = 245,
	it_OutlookMsgFile = 246,
	it_TreepadHjtFile = 247,
	it_PfsProfessionalWrite = 248,
    it_FilteredBinaryUnicodeStream = 249,
    it_DocFile = 250,
    it_Media = 251,
    it_NonTextData = 252,
    it_EML = 253,
    it_MicrosoftWord = 254,
    it_Utf8F = 255,
    it_UnformattedHTML = 256,
    it_DatabaseRecord2 = 257,
    it_Excel2003Xml = 258,
    it_Word2003Xml = 259,
    it_OpenOfficeDocument = 260,
    it_CSV = 261,
	it_MicrosoftAccess = 262,
	it_XBase = 263,
	it_OutlookExpressMessageStore = 264,
	it_IFilter = 265,
	it_Gzip = 266,
	it_Excel4 = 267,
	it_Word12 = 268,
	it_Excel12 = 269,
	it_PowerPoint12 = 270,
	it_TNEF = 271,
	it_XPS = 272,
	it_WMF = 273,       
	it_EMF = 274, 		
	it_TAR = 275,		
    it_Excel2           = 277,  
    it_Excel3			= 278,	
    it_MBoxArchive2    = 279,   
    it_Lotus123		   = 280,
	it_MsWorksWps4	   = 281,   
	it_MsWorksWps6 	   = 282,	
	it_QuattroPro 	   = 283,	
	it_QuattroPro8	   = 284,	 
	it_OutlookMsgAsContainer = 285,
    it_MimeContainer = 286, 	
    it_PdfWithAttachments = 287,
    it_MicrosoftAccess2 = 288, 
    it_FlashSWF = 289,
    it_RAR = 290,
	
    it_LastInternal     = 1002

    };


	/// <summary>
	/// Values for IndexProgressInfo.UpdateType, which indicates the reason for a callback sent
	/// through IIndexStatusHandler
	/// </summary>
	public __value enum MessageCode {

	/// <summary>
	/// Start of an indexing job.
	/// </summary>
	    dtsnIndexBegin = 3001,

	/// <summary>
	/// End of an indexing job
	/// </summary>
    dtsnIndexDone,


	/// <summary>
	/// Index is being created
	/// </summary>
    dtsnIndexCreate,


	/// <summary>
	/// dtSearch is checking files to see if they need to be indexed.
	/// </summary>
    dtsnIndexCheckingFiles,


	/// <summary>
	/// Update to amount of text to be indexed
	/// </summary>
    dtsnIndexToAddUpdate,

	/// <summary>
	/// Update to number of files/bytes added so far
	/// </summary>
    dtsnIndexAdded,


	/// <summary>
	/// Starting to index a file
	/// </summary>
    dtsnIndexStartingFile,

    /// <summary>
    /// Indexing a file
    /// </summary>
    dtsnIndexFileProgress,


    /// <summary>
    /// File indexed successfully
    /// </summary>
    dtsnIndexFileDone,


	/// <summary>
	/// File could not be opened, either due to an access error or
	/// file format problem
	/// </summary>
    dtsnIndexFileOpenFail,


	/// <summary>
	/// File not indexed because it is not in a recognized file format,
	/// and Options.BinaryFiles = dtsoIndexSkipBinary
	/// </summary>
    dtsnIndexFileBinary,

	/// <summary>
	/// Storing words in the index ("merge" in this context means merging into
	/// the main word list)
	/// </summary>
    dtsnIndexMergeProgress,

	/// <summary>
	/// Compressing an index
	/// </summary>
    dtsnIndexCompressProgress,

	/// <summary>
	/// File could not be indexed because of encryption
	/// </summary>
    dtsnIndexFileEncrypted,


	/// <summary>
	/// Words read from one or more files are being stored in the index
	/// </summary>
    dtsnIndexStoringWords,


    /// <summary>
    /// Index update starting
    /// </summary>
    dtsnIndexStartingUpdate,

    /// <summary>
	/// Indicates that a file was indexed but that part of the file could not be accessed due to encryption
    /// </summary>
    dtsnIndexFilePartiallyEncrypted,

    /// <summary>
	/// Indicates that a file was indexed but that part of the file could not be accessed due to data corruption
    /// </summary>
    dtsnIndexFilePartiallyCorrupt,

    /// <summary>
	/// Sent when an automatic index commit is starting (automatic commits are triggered by autoCommitIntervalMB)
    /// </summary>
    dtsnAutoCommitBegin,

    /// <summary>
    /// Sent when an automatic index commit is complete (automatic commits are triggered by autoCommitIntervalMB)
    /// </summary>
    dtsnAutoCommitDone,

    dtsnLastIndexStatusMessage = 3999,

    /// <summary>
	/// Sent during a merge by dtsIndexMergeJob.
    /// </summary>
    dtsnIndexMergeJobProgress,

    /// <summary>
	/// Sent during a verify by dtsIndexVerifyJob
    /// </summary>
	dtsnIndexVerifyProgress


	};

}

}
