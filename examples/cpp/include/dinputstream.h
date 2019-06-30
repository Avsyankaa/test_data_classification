//////////////////////////////////////////////////////////////////////

#if !defined(DInputStream_H)
#   define DInputStream_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef DTSVIEWR_H
#   include <dtsviewr.h>
#endif
#ifndef DSTRING_H
#   include <dstring.h>
#endif
#ifndef DFILE_H
#   include <dfile.h>
#endif
#ifndef DStringProperty_H
#   include <dstringproperty.h>
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

#define DInputSourceBase DInputStreamBase
#define DMemoryInputSource DMemoryInputStream
#define DFileInputSource DFileInputStream


/* Summary
   Base class for implementing an input stream representing a document returned from
   a data source
   Remarks
   A dtsInputStream is an abstraction of a document with seek() and read() methods
   for accessing data and a dtsFileInfo with file description information
   (modification date, size, etc.)

   A dtsDataSource object returns dtsInputStream objects in response to the
   getNextDoc callback function.

   Derived classes override seek() and read() to provide access to the data
   associated with the document.

   The derived class DMemoryInputStream can create a dtsInputStream from data in a
   memory buffer.

   The derived class DFileInputStream can create a dtsInputStream from a disk file.
                                                                                     */
class DInputStreamBase {
    public:
    /*
    Summary:
    String set containing pairs of field names and values to be
	associated with a document.  These field-value pairs are passed to the indexer as
	dtsInputStream.fields.

	Remarks:

	Field names can include nesting, using / to separate field name elements.  Example:
	<code>
		"Meta/Subject", "This is the subject", "Meta/Author", "This is the author"
	</code>

	In this example, you could search across both fields by searching for "Meta
	contains (something)", or you could search for "Author contains
	(something)", or you could search for "Meta/Author contains (something)" to
	distinguish this Author field from any other Author fields that might be
	present in the document.

	Add a * in front of a field name to prevent the field name from being
	indexed as part of the field. Example:
	<code>
		"*SampleField", "This is the text of SampleField"
	</code>

	Add two * in front of a field name to designate the field as a hidden stored
	field.  Example:
	<code>
		"**SampleField", "This is the text of SampleField"
	</code>

	Hidden stored fields are automatically stored in the index as document
	properties, and any text in a hidden stored field is not searchable.   After
	a search, the values of any hidden stored fields will be returned in
	dtsSearchResultsItem.userFields for each item.

	An application using the dtsDataSource API to generate data to be indexed
	can use fields to add field data to documents as they are being indexed,
	without modifying the original documents.

	External file parsers should ignore the contents of fields.
    */
        DStringSetProperty Fields;

        // Construct a DInputStreamBase, initializing it with the file properties in info
        DInputStreamBase(dtsFileInfo& info);
        // Default constructor
        DInputStreamBase();
        virtual ~DInputStreamBase();
        // Assign file properties (filename, modification date, etc.) to an input stream
        // Parameters:
        //    info:    The properties to assign
        void setFileInfo(dtsFileInfo& info) {
            fileInfo.copy(info);
            }
		// If a document could not be opened, use setOpenFailInfo to report the error code and message.
		// Parameters:
		//     code:   A numeric error code, which can be one of the values in the ErrorCodes enumeration.
	    void setOpenFailInfo(int code, const char *msg);
        // Get file properties (filename, modification date, etc.) for an input stream
        // Parameters:
        //    info:    Struct to receive file properties
        void getFileInfo(dtsFileInfo& info) {
            info.copy(fileInfo);
            }
        // Returns the filename associated with the input stream
        const char *getName() {
			return fileInfo.filename;
			}
		// Assigns a filename to the input stream
		// Parameters:
		//    name:      The name to assign.  The name does not have to correspond to a real file, but it must have the form of a Win32 filename.
		void setName(const char *name) {
			if (!name)
				name = "";
			strCopy(fileInfo.filename, name, sizeof fileInfo.filename);
			}
		// Change the seek pointer for the input
		// Parameters:
		//     where:    New position for the seek pointer
        virtual void seek(DFilePosition where) = 0;
        // Returns the current position of the seek pointer for the input
        virtual DFilePosition tell() = 0;
        // Reads data from the input stream
        // Parameters:
        // 	   dest:       Buffer to receive data
        //     bytes:      Number of bytes to read
        // Returns:        Number of bytes read
        virtual long read(void *dest, long bytes) = 0;

