/*
  dtsviewr.h

  Defines interface for dtSearch external file viewers

*/





#ifndef DTSVIEWR_H
    #define DTSVIEWR_H


#ifndef DTSEARCH_H
    #include <dtsearch.h>
#endif

#ifdef USE_DTSEARCH_NAMESPACE
#	define dtSearchNamespace dtSearch::
#else
#   define dtSearchNamespace
#endif

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



// Highest confidence level for file format recognition
const int dtsMaxConfidence = 100;

// For string type names in dtsFileInfo struct and dtsViewerInfo
const char maxTypeNameLen = 40;

// begin-internal
// OBSOLETE:
//  dtsBookMarks are used to build a map of an indexed file showing how to
//  quickly jump to the 1000th word, the 2000th word, etc., so that
//  hits can be located quickly.
//  The ptr and data[] members can contain whatever information a
//  viewer finds useful in jumping into the middle of a file.
//  For example, a WordPerfect viewer would store the file position in
//  ptr and would store tab settings and margins in data.
struct dtsBookMark {
    long ptr;
    char data[16];
    char hasValue;

    dtsBookMark();
    dtsBookMark& copy(const dtsBookMark& other);
    void clear();
    };
// end-internal

/* Summary
   Buffer to receive text extracted by a file parser.
   Remarks
   When a viewer extracts text from a file, it returns the text in a dtsTextBlock,
   which is simply a buffer to store the text.                                     */
struct dtsTextBlock {
	// Amount of text returned
    long textLen;
    // Size of the buffer pointed to by buf
    long bufferSize;
    // Buffer to receive text
    char *buf;
    // Obsolete
    dtsBookMark bookMark;
    // Portion of the input that has been read (0 = none, 1000 = all)
    unsigned long fractionRead;

    dtsTextBlock();
    void copy(const dtsTextBlock& other);
    void clear() {
		textLen = 0;
		buf[0] = 0;
		}
	ptrdiff_t append(const char *s, ptrdiff_t bytes = -1);
    };

/* Summary
   Abstract interface to any source of document data.
   Remarks
   The dtSearch Engine API uses dtsInputStream in two places: (1) the external file
   parser API, which lets you add a file parser to the dtSearch indexing engine, and
   (2) the dtsDataSource API, which lets you index text data from any source.
   * dtsInputStream in the external file parser API *
   Your file parser receives a dtsInputStream as the source of its input and is
   responsible for converting the data in the file into blocks of indexable text.
   The input could come from a file handle, a memory buffer, or any other object.
   The seek() and read() function pointers in the dtsInputStream provide a generic
   way to read data from the file your parser is processing. Input may come from a
   document extracted from a container rather than from a disk file, so parsers
   should not assume that the filename element refers to an existing file on disk.

   dtsInputStream may be extended in the future to provide additional flexibility.
   To avoid source-level dependencies on dtsInputStream, file parsers should use a
   dtsInputStreamReader to access a dtsInputStream.
   * dtsInputStream in the dtsDataSource API *
   In the dtsDataSource API, your data source creates dtsInputStream objects and
   \returns them to the dtSearch engine to be indexed. Thus, instead of using the
   read() and seek() pointers, you would implement them. After dtSearch is done
   indexing a dtsInputStream, dtSearch will call the release() function to request
   that the calling application dispose of the object. Your implementation can
   delete the underlying object in response to that call.                            */
struct dtsInputStream {
	// Name of the document.  The name does not have to correspond to a disk file,
	// but it must have the form of a valid Win32 file name.
    const char *filename;
    // If this stream provides access to a temporary file on disk, the filename of the temporary file.
    // This is used in situations where a file parser requires a disk file.  dtSearch internal file parsers
    // do not require disk files.
    const char *tempname;
    // Size of the document (obsolete -- use setSize() to set the size and getSize() to obtain the size).
    long size;
    // Modification date of the document.
    dtsFileDate modified;
    // Creation date of the document
    dtsFileDate created;

	// Data to pass to seek and read function pointers
    void *pData;
    // Pointer to the function used to change the seek pointer for the input. 
    void (*seek)(void *pData, long where);
    // Pointer to the function used to read data from the input. Read returns the number of bytes read, or 0 if no data could be read
    // To indicate an I/O error that should force processing of the input file to halt, return a value of less than -10,000.
    long (*read)(void *pData, void *dest, long len);
    /*
    Pointer to a function that will destroy the dtsInputStream. This function
	pointer is provided for use in the dtsDataSource API. External file parsers
	must not call release() on a dtsInputStream passed to them.
	*/
    void (*release)(void *pData);
	// A user-friendly alternative filename for the document.
    const char *displayName;
    /* Summary
       Null-delimited string set containing pairs of field names and values to be
       associated with this document in the index.
       Remarks
       \Example:
       <code>
           "SampleField\\0This is the text of SampleField\\0\\0".
       </code>
       Field names can include nesting, using / to separate field name elements.
       \Example:
       <code>
           "Meta/Subject\\0 This is the subject\\0Meta/Author\\0 This is the author\\0\\0"
       </code>
       In this example, you could search across both fields by searching for "Meta
       contains (something)", or you could search for "Author contains (something)", or
       you could search for "Meta/Author contains (something)" to distinguish this
       \Author field from any other Author fields that might be present in the document.

       Add a * in front of a field name to prevent the field name from being indexed as
       part of the field. Example:
       <code>
           "*SampleField\\0 This is the text of SampleField\\0\\0".
       </code>
       Add two * in front of a field name to designate the field as a hidden stored
       field. Example:
       <code>
           "**SampleField\\0 This is the text of SampleField\\0\\0"
       </code>
       Hidden stored fields are automatically stored in the index as document
       properties, and any text in a hidden stored field is not searchable. After a
       search, the values of any hidden stored fields will be returned in
       dtsSearchResultsItem.userFields for each item.

       An application using the dtsDataSource API to generate data to be indexed can use
       fields to add field data to documents as they are being indexed, without
       modifying the original documents.

       External file parsers should ignore the contents of fields.                         */
    const char *fields;
    /* The type id of the file parser that should be used with this input
	stream.  If typeId is 0, the file format will be detected automatically.
	typeId should only be set to a non-zero value in situations in which it is
	necessary to override the standard file type detection. */
    long typeId;

