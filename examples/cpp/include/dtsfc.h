/*

dtSearch C++ API

Copyright 1995-2016 dtSearch Corp.  All Rights Reserved

*/

//////////////////////////////////////////////////////////////////////

#if !defined(DTSFC_H)
#   define DTSFC_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef DTSEARCH_H
#   include <dtsearch.h>
#endif
#ifndef DTSVIEWR_H
#   include <dtsviewr.h>
#endif

#ifndef DFILE_H
#   include <dfile.h>
#endif
#ifndef DSTRING_H
#   include <dstring.h>
#endif
#ifndef DTS_WLB_H
#   include <dts_wlb.h>
#endif
#ifndef DStringProperty_H
#   include <dstringproperty.h>
#endif
#ifndef DInputStream_H
#	include <dinputstream.h>
#endif
#ifndef DTHREAD_H
#	include <dthread.h>
#endif

#include <string.h>
#include <time.h>

// Make sure dtsTCHAR is mapped either to char or wchar_t
// Use dtsTCHAR instead of TCHAR because this header might be
// included before TCHAR is defined in some other way in a
// later header
#ifdef _MSC_VER
    #ifndef _INC_TCHAR
        #include <tchar.h>
    #endif
    #define dtsTCHAR TCHAR
#else
    // Use this instead of TCHAR
    #ifdef _UNICODE
        #define dtsTCHAR wchar_t
    #else
        #define dtsTCHAR char
    #endif
#endif

#ifdef _AFXDLL
    #ifndef MfcAvailable
        #define MfcAvailable 
    #endif
#endif

#ifdef USE_DTSEARCH_NAMESPACE

// Namespace for C++ support classes.
// The namespace is conditionally included in header files if USE_DTSEARCH_NAMESPACE is defined
namespace dtSearch {

#endif

#ifndef SUCCESS
    #define SUCCESS 0
#endif
#ifndef FAIL
    #define FAIL (-1)
#endif

#define ToFlag(v) ((short) (v? 1 : 0))

/* Summary
   Class wrapper around the dtsOptions interface structure; used to change option
   settings
   Remarks
   For convenience, DStringProperty members are provided for each of the string
   values in dtsOptions.                                                          */
class DOptions : public dtsOptions {
    public:
		DOptions(bool bInitialize = true);
		void copy(const dtsOptions& other);
		DOptions& operator=(const DOptions& other) {
			copy(other);
			return *this;
			}
		DOptions& operator=(const dtsOptions& other) {
			copy(other);
			return *this;
			}

        void Save();
        int GetHyphenOption();
        void SetHyphenOption(int v);
        void SetBinaryFilesOption(long v) {
            binaryFiles = v;
            }
        int GetBinaryFilesOption() {
            return (int) binaryFiles;
            }
        const char *GetAlphabetFile() {
            return AlphabetFile;
            }
        const char *GetStemmingRulesFile() {
            return StemmingRulesFile;
            }
        const char *GetNoiseWordFile() {
            return NoiseWordFile;
            }
        void SetAlphabetFile(const char *s) {
            AlphabetFile.set(s);
            }
        void SetStemmingRulesFile(const char *s) {
            StemmingRulesFile.set(s);
            }
        void SetNoiseWordFile(const char *s) {
            NoiseWordFile.set(s);
            }
        /* Name of dtSearch alphabet file to use when parsing text into words. The
			alphabet file determines which characters are letters and what the rules are
			for handling capitalization and punctuation. If blank, dtSearch will use 
			a default alphabet file.

			The alphabet file only controls the indexing of characters in the range
			32-127. Unicode character properties control the treatment of other
			characters. */
        DStringProperty AlphabetFile;
          /* Stemming rules for stemming searches (default: "stemming.dat")

			The stemming.dat file uses a plain text format and includes comments in the file that describe the file format.
			*/
        DStringProperty StemmingRulesFile;
			/* List of noise words to skip during indexing (default: "noise.dat") A
			noise word is a word such as the or if that is so common that it is not
			useful in searches. To save time, noise words are not indexed and are
			ignored in index searches.

			When an index is created, dtSearch copies the list of words from noise.dat
			into the index directory and also builds the word list into other index
			files. After an index is created, subsequent changes to the noise word list
			will not affect indexing for that index */
        DStringProperty NoiseWordFile;
            /* Comma-separated list of tags to ignore when indexing XML
				If your XML data includes HTML tags like P, B, I, etc., use XmlIgnoreTags to prevent these tags from affecting the field structure of XML. Example:
				options.XmlIgnoreTags = "b,p,i,u,h1,h2,h3,br,hr"
			*/
        DStringProperty XmlIgnoreTags;
           // Define characters considered to be text if binaryFiles is set ot dtsoFilterBinary
        DStringProperty BinaryFilterTextChars;
           /* File segmentation rules, used to split up long text files into logical
			subdocuments during indexing.

		    The SegmentationRulesFile is an XML file. To create the file, start dtSearch
			Desktop, click Options > Preferences > File Segmentation Rules and use the
			dialog box to set up the rules. The XML file will be saved as fileseg.xml in
			your dtSearch UserData folder. See File Segmentation Rules in the dtSearch
			Desktop help for information on this setting.
			*/
        DStringProperty SegmentationRulesFile;

			/* Name of the file containing rules for extraction of field data from text
			files based on markers in the next

			The TextFieldsFile is an XML file. To create the file, start dtSearch
			Desktop, click Options > Preferences > Text Fields and use the dialog box to
			set up the text field definitions. The XML file will be saved as fields.xml
			in your dtSearch UserData folder. See Define Text Fields in the dtSearch
			Desktop help for information on this setting. */
        DStringProperty TextFieldsFile;
            /* User-defined synonym sets. The UserThesaurusFile is an XML
		       \file. To create the file, start dtSearch Desktop, click <b>Options
		       \> Preferences \> User Thesaurus</b> and use the dialog box
		       to set up the synonym definitions. The XML file will be saved
		       as thesaur.xml in your dtSearch UserData folder. See User
		       Thesaurus in the dtSearch Desktop help for information on
		       this setting.                                                       */
        DStringProperty UserThesaurusFile;
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

        DStringProperty HomeDir;
        /* Summary
           A directory that the dtSearch Engine can use to store temporary files. Most
           applications should not set PrivateDir.
           Remarks
           PrivateDir, if non-blank, must be unique for each concurrent user of the dtSearch
           Engine.

           For more information, see <link Home and Private Directories, The HomeDir\, PrivateDir\, and TempFileDir option settings>. */
        DStringProperty PrivateDir;
            /* Name of the file containing a table of filename patterns for file formats
			that dtSearch cannot detect automatically, such as older versions of
			WordStar.

			The FileTypeTableFile is an XML file. To create the file, start dtSearch
			Desktop, click Options > Preferences > File Types and use the dialog box to
			set up the file type definitions. The XML file will be saved as filetype.xml
			in your dtSearch UserData folder. See File Types in the dtSearch Desktop
			help for information on this setting.	*/
        DStringProperty FileTypeTableFile;
			/* Use to replace the default connectors used in search requests.  See dtsOptions.booleanConnectors */
        DStringProperty BooleanConnectors;
        DStringProperty MacroFile;
	/* Summary
	   Directory to use for temporary files.
	   Remarks
	   Use tempFileDir to control the location of most temporary files that the dtSearch
	   Engine creates, such as temporary files used for large file conversion operations
	   \or extraction of large documents stored in containers.

	   IndexJob.TempFileDir will still control the location of temporary lists and sort
	   buffers created during indexing. See                                              */

		DStringProperty TempFileDir;

    };

/* Summary
   Class wrapper around the dtsOptions2 interface structure; used to change option
   settings
   Remarks
   For convenience, DStringProperty members are provided for each of the string
   values in dtsOptions2.                                                          */
class DOptions2 : public dtsOptions2 {
    public:
		DOptions2();
		bool Get();
       void Save();
 		void copy(const dtsOptions& other);
		void copy(const dtsOptions2& other);
		void copyTo(const dtsOptions& other);

		DOptions2& operator=(const DOptions2& other) {
			copy(other);
			return *this;
			}
		DOptions2& operator=(const dtsOptions2& other) {
			copy(other);
			return *this;
			}

        int GetHyphenOption();
        void SetHyphenOption(int v);
        void SetBinaryFilesOption(int v) {
            binaryFiles = v;
            }
        int GetBinaryFilesOption() {
            return binaryFiles;
            }
        const char *GetAlphabetFile() {
            return AlphabetFile;
            }
        const char *GetStemmingRulesFile() {
            return StemmingRulesFile;
            }
        const char *GetNoiseWordFile() {
            return NoiseWordFile;
            }
        void SetAlphabetFile(const char *s) {
            AlphabetFile.set(s);
            }
        void SetStemmingRulesFile(const char *s) {
            StemmingRulesFile.set(s);
            }
        void SetNoiseWordFile(const char *s) {
            NoiseWordFile.set(s);
            }
        /* Name of dtSearch alphabet file to use when parsing text into words. The
			alphabet file determines which characters are letters and what the rules are
			for handling capitalization and punctuation. If blank, dtSearch will use 
			a default alphabet file.

			The alphabet file only controls the indexing of characters in the range
			32-127. Unicode character properties control the treatment of other
			characters. */
        DStringProperty AlphabetFile;
          /* Stemming rules for stemming searches (default: "stemming.dat")

			The stemming.dat file uses a plain text format and includes comments in the file that describe the file format.
			*/
        DStringProperty StemmingRulesFile;
			/* List of noise words to skip during indexing (default: "noise.dat") A
			noise word is a word such as the or if that is so common that it is not
			useful in searches. To save time, noise words are not indexed and are
			ignored in index searches.

			When an index is created, dtSearch copies the list of words from noise.dat
			into the index directory and also builds the word list into other index
			files. After an index is created, subsequent changes to the noise word list
			will not affect indexing for that index */
        DStringProperty NoiseWordFile;
            /* Comma-separated list of tags to ignore when indexing XML
				If your XML data includes HTML tags like P, B, I, etc., use XmlIgnoreTags to prevent these tags from affecting the field structure of XML. Example:
				options.XmlIgnoreTags = "b,p,i,u,h1,h2,h3,br,hr"
			*/
        DStringProperty XmlIgnoreTags;
           // Define characters considered to be text if binaryFiles is set ot dtsoFilterBinary
        DStringProperty BinaryFilterTextChars;
           /* File segmentation rules, used to split up long text files into logical
			subdocuments during indexing.

		    The SegmentationRulesFile is an XML file. To create the file, start dtSearch
			Desktop, click Options > Preferences > File Segmentation Rules and use the
			dialog box to set up the rules. The XML file will be saved as fileseg.xml in
			your dtSearch UserData folder. See File Segmentation Rules in the dtSearch
			Desktop help for information on this setting.
			*/
        DStringProperty SegmentationRulesFile;