        static void seekCB(void *pData, long where);
        static void seekCB64(void *pData, DFilePosition where);
        static long readCB(void *pData, void *data, long bytes);
        static int getFractionReadCB(void *pData, int scale);
        static void releaseCB(void *pData);
        static DInputStreamBase *safeCast(void *pData);
        static  DInputStreamBase *makeInputSource(dtsFileInfo& fileInfo,
            const char *docText, const DStringSet& docFields, int fDocIsFile);
        dtsInputStream *getInterface();
        virtual void makeInterface(dtsInputStream& dest);
        virtual int getFractionRead(int scale = 1000) {
			return ScaleValue64(tell(), fileInfo.getSize(), scale);
			}
		DFilePosition getSize() {
			return fileInfo.getSize();
			}
		void SuppressDeleteAfterIndexing() {
			m_bSuppressDeleteOnReleaseCB = true;
			}
    protected:
        long id;
        dtsFileInfo fileInfo;
        dtsInputStream inputSourceInterface;
        DString tempFile;
        int m_errorCode;
        DString m_errorText;
        int m_typeId;
        // Normally when releaseCB is called, the object is deleted.
        // Set this flag to suppress if the caller will manage the object lifetime.
        bool m_bSuppressDeleteOnReleaseCB;

    };
/* Summary
   \Input stream representing a document stored in memory.
   Remarks
   \Input streams are used in the dtsDataSource API.

   The contents of the input stream can be plain text or binary data. Binary data is
   parsed in the dtSearch Engine exactly as if it were a disk file, so a memory
   \input stream can be used with binary file formats such as Microsoft Word
   documents, PDF files, etc.
   See Also
   DFileInputStream, DDataSourceBase, dtsInputStream                                 */
class DMemoryInputStream : public DInputStreamBase {
    public:
    	/*  Create an input stream in memory from file properties and plain text
    		Parameters:
    			info:    Document properties (filename, modification date, etc.)
    			text:	 Text contents of the document, as a null-terminated single-byte string.
    	*/
        DMemoryInputStream(dtsFileInfo& info, const char *text);
        // Constructor for an empty input stream.   Use setTextData or setBinaryData
        // to store document contents for the input stream.
        DMemoryInputStream();
        virtual ~DMemoryInputStream();
        /*  Store plain text as the contents of this document
            Parameters:
          		text:	Text contents of the document, as a null-terminated single-byte string.
         */
        void setTextData(const char *text);
        /*  Set binary data, such as the contents of a Microsoft Word document or a PDF file,
            as the contents of this document.
            Parameters:
            	pData:	Binary data for this document
            	bytes:  Size of the data
            Remarks:
            	The data will be copied into an internal buffer managed by DMemoryInputStream
        */
        void setBinaryData(const char *pData, ptrdiff_t bytes);
        void setBinaryDataAsBuffer(void *pData, ptrdiff_t bytes);
		// Change the seek pointer for the input
		// Parameters:
		//     where:    New position for the seek pointer
        virtual void seek(DFilePosition where);
        // Returns the current position of the seek pointer for the input
        virtual DFilePosition tell() {
			return m_pos;
			}
        // Reads data from the input stream
        // Parameters:
        // 	   dest:       Buffer to receive data
        //     bytes:      Number of bytes to read
        // Returns:        Number of bytes read
        virtual long read(void *dest, long bytes);

        // Returns the size of the buffer DMemoryInputStream uses to store data
        size_t getBufferSize() {
            return m_data.getMaxCount();
            }
        // Returns a pointer to the start of the buffer DMemoryInputStream uses to store data
        char *getBuffer() {
            return m_data.getBuffer();
            }
        bool allocate(ptrdiff_t size) {
			return m_data.allocate(size);
			}
        void setDataSize(ptrdiff_t bytes);
        // Returns the size of the data stored in this stream
        ptrdiff_t getDataSize() {
			return m_length;
			}
		// Allocate a new DMemoryInputStream with its own copy of the data in this one.
		DMemoryInputStream *clone();

		void deleteData(ptrdiff_t offset, ptrdiff_t bytes);
    private:
    	// Memory buffer containing the data
		DMemBuffer<char> m_data;
		// Current read position
        ptrdiff_t m_pos;
        // Length of the data in m_data
        ptrdiff_t m_length;
    };

/* Summary
   Implements an input stream representing a document stored in a disk file.
   Description
   \Input streams are used in the dtsDataSource API.
   See Also
   DDataSourceBase, DMemoryInputStream, dtsInputStream                       */
class DFileInputStream : public DInputStreamBase {
    public:
    	// Constructor
        DFileInputStream();
        /*  Open a disk file to provide input for the input stream using a UTF-8 filename
            Parameters:
            	fn:      Filename encoded using UTF-8
            Returns:
            	0 on success, or non-zero on failure
        */
        int open(const char *fn);
        /*  Open a disk file to provide input for the input stream using a const TCHAR * filename
            Parameters:
            	fn:      Filename encoded as a const TCHAR * string, which is either Unicode or Ansi depending on whether the UNICODE macro is defined
            Returns:
            	0 on success, or non-zero on failure
        */
        int openUi(const TCHAR *fn) {
            return open(UiToUtf8(fn));
            }
        /*  Open a disk file to provide input for the input stream using a Unicode
            Parameters:
            	fn:      Unicode filename
            Returns:
            	0 on success, or non-zero on failure
        */
        int open(const wchar_t *fn) {
            DString tmp;
            tmp.storeUnicodeAsUtf8(fn);
            return open(tmp);
            }
            
