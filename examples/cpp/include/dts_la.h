#ifndef DTS_LA_H
	#define DTS_LA_H

#ifndef DTSEARCH_H
	#include <dtsearch.h>
#endif

#ifndef DTSVIEWR_H
	#include <dtsviewr.h>
#endif

// Flags used in dtsLaWordInfo.flags
enum LanguageAnalyzerWordFlags {
	// Use the offset of the previous word for this word; this makes it possible to have
	// multiple words occupy the same position in the file (for synonyms or morphological
	// variations).
	dtsLaUsePreviousWordOffset = 1,
	// A language analyzer can use this flag to indicate that a block of text in the input
	// was not processed, and should be handled by dtSearch's internal word breaker.
	dtsLaBlockWasSkipped = 2	

	};

// API structures used by the dtSearch Engine

// Information about a single word returned by the analyzer
struct dtsLaWordInfo {
	dtsLaWordInfo() {
		offsetInInputBuffer = 0;
		lengthInInputBuffer = 0;
		offsetInOutputBuffer = 0;
		lengthInOutputBuffer = 0;
		flags = 0;
		}
	dtsLaWordInfo(const dtsLaWordInfo& other) {
		*this = other;
		}
	dtsLaWordInfo& operator=(const dtsLaWordInfo& other) {
		offsetInInputBuffer = other.offsetInInputBuffer;
		lengthInInputBuffer = other.lengthInInputBuffer;
		offsetInOutputBuffer = other.offsetInOutputBuffer;
		lengthInOutputBuffer = other.lengthInOutputBuffer;
		flags = other.flags;
		return *this;
		}
	// Character offset of the start of this word in the input buffer
	long offsetInInputBuffer;
	// Number of characters in this word in the input buffer (including any internal punctuation)
	long lengthInInputBuffer;
	// Character offset of the start of this word in the output buffer
	long offsetInOutputBuffer;
	// Number of characters in this word in the output buffer
	long lengthInOutputBuffer;

	// LanguageAnalyzerWordFlags value specifying options that apply to this word.
	long flags;
	};

// Used in the flags member of dtsLaJob to provide information about the text to be processed
enum LanguageAnalyzerJobFlags {
	// Summary
	// Indicates that the input data is a search request.
	// This occurs when dtsSearchApplyLanguageAnalyzer was set in a search, and the language analyzer
	// is being called to process the search request.
	dtsLaInputIsSearchRequest = 0x0001,
	// Indicates that the input data is from a document that is being indexed.
	dtsLaInputIsDocumentData =  0x0002,
	// Input data is the first block of data in a document
	dtsLaInputIsFirstBlockInDocument = 0x0004,
	// Summary
	// Indicates that the input data is a single word or phrase from a search request.
	// This occurs when dtsSearchLanguageAnalyzerSynonyms was set in a search, and the language analyzer
	// is being called to process each term in the search request.
	dtsLaInputIsSearchTerm = 0x0008
	};

/* Summary
   Passed to the language analyzer with a series of blocks of text to analyze
   Remarks
   When the pAnalyze function in a language analyzer is called with a dtsLaJob to
   analyze, the language analyzer first decides whether it will process the text or
   not. If pAnalyze returns false, dtSearch will apply its internal word breaker to
   the text.
   
   If pAnalyze returns true, then dtSearch will use information in the outputBuffer
   and wordTable to determine where word breaks occur in the input text, and what
   text should be indexed for each word.
   
   In each wordTable entry, the offsetInInputBuffer and lengthInInputBuffer specify
   the range of text in the input buffer that is associated with a word. The
   \outputBuffer for the dtsLaJob contains the text as modified by the language
   analyzer, and the offsetInOutputBuffer and lengthInOutputBuffer for each
   wordTable entry specify the word to index at each word position.
   
   Multiple words can be generated in the output for a single word in the input. To
   do this, set the flag dtsLaUsePreviousWordOffset in the flags member of the
   wordTable entry for a word.
   
   Words in the input cannot overlap, and cannot be in different order from the
   \input. dtSearch assumes that the values of offsetInInputBuffer will be in
   increasing order, and the text ranges specified by the offsetInInputBuffer and
   lengthInInputBuffer will not overlap.
   
   For example, suppose the input consists of the following:
   <code>
   abc123def456
   </code>
   The language analyzer could partition the text as follows:
   <code>
   abc, 123, def, 456
   </code>
   Additionally, the language analyzer could return multiple words for each word
   position. For example, the "def" text in the input could be associated with three
   different words in the output, "def", "def2", and "def3", all at word offset 3.
   The resulting word table would be as follows:
   <table>
   \offsetInInputBuffer   lengthInInputBuffer   Word in   Word in   flags
                                                 \input    \output   
   ---------------------  --------------------  --------  --------  ---------------------------
   0                      3                     abc       abc       0
   3                      3                     123       123       0
   6                      3                     def       def       0
   6                      3                     def       def2      dtsLaUsePreviousWordOffset
   6                      3                     def       def3      dtsLaUsePreviousWordOffset
   9                      3                     456       456       0
   </table>                                                                                     */