			/* Name of the file containing rules for extraction of field data from text
			files based on markers in the next

			The TextFieldsFile is an XML file. To create the file, start dtSearch
			Desktop, click Options > Preferences > Text Fields and use the dialog box to
			set up the text field definitions. The XML file will be saved as fields.xml
			in your dtSearch UserData folder. See Define Text Fields in the dtSearch
			Desktop help for information on this setting. */
        DStringProperty TextFieldsFile;
            /* User-defined synonym sets. The UserThesaurusFile is an XML
		       \file. To create the file, start dtSearch Desktop, click <b>Options
		       \> Preferences \> User Thesaurus</b> and use the dialog box
		       to set up the synonym definitions. The XML file will be saved
		       as thesaur.xml in your dtSearch UserData folder. See User
		       Thesaurus in the dtSearch Desktop help for information on
		       this setting.                                                       */
        DStringProperty UserThesaurusFile;
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

        DStringProperty HomeDir;
        /* Summary
           A directory that the dtSearch Engine can use to store temporary files. Most
           applications should not set PrivateDir.
           Remarks
           PrivateDir, if non-blank, must be unique for each concurrent user of the dtSearch
           Engine.

           For more information, see <link Home and Private Directories, The HomeDir\, PrivateDir\, and TempFileDir option settings>. */
        DStringProperty PrivateDir;
            /* Name of the file containing a table of filename patterns for file formats
			that dtSearch cannot detect automatically, such as older versions of
			WordStar.

			The FileTypeTableFile is an XML file. To create the file, start dtSearch
			Desktop, click Options > Preferences > File Types and use the dialog box to
			set up the file type definitions. The XML file will be saved as filetype.xml
			in your dtSearch UserData folder. See File Types in the dtSearch Desktop
			help for information on this setting.	*/
        DStringProperty FileTypeTableFile;
			/* Use to replace the default connectors used in search requests.  See dtsOptions.booleanConnectors */
        DStringProperty BooleanConnectors;
        DStringProperty MacroFile;
	/* Summary
	   Directory to use for temporary files.
	   Remarks
	   Use tempFileDir to control the location of most temporary files that the dtSearch
	   Engine creates, such as temporary files used for large file conversion operations
	   \or extraction of large documents stored in containers.

	   IndexJob.TempFileDir will still control the location of temporary lists and sort
	   buffers created during indexing. See                                              */

		DStringProperty TempFileDir;

    };
/* Summary
   Base class for DFileConverter, DIndexJob, DIndexMergeJob, DIndexListJob,
   DSearchJob, and DSearchReportJob.
   Remarks
   DJobBase provides common task-management functions for starting, monitoring, and
   cancelling a job, and for obtaining error information after a job completes.

   The Job classes include several callback methods implemented as virtual
   functions, such as OnCheckAbort and OnError (in all classes), OnProgressUpdate
   (DIndexJob), and OnFound (DSearchJob). To use these methods, create a class based
   \on the underlying Job class and override the virtual function you want to
   implement. 

   <code>
   class CMySearchJob : public DSearchJob {
        public:
              ...
              virtual void OnError(long errorCode, const char *msg) {
                    // First call base class implementation
                    DSearchJob::OnError(errorCode, msg);
                    // Now show the message to the user
                    CString csMsg = GetMyErrorForErrorCode(errorCode);
                    ShowMessageToUser(csMsg);
                    }
              virtual void OnFound(long totalFiles,
                    \long totalHits, const char *name, long hitsInFile, dtsSearchResultsItem& item) {
                    // First call base class implementation
                    DSearchJob::OnFound(totalFiles, totalHits, name, hitsInFile, item);
                    // Now show the file to the user
                    ShowFoundFileToUser(name);
                    }
        };
   </code>                                                                                            */
class DJobBase {
    public:
        DJobBase(dtsJobBase& aBaseJob);
        virtual ~DJobBase();
        /* Requests termination of the job in progress. For indexing jobs, the
		job will be halted after information in documents already indexed is
		saved in the index. */
        virtual void Cancel();
        /* Requests immediate termination of the job in progress. Indexing jobs
		will be halted without saving anything in the index. */
        virtual void CancelImmediate();
        void beginExecution();
        void endExecution(int aFailed);
        // Returns true if the job has started and is not yet done.
        int IsRunning();
        // Returns true if the job has been cancelled
        int IsCancelled() {
            return fCancelRequested || fCancelImmediateRequested;
            }
       	/*
       		Summary:
			Called to execute a job.

			Remarks:

			A derived class should call the base
			class's Execute method to carry out the job. Example:
			<code>
     		void CMyIndexJob::Execute()
		    {      ... do things to get ready for indexing ...
		          DIndexJob::Execute
		          ... do things after indexing is done ...
     		}
     		</code>
     	*/
        virtual void Execute() = 0;
        /*  Called while a job is executing every time the dtSearch Engine
		checks to see if the job has been cancelled.  */
        virtual void OnCheckAbort() { };
		/*  Called while a job is executing if an error is encountered, with the
		  error code and message.

		  Parameters:
		  	errorCode:   A code in the ErrorCodes enumeration identifying the error
		  	message:	 Text version of the error message
		  	arg1:		 For messages that relate to a file or path, such as "Unable to access index", the file or path that the error concerns.
		  	arg2:        Not used.

		  Remarks:

		  Unless your
		  program needs to react to errors immediately, it is usually easier to
		  call GetErrors() after the job returns to see if anything went wrong.
		  */
        virtual void OnError(long /* errorCode */, const char * /* message */, const char * /* arg1 */, const char * /* arg2 */) { };
        // Not used
        virtual int OnQuestion(long /* errorCode */, const char * /* message */ , const char *, const char *) { return 0;};
        const dtsErrorInfo *getErrors() { // deprecated
                return &errorHandler;
            }
		// Returns the number of errors recorded during execution
        int GetErrorCount() {
			return (int) errorHandler.getCount();
			}
		int GetErrorCode(int iError = 0) {
			return (int) errorHandler.getMessageCode(iError);
			}
		// Returns a dtsErrorInfo with any errors that occurred during execution
        const dtsErrorInfo *GetErrors() {
                return &errorHandler;
                }
		/*  SetTimeout() causes a job to cancel automatically after the specified number of seconds
			Parameters:
				nSeconds:  Number of seconds to allow the job to run
		*/
        void SetTimeout(long nSeconds) {
            baseJob.timeoutSeconds = nSeconds;
            }
        // Returns non-zero if the job failed
        int Failed() {
            return fFailed;
            }
        // Format all errors that occurred during execution as a single string, with line breaks
        // separating the errors
        void FormatErrors(DString& dest);

        bool HasMessage(int code) {
			return errorHandler.haveMessage(code);
		}

		bool WasInputEncrypted() {
			return HasMessage(dtsErFileEncrypted) || HasMessage(dtsErContainerItemEncrypted);
		}
		bool WasInputCorrupt() {
			return HasMessage(dtsErFileCorrupt) || HasMessage(dtsErContainerItemCorrupt);
		}

		// Write an error message to the error handler object attached to this job
		void reportError(int msgCode, const char *msg, const char *arg1 = 0, const char *arg2 = 0) {
			errorHandler.handleMessage(msgCode, msg, arg1, arg2);
			}

		static void FormatErrors(DString& dest, dtsErrorInfo *errors, int maxLen=65536);

        // Do not pump waiting windows messages during callbacks
        void SuppressMessagePump() {
            fShouldPumpMessages = false;
            }

        void SetParentJob(DJobBase& parent);
        static int PumpMessages();
    protected:
        virtual void handleMessage(dtsMessage& ) { };
        void initBase();
        long jobId;
        dtsErrorInfo errorHandler;
        int fFailed;
        int fRunning;
        int fDone;
        volatile int fCancelRequested;
        volatile int fCancelImmediateRequested;
        int fShouldPumpMessages;
        dtsJobBase& baseJob;

        static DJobBase *safeCast(void *pData);
        static int reportCB(void *pData, dtsMessage& msg);
        static int errorCB(dtsErrorInfo *pErrorInfo);
	private:
        int onReportBase(dtsMessage& msg);
        int onErrorBase(dtsErrorInfo& errorInfo);
        DJobBase *pParentJob;
    };

/* Extends the dtsSearchResults class to add simpler access to string properties of
   retrieved documents.                                                             */
class DSearchResults : public dtsSearchResults {
public:
    DSearchResults(long aMaxCount = 256, long aFlags = 0);
    DSearchResults(const DSearchResults& other);
    /*  Fills the document property members with information about one document
	in the search results.
	    Parameters:
	    	iDoc: 	zero-based index of the document in the	search results list.
	    Returns:
	    	*/
    inline int GetNthDoc(long i) {
        return getNthDoc(i);
        }
    /* Retrieves a single field from the field-value pairs
       containing document properties. For a list of the properties
       included, see <link dtsSearchResults::getDocInfoStrings@long@char *@int, dtsSearchResults::getDocInfoStrings Method>
       Parameters
       fieldName :  Name of the field to retrieve
       dest :       Buffer to receive the value of the field
       Returns
       0 on success, or non-zero if the field could not be found.                                                           */
    int GetDocInfoItem(const char *fieldName, DStringProperty& dest) {
    	initDocInfoStrings();
        DString tmp;
        if (getDocInfoItem(fieldName, tmp) == FAIL)
            return FAIL;
        dest.setU8(tmp);
        return SUCCESS;
        }
    dtsSearchResultsItem DocInfo;

    // Name of the document selected by GetNthDoc
    DStringProperty DocName;

    // Title of the document selected by GetNthDoc
    DStringProperty DocTitle;

    // User-friendly name for document selected by GetNthDoc
    DStringProperty DocDisplayName;

    /* Field-value pairs containing document properties. For a list
       \of the properties included, see <link dtsSearchResults::getDocInfoStrings@long@char *@int, dtsSearchResults::getDocInfoStrings Method> */
    DStringSetProperty& getDocInfoItems() {
    	initDocInfoStrings();
    	return DocInfoItems;
    	}
	DStringProperty& getDocInfoString() {
		initDocInfoStrings();
		return DocInfoString;
		}

    // Search request that generated this set of search results
    DStringProperty Request;

    // Number of hits in the document selected by GetNthDoc
    long DocHitCount;

    /* \Returns a pointer to an array of word offset of hits in the
       document selected by GetNthDoc                               */
    long *GetDocHits() {
        return getDocHits();
        }

    /* Summary
       Generates hit highlighting data for Adobe Reader
       Remarks
       Adobe Reader has an interface for highlighting hits in a
       browser window, which is documented in the Adobe Acrobat SDK.
       The interface requires the server to provide an XML file
       containing information about the hits in a document.
       MakePdfWebHighlightFile generates the XML necessary to
       highlight hits in the currently-selected item in search
       results.See <b><extlink dtengine.chm::/Highlighting_hits_in_PDF_files.htm>Highlighting
       hits in PDF files</extlink></b> for more information on
       working with PDF files.

       Parameters
       dest :  String to receive output XML                                                   */
    void MakePdfWebHighlightFile(DString& dest);