        // Close the file associated with the input stream
        void close();
        
        virtual ~DFileInputStream();

		// Change the seek pointer for the input
		// Parameters:
		//     where:    New position for the seek pointer
        virtual void seek(DFilePosition where);
        // Returns the current position of the seek pointer for the input
        virtual DFilePosition tell() {
			return pos;
			}
        // Reads data from the input stream
        // Parameters:
        // 	   dest:       Buffer to receive data
        //     bytes:      Number of bytes to read
        // Returns:        Number of bytes read
        virtual long read(void *dest, long bytes);

        int good() {
            return file.good();
            }
        void deleteFileWhenDone() {
            fShouldDeleteFile = true;
            }
	    int getLastError() {
			return file.getLastError();
		}

    protected:
    	// The file
        DFile file;
        // Current read position
        DFilePosition pos;
        // Length of the file
        DFilePosition length;

        int fShouldDeleteFile;
    };

/* Summary
   Abstract base class for an object that implements the dtsDataSource indexing API.
   Remarks
   Override the virtual functions getNextDoc and rewind to implement a data source.

   DDataSourceBase2 is an extended version of this class that enables the dtSearch
   Engine to check document modification dates more efficiently.
   See Also
   <link Indexing Databases>

   dtsDataSource

   DInputStreamBase

   dtsInputStream                                                                    */

class DDataSourceBase {
    public:
    	// Constructor
        DDataSourceBase();
        virtual ~DDataSourceBase();
        // Get the next document in the data source
        // Parameters:
        //     dest:     Structure to receive the next document
        // Returns:   0 if a document was returned, or -1 on failure
        virtual int getNextDoc(dtsInputStream& dest) = 0;
        // Initialize the data source so the next getNextDoc call will return the first document
        // Returns:   0 on success, or -1 on failure
        virtual int rewind() = 0;
        // Get interface that can be attached to dtsIndexJob.dataSourceToIndex
        dtsDataSource *getInterface();
        virtual void abort() { };
        static int getNextDocCB(void *pData, dtsInputStream& dest);
        static int rewindCB(void *pData);
        static DDataSourceBase *safeCast(void *pData);
        void forwardErrorsTo(dtsErrorInfo& eh) {
				errorHandler.forwardTo(eh);
				errorHandler.forwardTo(errorLog);
				errorHandler.clear();
			}
    protected:
        long idDDataSourceBase;
        dtsErrorInfo errorHandler;
        dtsErrorInfo errorLog;
        dtsDataSource dataSourceInterface;
        virtual void makeInterface(dtsDataSource& dest);
        void reportError(int code, const char *msg, const char *arg1 = 0, const char *arg2 = 0) {
			errorHandler.handleMessage(code, msg, arg1, arg2);
			}
    };

// Used to return file properties in data source API
//
class DDataSourceFileInfo : public dtsDataSourceFileInfo {
	public:
		// Constructor
		DDataSourceFileInfo() :
			Filename(&filename),
			ErrorMessage(&errorMessage)
			{ }
		// Name of the file
		DStringProperty Filename;
		// Error message if there was an error accessing the file
		DStringProperty ErrorMessage;
		// Copy properties from a dtsDataSourceFileInfo
		DDataSourceFileInfo& operator=(const dtsDataSourceFileInfo& other) {
			copy(other);
			return *this;
			}
		// Copy properties from a DDataSourceFileInfo
		DDataSourceFileInfo& operator=(const DDataSourceFileInfo& other) {
			copy(other);
			return *this;
			}
		// Copy properties from a DDataSourceFileInfo
		void copy(const dtsDataSourceFileInfo& other);
		// Copy properties from a dtsInputStream
		void getFrom(const dtsInputStream& stream);
		// Clear all properties
		void clear() {
			dtsDataSourceFileInfo::clear();
			Filename.clear();
			ErrorMessage.clear();
			}
		// Store an error message and code
		void setError(int code, const char *msg) {
			ErrorMessage = msg;
			errorCode = code;
			}
	};

/* Summary
   Extension of DDataSourceBase to support separately retrieving document properties
   and contents during indexing
   See Also
   <link Indexing Databases>

   dtsDataSource

   DInputStreamBase

   dtsInputStream                                                                    */