struct dtsLaJob {
	long structSize;
	// Text to be processed.
	const wchar_t *inputBuffer;
	// Length of the text to be processed, in characters
	long inputTextLength;
	// LanguageAnalyzerJobFlags values.
	long flags;
	/* Pointer to text after morphological analysis.  This pointer must remain valid until the next block of data is analyzed, or until the dtsLaJob is destroyed.
	*/
	const wchar_t *outputBuffer;
	// Length of the output text, in characters.
	long outputTextLength;
	/* Summary
	   Table of structures providing, for each word in the inputBuffer, the offset and
	   length in the inputBuffer, and the offset and length in the outputBuffer, of the
	   word.
	   Remarks
	   The wordTable tells dtSearch where word breaks occur in the input text and also
	   provides the searchable text of the words associated with each word in the input.
	                                                                                     */
	dtsLaWordInfo *wordTable;
	// Number of items in wordTable
	long wordCount;
	// Pointer to instance data allocated by the pInitializeJob() function, and to be released by the pDestroyJob function.
	void *pData;
	// Information about the document that is the source of the text, if the input is from a document
	dtsFileInfo *pFileInfo;
	
	// Search request punctuation characters to preserve in the output, when dtsLaInputIsSearchRequest is set
	// in flags
	const char *searchRequestPunct;
	
	// Summary
	// Location of the alphabet file to use for word breaking.
	//
	// Remarks
	// If the file was retrieved from an index search, use indexRetrievedFrom instead of alphabetLocation.
	const char *alphabetLocation;
	
	// Path to the index that this document was found in, if the document was retrieved in a search
	const char *indexRetrievedFrom;
	
	long reserved[2];

	// Constructor
	dtsLaJob() {
		clear();
		}
	// Initialize a dtsLaJob
	void clear() {
		memset(this, 0, sizeof(dtsLaJob));
		structSize = sizeof(dtsLaJob);
		}
	};

// Handle to an instance of a language analyzer.   See dtsLanguageAnalyzerInterface.
typedef void *dtsAnalyzerHandle;

/* Summary
   Provide information about an analyzer to the dtSearch Engine.
   Remarks
   If your language analyzer class is based on CLanguageAnalyzerBase, then calling
   CLanguageAnalyzerBase::makeInterface() will set up all of these function pointers
   except for pCreateAnalyzer                                                        */
struct dtsLanguageAnalyzerInterface {
	long structSize;

	// A name to identify this analyzer (i.e., "Japanese Morphological Analyzer")
	const char *name;

	/* Summary
	   Initialize analyzer
	   Remarks
	   This will be called once per process, and is where one-time inititialization,
	   such as loading dictionaries, should occur. If the analyzer is created and
	   initialized successfully, return a dtsAnalyzerHandle identifying the analyzer
	   (usually by casting the this pointer for the class object implementing the
	   analyzer). If the analyzer could not be initialized, return zero. The analyzer
	   will not be called to process text. Note: The dtSearch Engine will not be able to
	   destroy this object because it has no way to know when it is safe to do so. Therefore,
	   the calling application or the language analyzer DLL must release any resources 
	   acquired by creation of a language analyzer, usually on termination.
	   Returns
	   Handle to the analyzer, which can be used in calls to pInitializeJob, pAnalyzer, etc.
	                                                                                          */
	dtsAnalyzerHandle (*pCreateAnalyzer)();

	// Release resources acquired by pCreateAnalyzer
	// Parameters
	// hAnalyzer:  Handle to the analyzer, created by pCreateAnalyzer.
	// Remarks
	// See comments for pCreateAnalyzer.   The language analyzer should ensure 
	// release of any allocated resources on DLL unload, even if 
	// pDestroyAnalyzer is not called.
	void (*pDestroyAnalyzer)(dtsAnalyzerHandle hAnalyzer);

	/* \ \
	   Summary
	   Initialize structure for output data
	   Parameters
	   hAnalyzer:    Handle to the analyzer, created by pCreateAnalyzer
	   toInitialize:  The job to initialize
	   Returns
	   SUCCESS if the job was initialized successfully, or FAIL if an error occurred and the job should not be used to analyze text.
	   Remarks
	   Set up a dtsLaJob for one or more analyze() calls.
	   dtsLaJob includes a pData pointer that can be used to store a this pointer for an
	   object that will handle analysis of text.
	*/
	int (*pInitializeJob)(dtsAnalyzerHandle hAnalyzer, dtsLaJob& toInitialize);

	/* Summary
	   Analyze a block of data
	   Returns
	   TRUE if block of data was processed, or FALSE if no changes were made. When
	   pAnalyze() returns false, the default word breaking algorithm is used.
	   Parameters
	   hAnalyzer :  Handle to the analyzer, created by pCreateAnalyzer.
	   job :        Pointer to a dtsLaJob containing the text to analyze and output buffers
	                for the result.                                                         */
	bool (*pAnalyze)(dtsAnalyzerHandle hAnalyzer, dtsLaJob& job);