    long *getDocHits() {
        if (iDoc > FAIL)
            return DocInfo.hits;
        else
            return 0;
        }
    int getNthDoc(long i);
    // Returns the zero-based index of the currently-selected item in search results
    long getSelectedDoc() {
        return iDoc;
        }
	// Sorts the search results set according to the SortFlags values in flags.
	// parameters:
	// flags:       Specifies the type of sort.  See SortFlags for values.
	// userFields:  If dtsSortByField is set in flags, the field in dtsSearchResultsItem.userFields to use as the sort key
     virtual void sort(long aflags, const char *userField) {
		iDoc = -1;
        dtsSearchResults::sort(aflags, userField);
        }

    int getDocInfoItem(const char *fieldName, DString& dest);
    // Copy search results
    DSearchResults& operator=(const DSearchResults& other);
    void updateFromSearch() {
        init();
        }
    int IsNaturalLanguageSearch() {
        return getSearchFlags() & dtsSearchAutoTermWeight;
        }
    /* Read search results from an XML file
       Parameters
       s :  Buffer containing the XML to read. */
    void serializeFromXml(const char *s) {
        dtsSearchResults::serializeFromXml(s);
        updateFromSearch();
        }

    // Get the selection state of the items in search results 
    // Parameters
    // dest : Array of integers to receive the selected items
	void getSelections(DIntArray& dest);        

    static const char *getDisplayName(dtsSearchResultsItem& item, int fPdfTitleAsName, int fHtmlTitleAsName);
	clock_t getCreatedTicks() {
		return whenCreated;
		}
	/* URL-encode an item in search results.  An item is url-encoded by storing the index path, document id, and hit offsets.
       Some browsers and web servers are unable to handle very long URLs, so the string generated should be limited, using the
       maxLen parameter, to 1024 characters.
	   Parameters:
	   item :  The search results item to encode
	   dest :  String buffer to receive encoded item
	   maxLen :  Maximum length of the string to generate */

	static void UrlEncodeItem(dtsSearchResultsItem& item, DString& dest, ptrdiff_t maxLen);
	/* URL-decode an item in search results.  An item is url-encoded by storing the index path, document id, and hit offsets.
		Decoding extracts these values from the url-encoded string.
	*/

	static void UrlDecodeItem(const char *url, DString& indexPath, long& docId,
		DArrayAsVector<long>& hits);

protected:
    void init();

    long iDoc;
    clock_t whenCreated;
    DString temp;

	// Used to implement lazy initialization of the DocInfoString and DocInfoItems properties,
	// which are collections of named properties that are time-consuming to construct
	// and often not used.
    void initDocInfoStrings();
    void initUserFields();
    bool bHaveDocInfoStrings;
    bool bHaveUserFields;
    DStringSet userFields;
    DStringSetProperty DocInfoItems;
    DStringProperty DocInfoString;

    };

/* Class wrapper around dtsSearchJob; provides access to a simplified version of the
   searching interface.                                                              */
class DSearchJob  : public DJobBase
{
public:
    /* What you want to search for. See <link Search Requests Overview, Search Requests>
       for information on search request syntax.                                         */
    DStringProperty Request;
		/// Summary
		/// In an unindexed search, the directories or files to search.
		///
		/// Remarks
		///
		/// To specify a directory and its subdirectories,
		/// add &lt;+&gt; (less than, plus, greater than) following the directory name.
		///
		/// To specify one or more files, add the filenames to the FoldersToSearch list.
		/// This is more efficient than specifying the folder name in FoldersToSearch
		/// and the filename in IncludeFilters,
		/// because it eliminates the need for a directory scan to locate the file.
    DStringSetProperty IncludeFilters;
    /* Filename filters specifying folders to be excluded from an
       unindexed search.                                          */
    DStringSetProperty ExcludeFilters;
    /* Summary
       The FoldersToSearch, IncludeFilters, and ExcludeFilters
       specify files to be searched in an unindexed search.
       Remarks
       To search a folder and all subfolders under it, add \<+\>
       after the folder name in the FoldersToSearch (as part of the
       same string, like this: "c:\\docs\<+\>"). A file will be
       searched if (1) it is in one of the Folders To Search, (2) it
       matches one of the IncludeFilters, and (3) it does not match
       any of the Exclude Filters. A filename filter that does not
       contain a backslash is compared to the name of each file. A
       filename filter that contains a backslash is compared to the
       fully-qualified pathname of each file. Examples:
       <code>
           \*.DOC *\\DOCS\\*.WPD *\\My Documents\\*
       </code>

                                                                     */
    DStringSetProperty FoldersToSearch;
    /* One or more indexes to be searched. For each index, provide
       the full path to the index folder.                          */
    DStringSetProperty IndexesToSearch;
    /* <link File Conditions, File conditions> that a document must meet to be retrieved
       in a search. See <link Search Requests Overview, Search Requests> for information
       \on search request syntax.                                                        */
    DStringProperty FileConditions;
    /* Additional <link Boolean Search Requests, boolean> conditions that a document
       must meet to be retrieved in a search. See <link Search Requests Overview, Search Requests>
       for information on search request syntax.                                                   */
    DStringProperty BooleanConditions;
    /* If set to a filename, each file retrieved in the search will
       be written to this file. If the dtsSearchDelayDocInfo flag is
       set, the list will be built from the best-matching files
       after the search is done, sorted by relevance, and will
       contain at most MaxFiles items. Otherwise, the list will
       include all files retrieved in the search, in the order they
       were found.                                                   */
    DStringProperty ListingName;
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
    DStringProperty FieldWeights;
    /* Constructor */
    DSearchJob();
    virtual ~DSearchJob();
    /* Resets the job to default values. */
    void Clear();
    /* Execute the search. */
    virtual void Execute();

    /* Add an index to the IndexesToSearch list.
       Parameters
       indexPath :  Full path of the index folder. */
    void AddIndexToSearch(const dtsTCHAR * indexPath) {
        IndexesToSearch.append(indexPath);
        searchJob.action.searchIndexes = true;
        }
    /* Summary
       Add a folder to the FoldersToSearch list. Append \<+\> to the
       folder name to specify that subfolders should also be
       searched.

       Parameters
       searchFolder :  Path of the folder to search.                 */
    void AddFolderToSearch(const dtsTCHAR *searchFolder) {
        FoldersToSearch.append(searchFolder);
        searchJob.action.searchFiles = true;
        }
    void SetFilters(const dtsTCHAR * incl, const dtsTCHAR * excl = 0) {
        IncludeFilters.tokenize(incl);
        ExcludeFilters.tokenize(excl);
        }
    void SetRequest(const dtsTCHAR * request) {
        Request.set(request);
        }
    /* \ \
       Summary
       Get the value of a single search flag
       Parameters
       flag :  A SearchFlags value specifying the flag to get

       Returns
       0 if the flag is not set, or non-zero if the flag is set. */
    long GetSearchFlag(long flag) { return searchJob.searchFlags2 & flag; }
    /* Set UnindexedSearchFlags for this search */
    void SetUnindexedSearchFlags(short v) {
		searchJob.unindexedSearchFlags = v;
		}
    /* Get all SearchFlags that were set for this search job. */
    long GetSearchFlags() { return searchJob.searchFlags2; }
    /* \ \
       Summary
       Set one of the SearchFlags values for this search.

       Parameters
       flag :   The SearchFlags value indicating the flag to set.
       value :  Non\-zero or true to set the flag; zero or false to clear the flag. */
    void SetSearchFlag(long flag, int value);
    /* Clear all search flags in the SearchJob */
    void ClearSearchFlags();
        /* \ \
       Summary
       Set all SearchFlags values for this search.

       Parameters
       flags :   The SearchFlags to set.
		*/
    void SetSearchFlags(long flags);

    void SetFuzzySearching(int fuzziness = 2);
    /* \Returns the level of fuzziness in this search */
    long GetFuzziness() {
        return searchJob.fuzziness;
        }
    void SetPhonicSearching(int v = true);
    void SetStemming(int v = true);
    void SetNaturalLanguage(int v = true);
    void SetSynonymSearching(int v = true, int fWordNet = true, int fUserSynonyms = true, int fWordNetRelated = false);
    /* \ \
       Summary
       After a search, DSearchJob will return a DSearchResults object that contains up
       to this many files. If more files were retrieved than the MaxFilesToRetrieve
       value, only the best-matching files will be returned.
       Parameters
       aMaxFiles :  The maximum number of files to allow this search to retrieve.
       Remarks
       When MaxFilesToRetrieve is non-zero, it controls the maximum size of the
       DSearchResults that can be returned after a search. The most relevant documents
       from all matching documents in the index will be included in search results.

       Regardless of the MaxFilesToRetrieve setting, GetTotalFiles will return the a
       count that includes <i>all</i> matching documents. MaxFilesToRetrieve limits the
       number of items saved in the returned DSearchResults, but it does not stop the
       search before all matching documents have been identified.

       For example, if you set MaxFilesToRetrieve=10 and AutoStopLimit=5000, then search
       results will contain the 10 most relevant documents from the first 5000 found. Documents
       after the first 5000 found will not be considered, because AutoStopLimit=5000
       will force the search to halt after 5000 matching documents are found.

       If the dtsSearchSelectMostRecent flag is set in SearchFlags, the most <i>recent</i>
       documents will be returned instead of the most relevant.                                 */
    void SetMaxFiles(long aMaxFiles);
    /* Summary
       Request that a search automatically halt after more than a
       specified number of files have been retrieved
       Parameters
       aLimit :  The limit on the number of files to retrieve.
       Remarks
       The AutoStopLimit value forces a search to halt after a
       certain number of files have been found. The
       MaxFilesToRetrieve setting, in contrast, lets the search
       complete, and limits the number of items collected in search
       results to the best-matching files.                          */
    void SetAutoStopLimit(long aLimit) {
		if (aLimit < 0)
			aLimit = 0;
        searchJob.autoStopLimit = aLimit;
        }

    long GetAutoStopLimit() {
        return searchJob.autoStopLimit;
        }

    void SetStopLimit(long v) {
        searchJob.autoStopLimit = v;
        }

	// Set up search to use an index cache
	void SetIndexCache(dtsIndexCacheHandle hCache) {
		searchJob.indexCacheHandle = hCache;
		}

    /* \Returns the maximum files this search can retrieve. */
    long GetMaxFiles() ;
    void SetDelayDocInfo(int v = true) {
            fDelayDocInfo = v;
            }
    /* \Returns the total number of files that were retrieved in
       this search. If maxFilesToRetrieve was set, the value
       returned by GetTotalFiles may exceed the number of items in
       search results.                                             */
    long GetTotalFiles() {
            return searchJob.totalFiles;
            }
    /* \Returns the total number of hits in all files retrieved in
       this search.                                                */
    long GetTotalHits() {
            return searchJob.totalHits;
            }
    /* \ \
       Summary
       Return the search results generated by this search.
       Remarks
       After a search, use GetResults to access search results. The
       DSearchJob will continue to own the search results object, so
       you may not delete the pointer returned from GetResults
       because the DSearchJob destructor will delete the search
       results. To obtain ownership of the search results object,
       use DetachResults to get the search results object and tell
       the DSearchJob to relinquish ownership.                       */
    DSearchResults *GetResults() {
        return getResults();
        }
    DSearchResults *getResults() {
        return results.peek();
        }
    /* \ \
       Summary
       Detach and return the search results generated by this
       search.

       Remarks
       After a search, use GetResults to access search results. The
       DSearchJob will continue to own the search results object, so
       you may not delete the pointer returned from GetResults
       because the DSearchJob destructor will delete the search
       results. To obtain ownership of the search results object,
       use DetachResults to get the search results object and tell
       the DSearchJob to relinquish ownership.                       */
    DSearchResults *DetachResults() {
        return detachResults();
        }
    DSearchResults *detachResults() {
        return results.detach();
        }