    // begin-internal
    long structSize;
    const char *openErrorMessage;
    long openError;
    int (*openStream)(void *pData, const char *streamName, dtsInputStream& streamSource);
    long flags;
    const char *authInfo;
    int (*getFractionRead)(void *pData, int scale);
    int depth;
    // end-internal

    /* Summary
       Size of the document */
    __int64 size64;
    // Pointer to the function used to change the seek pointer for the input.
    void (*seek64)(void *pData, __int64 where);

	// If item was extracted from a container, the type id of the container
	int parentTypeId;

    // begin-internal

    // Build number of dtSearch that was used to index this file (passed through
    // from dtsFileConvertJob2.indexedBy)
    long indexedBy; 
    long reserved[9];
    // end-internal

    dtsInputStream();
    // Copy a dtsInputStream (shallow copy)
    dtsInputStream& copy(const dtsInputStream& other);
    // Clear all data in dtsInputStream
    void clear();
    void setSize(__int64 s);

    int isValid();
    int isEmpty() {
		return (read == 0);
		}
	__int64 getSize() const {
		if (size64 > 0)
			return size64;
		else
			return size;
		}
    };

#define dtsInputSource dtsInputStream
#define dtsInputSourceReader dtsInputStreamReader


//begin-internal
// Reserved for internal use
struct dtsDataSourceDocExtractor {
    dtsDataSourceDocExtractor();
    long structSize;
    void *pData;
    int (*hasDataType)(void *pData, const char *name);
    int (*getDocByName)(void *pData, const char *name, dtsInputStream& dest, dtsErrorInfo *pErrorHandler);
    int (*getDocByName2)(void *pData, const char *name, const char *userFields, dtsInputStream& dest, dtsErrorInfo *pErrorHandler);
    long reserved[4];
    };
//end-internal


//  Used to package basic information about a file that a file parser must provide.
struct dtsFileInfo {
	// Name of the file
    char filename[FileNameLen];
    // User-friendly name for the file
    char displayName[FileNameLen];
    // Size of the file (superseded by size64)
    long size;
    // Modification date
    dtsFileDate modified;
    // Created date
    dtsFileDate created;
    // Up to 80 characters of text; generally the first line of text in the file
    char title[FileNameLen];

    // The typeId of the dtsViewerInfo for this file parser
    unsigned short typeId;
    // The typeId of the dtsViewerInfo for this file parser
    unsigned short parserId;
    // Name of this file type, for display purposes
    char typeName[maxTypeNameLen];
    // FileInfoFlags value describing properties of this item
    long flags;

	// Size of the dtsFileInfo structure (the constructor will initialize this)
    long structSize;
    // Size of the file (64-bit)
    __int64 size64;

	// begin-internal
    // reserved -- must be initialized to 0
    long additionalDataLen;
    const void *additionalData;


    dtsFileInfo();
    dtsFileInfo& copy(const dtsFileInfo& other);
    void clear();
	void setSize(__int64 s) {
		size64 = s;
		if (s > 0x7fffffff)
			size = 0;
		else
			size = (long) s;
		}
	__int64 getSize() const {
		if (size64)
			return size64;
		else
			return size;
		}
	void setTypeId(int aTypeId, const char *aTypeName = 0);
    // end-internal
    };

// Values for dtsDataSourceFileInfo.accessResult
enum DataSourceAccessResult {
	// Item accessed successfully
	dtsArOK			  = 88,
	// Item not found
	dtsArNotFound 	  = 2,
	// Item could not be accessed due to a permissions problem
	dtsArAccessDenied   = 5,
	// Unable to connect to the server to access this item
	dtsArConnectFailed  = 500,
	// Error accessing item
	dtsArOtherError     = 999
	};

// To be returned from dtsDataSource.getNextDocInfo or dtsDataSource.getFileInfoByName from a data source
struct dtsDataSourceFileInfo {
	long structSize;
	// Name of the item
	const char *filename;
	/* Size of the document in bytes.  When a dtsDataSourceFileInfo is returned
	from getNextDocInfo() or getDocInfoByName(), size can be returned as zero.
	(This can make getNextDocInfo or getDocInfoByName more efficient in
	situations where determining the size of the document is a time-consuming
	operation.)  */
	__int64 size;
	/* Error code, if any, returned from the data source.  The indexer will pass
	this error through to the code that executed the IndexJob through the
	indexing status notification callback. */
	long errorCode;
	// Error message, if any, returned from the data source
	const char *errorMessage;
	// Modification date of the item (UTC)
	dtsFileDate modified;
	// Creation date of the item (UTC)
	dtsFileDate created;
	/* DataSourceAccessResult value indicating whether the item could be accessed.
	When the indexer is checking for documents that should be removed from the
	index because they no longer exist, only documents that are returned with
	dtsArNotFound are removed.
	*/
	long accessResult;
	long reserved[8];