class DDataSourceBase2 : public DDataSourceBase {
	public:
		// Constructor
		DDataSourceBase2();
		virtual ~DDataSourceBase2();
		/*  Get information about a the next document in the data source.
			Parameters:
				dest:    Structure to receive the properties of the document
			Returns:     0 on success, or -1 if there are no more documents to return
		*/
        virtual int getNextDocInfo(dtsDataSourceFileInfo& dest) = 0;
        /*  Get information about a named document in the data source.
        	Parameters:
        		docName:     The name of the document in the data source
				userFields:  String set containing any stored fields associated with this document.
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
		   the next document.
		   */
        virtual int getDocInfoByName(const char *docName, const char *userFields, dtsDataSourceFileInfo& fi) = 0;
          /* Get the document whose properties were just returned by
		       getNextDocInfo or getDocInfoByName
		       Parameters
		       pData :  The value of dtsDataSource.pData
		       dest :   Structure to receive the dtsInputStream for the
		                document
		       Returns
       				0 on success, or -1 if the document could not be accessed. */
        virtual int getCurrentDoc(dtsInputStream& dest) = 0;
        static int getNextDocInfoCB(void *pData, dtsDataSourceFileInfo& dest);
        static int getCurrentDocCB(void *pData, dtsInputStream& dest);
        static int getDocInfoByNameCB(void *pData, const char *docName, const char *userFields, dtsDataSourceFileInfo& fi);
        // Make interface that can be attached to dtsIndexJob.dataSourceToIndex
        virtual void makeInterface(dtsDataSource& dest);
	protected:
        static DDataSourceBase2 *safeCast(void *pData);
		long idDDataSourceBase2;
	};


// begin-internal
// Class to access a data source and iterate over its members.   This is not needed to
// use the dtSearch Engine's data source API, but may be useful for informational purposes,
// to see how your data source will be called.
//
// The code is designed to provide a simplified interface to a data source, which may or may
// not implement the getDocInfoByName and getNextDocInfo functions
class DDataSourceClient {
	public:
		DDataSourceClient(dtsDataSource *ps = 0) {
			if (ps)
				SetInterface(*ps);
			}
		~DDataSourceClient() {
			releaseSavedStream();
			}

		void SetInterface(dtsDataSource& s) {
			dataSource.copy(s);
			}
		int IsEmpty() {
			return (dataSource.rewind == 0);
			}
		bool Rewind() {
			if (dataSource.rewind)
				return dataSource.rewind(dataSource.pData) == SUCCESS;
			return false;
			}
		bool GetNextDocInfo(dtsDataSourceFileInfo& fi) {
			releaseSavedStream();
			if (dataSource.getNextDocInfo) {
				if (dataSource.getNextDocInfo(dataSource.pData, fi) == SUCCESS) {;
					// This puts string members of the fileInfo into buffers owned by this class
					currentFileInfo.copy(fi);
					fi.copy(currentFileInfo);
					return true;
					}
				return false;
				}
			else if (dataSource.getNextDoc) {
				// If data source does not implement getNextDocInfo, use getNextDoc to get the info at the same time
				// and save the stream.  If caller then requests the document by calling GetCurrentDoc, the
				// saved stream is returned.  If not, the saved stream is released on the next call to GetNextDocInfo
				int v= dataSource.getNextDoc(dataSource.pData, savedStream);
				if (v == SUCCESS) {
					currentFileInfo.getFrom(savedStream);
					fi.copy(currentFileInfo);
					}
				return v == SUCCESS;
				}
			else
				return false;
			}
		bool GetCurrentDoc(dtsInputStream& dest) {
			if (!savedStream.isEmpty()) {
				returnSavedStream(dest);
				return true;
				}
			if (dataSource.getCurrentDoc) {
				if (dataSource.getCurrentDoc(dataSource.pData, dest) == SUCCESS) {
					return true;
					}
				}
			return false;
			}
		bool GetDocInfoByName(const char *docName, const char *userFields, dtsDataSourceFileInfo& fi) {
			if (dataSource.getDocInfoByName &&
				(dataSource.getDocInfoByName(dataSource.pData, docName, userFields, fi) == SUCCESS)) {
				// This puts string members of the fileInfo into buffers owned by this class
				currentFileInfo.copy(fi);
				fi.copy(currentFileInfo);
				return true;
				}
			else
				return false;
			}
		bool CanGetDocInfoByName() {
			return dataSource.getDocInfoByName != 0;
			}
	protected:
		dtsDataSource dataSource;
		dtsInputStream savedStream;
		DDataSourceFileInfo currentFileInfo;

		void releaseSavedStream() {
			if (savedStream.pData) {
				savedStream.release(savedStream.pData);
				savedStream.clear();
				}
			}
		void returnSavedStream(dtsInputStream& dest) {
			if (savedStream.pData) {
				dest.copy(savedStream);
				savedStream.clear();
				}
			}
	};
// end-internal

#ifdef USE_DTSEARCH_NAMESPACE
}  // dtSearch namespace
#endif

#endif
// end-internal