    void SetListingFlags(long f) {
        searchJob.output.listingFlags = f;
        }
    /* \ \
       Summary
       Attach a data source to the search job to be searched as part
       \of an unindexed search.

       Parameters
       ds :             Data source to attach (see dtsDataSource and
                        DDataSourceBase)
       fShouldDelete :  If true, the data source will be deleted by the
                        DSearchJob destructor.                          */
    void AttachDataSource(DDataSourceBase *ds, int fShouldDelete = false) {
        if (dataSourceToDelete) {
            delete dataSourceToDelete;
            dataSourceToDelete = 0;
            }

        if (ds) {
            searchJob.dataSourceToSearch = ds->getInterface();
            if (fShouldDelete)
                dataSourceToDelete = ds;
            }
        else
            searchJob.dataSourceToSearch = 0;
        }
#if !defined(DocumentationGeneratorOnly)
    virtual void OnFound(long /* totalFiles */, long /* totalHits */, const char * /*name */,
        long /* hitsInFile */, dtsSearchResultsItem& /* item */) {  };
    virtual void OnSearchingIndex(const char * /*indexPath */) { };
    virtual void OnSearchingFile(const char * /* file */, long /* filesSearched */, long /* filesToSearch */ ) { };
    virtual void OnProgressUpdate(dtsSearchProgressInfo&  /*prog*/ ) { }
#else
/* Summary
   Each time a document is retrieved, this function will be called.
   Parameters
   totalFiles :  Total number of files found so far
   totalHits :   Total number of hits found so far
   name :        Name of the file retrieved
   hitsInFile :  Number of hits in this file
   item :        Document properties (these will be mostly empty of the
                 dtsSearchDelayDocInfo SearchFlags flag was set.
   Example
   <code>

    CMySearchJob : public DSearchJob {
    public:
    ...
    virtual void OnError(long errorCode, const char *msg) {
        // First call base class implementation
        DSearchJob::OnError(errorCode, msg);
        // Now show the message to the user
        CString csMsg = GetMyErrorForErrorCode(errorCode);
        ShowMessageToUser(csMsg);
        }
    virtual void OnFound(long totalFiles, long totalHits,
        const char *name, long hitsInFile, dtsSearchResultsItem& item) {
        // First call base class implementation
        DSearchJob::OnFound(totalFiles, totalHits, name, hitsInFile, item);
        // Now show the file to the user
        ShowFoundFileToUser(name);
       }
       };
   </code>
   Remarks
   An application can call DSearchJob::VetoThisItem from the OnFound callback to
   prevent the item from being included in search results.
   An application can call DSearchJob::SetItemScore from the OnFound callback to 
   change the item score in search results.                       */
    virtual void OnFound(long totalFiles , long totalHits , const char * name ,
        long hitsInFile , dtsSearchNotificationItem&  item ) {  };

	/* Summary
	   This function will be called each time an index is searched. If several indexes
	   are listed in IndexesToSearch, this function will be called once for each index.
	   Parameters
	   indexPath :  Path of the index being searched.                                   */
    virtual void OnSearchingIndex(const char * indexPath ) { };
	/*   Summary
	   Called when an unindexed search begins searching a file.

	   Parameters
	   file\ :          Name of the file being searched
	   filesSearched :  Number of files searched so far
	   filesToSearch :  Files still to be searched              */

    virtual void OnSearchingFile(const char * file , long filesSearched , long filesToSearch  ) { };

	/*
	   Summary
	   Called to update the status of a SearchJob

	   Parameters
	   prog :           dtsSearchProgressInfo structure with information about the current status of the search
	*/
    virtual void OnProgressUpdate(dtsSearchProgressInfo&  prog) { }

#endif
    /* Access the dtsSearchJob struct that DSearchJob wraps. */
    dtsSearchJob& GetSearchJob() {
        return searchJob;
        }
    /* Apply a search filter to the search that can be used to
       exclude or permit specified documents to be returned in
       search results. See: dtsSearchFilter.                   */
    void SetSearchFilter(dtsSearchFilterHandle hFilter) {
        searchJob.searchFilterHandle = hFilter;
        }
    void SetSearchFilter(dtsSearchFilter *pFilter) {
        searchJob.searchFilterHandle = pFilter->getHandle();
        }
    void SetSearchFilter(dtsSearchFilter& pFilter) {
        searchJob.searchFilterHandle = pFilter.getHandle();
        }
    /* Summary
       Request a search filter containing the results of this search
       (see dtsSearchFilter)
       Parameters
       v :  If true, a search filter will be created based on the
            results of the search.

       Remarks
       Each document retrieved in the search will be selected in the
       filter. After the search, call DetachOutputSearchFilter to
       \obtain the filter. The DSearchJob destructor will delete any
       search filter that is not detached by a call to
       DetachOutputSearchFilter.

       See dtsSearchFilter for information on search filters.        */
    void SetWantResultsAsFilter(int v) {
		fWantResultsAsFilter = v;
		}
	/* Summary
	   Detach the output search filter and return it.
	   Returns
	   The search filter generated from this search.
	   Remarks
	   If SetWantResultsAsFilter(true) was called before the search,
	   the search results will include a search filter (see
	   dtsSearchFilter) with all of the documents retrieved in the
	   search.

	   \Note: The output search filter is separate from the optional
	   search filter that you can provide to limit the results of
	   the search, through SetSearchFilter().                        */
	dtsSearchFilter *DetachOutputSearchFilter() {
		dtsSearchFilter *ret = outputSearchFilter.detach();
		return ret;
		}
	/* Summary
	   Copy the output search filter and return it.
	   Returns
	   A copy of the search filter (use DetachOutputSearchFilter to
	   detach the original search filter, avoiding the need to make
	   a copy).
	   Remarks
	   If SetWantResultsAsFilter(true) was called before the search,
	   the search results will include a search filter (see
	   dtsSearchFilter) with all of the documents retrieved in the
	   search.

	   \Note: The output search filter is separate from the optional
	   search filter that you can provide to limit the results of
	   the search, through SetSearchFilter().                        */
	dtsSearchFilter *CopyOutputSearchFilter() {
		if (!outputSearchFilter)
			return 0;
		dtsSearchFilter *ret = new dtsSearchFilter;
		ret->copy(*outputSearchFilter.peek());
		return ret;
		}
	void SetMaxWordsToRetrieve(int v) {
		searchJob.maxWordsToRetrieve = v;
		}
	/* Summary
	   Prevents a found document from being included in search results.
	   Remarks
	   An application can veto individual documents during a search by implementing the
	   OnFound notification function and, in that function, calling VetoThisItem to veto
	   a document when OnFound is invoked.

	   Calling VetoThisItem does not cancel the search. It only excludes one document
	   from being included in search results or from counting toward the total hit count
	   for the search.                                                                   */
	void VetoThisItem() {
		bVetoThisItem = true;
		}
	/* Summary
	   Changes the score associated with an item in search results.
	   Remarks
	   An application can change the score of individual documents during a search by implementing the
	   OnFound notification function and, in that function, calling SetItemScore to 
	   modify the document's score when OnFound is invoked.
	                                 */
	void SetItemScore(dtsSearchResultsItem& item, int newVal) {
			item.score = newVal;
			bChangedItemScore = true;
		}
protected:

    dtsSearchJob searchJob;
    int fDelayDocInfo;
    long totalFiles;
    long totalHits;
    DHeapPtr<DSearchResults> results;
    DDataSourceBase *dataSourceToDelete;
    int fWantResultsAsFilter;
    DHeapPtr<dtsSearchFilter> outputSearchFilter;

    virtual void handleMessage(dtsMessage& msg);
    void allocResults();

private:
	bool bVetoThisItem;
	bool bChangedItemScore;
};

/* Summary
   Class wrapper around dtsIndexJob; used to create or update indexes.
   Remarks
   For more information on creating, updating, and maintaining indexes, see <link Building and Maintaining Indexes> */
class DIndexJob  : public DJobBase {
public:
    /* Name of the index.  */
    DStringProperty IndexName;
    /* Folder where the index files will be created. Each index must have its own
       folder.                                                                    */
    DStringProperty IndexPath;
    /* Filename filters specifying files to be indexed. Documents in the FoldersToIndex
       are indexed if they are selected in IncludeFilters and not selected in
       ExcludeFilters. See <link Filename Filters>                                      */
    DStringSetProperty IncludeFilters;
    /* Filename filters specifying files not to be indexed. Documents in the
       FoldersToIndex are indexed if they are selected in IncludeFilters and not
       selected in ExcludeFilters. See <link Filename Filters>                   */
    DStringSetProperty ExcludeFilters;
		/// Summary
		/// The directories or files to index.
		///
		/// Remarks
		///
		/// To specify a directory and its subdirectories,
		/// add &lt;+&gt; (less than, plus, greater than) following the directory name.
		///
		/// To specify one or more files, add the filenames to the FoldersToIndex list.
		/// This is more efficient than specifying the folder name in FoldersToIndex
		/// and the filename in IncludeFilters,
		/// because it eliminates the need for a directory scan to locate the file.
    DStringSetProperty FoldersToIndex;

    /* Summary
       Name of file containing a list of files to be removed from an index.
       Remarks
       The file should be a plain text file, in UTF-8 or Unicode text format, with one
       filename per line.

       The includeFilters and excludeFilters will not apply to listed files.           */

    DStringProperty ToRemoveListName;
    /* Summary
       Name of file containing a list of files to be added to an index.
       Remarks
       The file should be a plain text file, in UTF-8 or Unicode text format, with one
       filename per line.

       The includeFilters and excludeFilters will not apply to listed files.           */
    DStringProperty FileListName;
    DStringProperty DirListName; // deprecated
	/* Folder to use instead of the index folder for temporary files during indexing.  
		During indexing, the dtSearch Engine may need to create temporary files to store word lists that 
		are too large to fit into memory. By default, these files will be placed in the index folder.  Use 
		this setting to specify a different location for the word lists. The indexer will automatically 
		delete the word list files when the index update completes.
		*/
    DStringProperty TempFileDir;

    /* Summary
       Used to specify that the text in certain fields should be collected, stored in
       the index, and returned in searches.
       Remarks
       The field names in the list can contain wildcards (* and ?). A set containing a single entry "*" would match
       all fields, causing the text of every field to be stored in the index.

       The maximum amount of text that can be stored in each stored field is controlled
       by dtsOptions.maxStoredFieldSize.

       For more information, see <link Retrieving Fields in Search Results>              */
    DStringSetProperty StoredFields;