	dtsDataSourceFileInfo();
	dtsDataSourceFileInfo(const dtsDataSourceFileInfo& other);
	void copy(const dtsDataSourceFileInfo& other);
	void clear();
	};

/* Summary
   Used by dtsIndexJob to index non-file data that is passed to the indexer through
   callbacks.
   Remarks
   The dtsIndexJob structure provides two ways specify the data to index: by files
   (the toAdd member) and by data source (the dataSourceToIndex member). Most
   commonly, the text exists in disk files, in which case you would specify the
   files to be indexed using the toAdd member, which provides ways to specify the
   directories and files you want to index. In some situations, however, the text to
   be indexed may not be readily available as disk files. For example, the text may
   exist as records in a remote SQL database. You could copy the text from the
   database to local disk files and index the local disk files, but the
   dtsDataSource API provides a more direct and efficient solution. To supply this
   text to the dtSearch indexing engine, you create an object that accesses the text
   and then attach a dtsDataSource describing the object as the dataSourceToIndex
   member of a dtsIndexJob.
   * Basic Data Source Implementation *
   A dtsDataSource is a structure that provides access to any source of text data
   divided into logical documents. It consists of a set of function pointers for
   functions to retrieve documents and to iterate over the data to be indexed. The
   simplest possible data source would implement two function pointers: rewind, to
   initialize the data source, and getNextDoc, to get the next document to index. A
   dtsDataSource also contains a pData pointer that will be passed to the rewind()
   and getNextDoc() functions. To create a data source, you would do the following:
     1. Create an object that access the data source.
     2. Set the pData member of a dtsDataSource to point to this object.
     3. Make callback functions that convert the pData pointer to the object type
        and then call the appropriate member function, and pass those callback functions
        to the dtSearch engine in the dtsDataSource.
   Your data source object (the CMyDataSource object in the example below) will
   return logical documents using another structure, a dtsInputStream. For each
   document, dtsInputStream provides its filename (any legal Windows filename),
   creation and modification dates, and size as it will be recorded in the index.
   Like dtsDataSource, dtsInputStream relies on callback functions -- seek() and
   read() -- to provide access to the data to be indexed.

   dtSearch indexes the dtsInputStream objects created by the dtsDataSource by
   calling their seek() and read() members to get text data. After a dtsInputStream
   has been indexed, dtSearch will destroy it through a call to its release()
   member. dtSearch does not delete the dtsDataSource object so the caller is
   responsible for disposing of it when release() is called.

   To avoid requiring an unnecessary initial pass through the input data before
   indexing, dtsDataSource knows nothing about the total size of the data to be
   indexed or the number of documents to be indexed. As a result, the dtSearch
   Engine will not be able to report the percentage completion of an indexing job
   involving a dtsDataSource.

   For an example demonstrating use of the dtsDataSource API, see dsource.cpp.
   * Efficient Incremental Updates *
   Once a data source has been indexed, subsequent updates will be faster if the
   data source can quickly identify which documents have been modified since the
   last update, and which documents no longer exist. To make incremental updates
   more efficient, a data source can implement three additional functions:
   getNextDocInfo, getCurrentDoc, and getDocInfoByName.

   getNextDocInfo and getCurrentDoc allow dtSearch to separate getNextDoc into two
   calls, one to get the document properties, and a second to get the document
   itself. The advantage of doing this is that dtSearch can skip the second step for
   documents that have already been indexed. getNextDocInfo returns information
   about the next document, most importantly the name and modification date.
   dtSearch uses this to determine if the document needs to be indexed, by comparing
   the name and modification date with what is already in the index. If the document
   is not in the index, or an older version is in the index, dtSearch will request
   the document contents in a call to getCurrentDoc. If the document does not have
   to be indexed, dtSearch will skip the document and call getNextDocInfo again for
   the next document's name and modification date. When a getNextDocInfo pointer is
   provided, getNextDoc will never be called, and dtSearch will rely only on
   getNextDocInfo and getCurrentDoc.

   getDocInfoByName lets dtSearch check whether a particular document still exists,
   for purposes of implementing the "Remove Deleted" step in an index update.
   Example
   <code>
   // Assume that CMyDataSource is an object with rewind()
   // and getNextDoc() members that access text to be indexed.

   // The dtSearch engine will call the rewind function to initialize
   // the data source. Just pass the call through to the object.
   static int rewindCallback(void *pData)
   {   CMyDataSource *s = (CMyDataSource *) pData;
       return s-\>rewind();
   }

   // The dtSearch Engine will call the getNextDoc to get documents
   // to be indexed. Again, just pass the call through to the
   // CMyDataSource object.
   static int getNextDocCallback(void *pData, dtsInputStream& doc)
   {   CMyDataSource *s = (CMyDataSource *) pData;
       return s-\>getNextDoc(doc);
   }

   void BuildIndex()
   {   dtsIndexJob indexJob;
       ... set up the index path, name, etc. ...
       // Attach data source to be indexed
       CMyDataSource myData;
       dtsDataSource ds;
       ds.pData = &myData;
       ds.rewind = rewindCallback;
       ds.getNextDoc = getNextDocCallback;
       indexJob.dataSourceToIndex = &ds;

       // Build the index
       dtssDoIndexJob(indexJob, result);

       ...
   </code>                                                                               */

