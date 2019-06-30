//
// The File class provides an OS-independent interface to basic
//  file i/o functions.
//
#ifndef DFILE_H
#define DFILE_H

#include <time.h>

#ifndef DSTRING_H
    #include <dstring.h>
#endif
#ifndef DTSEARCH_H
    #include <dtsearch.h>
#endif

#ifdef USE_DTSEARCH_NAMESPACE

namespace dtSearch {

#endif

#if !defined(_WIN32) && !defined(BOOL)
#	define BOOL bool
#endif

//  Class for managed allocated pointers
template <class Type>
class DHeapPtr {
    public:
        DHeapPtr(Type *p = NULL) {
            init();
            attach(p);
            };
        void dealloc() {
            if (m_fShouldDelete) {
                Type *toDelete = m_ptr;
                m_ptr = 0;
                delete toDelete;
                }
            m_ptr = 0;
            m_fShouldDelete = false;
            }
		Type *allocOnce() {
			if (!m_ptr)
				attach(new Type);
			return m_ptr;
			}
		Type *allocNew() {
			dealloc();
			attach(new Type);
			return m_ptr;
			}
        ~DHeapPtr() {
            dealloc();
            };
        DHeapPtr<Type>& operator=(Type *p) {
            if (p != m_ptr) {
                dealloc();
                attach(p);
                }
             return *this;
             }
		bool has() const {
			return m_ptr != 0;
			}
        inline Type* operator->() const { return m_ptr; }
        inline Type& operator*() const  { return *m_ptr; }
        Type* detach()      {
            Type* old = m_ptr;
            m_ptr = NULL;
            m_fShouldDelete = false;
            return old;
            }
        Type *attach(Type *p){
            dealloc();
            m_ptr = p;
            m_fShouldDelete = (p != 0);
			return m_ptr;
            }
        Type *attachWithoutOwnership(Type *p) {
            dealloc();
            m_fShouldDelete = false;
            m_ptr = p;
            return m_ptr;
            }
     	inline Type *ptr() const {
			return m_ptr;
		}
        inline Type *peek()  const {
            return m_ptr;
            }
        inline Type& operator[](int i) const {
			return m_ptr[i];
			}
        inline bool operator!() const { return isEmpty(); }
        inline bool isEmpty() const { return m_ptr == 0; }
        bool hasOwnership() {
            return m_ptr && m_fShouldDelete;
            }
        inline operator Type *() const {
			return m_ptr;
		}
		void swap(DHeapPtr<Type>& other) {
			Type *ptr = m_ptr;
			bool shouldDelete = m_fShouldDelete;
			m_ptr = other.m_ptr;
			m_fShouldDelete = other.m_fShouldDelete;
			other.m_ptr = ptr;
			other.m_fShouldDelete = shouldDelete;
			}
    private:
        Type *m_ptr;
        bool m_fShouldDelete;
        DHeapPtr<Type>& operator=(const DHeapPtr<Type>&); // not implemented
        DHeapPtr(const DHeapPtr<Type>&);                  // not implemented
        void init() {
            m_fShouldDelete = false;
            m_ptr = 0;
            }
    };

class DFlag {
    public:
        DFlag() {
            Clear();
            }
        int operator!() const {
            return IsFalse();
            }
        int operator==(const DFlag& other) const {
            return (m_fKnown == other.m_fKnown) && (m_fValue == other.m_fValue);
            }
        DFlag& operator=(const DFlag& other) {
            m_fKnown = other.m_fKnown;
            m_fValue = other.m_fValue;
            return *this;
            }
        DFlag& operator=(bool other) {
            Set(other);
            return *this;
            }
        void Clear() {
            m_fKnown = false;
            m_fValue = false;
            }
        bool Set(bool v) {
            if (v)
                SetTrue();
            else
                SetFalse();
            return v;
            }
        bool SetTrue() {
            m_fKnown = true;
            m_fValue = true;
            return true;
            }
        bool SetFalse() {
            m_fKnown = true;
            m_fValue = false;
            return false;
            }
        bool IsFalse() const {
            return m_fKnown && !m_fValue;
            }
        bool IsTrue() const {
            return m_fKnown && m_fValue;
            }
        bool HasValue() const {
            return m_fKnown;
            }
        bool GetValue() const {
			if (m_fKnown)
				return m_fValue;
			else
				return false;
			}
		operator bool() const {
			return GetValue();
			}
    protected:
        bool m_fKnown;
        bool m_fValue;
    };



/*
    F_READ:     Allow read access to file
    F_WRITE:    Allow write access to file
    F_CREATE:   Create the file *if* it does not exist
    F_TRUNC:    Empty the file if it does exist

*/
#define F_READ          1
#define F_WRITE         2
#define F_CREATE        6
#define F_RW			(F_READ | F_WRITE)
#define F_ANY           (F_READ | F_WRITE | F_CREATE)
#define F_TRUNC         32  // Truncate file to 0 length on open

#define F_IsCreate(f)   (((f) & F_CREATE) == F_CREATE)

// By default, read-only files are opened shared, deny write
//             read-write files are opened deny read,write
#define F_SH_ALLOWWRITE 64
#define F_SH_ALLOWREAD  128
#define F_SH_ALLOWANY   (F_SH_ALLOWWRITE | F_SH_ALLOWREAD)

#define F_NO_CHANGE_LASTACCESS 256

// Concurrent is a quick way to ask for shared access to a file
// that only one user can update at a time but many users can read,
// even during an update.
#define F_CONCURRENT    1024

#if defined(_WIN32)
    typedef void * t_FileHandle;
#else
    typedef int t_FileHandle;
#endif

typedef __int64 DFilePosition ;

// Class wrapper around dtsFileDate, providing conversion to and from various Windows and C runtime library types
class DFileDate : public dtsFileDate {
    public:
    	// Copy constructor
        DFileDate(const DFileDate& other) {
            *this = other;
		}
		// Constructor
        DFileDate() { }
        // Construct a DFileDate from a dtsFileDate
        DFileDate(const dtsFileDate& other) {
            dtsFileDate::operator=(other);
            }
        // Copy a dtsFileDate
        DFileDate& operator=(const dtsFileDate& other) {
            dtsFileDate::operator=(other);
            return *this;
            }
		// True if the two dates are the same
		bool operator==(const DFileDate& other) const {
			return dtsFileDate::operator==(other);
			}
		// True if two dates are not the same
		bool operator!=(const DFileDate& other) const {
			return dtsFileDate::operator!=(other);
			}
		// True if this date is after the other date
		bool operator>(const DFileDate& other) const {
			return dtsFileDate::operator>(other);
			}
		// True if this date is before the other date
		bool operator<(const DFileDate& other) const {
			return dtsFileDate::operator<(other);
			}
		bool operator>=(const DFileDate& other) const {
			return (*this > other) || (*this == other);
			}
		bool operator<=(const DFileDate& other) const {
			return (*this < other) || (*this == other);
			}
            