    /* Summary
       Used to specify that the text in certain fields should be stored in the index in
       a way that permits unique field values to be enumerated quickly.
       Remarks
       When a field is listed in EnumerableFields, all of the values of that field are
       stored in the index in a way that permits the values to be enumerated using
       WordListBuilder.ListFieldValues. The field names in the list can contain
       wildcards (* and ?). A set containing a single entry "*" would match all fields.

       The maximum amount of text that can be stored in each stored field is the lesser
       \of the maxStoredFieldSize option setting or 128 characters.

       All enumerable fields are also automatically designated as stored fields (see
       StoredFields).
       See Also
       <link dtsWordListBuilder::listFieldValues@char *@char *@long, dtsWordListBuilder::listFieldValues Method>

       <link Retrieving Fields in Search Results>
                                                                                                                 */
    DStringSetProperty EnumerableFields;

    DIndexJob();
    virtual ~DIndexJob();
    /* Resets the job to default values. */
    void Clear();
    /* Execute the indexing job. See the DJobBase base class for execution control
       functions (Cancel(), GetErrors(), etc.) common to the C++ support classes   */
    void Execute();
    /* \ \
       Summary
       Attach a data source to the index job. See dtsDataSource and DDataSourceBase
       Parameters
       ds :             Data source to attach.
       fShouldDelete :  If the fShouldDelete flag is true, the DIndexJob destructor will
                        delete the pointer in ds.
       Remarks
       The data source in ds will be indexed along with any files specified by the
       FoldersToIndex.                                                                   */
    void AttachDataSource(DDataSourceBase *ds, int fShouldDelete = false) {
        if (fShouldDelete)
            DataSourceToIndex.attach(ds);
        else
            DataSourceToIndex.attachWithoutOwnership(ds);

        if (ds)
            indexJob.dataSourceToIndex = DataSourceToIndex->getInterface();
        else
            indexJob.dataSourceToIndex = 0;
        }
    /* Add a folder to the list of folders to be indexed */
    void AddFolderToIndex(const dtsTCHAR * folderName) {
        FoldersToIndex.append(folderName);
        }
    void SetFilters(const dtsTCHAR * incl, const dtsTCHAR * excl) {
        IncludeFilters.tokenize(incl);
        ExcludeFilters.tokenize(excl);
        }
    void SetIndex(const dtsTCHAR * path, const dtsTCHAR * name = 0) {
        IndexPath.set(path);
        IndexName.set(name);
        }
    /* Set the "add" action flag for the IndexJob (index documents that are not
       currently in the index, or that have been modified since they were last indexed). */
    void SetActionAdd(int v = true) {
            indexJob.action.add = ToFlag(v);
            }
    /* Set the "compress" action in the index job (compress optimizes an index and
       removes obsolete information)                                               */
    void SetActionCompress(int v = true) {
            indexJob.action.compress = ToFlag(v);
            }
    int GetActionCreate() {
            return indexJob.action.create;
            }
    /* Set the "create" action in the index job (create a new, empty index before any
       \other actions)                                                                */
    void SetActionCreate(int v = true) {
            indexJob.action.create = ToFlag(v);
            }
    /* Set the "remove deleted" action in the index job. Removing deleted documents
       cause the indexer to check whether each document in the index still exists, and
       to remove from the index any documents that no longer exist.                    */
    void SetActionRemoveDeleted(int v = true) {
            indexJob.action.removeDeleted = ToFlag(v);
            }
    /* If the "remove listed" action is true, the files listed in ToRemoveListName will
       be removed from the index.                                                       */
    void SetActionRemoveListed(int v = true) {
            indexJob.action.remove = ToFlag(v);
            }
    /* Clear all of the action flags (add, create, etc.) */
    void SetActionNone() {
            indexJob.action.add = 0;
            indexJob.action.compress = 0;
            indexJob.action.removeDeleted = 0;
            indexJob.action.remove = 0;
            indexJob.action.create = 0;
            }
    int IsEmpty() {
		return !indexJob.action.add && !indexJob.action.compress &&
			   !indexJob.action.removeDeleted && !indexJob.action.remove &&
			   !indexJob.action.create;
		}
    void SetFlagAllowConcurrentAccess(int v = true) {
            indexJob.allowConcurrentAccess = ToFlag(v);
            }
    /* Set to true to create an index that uses relative paths */
    void SetFlagCreateRelativePaths(int v = true) {
            indexJob.createFlags.relativePaths = ToFlag(v);
            }
    /* Set to true to create a case-sensitive index */
    void SetFlagCreateCaseSensitive(int v = true) {
            indexJob.createFlags.caseSensitive = ToFlag(v);
            }
    /* Set to true to create an accent-sensitive index. */
    void SetFlagCreateAccentSensitive(int v = true) {
            indexJob.createFlags.accentSensitive = ToFlag(v);
            }

#if !defined(DocumentationGeneratorOnly)
    virtual void OnProgressUpdate(dtsIndexProgressInfo& /* info */ ) {  };
#else
	/* Summary
	   During indexing, OnProgressUpdate will be called frequently with information
	   about the progress of the indexing job.
	   Parameters
	   info :  Structure with information on the status of the index upate
	   Remarks
	   During indexing, OnProgressUpdate will be called frequently with information
	   about the progress of the indexing job. You can use this information to show the
	   user a progress bar or other status indicator during indexing. To access this
	   information, create a class based on DIndexJob that overrides OnProgressUpdate. Example:

	   <code>
	    class CMyIndexJob : public DIndexJob {
	        public:
	            virtual void OnProgressUpdate(dtsIndexProgressInfo& info) {
	            // ... show the user the update info in a dialog box
	            }
	        };
	   </code>                                                                                  */
    virtual void OnProgressUpdate(dtsIndexProgressInfo& info ) {  };
#endif
    void SetFlagCheckDiskSpace(int v = true) {
        if (v)
            indexJob.indexingFlags |= dtsCheckDiskSpace;
        else
            indexJob.indexingFlags &= (~dtsCheckDiskSpace);
        }
    /* Limit the amount of memory that the indexer will use for workspace. nMegabytes is
       the number of megabytes to use, so 32 would mean 32 megabytes of memory. By
       default, the indexer will calculate how much memory to use based on the size of
       the document collection being indexed and the amount of physical RAM available
       (no more than 50% of the amount of physical RAM installed will be used).          */
    void SetMaxMemToUseMB(long v) {
        indexJob.maxMemToUseMB = v;
        }
    /* Set flags controlling how the index should be built. See IndexingFlags enumeration */
    void SetIndexingFlags(long v) {
		indexJob.indexingFlags = v;
		}
    virtual void Cancel();
    virtual void CancelImmediate();
    /* The dtsIndexJob that this DIndexJob object wraps. */
    dtsIndexJob indexJob;
protected:
    virtual void handleMessage(dtsMessage& msg);
    DHeapPtr<DDataSourceBase> DataSourceToIndex;
};

// Class wrapper around dtsIndexMergeJob, which combines one or more indexes into a single target index.
class DIndexMergeJob : public DJobBase {
    public:
    	// Indexes to be merged
        DStringSetProperty IndexesToMerge;

        DIndexMergeJob();
        // Clear job
        void Clear();
        // Merge the indexes
        void Execute();
        int IsEmpty() {
			return IndexesToMerge.getCount() == 0;
			}
		// Set IndexingFlags controlling the behavior of the merge
		void SetFlags(int f) {
			mergeJob.mergeFlags = f;
			}
		// Set the target index
		// Parameters:
		//	iTarget:  Identifies which of the indexes in IndexesToMerge is the target
		//  fShouldClear:   If true, the target index will be cleared before the merge begins
        void SetTarget(int iTarget, int fShouldClear);
#if !defined(DocumentationGenerationOnly)
        virtual void OnProgressUpdate(dtsMergeStatusInfo& ) {  };
#else
        // Called during merge to provide status updates
        // Parameters:
        // 	  info:  Structure with information on the status of the merge
        virtual void OnProgressUpdate(dtsMergeStatusInfo& info) {  };
#endif
    protected:
        dtsIndexMergeJob mergeJob;
        virtual void handleMessage(dtsMessage& msg);
    };

// Class wrapper around dtsIndexVerifyJob, which verifies that an index is not damaged
class DIndexVerifyJob : public DJobBase {
    public:
    	// Index to verify
        DStringProperty IndexPath;

        DIndexVerifyJob();
        // Clear the job
        void Clear();
        // Verify the index
        void Execute();
        int IsEmpty() {
			return IndexPath.isBlank();
			}
		// begin-internal
		void SetFlags(int v) {
			verifyJob.flags = v;
			}
		// end-internal
#if !defined(DocumentationGenerationOnly)
        virtual void OnProgressUpdate(dtsVerifyStatusInfo& ) {  };
#else
        // Called during verify to provide status updates
        // Parameters:
        // 	  info:  Structure with information on the status of the index verification
        virtual void OnProgressUpdate(dtsVerifyStatusInfo& info) {  };
#endif
    protected:
        dtsIndexVerifyJob verifyJob;
        virtual void handleMessage(dtsMessage& msg);
    };

/* Summary
   ExtractionOptions (attached to a DFileConvertJob) specify how embedded images and
   attachments should be handled.
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

   dtsExoExtractImages can be used when highlighting hits.                           */ 
class DExtractionOptions  {
	public:
		DExtractionOptions() :
			FilenamePrefix(&extractionOptions.filenamePrefix),
			OutputLocation(&extractionOptions.outputLocation),
			OutputReference(&extractionOptions.outputReference),
			OutputFileListName(&extractionOptions.outputFileListName),
			UnnamedAttachmentLinkText(&extractionOptions.unnamedAttachmentLinkText),
			DefaultExtension(&extractionOptions.defaultExtension),
			AllowedExtensions(&extractionOptions.allowedExtensions)
			{ }
		void copy(const DExtractionOptions& other) {
			copy(other.extractionOptions);
			}
		void copy(const dtsExtractionOptions& other) {
			FilenamePrefix.setU8(other.filenamePrefix);
			OutputLocation.setU8(other.outputLocation);
			OutputReference.setU8(other.outputReference);
			OutputFileListName.setU8(other.outputFileListName);
			AllowedExtensions.copyFromBuf(other.allowedExtensions);
			DefaultExtension.setU8(other.defaultExtension);
			UnnamedAttachmentLinkText.setU8(other.unnamedAttachmentLinkText);
			extractionOptions.flags = other.flags;
			}
		// Name of text file to create with the names of all images and attachments
		// extracted to the OutputLocation.
		DStringProperty  OutputLocation;
	/* Summary
	   URL of the OutputLocation for links in the conversion output.
	   Remarks
	   When the output format for the conversion is HTML or XML, the location of each
	   extracted image or attachment will be stored as a link. While OutputLocation must
	   be a folder name, OutputReference should be the equivalent location expressed as
	   a URL, so it will work when used in the links in the conversion output.

	   For example, on a web server OutputLocation might be c:\\inetpub\\user1\\temp and
	   \OutputReference would be the virtual path to this folder, /user1/temp.           */
		DStringProperty OutputReference;
	/* Summary
	   Name of text file to create with the names of all images and attachments
	   extracted to the OutputLocation.
	   Remarks
	   Each attachment or image file extracted will be listed in the file, with one name
	   per line. Filenames will be stored in UTF-8 format.                               */
		DStringProperty OutputFileListName;
	/* Summary
	   Filename prefix for extracted files stored in OutputLocation
	   Remarks
	   For example, if sample.jpg is extracted, and if filenamePrefix is "tmp_", then
	   the name of the file saved will be "tmp_sample.jpg".                           */
		DStringProperty FilenamePrefix;
	/* Summary
	   Text to use for link to an attachment that has no name
	   Remarks
	   If an attachment has no name attribute in the input document, the value in
	   UnnamedAttachmentLinkText will be used as the visible text for the HTML link in
	   the generated output.                                                           */
		DStringProperty UnnamedAttachmentLinkText;
	/* \ \ 
	   Summary
	   If the dtsExoLimitExtensions flag is set, image and attachment filename
	   extensions will be limited to the values listed in allowedExtensions.
	   Remarks
	   The AllowedExtensions option provides a way to guard against execution of malware
	   that may be embedded in documents. When an attachment or image is extracted,
	   unless the filename extension is one of the listed extensions in
	   AllowedExtensions, the filename will have the DefaultExtension appended to it.

	   For example, if "exe" is not included in the list of AllowedExtensions, and if
	   the DefaultExtension is ".data", then an attachment named "sample.exe" will be
	   saved as "sample.exe.data". AllowedExtensions is a null-delimited string set.

	   Do not include the "." in each extension.                                         */
		DStringSetProperty AllowedExtensions;
	/* Summary
	   Extension to add to images and attachments when the dtsExoLimitExtensions flag
	   is set and an image or attachment has an extension that is not listed in
	   AllowedExtensions.
	   Remarks
	   The AllowedExtensions list controls which attachments and images can be extracted
	   with the original filename extension preserved. Attachments and images with
	   extensions that are not in the list will have the DefaultExtension appended to
	   the output filename, effectively disabling the original extension.

	   Do not include the "." in the extension.                                          */
		DStringProperty DefaultExtension;