struct dtsDataSource {
    dtsDataSource();
    // Copy a dtsDataSource
    void copy(const dtsDataSource& other);
    long structSize;
    /*  Pointer to user data (generally used to hold the "this" pointer for your
	 object that implements the dtsDataSource). The rewind() and getNextDoc
	 functions will receive the pData pointer as the first parameter on each
	 call. */
    void *pData;
    /* Pointer to a function that will initialize the dtsDataSource so the next
       getNextDoc call will return the first document. The rewind function should return
       0 if the data source was succesfully initialized, or -1 if initialization failed.
       If rewind returns -1, nothing will be indexed from the data source.
       Parameters
       pData :  The value of dtsDataSource.pData

       Returns
       0 if the data source was succesfully initialized, or -1 if initialization failed. */
    int (*rewind)(void *pData);
    /* Pointer to a function that will return the next document in
       the data source as a dtsInputStream.
       Parameters
       pData :  The value of dtsDataSource.pData
       dest :   Structure to receive the dtsInputStream for the
                document
       Returns
       0 if a document was returned, or -1 if there are no more
       documents to return.
                                                                   */
    int (*getNextDoc)(void *pData, dtsInputStream& dest);

    // begin-internal
    void (*release)(void *pData);
    const char *name;
    dtsErrorInfo *pErrorHandler;
    // end-internal

    // Provides information on the most recently-indexed file
    dtsIndexFileInfo *pFileInfo;

	/* Pointer to a function that will return information about a the next document
	in the data source.

		Parameters:
			pData:       The value of dtsDataSource.pData
			fileInfo:    Structure to receive the properties of the document
		Returns: 		 0 on success, or -1 if there are no more documents to return

	Remarks:

	The data source implementation should ensure that string
	pointers in the dtsDataSourceFileInfo remain valid until the next
	getNextDocInfo call.  Following a getNextDocInfo call, the dtSearch Engine
	may either call getCurrentDoc to get the document contents, or it may call
	getNextDocInfo again to get the next document.
	*/
	int (*getNextDocInfo)(void *pData, dtsDataSourceFileInfo& fileInfo);

    /* Get the document whose properties were just returned by
       getNextDocInfo or getDocInfoByName
       Parameters
       pData :  The value of dtsDataSource.pData
       dest :   Structure to receive the dtsInputStream for the
                document
       Returns
       0 on success, or -1 if the document could not be accessed. */
    int (*getCurrentDoc)(void *pData, dtsInputStream& dest);

	/* Pointer to a function that will return information about a
	   named document in the data source.
	   Parameters
	   pData :       The value of dtsDataSource.pData
	   docName :     The name of the document in the data source
	   userFields :  Null\-delimited string set containing any stored
	                 fields associated with this document.
	   Returns
	   0 on success, or -1 if the document could not be accessed.

	   If the document could not be returned, the specific error
	   should be provided in dtsDataSourceFileInfo.accessResult.
	   Remarks
	   The data source implementation should ensure that string
	   pointers in the dtsDataSourceFileInfo remain valid until the
	   next getNextDocInfo call. Following a getNextDocInfo call,
	   the dtSearch Engine may either call getCurrentDoc to get the
	   document contents, or it may call getNextDocInfo again to get
	   the next document.                                             */
    int (*getDocInfoByName)(void *pData, const char *docName, const char *userFields, dtsDataSourceFileInfo& fi);

    long reserved;
    };



/* Stores information about a document that is contained in a container. */
struct dtsContainerItemInfo : public dtsFileInfo {
	// A string that can be used in a getInfoByName call to access this item in the container.
    char nameInContainer[FileNameLen];

    // Deprecated: Use indexInContainer64.
    // An integer that can be used in a getInfoByIndex call to access this item in the container. The integers used as indexes do not have to be sequential.
    //
    //
    long indexInContainer;
    
    long reserved3;
    
    // Size of the item inside the container (for example, the compressed size of an item in a ZIP)
    __int64 sizeInContainer;

	// Document properties supplied by the container (for example, the subject of an email)    
    const char *containerFields;
    
    // An integer that can be used in a getInfoByIndex call to access this item in the container. 
    // The integers used as indexes do not have to be sequential.
    __int64 indexInContainer64;
    
	void setIndexInContainer(__int64 v) {
		if (structSize >= sizeof(dtsContainerItemInfo))
			indexInContainer64 = v;
		if (v < 0x7fffffffL)
			indexInContainer = (long) v;
		else
			indexInContainer = 0;
		}
	__int64 getIndexInContainer() const {
		if (structSize < sizeof(dtsContainerItemInfo))
			return indexInContainer;
			
		if (indexInContainer64 != 0)
			return indexInContainer64;
		else
			return indexInContainer;
		}
    dtsContainerItemInfo();
    dtsContainerItemInfo& copy(const dtsContainerItemInfo& other);
    void clear();
    };


/* Provides a layer of abstraction around a dtsInputStream.
   Remarks
   Although dtsInputStream can be accessed directly, future enhancements to
   dtsInputStream may make code relying on the existing structure layout obsolete.
   Therefore, the recommended way to access a dtsInputStream is to attach it to a
   dtsInputStreamReader and then use the dtsInputStreamReader.                     */