        // Format date as follows YYYY-MM-DD HH:MM:SSZ
        void formatISO(DString& dest) const;
        // Parse date from string created by formatISO
        void parseISO(const char *s);

#if defined(_WIN32) || defined(_WIN32_UNIX)
		// Convert to a WIN32 SYSTEMTIME
        void convertTo(SYSTEMTIME& st, long fMakeLocal = true) const;
        // Convert to a WIN32 FILETIME
        void convertTo(FILETIME& ft, long fMakeLocal = true) const;
        // Convert from a WIN32 SYSTEMTIME
        void convertFrom(const SYSTEMTIME& st);
        // Convert from a WIN32 FILETIME
        void convertFrom(const FILETIME& ft);

#endif
		// Convert to a struct tm
		void convertToTm(struct tm & t, bool bFixDayOfWeek) const;
		// Convert from a struct tm
		void convertFrom(struct tm & t);
		// Convert from a time_t
		void convertFrom(time_t tt);
		// Convert from UTC to Local time, using system time zone information
		void utcToLocal();
		void formatLocal(DString& dest) const;
        void nowUTC();
        void now();
        
        void formatDate(DString& dest, bool makeLocal = true) const;

#ifdef MfcAvailable
		// Format as date, optionally converting to local time
        void formatDate(CString& s, bool fMakeLocal = true) const;
		// Format as time, optionally converting to local time
        void formatTime(CString& s, bool fMakeLocal = true) const;
		// Format as date, optionally converting to local time, returning a CString
        CString formatDate(bool fMakeLocal = true) const {
            CString ret;
            formatDate(ret, fMakeLocal);
            return ret;
            }
		// Format as time, optionally converting to local time, returning a CString
        CString formatTime(bool fMakeLocal = true) const {
            CString ret;
            formatTime(ret, fMakeLocal);
            return ret;
            }
        // Convert to MFC CTime
        CTime convertToCTime(bool fMakeLocal = true) const;
#endif
    };

struct DFileDateTz : public DFileDate {
	DFileDateTz() {
		tzInMinutes = 0;
		}
	// Time zone displacement in minutes.  EST, 5 hours behind UTC, is 300, not -300,
	// following the convention of TIME_ZONE_INFORMATION.Bias
	int tzInMinutes;
	