		operator dtsExtractionOptions *()  {
			return &extractionOptions;
			}
		operator const dtsExtractionOptions *()  const {
			return &extractionOptions;
			}
		void clear() {
			FilenamePrefix.clear();
			OutputLocation.clear();
			OutputReference.clear();
			OutputFileListName.clear();
			UnnamedAttachmentLinkText.clear();
			}
		bool ShouldMarkAsInternetZone() const {
			return (extractionOptions.flags & dtsExoMarkAsInternetZone? true : false);
			}
		bool ShouldExtractAttachments() const {
			return (extractionOptions.flags & dtsExoExtractAttachments? true : false);
			}
		bool ShouldExtractImages() const {
			return (extractionOptions.flags &  dtsExoExtractImages? true : false);
			}
		bool ShouldConvertAttachments() const {
			return (extractionOptions.flags & dtsExoDoNotConvertAttachments? false : true);
			}
		bool ShouldExtractAnything() const {
			return ShouldExtractImages() || ShouldExtractAttachments();
			}
		int GetFlags() const {
			return extractionOptions.flags;
			}
		void SetFlags(int f) {
			extractionOptions.flags = f;
			}			
	protected:
		dtsExtractionOptions extractionOptions;
	};

/* Summary
   Class wrapper around dtsOutputInfo, which is used in DSearchReportJob and
   DFileConvertJob to specify the output format for file conversion and search
   \reports
   Remarks
   The header, footer, beforeHit, and afterHit markings should be in the format of
   the output. For example, if the output format is HTML, then beforeHit and
   afterHit markings could be \<b\> and \</b\>.

   BeforeHit, AfterHit, Header, and Footer control the appearance of converted text.

   The Header and Footer strings are inserted before and after the body of the
   document.

   The BeforeHit and AfterHit markers are inserted before and after each hit word.
   The BeforeHit and AfterHit markers can contain hypertext links.

   To facilitate creation of hit navigation markers, the strings "%%ThisHit%%",
   "%%NextHit%%", and "%%PrevHit%%" will be replaced with ordinals representing the
   current hit, the next hit, and the previous hit in the document.

   For example, the following code makes the beforeHit and nextHit marks act as HTML
   "Previous Hit" and "Next Hit" buttons.
   <code>
       job.BeforeHit =
                    "\<A NAME="hit%%ThisHit%%"\>"
                          "\<A HREF="#hit%%PrevHit%%"\>"
                    "\<IMG SRC="hit1.gif" ALIGN="middle" ALT="[Prev Hit]" BORDER=0\>";
       job.AfterHit =
                       "\<A HREF="#hit%%NextHit%%"\>"
                       "\<IMG SRC="hit2.gif" ALIGN="middle" ALT="[Next Hit]" BORDER=0\>";
   </code>                                                                                */

class DOutputBase : public DJobBase {
public:
	// For HTML output, a doctype declaration to go at the top of the output
	DStringProperty DocTypeTag;
	// For HTML output, HTML tags to insert in the header
    DStringProperty HtmlHead;
    // For HTML output, an HREF for a BASE tag to be inserted in the header.
    DStringProperty HtmlBaseHref;
    // Text inserted before each hit
    DStringProperty BeforeHit;
    // Text inserted after each hit
    DStringProperty AfterHit;
    // Text inserted at the start of the file
    DStringProperty Header;
    // Text inserted at the end of the file
    DStringProperty Footer;
    // Name of file to create, if the output is to be written to a file.
    DStringProperty OutputFile;

    DOutputBase(dtsJobBase& baseJob);
    virtual ~DOutputBase() { }
    // Clear all fields to default values
    void Clear();
    // Send output only to the virtual callback function, OnOutput
    void SetOutputToCallback();
    // Send output to a string buffer managed by the class, and set the maximum length of the
    // output string.
    void SetOutputToString(long aMaxLen = -1);
    // Send output to a file
    void SetOutputToFile(const char *fn) {
        OutputFile.set(fn);
        outputDestination = OutputToFile;
        linkOutputBuffers();
        }
    void SetHitMarks(const dtsTCHAR *before, const dtsTCHAR *after) {
        BeforeHit.set(before);
        AfterHit.set(after);
        }
    void SetHeader(const dtsTCHAR *aHeader) {
        Header.set(aHeader);
        }
    void SetFooter(const dtsTCHAR *aFooter) {
        Footer.set(aFooter);
        }
    void SetHtmlHead(const dtsTCHAR *s) {
        HtmlHead.set(s);
        }
    void SetHtmlBaseHref(const dtsTCHAR *s) {
        HtmlBaseHref.set(s);
        }
	// Specifies the format of the converted text. Currently supported options are: it_HTML, it_RTF, it_Utf8 (Unicode text), and it_Ansi.
    void SetOutputFormat(int fmtId = it_HTML);
    int GetOutputFormat() {
		return (int) outputInfo.outputFormat;
		}
    // Callback function to receive output as it is generated
    virtual void OnOutput(const char * /*text */, int /* len */) { };
    // Callback function to update the progress of the conversion or search report
    virtual void OnPercentDone(int /* pct */) { };
	// If output was to a string, returns a pointer to the output string
    const char *GetOutputString() {
        return outputString;
        }
#ifdef BSTR_DEFINED
    BSTR GetOutputBSTR();
#endif
	// Get the length of the output string
    ptrdiff_t GetOutputStringLen() {
        return outputString.getLength();
        }
    // Returns true if output was directed to a string
    int IsOutputToString() {
        return outputDestination == OutputToString;
        }
    DString outputString;
    virtual void Execute() { } ;
    virtual void OnCheckAbort() { };
    virtual void OnError(long /* errorCode */, const char * /* message */ , const char * /* arg1 */, const char * /* arg2 */) { };

protected:
    dtsOutputInfo outputInfo;

    enum OutputType {
        OutputToFile, OutputToString, OutputToCallback
        };
    OutputType outputDestination;
    long outputStringMaxLen;

    void linkOutputBuffers();
    void writeToString(const char *text, ptrdiff_t len);
    static DOutputBase *safeCast(void *pData);
    static int outputCB(void *pData, const char *text, int len);
    virtual void handleMessage(dtsMessage& msg);
    };

/* Summary
   Class wrapper around dtsFileConvertJob2; used to convert files to HTML, RTF, or
   Ansi text.
   Remarks
   DFileConvertJob provides an easy way to display retrieved files in a web browser,
   web browser control, or other viewer with hits highlighted.

   When working with search results, the recommended way to set up DFileConvertJob
   is to use SetInputItem, which automatically sets up the DFileConvertJob based on
   data in the search results object.

   If a list of hits is provided, these hits will be highlighted using the BeforeHit
   and AfterHit markers in the output document. The Header and Footer markers will
   appear at the beginning and end of the output file. Output can go to a disk file
   (SetOutputToFile), a memory buffer (SetOutputToString) or a callback function
   (SetOutputToCallback). Input can come from a disk file (InputFile) or from a
   memory buffer (SetInputText).

   See the DJobBase base class for execution control functions (Cancel(),
   GetErrors(), etc.) common to the C++ support classes.                             */
class DFileConvertJob : public DOutputBase {
public:
	// Input file to be converted
    DStringProperty InputFile;
    /* The location of the dtSearch alphabet file to use when highlighting hits.
	The alphabet file determines how dtSearch counts words, so it is important
	that the same alphabet file used to index or search a file also be used to
	highlight hits. For more information on how hit highlighting works, see
	Highlighting Hits in the online help. To ensure that the same alphabet used
	to index a file is used to highlight hits in that file, set the
	alphabetLocation to the folder where the index is located. The alphabet
	definition will be stored in this folder (in a file named INDEX_A.IX). If no
	alphabetLocation is specified, dtSearch will use the default alphabet.  */
    DStringProperty AlphabetLocation;
    // The index in which the document was found. SetInputItem will set this based on information in SearchResults.
    DStringProperty IndexRetrievedFrom;

	// A null-delimited string set containing the data returned by a search
	// when the dtsSearchWantHitsByWord and dtsSearchWantHitDetails search flags
	// are set.  This is only used when the flag dtsConvertMultiHighlight is
	// set to use different highlighting strings for each search term.
    DStringSetProperty HitsByWord;