class dtsInputStreamReader {
    public:
    	// Constructor.  After creating a dtsInputStreamReader, call attach() to attach
    	// a dtsInputStream
        dtsInputStreamReader();
    	// Constructor.
    	// Parameters:
    	//   aStream:   The input stream to attach
        dtsInputStreamReader(dtsInputStream& aStream);
        virtual ~dtsInputStreamReader();
        // Attach an input stream.  dtsInputStreamReader never allocates or deletes anything,
        // so the attachment process just involves wrapping the function pointers and providing
        // access to data members in the dtsInputStream.
        // Parameters:
        //   aStream:  The input stream to attach
        virtual void attach(dtsInputStream& aStream);

        dtsInputStream& getSource();
        // Change the seek pointer to pos
        // Parameters:
        //    pos:   New seek position
        void seek(__int64 pos);
        // Read data from the dtsInputStream
        // Parameters:
        //    dest:    Buffer to receive data
        //    bytes:   Number of bytes to read
        // Returns:
        //    Number of bytes read
        long read(void *dest, long bytes);

		ptrdiff_t read2(void *data, ptrdiff_t bytes) {
			return read(data, static_cast<long>(bytes));
			}
        // Returns the size of the dtsInputStream
        __int64 getSize() const;
        // Returns the size of the dtsInputStream as a 32-bit integer
        int getSize32() const;
        // Returns the name of the dtsInputStream
        const char *getName() const;
        // Returns the modified date of the dtsInputStream
        dtsFileDate& getModified();
        // Returns the created date of the dtsInputStream
        dtsFileDate& getCreated() ;
        // Returns the current seek position of the input stream
        __int64 tell() const;

        int getFractionRead(int scale = 1000);

        virtual void getFileInfo(dtsFileInfo& fi);
    protected:
        dtsInputStream source;
        __int64 pos;
        };


/* Summary
   Provides information about how to recognize a particular file type.
   Remarks
   dtsRecognitionSignature is supplied as part of the dtsViewerInfo that a viewer
   uses to register itself.                                                       */
struct dtsRecognitionSignature {
    long offset;           // Position in the file where the pattern would be
    short length;          // Length of the pattern
    const char *text;            // Text to compare against the text in the file
    char convertToUpper;   // Convert text in file to upper case before matching
    char patternMatch;     // Signature contains wildcards * and/or ?
    short confidence;      // Ranges from 1 to dtsMaxConfidence

    dtsRecognitionSignature();
    dtsRecognitionSignature& copy(const dtsRecognitionSignature& other);
    void clear();
    };

typedef void *dtsViewerHandle;

// Values for dtsViewerInfo.flags
enum ViewerInfoFlags {
	// File type is a container (like ZIP
	viIsContainer 	= 0x01,
	// Obsolete
	viIsHypertext 	= 0x02,
	// Text is returned using the Ansi character set
	viAnsiCharSet 	= 0x04,
	// This file parser cannot accept input from a dtsInputStream and requires a disk file
	viNeedDiskFile 	= 0x08,
	// File parser can return RTF.
	// A file parser may indicate multiple possible output formats, in which case
	// dtsMakeViewerParams.outputFormat will specify the format requested by the caller.
	viReturnsRtf 	= 0x10,
	// Text is returned using the UTF-8 encoding
	viUtf8CharSet 	= 0x20,
	// File parser can return HTML.
	// A file parser may indicate multiple possible output formats, in which case
	// dtsMakeViewerParams.outputFormat will specify the format requested by the caller.
	viReturnsHtml 	= 0x40,
	
	// For container file parsers, indicates that the parser uses 64-bit values for
	// indexInContainer.  
	viIs64bit = 0x100,

	// Will be set only in beta builds of the dtSearch Engine for new file parsers that are
	// being beta tested
	viIsBeta			= 0x800000L,

// begin-internal
	viReturnsUtf8F 	= 0x80,
	viIsInternal		= 0x000400L,
	viIsBinary		= 0x000800L,
	viSelectByTypeId = 0x001000L,
	viReserved1		 = 0x002000L,
	viReserved2 	 = 0x004000L,
	viReserved3 	= 0x008000L,
	viIsObsolete		= 0x400000L,
// end-internal
};

// begin-internal
class ImportBase;

// Enables external viewer to call Engine functions
struct dtsEngineFunctions {
    long structSize;
    void (*dtssDoIndexJob)(dtsIndexJob& job, short& result);
    void (*dtssMergeIndexes)(dtsIndexMergeJob& job);
    void (*dtssDoSearchJob)(dtsSearchJob& job, short& result);
    void (*dtssGetIndexInfo)(const char *indexPath, dtsIndexInfo& info,
        short& result);
    void (*dtssVersion)(short& majorVersion, short& minorVersion);
    void (*dtssVersionEx)(dtsVersionInfo& versionInfo);
    void (*dtssDoInit)(dtsInitInfo& initInfo, short& result);
    void (*dtssDoShutDown)(short& result);
    void (*dtssDoGetFileText)(char *toExtractName, char *toCreateName,
        short& result);
    void (*dtssDebugLog)(const char *fn);
    void (*dtssDebugLogEx)(const char *fn, long flags);
    void (*dtssAddToLog)(const char *msg);
    void (*dtssSetOptions)(dtsOptions& opts, short& result);
    void (*dtssGetOptions)(dtsOptions& opts, short& result);
    void (*dtssDeleteIndex)(char *indexPath, short& result);
    void (*dtssRunScript)(char *scriptName, short& result);
    void (*dtssMapHitsInFile)(dtsMapHitsJob& job, dtsSearchNotificationItem *pItem, short& result);
    void (*dtssConvertFile)(dtsFileConvertJob& info, short& result);
    void (*dtssGetIndexList)(short& count, char *buf, long bufSize,
            long *bufSizeNeeded);
    void (*dtssConvertFile2)(dtsFileConvertJob2& info, short& result);