	/* \ \
	   Summary
	   Release any resources allocated during call to InitializeJob
	   Parameters
	   hAnalyzer:    Handle to the analyzer, created by pCreateAnalyzer
	   job:  		 The job to destroy
	 */
	void (*pDestroyJob)(dtsAnalyzerHandle hAnalyzer, dtsLaJob& toDestroy);

	// Constructor
	dtsLanguageAnalyzerInterface() {
		clear();
		}
	// Initialize a dtsLanguageAnalyzerInterface
	void clear() {
		pDestroyJob = 0;
		pAnalyze = 0;
		pInitializeJob = 0;
		pDestroyAnalyzer = 0;
		pCreateAnalyzer = 0;
		name = 0;
		structSize = sizeof(dtsLanguageAnalyzerInterface);
		}
	// Copy a dtsLanguageAnalyzerInterface (shallow copy)
	void copy(const dtsLanguageAnalyzerInterface& other) {
		size_t toCopy = (structSize > other.structSize? other.structSize : structSize);
		memmove(this, &other, toCopy);
		}
	int good() {
		return pDestroyJob && pAnalyze && pInitializeJob && pDestroyAnalyzer
			&& pCreateAnalyzer;
		}
	};

// begin-internal

///////////////////////////////////////////////////////////////
//
//  Classes for use by calling programs
//

#if defined(DARRAY_H) && defined(DSTRING_H)
#ifdef USE_DTSEARCH_NAMESPACE

// Namespace for C++ support classes.
// The namespace is conditionally included in header files if USE_DTSEARCH_NAMESPACE is defined
namespace dtSearch {

#endif
// Manages short-term buffers used in a single thread to process text
class CLanguageAnalyzerJobBase {
	public:
		CLanguageAnalyzerJobBase();
		virtual ~CLanguageAnalyzerJobBase() {
			m_id = 0;
			}
		virtual int initialize(dtsLaJob& job);
		virtual bool analyze(dtsLaJob& job);
		static CLanguageAnalyzerJobBase *safeCast(void *pData);
	protected:
		int m_id;
		DArrayAsVector<dtsLaWordInfo> m_wordTable;
		DWString m_outputBuf;

		void linkBuffers(dtsLaJob& job);
	};

// Implements the callback API
// Manages per-process resources, such as dictionaries
class CLanguageAnalyzerBase {
	public:
		CLanguageAnalyzerBase();
		virtual ~CLanguageAnalyzerBase() {
			m_id = 0;
			}
		static void makeInterface(dtsLanguageAnalyzerInterface& la);
		virtual void destroyJob(dtsLaJob& job);
		virtual int initializeJob(dtsLaJob& job);
		virtual bool analyze(dtsLaJob& job);

		// Override this to allocate and return a CLanguageAnalyzerJobBase-derived object
		virtual CLanguageAnalyzerJobBase *makeAnalyzerInstance()  = 0;

	protected:
		int m_id;
		static int initializeJobCB(dtsAnalyzerHandle hAnalyzer, dtsLaJob& job);
		static bool analyzeCB(dtsAnalyzerHandle hAnalyzer, dtsLaJob& job);
		static void destroyJobCB(dtsAnalyzerHandle hAnalyzer, dtsLaJob& job);
		static void destroyAnalyzerCB(dtsAnalyzerHandle hAnalyzer);
		static CLanguageAnalyzerBase *safeCast(dtsAnalyzerHandle hAnalyzer);
	};

#ifdef USE_DTSEARCH_NAMESPACE

	}

#endif	
	
	
#endif
// end-internal

#ifdef _WIN32
extern "C" {
	// Export from a DLL to have dtSearch Engine recognize a language analyzer
	__declspec(dllexport) BOOL GetLanguageAnalyzer(dtsLanguageAnalyzerInterface& la);
	}
	
#ifdef DTENGINE_IMPORTS

extern "C" {
	/* Summary
	   dtssGetWordBreaker provides a way to call the dtSearch Engine's internal word
	   breaker.
	   Remarks
	   dtssGetWordBreaker returns an implementation of the Language Analyzer interface
	   that uses the dtSearch Engine's internal word breaker. For sample code
	   demonstrating how to use dtssGetWordBreaker to apply the word breaker to blocks
	   \of text, see the WordBreak sample application.
	   
	   If an external language analyzer is in use, the interface returned from
	   dtssGetWordBreaker will call that language analyzer rather than using the
	   dtSearch Engine's internal word breaker. Therefore, the interface returned from
	   dtssGetWordBreaker should not be used within a language analyzer. Instead, if you
	   are implementing a language analyzer and want to let the default word breaker
	   handle a block of text, use dtsLaBlockWasSkipped to skip a segment of text, or
	   just return false from analyze() to skip all text passed in the call.             */
	__declspec(dllexport) void dtssGetWordBreaker(dtsLanguageAnalyzerInterface& la);
	}
#endif
	
#endif


#endif