    DFileConvertJob();
    virtual ~DFileConvertJob();
    /* Sets the input text to be converted, for conversion of a memory buffer instead of
       a disk file. The text must be a simple null-terminated string.
       Parameters
       text :  Text to use as input
       aFn :   Filename, indicating the source of the text (can be null)
       aTypeId : Type id specifying the file type of the input data.
      */
    void SetInputText(const char *text, const char *aFn, int aTypeId);
    /* Sets the input text to be converted, for conversion of a memory buffer instead of
       a disk file. The text must be a simple null-terminated string.
       Parameters
       text :  Text to use as input
       nBytes : Length of the text
       aFn :   Filename, indicating the source of the text (can be null)
       aTypeId : Type id specifying the file type of the input data.
      */
    void SetInputText(const char *text, ptrdiff_t nBytes, const char *aFn, int aTypeId);
    /* Set the word offsets of hits to highlight. SetInputItem will set this based on
       information in search results.
       Parameters
       hits :  Word offsets of hits
       ct :    Number of hits                                                         */
    void SetHits(long *hits, long ct);
    //! Reserved for internal use
	void SetInputFlags(int f) {
		inputFlags = f;
		}
	int GetInputFlags() const {
		return inputFlags;
		}

    /* Summary
       SetInputItem initializes a DFileConvertJob with information about a document that
       is available from search results.
       Parameters
       res :    Search results containing the item
       iItem :  The item in search results to use as input
       Remarks
       Data transferred from search results in SetInputItem includes:
         * The filename
         * The hit count and hit offsets
         * The file type of the document when it was indexed
         * The document id of the document in the index
         * The index path and alphabet location                                          */
    void SetInputItem(DSearchResults& res, int iItem = -1);
    /* Summary
       SetInputItem initializes a DFileConvertJob with information about a document that
       is available from search results.
       Parameters
       item :  The item to use as input
       Remarks
       Data transferred from search results in SetInputItem includes:
         * The filename
         * The hit count and hit offsets
         * The file type of the document when it was indexed
         * The document id of the document in the index
         * The index path and alphabet location                                          */
    void SetInputItem(const dtsSearchResultsItem& item);

    void SetInputFile(const dtsTCHAR *fn) {
        InputFile.set(fn);
        }
    void SetInputDocId(long docId) {
		convertJob.docId = docId;
		}
	/* \Returns the document id for the document in the index it was retrieved from. (This
	   must have been previously set either using SetInputItem or SetInputDocId).          */
	long GetInputDocId() {
		return convertJob.docId;
		}
	//! obsolete -- use SetInputStream
    void SetInputSource(dtsInputStream& aSource) {
		SetInputStream(aSource);
        }
    /* \ \
       Summary
       DFileConvertJob can get input from a file, a memory buffer, or from a
       dtsInputStream. Use SetInputStream to attach a dtsInputStream to use as input.
       Parameters
       aSource :  The input stream to use as input for the conversion                 */
    void SetInputStream(dtsInputStream& aSource, int aTypeId = 0) {
        inputSource.copy(aSource);
        InputFile.clear();
        inputType = InputFromISource;
        if (aTypeId > 0)
			inputSource.typeId = aTypeId;
        }
    dtsInputStream& GetInputStream() {
		return inputSource;
	}
    void SetAlphabetLocation(const dtsTCHAR *fn) {
        AlphabetLocation.set(fn);
        }
	// Execute the job
    void Execute();

    /* Set ConvertFlags for this job */
    void SetFlags(long v) {
        convertJob.flags = v;
        }

    /* Set a ConvertFlags value for this job */
    void SetFlag(long v, bool b = true) {
		if (b)
			convertJob.flags |= v;
		else
			convertJob.flags &= (~v);
        }
    /* \Returns the ConvertFlags for the job */
    long GetFlags() {
        return convertJob.flags;
        }
	/* The file type of the input document when it was indexed. SetInputItem() will set
	   this based on information in search results.                                     */
	void SetTypeId(int t) {
		convertJob.typeId = t;
		}
	/* The file type of the input document when it was indexed. SetInputItem() will set
	   this based on information in search results.                                     */
	int GetTypeId() {
		return (int) convertJob.typeId;
		}

	/* The file type of the input document, as detected by the dtSearch file parsers
	   during the conversion */
	dtsInputType GetDetectedTypeId() {
		return (dtsInputType) convertJob.detectedTypeId;
		}
	// Set the build number used to index this file for purposes of maintaining
	// backward compatibility (this will eliminate some compatibility
	// issues but not all).
	void SetIndexedBy(int v) {
		convertJob.indexedBy = v;
		}
	// Use an IndexCache for faster extraction of cached documents from indexes
	void SetIndexCache(dtsIndexCacheHandle hCache) {
		convertJob.indexCacheHandle = hCache;
		}
	dtsFileConvertJob2& getJob() {
		return convertJob;
		}
	void setExtractionOptions(const DExtractionOptions& opts) {
		extractionOptions.copy(opts);
		convertJob.extractionOptions = extractionOptions;
		}
protected:
    enum InputType {
        InputFromFile, InputFromString, InputFromISource, InputUndefined
        };
    InputType inputType;

    dtsFileConvertJob2 convertJob;
    DExtractionOptions extractionOptions;
    DMemoryInputStream inputText;
    dtsInputStream inputSource;
    DArrayAsVector<long> hits;
    DString encodedSearchItem;
    int inputFlags;
    void linkBuffers();
    };

#ifdef _WIN32
/* Summary
   Used to convert among virtual, local, and UNC paths (Windows only)
   Remarks
   Internet Information Server stores its mapping between virtual and local paths in
   the Metabase. Reading this information from the Metabase is time-consuming and
   requires access rights that may not be available to a process running in the

   web server context. Therefore, the dtSearch Engine caches this information in the
   registry, under the key HKEY_LOCAL_MACHINE\\Software\\dtSearch
   Corp.\\dtSearch\\Virtual Roots. By default, the conversion will use the cached
   table values. Call ConvertPath with the dtsCpRefreshMappings +
   dtsCpStoreInRegistry flags to update the cached information. (Running
   dtWebSetup.exe, the dtSearch Web Setup program, will also update this
   information.)

   If path conversion returns a blank string, it means that either (1) the folder
   passed in does not have a virtual directory associated with it, or (2) the
   registry tables have not been refreshed since the virtual directory was created.
                                                                                     */
class DConvertPathRequest {
    public:
        /* \Input path for conversion */
        DStringProperty Path;
        /* \Result of the conversion */
        DStringProperty ConvertedPath;
        /* Registry key where virtual-to-local tables are kept. Unless your application sets
           up these tables in an alternative location, leave blank to use the default
           location.                                                                         */
        DStringProperty TableRegistryKey;
        DConvertPathRequest();
        /* Read virtual-to-local mappings from the Metabase and store them in the registry. This
           call will often fail if made from within an IIS process, due to lack of
           sufficient rights.                                                                    */
        void RefreshMappings();
        /* Convert virtual folder name Path to a local path, which will be returned in
           ConvertedPath
           Parameters
           siteId :            ID of the web site to use for the conversion (each web site maintains its
                               own set of virtual\-to\-local path mappings in Internet Information
                               Server).
           readFromRegistry :  If true, the tables will be read from the registry (otherwise, tables are
                               cached inside the dtSearch Engine for faster performance).                */
        void MakeLocal(long siteId = 0, bool readFromRegistry = false);
        /* Convert local folder name Path to a virtual path, which will be returned in
           ConvertedPath
           Parameters
           siteId :            ID of the web site to use for the conversion (each web site maintains its
                               own set of virtual\-to\-local path mappings in Internet Information
                               Server).
           readFromRegistry :  If true, the tables will be read from the registry (otherwise, tables are
                               cached inside the dtSearch Engine for faster performance).                */
        void MakeVirtual(long siteId = 0, bool readFromRegistry = false);
        /* Convert Path to a UNC path, which will be returned in ConvertedPath */
        void MakeUNC();
    protected:
        /* Execute the conversion, producing output in ConvertedPath. */
        void executeRequest();
        dtsConvertPathRequest m_request;
    };
#endif

// List the contents of an index (words, files, or field names)
class DListIndexJob : public DJobBase {
    public:
        DListIndexJob();
		/* ToMatch is an optional search expression specifying the text to match
		against items being listed. For example, to list all field names starting
		with "A", you would set SetListFlags(dtsListIndexFields), and set ToMatch to
		"A*". */
        DStringProperty ToMatch;
        // Index to list
        DStringProperty IndexPath;
        // Name of file to create
        DStringProperty OutputFile;
        // Destination for list, if SetOutputToString() was called.
        DStringProperty OutputString;
        // Direct output to OutputString
        void SetOutputToString();
        // SearchFlags specifying search features to be used in matching the toMatch expression, such as fuzziness, stemming, etc
		void SetSearchFlags(long v) {
            m_listJob.searchFlags = v;
            }
	    // ListIndexFlags specifying the type of list to generate
        void SetListFlags(long v) {
            m_listJob.listFlags = v;
            }
        void SetFuzziness(long v) {
            m_listJob.fuzziness = v;
            }
        // Generate the list
        void Execute();
		// Maximum size of the output string to generate
        void SetMaxSize(long v) {
            m_listJob.outputStringMaxSize = v;
            }
        dtsListIndexJob m_listJob;
    };

/* Summary
   Describes a search report to be prepared from some or all of the items in a
   DSearchResults set.
   Remarks
   To generate a search report,

   (1) Start with a dtsSearchResults object representing the results of a search.

   (2) Declare a DSearchReportJob, passing the dtsSearchResults to the constructor

   (3) Select the items to include in the search report using the Select*() methods
   in DSearchReportJob

   (4) Specify the amount of context to include using SetContextWords() or
   SetContextParagraphs()

   (5) Set the output format for the report

   (6) Call Execute() to generate the report
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
   Use %% around each symbol, like this: %%FullName%%

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
   Example
   <code>
   // Assume m_pResults is a pointer to dtsSearchResults
   DSearchReportJob reportJob(*m_pResults);
   // Request exactly 10 words of context around each hit
   reportJob.SetContextWords(10, true);
   // Specify which items in search results to include
   reportJob.SelectRange(firstItemToInclude, lastItemToInclude);
   // Highlight hits in bold, with a line between each block of context
   reportJob.SetOutputFormat(it_Html);
   reportJob.BeforeHit.setU8("\<b\>");
   reportJob.AfterHit.setU8("\</b\>");
   reportJob.ContextHeader = "\<hr\>";
   // Generate output to a string limited to 512 bytes
   reportJob.SetOutputToString(512);
   // Request at most two blocks of context for each document included in the report
   reportJob.SetMaxContextBlocks(2);
   // Use cached text in the index, if present
   reportJob.SetFlag(dtsReportGetFromCache);
   // Generate the report
   reportJob.Execute();
   </code>                                                                            */

class DSearchReportJob : public DOutputBase {
    public:
	    // Text to appear in front of each block of context.
        DStringProperty ContextHeader;
        // Text to appear after each block of context
	    DStringProperty ContextFooter;
	    // Text to appear between blocks of context (after the ContextFooter)
	    DStringProperty ContextSeparator;
	    // Text to appear after each file in the search report
        DStringProperty FileHeader;
        // Text to appear after each file in the search report
        DStringProperty FileFooter;