	void copy(const DFileDateTz& other) {
		dtsFileDate::copy(other);
		tzInMinutes = other.tzInMinutes;
		}
	void copy(const dtsFileDate& other) {
		dtsFileDate::copy(other);
		tzInMinutes = 0;
		}
	DFileDateTz& operator=(const DFileDateTz& other) {
		copy(other);
		return *this;
		}
	};



class CAccessFlags {
	public:
		CAccessFlags() :
			m_fOpenRead(false),
			m_fOpenRW(false),
			m_fOpenNew(false)
			{ }
		CAccessFlags& operator=(const CAccessFlags & other) {
			m_fOpenRead = other.m_fOpenRead;
			m_fOpenRW = other.m_fOpenRW;
			m_fOpenNew = other.m_fOpenNew;
            return *this;
			}
		void setOpenRead() {
			setClosed();
			m_fOpenRead = true;
			}
		void setOpenRW() {
			setClosed();
			m_fOpenRW = true;
			m_fOpenRead = true;
			}
		void setOpenNew() {
			setOpenRW();
			m_fOpenNew = true;
			}
		int isOpen() {
			return m_fOpenRW || m_fOpenRead;
			}
		int isOpenRW() {
			return m_fOpenRW;
			}
		int isOpenRead() {
			return m_fOpenRead;
			}
		int isOpenReadOnly() {
			return m_fOpenRead && !m_fOpenRW;
			}
		void setClosed() {
			m_fOpenRead = false;
			m_fOpenRW = false;
			m_fOpenNew = false;
			}
	protected:
		int m_fOpenRW;
		int m_fOpenRead;
		int m_fOpenNew;
		};

#define DFile_BadHandle  ((t_FileHandle) -1 )

/* Abstract class representing a data stream; used as the base for DFile */
class DStreamBase {
	public:
	virtual ~DStreamBase() { };
	// Return the length of the stream
	virtual DFilePosition getLength() = 0;
	// Return the current file position
	virtual DFilePosition tell() = 0;
	// Read data from the stream
	virtual int read(void *dest, DFilePosition len) = 0;
	// Write data to the stream
	virtual int write(const void *data, DFilePosition len) = 0;
	// Change the file position in the stream
	virtual int seek(DFilePosition pos) = 0;
	// Set the length of the stream
	virtual int setLength(DFilePosition len = 0) = 0;
	// Returns true if the stream was opened with write access
	virtual bool canWrite() = 0;
	// Returns true if the stream was opened with read access
	virtual bool canRead() = 0;
	// Returns true if the stream supports seek()
	virtual bool canSeek() = 0;
	};

#ifdef _WIN32
struct FileTimes {
	bool bHaveData;
	bool bHaveLastAccess;
	bool bHaveCreated;
	bool bHaveModified;
	FILETIME lastAccess;
	FILETIME created;
	FILETIME modified;
	FileTimes() {
		clear();
		}
	void clear() {
		memset(this, 0, sizeof(FileTimes));
		}
	void get(t_FileHandle fh);
	void set(t_FileHandle fh);
	void getFrom(const BY_HANDLE_FILE_INFORMATION& bhfi);
	};
#endif
	

/* Summary
   \File class, used in the C++ Support Classes and sample code to provide a
   portable way to read and write files.
   Remarks
   DFile supports 64-bit file sizes and offsets.
   
   DFile supports Unicode filenames on operating systems that have Unicode support */
class DFile : public DStreamBase {
  protected:
	bool bThrowOnError;
	bool bRetryOnError;
    t_FileHandle handle;
    bool bShouldCloseHandle;
    bool bShouldDeleteOnClose;
	int accessMode;
    int rwFlags;
    int shareMode;
    struct {
        unsigned is_open : 1;
        unsigned is_frozen : 1;
        unsigned reserved : 1;
        unsigned read : 1;
        unsigned write : 1;
        unsigned create : 1;
        unsigned trunc : 1;
        unsigned rshare : 1;
        unsigned wshare : 1;
        unsigned restoreLastAccess : 1;
        } flags;
    int error;
    DFilePosition filePosition;
	DFilePosition savedPosition;
    DFileDate createdDate;
    DFileDate modifiedDate;
#ifdef _WIN32
    FileTimes savedFileTimes;
#endif
#ifdef __UNIX__
	// From stat
	__int64 inode;
	__int64 device;
#endif
    DFilePosition length;
    DString name;