    dtsSearchResultsHandle (*dtsSR_Alloc)(long maxCount, long flags);
    void (*dtsSR_Free)(dtsSearchResultsHandle h);
    dtsSearchResultsHandle (*dtsSR_Copy)(dtsSearchResultsHandle h);
    long (*dtsSR_GetDocInfo)(dtsSearchResultsHandle h, long iItem, dtsSearchResultsItem& item);
    void (*dtsSR_SetDocInfo)(dtsSearchResultsHandle h, long iItem, const dtsSearchResultsItem& item);
    void (*dtsSR_SetSortKey)(dtsSearchResultsHandle h, long iItem, const char *key);
    long (*dtsSR_GetDocInfoStrings)(dtsSearchResultsHandle h, long iItem, char *dest, long maxLen);
    void (*dtsSR_GetCounts)(dtsSearchResultsHandle h, long *pInfoCount, long *pTotalFileCount, long *pTotalHitCount);
    void (*dtsSR_GetSearchRequest)(dtsSearchResultsHandle h, char *dest, int maxLen);
    void (*dtsSR_Sort)(dtsSearchResultsHandle h, long flags, const char *userFieldName);
    void (*dtsSR_MakeReport)(dtsSearchResultsHandle h, struct dtsSearchReportJob& job);
    void (*dtsSR_MakePdfHighlightFile)(dtsSearchResultsHandle h, long iItem, const char *outputFilename, const char *tempFilename);
    dtsStringHandle (*dtsSR_MakePdfWebHighlightFile)(dtsSearchResultsHandle h, long iItem, const char *tempFilename);
    void (*dtsSR_ReadDocInfo)(dtsSearchResultsHandle h);
    long (*dtsSR_Serialize)(dtsSearchResultsHandle h, const char *fn, int flags);
    dtsStringHandle (*dtsSR_SerializeAsXml)(dtsSearchResultsHandle h, int iItem);
    void (*dtsSR_SerializeFromXml)(dtsSearchResultsHandle h, const char *s);
    void (*dtsSR_LogDocInfo)(dtsSearchResultsHandle h, long iItem, dtsSearchResultsItem *pItem);
    void (*dtsSR_UnionWith)(dtsSearchResultsHandle h, dtsSearchResultsHandle h2);
    void (*dtsSR_IntersectionOf)(dtsSearchResultsHandle h, dtsSearchResultsHandle a, dtsSearchResultsHandle b);
    int  (*dtsSR_UrlDecodeItem)(dtsSearchResultsHandle h, const char *s);
    int  (*dtsSR_UrlEncodeItem)(dtsSearchResultsHandle h, long iItem, char *dest, int maxLen);
    dtsStringHandle  (*dtsSR_UrlEncodeItem2)(dtsSearchResultsHandle h, int iItem, int maxLen, int flags);
    void (*dtsSR_GetSearchDate)(dtsSearchResultsHandle h, dtsFileDate& date);
    int (*dtsSR_AddItem)(dtsSearchResultsHandle h, const char *indexRetrievedFrom, long docId, long hitCount, long *hits);
    int (*dtsSR_AddItems)(dtsSearchResultsHandle h, const char *indexRetrievedFrom, long count, long *docIds);
    long (*dtsSR_GetSearchFlags)(dtsSearchResultsHandle h, long *pFlags, int *pFuzziness);
	void (*dtsSR_SetSelections)(dtsSearchResultsHandle hResults, long count, long *selections);
	dtsIntArrayHandle (*dtsSR_GetSelections)(dtsSearchResultsHandle hResults);
	void (*dtssGetOptions2)(dtsOptions2& dest, char *dataBuf, int dataBufSize, int *pDataSize);
	void (*dtssSetOptions2)(dtsOptions2& opts, short& resultFlag);
	void (*dtssVerifyIndex)(dtsIndexVerifyJob& job);
	void (*dtssConvertPath)(const char *p, char *d, int maxLen, long flags );
    void (*dtssConvertPath2)(dtsConvertPathRequest& request);
	int (*dtsSF_AddIndex)(dtsSearchFilterHandle hFilter, const char *indexPath);
	dtsSearchFilterHandle (*dtsSF_Alloc)();
	void (*dtsSF_Free)(dtsSearchFilterHandle hFilter);


    long (*dtssStringGetLength)(dtsStringHandle hStr);
    long (*dtssStringGetText)(dtsStringHandle hStr, char *buf, long bufferSize);
    void (*dtssStringDelete)(dtsStringHandle hStr);
    long (*dtssIntArrayGetLength)(dtsIntArrayHandle hArray);
    long (*dtssIntArrayGetItems)(dtsIntArrayHandle hArray, long *pDest, long maxItems);
    long (*dtssIntArrayGetItemRange)(dtsIntArrayHandle hArray, int nFirstItem, int nLastItem, long *pDest);
    void (*dtssIntArrayDelete)(dtsIntArrayHandle hArray);
    dtsStringHandle (*dtssGetSynonyms)(const char *word, long flags);
    void (*dtssListIndex)(dtsListIndexJob& job, short& resultFlag);



    void *reserved[65];

    dtsEngineFunctions();
    void makeFromHandle(void * dllInstance);
    void copy(const dtsEngineFunctions& other);
    };