		/* Summary
		Constructor for a DSearchReportJob associated with a search results list
		Remarks
		A DSearchReportJob has to have a search results list associated with
		it. You can either use the constructor to create this association or you
		can use SetResults to copy the search results into the DSearchReportJob.
		DSearchReportJob keeps its own private copy of the search results
		list. */
        DSearchReportJob(dtsSearchResults& results);
		/* Summary
		Constructor for a DSearchReportJob with no associated search results list.
		Remarks
		A DSearchReportJob has to have a search results list associated with
		it. You can either use the constructor to create this association or you
		can use SetResults to copy the search results into the DSearchReportJob.
		DSearchReportJob keeps its own private copy of the search results
		list. */

        DSearchReportJob();
        virtual ~DSearchReportJob();
        // Attach search results to the DSearchReportJob, making a private internal
        // copy of the search results.
        void SetResults(dtsSearchResults& results) {
            AttachResults(&results, true);
            }
       	// Attach search results to the DSearchReportJob.  Unlike SetResults, AttachResults
       	// provides the option not to make a copy of the search results
        void AttachResults(dtsSearchResults *pRes, int fMakePrivateCopy = false);
        // Set the number of words of context around each hit, and specify whether
        // the exact number of words should be collected.  If fExact is false, enough paragraphs
        // will be collected around each hit to include at least the specified number of words.
        void SetContextWords(int n, int fExact = true);
        // Set the amount of context around each hit as a number of paragraphs
        void SetContextParagraphs(int n);
        // Include the whole file for each item in search results
        void SetContextWholeFile();
        // Generate the report
        void Execute();
        // Select all items in the SearchResults.
        void SelectAll();
        /* Summary
           Select a range of items in the SearchResults
           Parameters
           nFirst :  First item to include (0\-based index into SearchResults)
           nLast :   Last item to include (0\-based index into SearchResults)  */
        void SelectRange(long nFirst, long nLast);
        // Clear all selections previously made using SelectAll or SelectRange
        void ClearSelections();
        // Set ReportFlags for this report
        void SetFlags(int v) {
            reportJob.flags = v;
            }
        // Get ReportFlags for this report
        long GetFlags() {
            return reportJob.flags;
            }
        void SetFlag(int v) {
            reportJob.flags |= v;
            }
        //!  Internal use only
        void SetDocExtractor(dtsDataSourceDocExtractor *pExtractor) {
            reportJob.docExtractor = pExtractor;
        }
        // Number of blocks of context to include in the report for each document.
   		void SetMaxContextBlocks(int n) {
			reportJob.maxContextBlocks = n;
			}
		/* Number of words to scan in each document looking for blocks of
		   context to include in the report For example, if maxWordsToRead=5000,
		   then the search report will only include hits that occur in the first
		   5000 words in each document. This setting can be used to generate
		   faster (but not necessarily complete) reports by limiting the search
		   report to hits near the beginning of each document */
		void SetMaxWordsToRead(int n) {
			reportJob.maxWordsToRead = n;
			}

    protected:
        dtsSearchReportJob reportJob;
        DBitVector selections;
        int fShouldDeleteResults;
        dtsSearchResults *pResults;
        void linkBuffers();
    };

// Class wrapper for dtsWordListBuilder
class DWordListBuilder {
    public:
        DWordListBuilder() { };
        /* Open index with a const TCHAR * path
           Parameters:
           	aIndexPath:   The index to open
           Returns: 0 on success, or non-zero if the index could not be opened
         */
        long OpenIndex(const dtsTCHAR *aIndexPath) {
            IndexPath.setUi(aIndexPath);
            return wlbuilder.openIndex(IndexPath);
            }
        // Close index
        void CloseIndex() {
            wlbuilder.closeIndex();
            }
       // List words surrounding a const TCHAR * word
        void ListWords(const dtsTCHAR *aCursorWord, long aRange) {
            Word.setUi(aCursorWord);
            wlbuilder.listWords(Word, aRange);
            }
        // List words matching a const TCHAR * word
        void ListMatchingWords(const dtsTCHAR *toMatch, long maxWords, long searchFlags, long fuzziness) {
            Word.setUi(toMatch);
            wlbuilder.listMatchingWords(Word, maxWords, searchFlags, fuzziness);
            }
        // Returns the number of words in the list
        long GetCount() {
            return wlbuilder.getCount();
            }
        // List all fields in the index
        long ListFields() {
            wlbuilder.listFields(-1);
            return wlbuilder.getCount();
            }
       	// Get a word from the list and store it in Word
        void GetWord(long iWord) {
            char temp[512];
            wlbuilder.getWord(iWord, temp, sizeof temp, WordHitCount, WordDocCount);
            Word.setU8(temp);
            }
        // Get all words in the list in a DStringSetProperty
        void GetAll(DStringSetProperty& set) {
            set.clear();
            long ct = wlbuilder.getCount();
            char temp[512];
            long dummy;
            for (long i = 0; i < ct; ++i) {
                wlbuilder.getWord(i, temp, sizeof temp, dummy);
                set.appendU8(temp);
                }
            }
        // Buffer to hold the currently-selected word
        DStringProperty Word;
        // Number of times the currently-selected word occurs in the index
        long WordHitCount;
        // Number of documents in which the currently-selected occurs in the index
        long WordDocCount;
    protected:
        DStringProperty IndexPath;
        dtsWordListBuilder wlbuilder;
    };

/* Class wrapper around dtsIndexInfo
*/
class DIndexInfo : public dtsIndexInfo {
public:
	// Constructor
    DIndexInfo() { };
    // Copy constructor
    DIndexInfo(const DIndexInfo& other) {
        copy(other);
        }
    virtual ~DIndexInfo() {

    }
    // Copy a DIndexInfo
    DIndexInfo& operator=(const DIndexInfo& other) {
        copy(other);
        return *this;
        }
    // Copy a DIndexInfo
    void copy(const DIndexInfo& other) {
            structSize = sizeof(dtsIndexInfo);
            docCount = other.docCount;
            wordCount = other.wordCount;
            dirCount = other.dirCount;
            fragmentation = other.fragmentation;
            obsoleteCount = other.obsoleteCount;
            strCopy(name, other.name, sizeof name);
            created.copy(other.created);
            updated.copy(other.updated);
            compressed.copy(other.compressed);
            flags.accentSensitive = other.flags.accentSensitive;
            flags.caseSensitive = other.flags.caseSensitive;
            indexSize = other.indexSize;
            indexSize64 = other.indexSize64;
            strCopy(creator, other.creator, sizeof creator);
            IndexPath = other.IndexPath;
            };
    // Get index properties using a UTF-8 path
    // Returns:
    //   0 on success, or non-zero if the index could not be accessed
	virtual int GetU8(const char *utf8Path) {
		IndexPath.setU8(utf8Path);
		return Get();
		}
    // Get index properties using an Ansi path
    // Returns:
    //   0 on success, or non-zero if the index could not be accessed
    virtual int GetA(const char *ansiPath) {
        IndexPath.setA(ansiPath);
        return Get();
        }
    // Get index properties using a TCHAR or CString path
    // Returns:
    //   0 on success, or non-zero if the index could not be accessed
    virtual int GetUi(const TCHAR *indexPath) {
        IndexPath.setUi(indexPath);
        return Get();
        }
    // Get index properties using a Unicode path
    // Returns:
    //   0 on success, or non-zero if the index could not be accessed
    virtual int GetW(const wchar_t *wPath) {
        IndexPath.set(wPath);
        return Get();
        }
	void GetAsStrings(DStringSet& ss);

    DStringProperty IndexPath;
    int Get() {
        short r;
        dtssGetIndexInfo(IndexPath, *this, r);
        return r;
        }
};

//! internal use
class DIndexProgressInfo : public dtsIndexProgressInfo {
    public:
        DIndexProgressInfo() :
            IndexPath(&indexPath),
            IndexName(&indexName),
            FileName(&file.name),
            FileLocation(&file.location),
            FileType(&file.type),
            FileOpenFailMessage(&file.openFailMessage)
            { }
        DIndexProgressInfo(const dtsIndexProgressInfo& other) {
            Store(other);
            }
        DIndexProgressInfo& operator=(const dtsIndexProgressInfo& other) {
            Store(other);
            return *this;
            }
        void Store(const dtsIndexProgressInfo& info) {
			copyNumbers(info);
            IndexPath.setU8(info.indexPath);
            IndexName.setU8(info.indexName);
            FileName.setU8(info.file.name);
            FileLocation.setU8(info.file.location);
            FileType.setU8(info.file.type);
            FileOpenFailMessage.setU8(info.file.openFailMessage);
            };

        DStringProperty IndexPath;
        DStringProperty IndexName;
        DStringProperty FileName;
        DStringProperty FileLocation;
        DStringProperty FileType;
        DStringProperty FileOpenFailMessage;
    };

#ifndef THREAD_PRIORITY_NORMAL
#	define THREAD_PRIORITY_NORMAL 0
#endif

// Spawn a job in a separate thread
class DJobThread {
    public:
        DJobThread(DJobBase& aJob);
        ~DJobThread();
        // Start with default priority,  The thread is started with _beginthreadex.
        void start();
        // Start with specified priority.  The thread is started with _beginthreadex,
        // so it must be ended with _endthreadex
#ifdef _WIN32        
        bool start(int priority);
#endif        
        void abort(int type = dtsAbort);
        int done() {
            return bDone;
            }
        int running() {
            return bRunning;
            }
        void terminate();
#ifdef _WIN32
		HANDLE getThread() {
			return hThread;
			}
#else
		pthread_t getThread() {
			return thread.getThread();
			}

#endif
    protected:
        long validId;
        bool bAbort;
        bool bDone;
        bool bRunning;

#ifdef _WIN32
		HANDLE hThread;
        static unsigned __stdcall startCB2(void *pData);
        void closeHandle();
#else
        DThread thread;
        void closeHandle() { } 
#endif
        DJobBase& job;
#ifdef __UNIX__
        static void *startCB(void *pData);
#else
        static void startCB(void *pData);
#endif
        static DJobThread *SafeCast(void *pData);
        };

// 'this' used in base member initializer list
//#pragma warning(disable:4355)

template <class T>
class DJobInThread : public T  {
    public:
        DJobInThread() :
            m_thread(*this)
            { }
        ~DJobInThread() { }
        void ExecuteInThread() {
            // A job executing in a thread must not access the MFC message pump
            this->SuppressMessagePump();
            m_thread.start();
            }
        void AbortThread(int type = dtsAbort) {
            m_thread.abort(type);
            }
        int IsThreadDone() {
            return m_thread.done();
            }
        int IsThreadRunning() {
            return m_thread.running();
            }
        void TerminateThread() {
            m_thread.terminate();
            }
    protected:
        DJobThread m_thread;
        };

class DCrashHandlerInfo : public dtsCrashHandlerInfo {
	public:
		DStringProperty LogName;
		DStringProperty MiniDumpName;

		DCrashHandlerInfo();

		DCrashHandlerInfo& operator=(const dtsCrashHandlerInfo& other);
		void copy(const dtsCrashHandlerInfo& other);
	};

#ifdef USE_DTSEARCH_NAMESPACE
}  // dtSearch namespace
#endif

#endif
// end-internal