    void openHandle(void);
    void closeHandle();
    void clear();
    void getHandleInfo();
    DFilePosition os_read(void *buf, DFilePosition len);
    DFilePosition os_write(const void *buf, DFilePosition len);
    void storeAccessFlags(int access);
    void saveError();
	// After an operation has completed, throw a DFileException if an error occurred
    int logAndSaveError(const char *comment, __int64 val = 0, bool bCritical = false);
	void logRetry(DWORD code, const char *cmt, __int64 val, bool bOK);
	void makeErrorReport(DString& dest, const char *cmt, __int64 val, DWORD error);
    
public:
    DFile();
    void setRetryOnError(bool v) {
		bRetryOnError = v;
		}
	void setThrowOnError(bool v) {
		bThrowOnError = v;
		}
    virtual ~DFile();
    // Returns an operating system handle to the file
    t_FileHandle getHandle() {
        return handle;
        }
    int getAccess() {
		return accessMode;
		}
	// Close the file so it can be reopened without changing state
	void freeze() {
		if (good()) {
			savedPosition = filePosition;
			closeHandle();
			flags.is_frozen = true;
			}
		}
	bool thaw() {
		if (flags.is_frozen) {
			flags.create = false;
			flags.trunc = false;
			openHandle();		
			if (good()) {
				filePosition = savedPosition;
				seek(filePosition);
				savedPosition = 0;
				flags.is_frozen = 0;
				return true;
				}
			}
		return false;
		}
				
		
	virtual t_FileHandle detachHandle() {
		t_FileHandle ret = getHandle();
		clear();
		return ret;
		}
	// Open a file, using a UTF-8 filename
    virtual int open(const char *name, int access);
	int openU8(const char *utf8name, int access) {
		return open(utf8name, access);
		}
	// Open a file, using a TCHAR or CString filename
	int openUi(const TCHAR *uiName, int access);
	// Open a file, using a Unicode filename
	int openW(const wchar_t *wName, int access) {
		DString tmp;
		tmp.storeUnicodeAsUtf8(wName);
		return openU8(tmp, access);
		}
	int openCreate(const char *fn) {
		return openU8(fn, F_ANY | F_TRUNC);
		}
	// Open a temporary file
	int createTemp(const char *nameSuffix, bool bDeleteOnClose);
	
	// Attach a file handle
    void attach(t_FileHandle handle, int access = F_READ, bool fShouldClose = true);
    // Close the file
    virtual void close();
    // Returns true if the file was opened successfully
    int good() const;