const long dtsvsPdfUseTitleAsName = 0x0001;

// Obsolete -- dtsMakeViewerParams provides access to option settings formerly provided through dtsViewerSetupInfo
struct dtsViewerSetupInfo {
    void (*logMessage)(const char *str);  // adds a string to the dtSearch debug log
    long lzwEnableCode_unused;
    dtsEngineFunctions *engineFunctions;
    long flags;
    long engineBuild;
    long reserved[15];
    void setFlag(long f, int fOn) {
        if (fOn)
            flags |= f;
        else
            flags &= (~f);
        }
    bool checkFlag(long f) {
        return (flags & f)? true : false;
        }

    // Reserved for internal use
    void *alphabet_unused;
    dtsViewerSetupInfo();
    };
// end-internal


/* Information passed to the makeViewer2 callback function in
   dtsViewerInfo                                             */
struct dtsMakeViewerParams {
    long structSize;
    // Input stream to parse
    dtsInputStream *input;
    // Error handler to report errors through
    dtsErrorInfo *errorHandler;
	// internal use only
    void *reserved_unused;
    // Internal use only.  If this viewer is being created to handle a FileConverter conversion,
    // convertFlags will have the value of any ConvertFlags passed in the FileConverter.
    long convertFlags;
    // Obsolete.  fieldFlags are now provided with the entire dtsOptions struct in options
    long fieldFlags;
    // dtsInputType specifying the requested output format for the parsed text.  This must be one of the
    // possible output formats specified by the ViewerInfo flags for this parser
    long outputFormat;
    // Option settings currently in effect (old structure format)
    dtsOptions *options;
    // Options for extraction of embedded objects during conversion
    dtsExtractionOptions *extractionOptions;

// begin-internal    
    void *pStatusUpdateCallback;
// end-internal
    // Option settings currently in effect
	dtsOptions2 *options2;
// begin-internal
	void *reserved_alphabet;
    void *pReserved[3];
// end-internal    
    dtsMakeViewerParams();
    };

// begin-internal
struct dtsHighlightDataRequest {
    dtsHighlightDataRequest();
    long structSize;
    dtsSearchResultsItem *resultsItem;
    char *dest;
    long maxLen;
    long dataSize;
    long reserved[4];
    };
// end-internal

/* Summary
   Register an external file parser. See <link File Parser API>   */
struct dtsViewerInfo {
	void setVersion(const char *s);
	void setName(const char *s);

    unsigned long reserved;
    // Name of the file format
    char name[maxTypeNameLen];
    /* Numeric identifier for the viewer. Internal viewers use the types listed
	in the dtsInputType enumeration. External viewers should use values starting
	at it_LastInternal */
    unsigned short typeId;
    // For diagnostic purposes only, a string identifying the version of the parser.
    char version[maxTypeNameLen];
	// A dtsRecognitionSignature that is used to determine whether a file belongs to this file parser.
    dtsRecognitionSignature signature;
    /* Used to determine whether a file belongs to this file parser. If
	non-NULL, this points to a block of memory containing a series of
	null-terminated filename filters such as "*.DOC", terminated by a
	double-null. */
    const char *filenamePattern;
    /* If the filename matches filenamePattern, identifies how confident the
	parser is that the file belongs to the parser. Ranges from 0 to
	dtsMaxConfidence. */
    long filenameConfidence;
    /* Size of the text blocks to pass to the readTextBlock function. (Note: The block
       size actually provided to readTextBlock may be different from the requested size,
       so readTextBlock implementations should check the size of the dtsTextBlock passed
       in and should not assume that the provided buffer is of the requested size.)      */
    unsigned long blockSize;

	//begin-internal
	// obsolete
    const char *pageName;
    const char *paraName;
	// end-internal

	/* If not NULL, will be called to determine whether the a file has the format
	   recognized by the parser. If possible, file parsers should use the
	   recognitionSignature to identify the file format and initialize recognize to
	   NULL.
	   Parameters
	   aStream :  The input stream to check
	   Returns
	   0 if the file format is not recognized, or a confidence level between 1 and
	   dtsMaxConfidence (100) if the file format is recognized                      */
    int (*recognize)(dtsInputStream& aStream);
    /* Create a file parser for a dtsInputStream.  Deprecated: Use dtsMakeViewer2
    Parameters:
    	aStream:  The input stream to parse
    Returns:
    	a dtsViewerHandle identifying the parser, or NULL if the file cannot be parsed
    */
    dtsViewerHandle (*makeViewer)(dtsInputStream& aStream);
    // Delete the object represented by a handle previously returned by makeViewer.
    void (*destroyViewer)(dtsViewerHandle handle);
    /* read a block of text from the input and store it in a dtsTextBlock.
    Parameters:
    	handle:    Handle identifying the file parser
    	block:     Buffer to receive parsed data
    Remarks:
    The text should be stored using the output format and encoding specified by the
   parser's ViewerInfoFlags and the outputFormat requested in the
   dtsMakeViewerParams. The size of the block read is up to the viewer. It must
   be less than the blockSize supplied in the viewer's dtsViewerInfo, since this
   is used to allocate the dtsTextBlock's buffer. When all of a file has been
   parsed, readTextBlock should return an empty text block (no text, zero
   length).

   If data is returned in a format such as RTF or HTML, the returned data must conform to
   the specification for that data.  For example, if the output format is it_RTF, then the
   data stored in the dtsTextBlock must be valid RTF.
   */
    void (*readTextBlock)(dtsViewerHandle handle, dtsTextBlock& block);
    /* Reposition the parser's input pointer to the start of the text block
	identified by the dtsBookMark, or any earlier position in the file.
	Most file parsers should just rewind the input to the beginning of the file.
	Parameters:
    	handle:    Handle identifying the file parser
		bookMark:  Location
	*/
    void (*gotoBookMark)(dtsViewerHandle handle, dtsBookMark& bookMark);
    /* Get basic information about the document associated with handle. All of
	this will be available in the dtsInputStream, but a viewer may wish to
	modify the information in some cases.

	Parameters:
    	handle:    Handle identifying the file parser
		info:	   Struct to receive the information
	*/
    void (*getFileInfo)(dtsViewerHandle handle, dtsFileInfo& info);