	//! Obsolete
    int read32(void *buf, DFilePosition len) {
        DFilePosition ret = read64(buf, len);
        return (int) ret;
        }
    //! Obsolete
    int write32(const void *buf, DFilePosition len) {
        DFilePosition ret = write64(buf, len);
        return (int) ret;
        }
	// Read data from the file
    int read(void *buf, DFilePosition len) {
        return (int) read64(buf, len);
        }
    // Write data to the file
    int write(const void *buf, DFilePosition len) {
        return (int) write64(buf, len);
        }
	int readIntoString(DString& s, ptrdiff_t maxBytes = -1);
    virtual DFilePosition read64(void *buf, DFilePosition len);
    virtual DFilePosition write64(const void *buf, DFilePosition len);
	// Change the file position
    virtual int seek(DFilePosition where);
    // Get the file position
    DFilePosition tell();
	// Close and reopen the file handle.  DFile does not maintain buffers
	// so all I/O is unbuffered.
    void flush();
    // True if the file is open
    int isOpen() const;
    // Return the length of the file
    DFilePosition getLength();
	DFilePosition getLengthFromHandle();
	// Set the length of the file (this can be used to truncate a file)
    int setLength(DFilePosition len);
    void updateLength(DFilePosition newLen) {
		length = newLen;
		}
	// Get the last-modified date of the file
    void getModifiedDate(dtsFileDate& d) {
        d = modifiedDate;
        }
    // Get the created date of the file
    void getCreatedDate(dtsFileDate& d) {
        d = createdDate;
        }
	// Get the last access date of the file
	void getLastAccessDate(dtsFileDate& d);
    // Set the last-modified date of the file
    int setModifiedDate(dtsFileDate& d);
    // Set the created and modified date of the file
    int setDates(dtsFileDate& created, dtsFileDate& modified);
    // Set the last access date of the file
    int setLastAccessDate(dtsFileDate& lastAccess);
    // Get the name that was used to open the file
    char *getName();
    // Get the last system error that occurred when trying to access this file
    int getLastError();
    int save();
    // Force file changes to be written to disk
    int flushFileBuffers();
    // Returns true if the the file was opened with write access
	virtual bool canWrite() {
		return flags.write;
		}
    // Returns true if the the file was opened with read access
	virtual bool canRead() {
		return flags.read;
		}
	virtual bool canSeek() {
		return true;
		}
	void throwOnError(const char *comment);
#ifdef __UNIX__
	__int64 getInode() {
			return inode;
		}
	__int64 getDevice() {
			return device;
		}
#endif				
		
	static void fixLongFilename(DWString& wName);
	static void fixLongFilename(DString& wName);
    static int getOpenFileCount();
#ifdef _WIN32

	static HANDLE CreateFileU8(
		const char *fileName,
		DWORD dwDesiredAccess,
		DWORD dwShareMode,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		DWORD dwCreationDisposition,
		DWORD dwFlagsAndAttributes,
		HANDLE hTemplateFile);

    static DWORD GetFileAttributesU8(const char *fn);
    static BOOL SetFileAttributesU8(const char *fn, DWORD fa);
	static BOOL SetFilePointer64(HANDLE hFile, DFilePosition position, DWORD seekMethod);
	static BOOL CopyFileU8(const char *existingFile, const char *newFile, BOOL bFailIfExists);
	static BOOL CopyPreservingTimes(const char *existingFile, const char *newFile, BOOL bFailIfExists);
#endif
	static BOOL DeleteFileU8(const char *fn);
    static bool IsFileU8(const char *fn);
    static bool IsDirectoryU8(const char *fn);
    static BOOL CreateDirectoryU8(const char *fn);
    static BOOL CreateDirectoryRecursiveU8(const char *fn);
    static bool ExistsU8(const char *fn);
#ifdef HAVE_WINAPI
	static DWORD GetModuleFilenameU8(HMODULE hModule, char *fn, DWORD maxLen);
	static DWORD GetModuleFilenameU8(HMODULE hModule, DString& dest);
#endif
    };

class DFileException {
	public:
		DFileException() {
			clear();
			}
		DFileException(DFile& file) {
			clear();
			setFile(file.getName());
			if (file.good()) {
				length = file.getLength();
				pos = file.tell();
			}
		}

		DFileException(const DFileException& other) {
			clear();
			copy(other);
			}
		DFileException& operator=(const DFileException& other) {
			clear();
			copy(other);
			return *this;
			}
		void clear() {
			errorCode = 0;
			message[0] = 0;
			filename[0] = 0;
			pos = 0;
			length = 0;
			}
		void copy(const DFileException& other) {
			errorCode = other.errorCode;
			strCopy(message, other.message, sizeof message);
			strCopy(filename, other.filename, sizeof filename);
			pos = other.pos;
			length = other.length;
			}
		__int64 getErrorCode() {
			return errorCode;
			}
		void setErrorCode(__int64 v) {
			errorCode = v;
			}
		void setMessage(const char *s) {
			if (s) 
				strCopy(message, s, sizeof message);
			else
				message[0] = 0;
			}
		operator const char *() const {
			return message;
			}

		void setFile(const char *name) {
			strCopy(filename, name, sizeof filename);
		}
		const char *getFilename() {
			return filename;
			}
		DFilePosition pos;
		DFilePosition length;
	protected:
		__int64 errorCode;
		char message[1024];
		char filename[512];
	};


#ifdef USE_DTSEARCH_NAMESPACE
}  // dtSearch namespace



#endif

#endif