	/* ViewerInfoFlags describing this file parser */
    long flags;

	/* Pointer to function that will return the number of files in the container.
	   Remarks:
	   		Only needed for container file parsers.
	*/
    long (*getCount)(dtsViewerHandle handle);

    /*
       Pointer to function that will get information about the first item in the container.
       Returns: 0 on success, non-zero on failure (if container is empty).
    */
    int (*getFirst)(dtsViewerHandle handle, dtsContainerItemInfo& info);
    /* Pointer to function that will get information about the next item in the container.
       Returns: 0 on success, non-zero on failure.
    */
    int (*getNext)(dtsViewerHandle handle, dtsContainerItemInfo& info);
    /*
       Pointer to function that will get information about a named item in the container.
       Returns: 0 on success, non-zero on failure.
    */
    int (*getInfoByName)(dtsViewerHandle handle, const char *name,
        dtsContainerItemInfo& info);
    /*
       Pointer to function that will get information about an item in the container, identified by its numerical index.
       (See dtsContainerItemInfo.indexInContainer.)

       Returns: 0 on success, non-zero on failure.
       
       Parameters:
		handle:		Handle identifying the container
		index:      An integer identifying this item in the container.
		info:		Identifies the item to extract       	  
		
	   To support 64-bit containers without changing the function signature, the following
	   convention is used:  
	   (1) A container file parser that uses 64-bit values should set the flag viIs64bit in
	   its dtsViewerInfo.flags.
	   (2) For 64-bit bit containers, If index is 0 and info.getIndexInContainer() is non-zero, the 
	   container parser should use the 64-bit value returned by info.getIndexInContainer() 
	   to locate the item.
    */
    int (*getInfoByIndex)(dtsViewerHandle handle, long index,
        dtsContainerItemInfo& info);
	/*
	Pointer to function that will extract an item from the container to a memory buffer.

	Parameters:
		handle:		Handle identifying the container
		info:		Identifies the item to extract
		dest:		Buffer to receive the contents of the item
		bufLen:		Size of output buffer
	Returns:
		0 on success, non-zero on failure

	*/
    int (*extractToMem)(dtsViewerHandle handle, dtsContainerItemInfo& info,
        void *dest, long bufLen);

	/* Pointer to function that will extract an item from the container to a temporary file.

	Parameters:
		handle:		Handle identifying the container
		info:		Identifies the item to extract
		fileToCreate: Name of the temporay file to create
	Returns:
		0 on success, non-zero on failure
	*/
    int (*extractToFile)(dtsViewerHandle handle, dtsContainerItemInfo& info,
        const char *fileToCreate);
    /* True if this file parser implements extractToMem */
    char canExtractToMem;

    // begin-internal
    char disabled;
    char targetMark;
    int (*extractToStream)(dtsViewerHandle handle, dtsContainerItemInfo& info,
    	void *hStream, int outputFormat, int flags);
    void (*setupViewer)(dtsViewerSetupInfo& );
    // end-internal

    /* Pointer to a function that will create a file parser for a dtsInputStream, using information provided in
    a dtsMakeViewerParams struct.

    Parameters:
    	params:    Includes a pointer to the dtsInputStream, and information about the output format requested and the option settings in effect
    Returns:
    	a dtsViewerHandle identifying the parser, or NULL if the file cannot be parsed
    */
    dtsViewerHandle (*makeViewer2)(dtsMakeViewerParams& params);

    // begin-internal
    dtsOptions2 *options;
   	long parserId;
    char reserved2[44];

    dtsViewerInfo *next;
    // end-internal

    dtsViewerInfo();
    // Copy a dtsViewerInfo
    dtsViewerInfo& copy(const dtsViewerInfo& other);
    // Clear all data from a dtsViewerInfo
    void clear();
    };

inline int IsHiddenStoredField(const char *name) {
	return name && (name[0] == '*') && (name[1] == '*');
}

#define HiddenStoredFieldMark "**"



extern "C" {
	/*
	An external file parser should call this function once at startup to register itself with the dtSearch Engine.
	Parameters:
		info:    Description of the file parser
	*/
    void DLLFUNC dtsRegisterViewer(const dtsViewerInfo& info);
    }

//
//  The Engine will call this function in the viewer to pass in
//  setup information
//
extern "C" {
     void DllExport dtsSetupViewer(dtsViewerSetupInfo& info);
    }

/* Restore alignment */
#if defined(__BORLANDC__)
    #pragma option -a.
#elif defined(_MSC_VER)
    #if defined(_WIN32)
        #pragma pack(pop, dtSearchHeader)
    #else
        #pragma pack( )
    #endif
#endif



#endif

