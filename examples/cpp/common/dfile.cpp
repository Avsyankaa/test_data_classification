#include <stdafx.h>
#ifndef DFILE_H
#	include <dfile.h>
#endif
#include <dtconfig.h>
#include <limits.h>
#if defined(__UNIX__)
	#include <unistd.h>
    #include <fcntl.h>
    #include <sys/stat.h>
    #include <time.h>
    #define O_BINARY 0   //  Not used
    #include <utime.h>
    #include <sys/mman.h>
#else
	#include <stdlib.h>
	#include <io.h>
    #include <fcntl.h>
    #include <sys/stat.h>
    #include <time.h>
	#include <share.h>
 #endif

#if defined(_ANDROID)
#include <time64.h>
#include <sys/time.h>
#endif

#if defined(_MAC)
#  define O_LARGEFILE 0
#  define lstat64 lstat
#  define fstat64 fstat
#  define open64 open
#  define stat64 stat
#  define ftruncate64 ftruncate
#  define lseek64 lseek
#endif

#include <errno.h>

#ifndef _WIN32
    #define INVALID_HANDLE_VALUE  ((t_FileHandle)(-1))

#endif

#ifdef _WIN32
     const int defaultRetryCount = 3;
#endif

#ifdef USE_DTSEARCH_NAMESPACE
using namespace dtSearch;
#endif




#if defined(_WIN32) || defined(_WIN32_UNIX)
void DFileDate::convertTo(SYSTEMTIME& st, long fMakeLocal) const
{	memset(&st, 0, sizeof st);
	st.wDay = day;
	st.wHour = hour;
	st.wMinute = minute;
	st.wMonth = month;
	st.wSecond = second;
	st.wYear = year;

	if (fMakeLocal) {
		FILETIME utcft, localft;
		memset(&utcft, 0, sizeof utcft);
		memset(&localft, 0, sizeof localft);
		SystemTimeToFileTime(&st, &utcft);
		FileTimeToLocalFileTime(&utcft, &localft);
		FileTimeToSystemTime(&localft, &st);
		}
}

void DFileDate::convertTo(FILETIME& ft, long fMakeLocal) const
{	SYSTEMTIME st;
	convertTo(st, false);
	memset(&ft, 0, sizeof ft);
	if (fMakeLocal) {
		FILETIME utcft;
		memset(&utcft, 0, sizeof utcft);
		SystemTimeToFileTime(&st, &utcft);
		FileTimeToLocalFileTime(&utcft, &ft);
		}
	else
		SystemTimeToFileTime(&st, &ft);
}


void DFileDate::convertFrom(const FILETIME& ft)
{   SYSTEMTIME st;
	memset(&st, 0, sizeof st);
		
    if (FileTimeToSystemTime(&ft, &st))
		convertFrom(st);
	else
		clear();
}

void DFileDate::convertFrom(const SYSTEMTIME& st)
{   day = (char) st.wDay;
    month = (char) st.wMonth;
    year = st.wYear;
    hour = (char) st.wHour;
    minute = (char) st.wMinute;
    second = (char) st.wSecond;
}
#endif

void DFileDate::formatDate(DString& dest, bool makeLocal) const
{	
#ifdef _WIN32
	SYSTEMTIME st;
	convertTo(st, makeLocal);

	TCHAR buf[80];
	memset(buf, 0, sizeof buf);
	GetDateFormat(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &st, 0, buf, _countof(buf));
	dest << UiToUtf8(buf);
#else
	DFileDate toFormat;
	toFormat.copy(*this);
	if (makeLocal)
		toFormat.utcToLocal();
	dest << PadNum(toFormat.year, 4) << "/" << PadNum(toFormat.month, 2) << "/" << PadNum(toFormat.day, 2);

#endif
}
	

#ifdef MfcAvailable
CTime DFileDate::convertToCTime(bool fMakeLocal) const
{	SYSTEMTIME st;
	convertTo(st, fMakeLocal);
	CTime ret(st);
	return ret;
}

void DFileDate::formatDate(CString& s, bool fMakeLocal) const
{	SYSTEMTIME st;
	convertTo(st, fMakeLocal);

	const int bufSize = 40;
	TCHAR *buf = s.GetBuffer(bufSize);
	GetDateFormat(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &st, 0, buf, bufSize);
	s.ReleaseBuffer();
}

void DFileDate::formatTime(CString& s, bool fMakeLocal) const
{	SYSTEMTIME st;
	convertTo(st, fMakeLocal);

	const int bufSize = 40;
	TCHAR *buf = s.GetBuffer(bufSize);
	GetTimeFormat(LOCALE_USER_DEFAULT, 0, &st, 0, buf, bufSize);
	s.ReleaseBuffer();

}
#endif

void DFileDate::formatISO(DString& dest) const
{	DString d, t;
	d.ymd(year, month, day, '-');
	t.hms(hour, minute, second);
	dest << d << ' ' << t << 'Z';
}

void DFileDate::parseISO(const char *s)
{	clear();
	DStringSet temp;
	temp.tokenize(s, " /:-ZT");
	if (temp.getCount() >= 3) {
		year = (short) atol(temp.getString(0));
		month = (char) atol(temp.getString(1));
		day = (char) atol(temp.getString(2));
		}
	if (temp.getCount() >= 6) {
		hour = (char) atol(temp.getString(3));
		minute = (char) atol(temp.getString(4));
		second = (char) atol(temp.getString(5));
		}
}

void DFileDate::convertToTm(struct tm & tmTime, bool bFixDayOfWeek) const
{	memset(&tmTime, 0, sizeof tmTime);
	tmTime.tm_hour = hour;
	tmTime.tm_min = minute;
	tmTime.tm_sec = second;
	tmTime.tm_year = year - 1900;
	tmTime.tm_mon = month - 1;  // tm uses 0-11 for months
	tmTime.tm_mday = day;
	
	if (bFixDayOfWeek) {
#ifdef _WIN32
	#ifdef HAVE_SAFE_GMTIME64
		// Fix missing day of week
		__time64_t t64 = _mkgmtime64(&tmTime);
		_gmtime64_s(&tmTime, &t64);
	#else
		time_t t = mktime(&tmTime);
		memmove(&tmTime, gmtime(&t), sizeof tmTime);
	#endif
#else
		time_t t = mktime(&tmTime);
		gmtime_r(&t, &tmTime);
#endif
		}
}

void DFileDate::convertFrom(struct tm & t)
{	hour = (char) t.tm_hour;
	minute = (char) t.tm_min;
	second = (char) t.tm_sec;
	year = (unsigned short) t.tm_year;
	if (year < 500)
		year += 1900;
	month = (char) t.tm_mon + 1;
	day = (char) t.tm_mday;
}

void DFileDate::convertFrom(time_t tt) {
#ifdef HAVE_SAFE_GMTIME
	struct tm tmp;
	gmtime_s(&tmp, &tt);
	convertFrom(tmp);
#else
	struct tm *tmp = gmtime(&tt);
	convertFrom(*tmp);
#endif
	}

void DFileDate::utcToLocal()
{
	struct tm tmDate;
	convertToTm(tmDate, false);
// VS 2005 or later
#if defined(_WIN32) && (_MSC_VER >= 1400)
	// mkgmtime64 converts a struct tm to a time_t with no timezone adjustment.
	__time64_t tt = _mkgmtime64(&tmDate);
	// localtime then applies a timezone adjustment to convert from UTC to local time
	_localtime64_s(&tmDate, &tt);
	// Finally, convertFrom just transfers the time fields with no adjustment
	convertFrom(tmDate);
// VS 2003
#elif defined(_WIN32)
	SYSTEMTIME st;
	convertTo(st, true);
	convertFrom(st);
#elif defined(_ANDROID)
	time_t tt = timegm64(&tmDate);
	localtime_r(&tt, &tmDate);
	convertFrom(tmDate);
#else
	
	time_t tt = timegm(&tmDate);
	localtime_r(&tt, &tmDate);
	convertFrom(tmDate);
#endif	

}

void DFileDate::nowUTC() {
#ifdef HAVE_SAFE_GMTIME64
	__time64_t t = _time64(0);
	struct tm gmt;
	_gmtime64_s(&gmt, &t);
	convertFrom(gmt);
#elif defined(_WIN32) && (_MSC_VER > 1200)
	__time64_t t = _time64(0);
	struct tm *gmt = _gmtime64(&t);
	convertFrom(*gmt);
#else
	time_t tt = time(0);
	struct tm *pt = gmtime(&tt);
	if (pt)
		convertFrom(*pt);
#endif
}

void DFileDate::now() {
#ifdef HAVE_SAFE_LOCALTIME64
	__time64_t t = _time64(0);
	struct tm gmt;
	_localtime64_s(&gmt, &t);
	convertFrom(gmt);
#elif defined(_WIN32) && (_MSC_VER > 1200)
	__time64_t t = _time64(0);
	struct tm *gmt = _localtime64(&t);
	convertFrom(*gmt);
#else
    time_t tt = time(0);
    struct tm *pt = localtime(&tt);
    if (pt)
        convertFrom(*pt);
#endif
	} 

void DFileDate::formatLocal(DString& dest) const
{	if (!isValid()) {
		dest.clear();
		return;
		}
	DFileDate localDate;
	localDate.copy(*this);
	localDate.utcToLocal();

	struct tm t;
	localDate.convertToTm(t, false);
	
	if ((t.tm_year < 1) || (t.tm_year > 3000) || (t.tm_mday < 0) || (t.tm_mday > 31) || (t.tm_mon < 0) || (t.tm_mon > 11)) {
		dest.clear();
		return;
		}
	char temp[80];
	temp[0] = '\0';
	strftime(temp, sizeof temp, "%x %X", &t);
	dest = temp;
}

static int closeCount = 0;
static int openCount = 0;


DFile::DFile() :
	bThrowOnError(false),
	bRetryOnError(false)
{	
   clear();
}

int DFile::getLastError()
{   return error;
}

DFile::~DFile()
{
    close();
}

int DFile::flushFileBuffers()
{
#ifdef _WIN32
	if (good()) {
		bool ok = (FlushFileBuffers(handle) ? true : false);
		if (ok)
			return SUCCESS;
		return logAndSaveError("flushFileBuffers");
		}
	return FAIL;
#else
	fsync(handle);
	return SUCCESS;
#endif
}


void DFile::clear()
{   handle = DFile_BadHandle;
	bShouldCloseHandle = false;
	bShouldDeleteOnClose = false;
    rwFlags = 0;
    shareMode = 0;
    memset(&flags, 0, sizeof flags);
    error = 0;
    filePosition = 0;
    savedPosition = 0;
    length = 0;
    name.clear();
    accessMode = 0;
#ifdef __UNIX__
	inode = 0;
	device = 0;
#endif   
}

char *DFile::getName()
{   return name.str();
}

DFilePosition DFile::tell()
{   return filePosition;
}

void DFile::attach(t_FileHandle aHandle, int access, bool fShouldClose)
{   clear();
    handle = aHandle;
    bShouldCloseHandle = fShouldClose;
    storeAccessFlags(access);
    if (good())
        getHandleInfo();
}

void DFile::storeAccessFlags(int access)
{   flags.read = (access & F_READ? 1 : 0);
    flags.write = (access & F_WRITE? 1 : 0);
    flags.create = (F_IsCreate(access) ? 1 : 0);
    flags.trunc = (access & F_TRUNC? 1 : 0);
    flags.restoreLastAccess = (access & F_NO_CHANGE_LASTACCESS? 1 : 0);

    flags.rshare = 0;
    flags.wshare = 0;
    if (!flags.write)
        flags.rshare = true;
    if (access & F_SH_ALLOWWRITE)
        flags.wshare = true;
    if (access & F_SH_ALLOWREAD)
        flags.rshare = true;
    if (access & F_CONCURRENT) {
        flags.rshare = true;
        if (!flags.write)
            flags.wshare = true;
        }

    // If not opening for write, force sharing of read access
    if (!(access & F_WRITE))
        flags.rshare = 1;
    accessMode = access;
}

static const char *TempFilePrefix = "~~dtSearchTemp";
int DFile::createTemp(const char *namesuffix, bool bDeleteOnClose) {
	DString n;
	n << TempFilePrefix << "_" << namesuffix;
#ifdef __UNIX__
	n.hex((long) getpid());
#else
	n.hex((long) GetCurrentProcessId());
#endif
	n << "_";
	__int64 t = (long) time(0);
	__int64 c = (long) clock();
	__int64 num = 100000 * (t % 100000) + (c % 100000);
	n << HexNum((__int64) this);
	n << HexNum(num);
	DFilename tmpname;
	tmpname.makeTempName(n);
	tmpname.setExt(".tmp");
	bShouldDeleteOnClose = bDeleteOnClose;
	return openU8(tmpname, F_TRUNC | F_ANY | F_SH_ALLOWREAD);
	}



int DFile::open(const char *to_open, int access)
{   name = to_open;

    storeAccessFlags(access);

    openHandle();

    if (good()) {
        filePosition = 0L;
        if (flags.write && (access & F_TRUNC))
            setLength(0);
        return SUCCESS;	
        }
    else
        return FAIL;
}

int DFile::openUi(const TCHAR *uiName, int access) {
	return openU8(UiToUtf8(uiName), access);
}

void DFile::close()
{
    closeHandle();

    if (bShouldDeleteOnClose && flags.write && name.contains(TempFilePrefix))
		DeleteFileU8(name);

    flags.is_frozen = false;
    accessMode = 0;
}

int DFile::isOpen() const
{
    return good();
}

DFilePosition DFile::read64(void *vbuf, DFilePosition len)
{	char *buf = (char *) vbuf;
    DFilePosition v;
    if (filePosition >= length) {
        memset(buf, 0, (size_t) len);
        return FAIL;
        }

    if (flags.read) {
        // If we are reading past the end of the DFile
        // then fill the remainder with NULLs
        if (filePosition + len > length) {
            DFilePosition readLen = length - filePosition;  // has to be positive

            v = os_read(buf, readLen);
            if (readLen < len)
                memset(buf + readLen, 0, (size_t) (len - readLen));
            }
        else
            v = os_read(buf, len);
        if (v >= 0)
			filePosition += v;
        return v;
        }
    else {
        memset(buf, 0, (size_t) len);
        return FAIL;
        }
}

DFilePosition DFile::write64(const void *buf, DFilePosition len)
{   if (flags.write) {
        if (handle < 0)
            return FAIL;
		DFilePosition bytesWritten = os_write(buf, len);
        if (bytesWritten >= 0)
			filePosition += bytesWritten;
        if (length < filePosition)
            length = filePosition;
        return bytesWritten;
        }
    else
        return FAIL;
}


DFilePosition DFile::getLength()
{   return length;
}

int DFile::readIntoString(DString& dest, ptrdiff_t maxBytesToRead)
{
	ptrdiff_t bytes = ToPtrDiffT(getLength());
    if (maxBytesToRead < 0)
		maxBytesToRead = bytes;
    if (bytes > maxBytesToRead)
		bytes = maxBytesToRead;
	if (bytes < 1024*1024*1024) {
		if (dest.tryExtend(ToPtrDiffT(bytes+1))) {
			if (dest.getMaxLength() >= bytes) {
				seek(0);
				read(dest.str(), bytes);
				dest.setLength(bytes);
				return SUCCESS;
				}
			}
		}
	return FAIL;
}

///////////////////////////////////////////////////////////////////
//
//  Environment-dependent interfaces
//

void DFile::saveError()
{
#ifdef _WIN32
	if (!error)
		error = GetLastError();
#else
	if (!error) 
		error = errno;
#endif
}


void DFile::makeErrorReport(DString& msg, const char *cmt, __int64 val, DWORD errorCode)
{
	msg << "File I/O error ";
	if (!strIsBlank(cmt))
		msg << cmt << ' ';
	if (val)
		msg << val;
	if (errorCode) {
		msg << " error: " << errorCode << ' ';
#ifdef DLOGGER_H
		DString256 errorText;
		DLogger::FormatLastError(errorText, errorCode);
		msg << "\"" << errorText << "\"";
#endif
	}
	msg << "(";
	if (good()) 
		msg << "file size = " << getLength() << " pos = " << this->tell() << " ";
	msg << "file = \"" << getName() << "\" ";
	msg << ")";
	debugLog(msg);
}

void DFile::throwOnError(const char *cmt)
{
	if (error) {
		DString msg;
		makeErrorReport(msg, cmt, 0, error);;

		DFileException ex(*this);
		ex.setErrorCode(error);
		ex.setMessage(msg);
		throw ex;
		}
}

int DFile::logAndSaveError(const char *cmt, __int64 val, bool bCritical)
{	saveError();
	if (gfLoggingEnabled || (bThrowOnError && bCritical)) {
		DString256 msg;
		makeErrorReport(msg, cmt, val, error);
		
		if (bCritical && bThrowOnError) {
			DFileException ex(*this);
			ex.setErrorCode(error);
			ex.setMessage(msg);
			throw ex;
			}
		}
	return FAIL;
}

void DFile::logRetry(DWORD code, const char *cmt, __int64 val, bool bOK)
{
	if (gfLoggingEnabled) {
		DString256 msg;
		makeErrorReport(msg, cmt, val, code);
		debugLog(msg);
		debugLog("Retry result: ", bOK? "OK" : "FAILED");
		}
}


int DFile::good() const
{   return handle != (t_FileHandle) INVALID_HANDLE_VALUE;
}

#if defined(_WIN32)

void DFile::openHandle()
{   closeHandle();

    rwFlags = 0;
    if (flags.read)
        rwFlags |= GENERIC_READ;
    if (flags.write)
        rwFlags |= GENERIC_WRITE;
    shareMode = 0;
    if (flags.rshare)
        shareMode |= FILE_SHARE_READ;
    if (flags.wshare)
        shareMode |= FILE_SHARE_WRITE;

    int createMode;
    if (flags.trunc && flags.create)
        createMode = CREATE_ALWAYS;
    else if (flags.trunc)
        createMode = TRUNCATE_EXISTING;
    else if (flags.create)
        createMode = OPEN_ALWAYS;
    else
        createMode = OPEN_EXISTING;
        
	if (flags.restoreLastAccess) {
		// If we need to restore the last access time, try to open with write access to attributes.
		// If this fails, try again so the file still is opened.  In this case, preserving the
		// last access time is not possible, because we do not have a way to change the file's 
		// attributes.
		handle = CreateFileU8(name, rwFlags | FILE_WRITE_ATTRIBUTES, shareMode, 0, createMode,
			FILE_ATTRIBUTE_NORMAL, 0);
		if (good()) {
			// Microsoft API documentation for SetFileTime:
			// http://msdn.microsoft.com/en-us/library/windows/desktop/ms724933(v=vs.85).aspx
			// "To prevent file operations using the given handle from modifying the 
			// last access time, call SetFileTime immediately after opening the file handle 
			// and pass a FILETIME structure whose dwLowDateTime and dwHighDateTime members are both set to 0xFFFFFFFF."
			FILETIME noChangeAccess;
			noChangeAccess.dwHighDateTime = 0xffffffff;
			noChangeAccess.dwLowDateTime = 0xffffffff;
			SetFileTime(handle, 0, &noChangeAccess, 0);
			}	
		}			
	if (!good()) 
		handle = CreateFileU8(name, rwFlags, shareMode, 0, createMode,
			FILE_ATTRIBUTE_NORMAL , 0);
		
    if (good()) {
        openCount++;
        bShouldCloseHandle = true;
        getHandleInfo();
        }
    else
		saveError();
}

#else

void DFile::openHandle()
{
    closeHandle();

    // Sharing flags
    shareMode = 0;
#ifndef __UNIX__
    if (!flags.rshare && !flags.wshare)
        shareMode = SH_DENYRW ;
    else if (!flags.rshare)
        shareMode = SH_DENYRD;
    else if (!flags.wshare)
        shareMode = SH_DENYWR;
    else
        shareMode = SH_DENYNO;
#endif

    // Read/write flags
    rwFlags = 0;
    if (flags.read && flags.write)
        rwFlags = O_RDWR;
    else if (flags.read)
        rwFlags = O_RDONLY;
    else if (flags.write)
        rwFlags = O_WRONLY;

    if (flags.create)
        rwFlags |= O_CREAT;
    if (flags.trunc)
        rwFlags |= O_TRUNC;
    int permission = 0;
    if (flags.create)
    	permission = ( S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH); // 644 permission

#if defined(LARGEFILES) && !defined(__clang__)
    rwFlags |= O_LARGEFILE;
	#ifdef _ANDROID
    handle = ::open(name, O_BINARY | rwFlags | shareMode, permission);
	#else
    handle = ::open64(name, O_BINARY | rwFlags | shareMode, permission);
	#endif
#else
    handle = ::open(name, O_BINARY | rwFlags | shareMode, permission);
#endif

    if (good()) {
        openCount++;
        bShouldCloseHandle = true;
        getHandleInfo();
        }
    else
        logAndSaveError("openHandle");

}

#endif

// OS X deprecates fstat64 and stat64 and does not have lseek64 or ftruncate64
#ifdef __clang__
#ifndef fstat64
#define fstat64 fstat
#endif
#ifndef stat64
#define stat64 stat
#endif
#ifndef lseek64
#define lseek64 lseek
#endif
#ifndef ftruncate64
#define ftruncate64 ftruncate
#endif
#endif

DFilePosition DFile::getLengthFromHandle()
{	if (!good())
        return -1;

    #if defined(_WIN32)

    BY_HANDLE_FILE_INFORMATION bhfi;
    memset(&bhfi, 0, sizeof bhfi);
    if (!GetFileInformationByHandle(handle, &bhfi))
        return logAndSaveError("getLengthFromHandle");
    else {
		__int64 ret = ((__int64)(bhfi.nFileSizeHigh)) * (0x100000000LL) +
                       (__int64)(bhfi.nFileSizeLow);
        return ret;
        }

    #elif defined(LARGEFILES)
    struct stat64 s;
    if (fstat64(handle, &s) == SUCCESS)
		return s.st_size;
	else
		return logAndSaveError("getLengthFromHandle");
    #else
            #warning (Using 32-bit file offsets)
    struct stat s;
    if (fstat(handle, &s) == 0)
    	return s.st_size;
	else
		return logAndSaveError("getLengthFromHandle");
	#endif
}

void DFile::getLastAccessDate(dtsFileDate& d)
{
#ifdef _WIN32
	FILETIME lastAccess;
	memset(&lastAccess, 0, sizeof lastAccess);
	GetFileTime(getHandle(), 0, &lastAccess, 0);
	DFileDate fd;
	fd.convertFrom(lastAccess);
	d.copy(fd);
#else
	d.clear();
#endif
}

#ifdef _WIN32
int DFile::setLastAccessDate(dtsFileDate& d)
{

	FILETIME lastAccess;
	DFileDate fd;
	fd.copy(d);
	fd.convertTo(lastAccess, false);
	if (SetFileTime(getHandle(), 0, &lastAccess, 0))
		return SUCCESS;
    return FAIL;
}
#else
int DFile::setLastAccessDate(dtsFileDate& )
{   return FAIL;
}

#endif



void DFile::getHandleInfo()
{   if (!good())
        return;

#if defined(_WIN32)
    BY_HANDLE_FILE_INFORMATION bhfi;
    memset(&bhfi, 0, sizeof bhfi);
    if (!GetFileInformationByHandle(handle, &bhfi))
        logAndSaveError("getHandleInfo");
    else {
        length = ((__int64)(bhfi.nFileSizeHigh)) * (0x100000000LL) +
                       (__int64)(bhfi.nFileSizeLow);

        createdDate.convertFrom(bhfi.ftCreationTime);
        modifiedDate.convertFrom(bhfi.ftLastWriteTime);
		savedFileTimes.getFrom(bhfi);    
	    }

#else

    #if defined(LARGEFILES)
    struct stat64 s;
    if (fstat64(handle, &s) != SUCCESS)
		logAndSaveError("getHandleInfo - fstat64");
    #else
        #warning (Using 32-bit file offsets)
    struct stat s;
    if (fstat(handle, &s) != SUCCESS)
		logAndSaveError("getHandleInfo - fstat64");
    #endif
    length = s.st_size;
    time_t tt = s.st_mtime;
    struct tm *t = localtime(&tt);
    modifiedDate.second = t->tm_sec;
    modifiedDate.hour = t->tm_hour;
    modifiedDate.minute = t->tm_min;
    modifiedDate.day= t->tm_mday;
    modifiedDate.month = t->tm_mon+1;
    modifiedDate.year = t->tm_year + 1900;
    inode = s.st_ino;
    device = s.st_dev;
#endif
}

int DFile::seek(DFilePosition pos)
{    filePosition = pos;
#ifdef _WIN32
	if (SetFilePointer64(handle, pos, SEEK_SET))
		return SUCCESS;
	else
		return FAIL;
#else
    #ifdef LARGEFILES
    return lseek64(handle, pos, SEEK_SET) < 0? FAIL : SUCCESS;
    #else
        #warning (Using 32-bit file offsets)
    return lseek(handle, pos, SEEK_SET) < 0? FAIL : SUCCESS;
    #endif
#endif
}


DFilePosition DFile::os_write(const void *buf, DFilePosition len)
{   unsigned long bytesWritten = 0;
#ifdef _WIN32
	BOOL bOK = WriteFile(handle, buf, (int) len, &bytesWritten, 0);
	int retryCount = defaultRetryCount;	
	while(!bOK && bRetryOnError && (retryCount-- > 0)) {
		DWORD errorCode = GetLastError();
		Sleep(100);
		seek(filePosition);
		bOK = WriteFile(handle, buf, (int) len, &bytesWritten, 0);
		
		// For diagnostic purposes only, log the error code that occurred on the last call, and whether the retry worked
		logRetry(errorCode, "os_write", len, makeBool(bOK));
		}

	if (!bOK) {
		bytesWritten = 0;
		logAndSaveError("os_write", len, true);
		}

#else
    bytesWritten = ::write(handle, buf, (int) len);
    if (bytesWritten < len)
    	logAndSaveError("os_write (linux)", len, true);
#endif
    return bytesWritten;
}

DFilePosition DFile::os_read(void *buf, DFilePosition len)
{
#ifdef _WIN32
	DWORD bytesRead = 0;
	BOOL bOK = ReadFile(handle, buf, (int) len, &bytesRead, 0);
	
	int retryCount = defaultRetryCount;
	while(!bOK && bRetryOnError && (retryCount-- > 0)) {
		DWORD errorCode = GetLastError();
		Sleep(100);
		seek(filePosition);
		bOK = ReadFile(handle, buf, (int) len, &bytesRead, 0);;
		logRetry(errorCode, "os_read", len, makeBool(bOK));
		}
    if (!bOK) {
        bytesRead = 0;
        logAndSaveError("os_read", len, true);
        }
#else
    DFilePosition bytesRead = 0;
   bytesRead = ::read(handle, buf, (int) len);
    if (bytesRead < 0) {
    	bytesRead = 0;
    	logAndSaveError("os_read (linux)", len, true);
    }
#endif
	if (bytesRead < len) {
		memset((char *)buf + bytesRead, 0, (size_t) (len-bytesRead));
		}
    return bytesRead;
}

int DFile::getOpenFileCount()
{   return openCount - closeCount;
}


#ifdef _WIN32

static void SetFileTimes(const char *filename, FileTimes& fileTimes)
{
    HANDLE hFile = DFile::CreateFileU8(filename, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL );
    if (hFile != INVALID_HANDLE_VALUE) {
        // put back the original last accessed date and time
        fileTimes.set(hFile);
        CloseHandle( hFile );
        }
    else {
        // Remove read-only flag, try again to set the times, and restore the flag
        DWORD dwFA = DFile::GetFileAttributesU8(filename);
        if ((dwFA != 0xFFFFFFFF) && (dwFA & FILE_ATTRIBUTE_READONLY)) {
            dwFA &= (~FILE_ATTRIBUTE_READONLY); 
            if (DFile::SetFileAttributesU8(filename, dwFA)) {
                hFile = DFile::CreateFileU8(filename, GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
                if (hFile != INVALID_HANDLE_VALUE) {
					 fileTimes.set(hFile);
                     CloseHandle(hFile);
                    }
                // Restore the read-only flag
                dwFA |= FILE_ATTRIBUTE_READONLY; 
                DFile::SetFileAttributesU8(filename, dwFA);
               }

            }
         }
}




#endif


int DFile::setModifiedDate(dtsFileDate& d)
{
#ifdef _WIN32
	DFileDate fileDate = d;
	FILETIME ft;
	fileDate.convertTo(ft, false);
	if (SetFileTime(handle, NULL, NULL, &ft))
		return SUCCESS;
	else
		return FAIL;
#else
	return FAIL;
#endif
}

int DFile::setDates(dtsFileDate& created, dtsFileDate& modified)
{
#ifdef _WIN32
    DFileDate fdCreated = created;
	DFileDate fdModified = modified;
	FILETIME ftCreated, ftModified;
	fdCreated.convertTo(ftCreated, false);
	fdModified.convertTo(ftModified, false);
	if (SetFileTime(handle, &ftCreated, NULL, &ftModified))
		return SUCCESS;
	else
		return FAIL;
#else
    return FAIL;
#endif
}


int DFile::save()
{
    closeHandle();
    // Don't want to re-truncate the file when we reopen it
    int oldTrunc = flags.trunc;
    flags.trunc = false;
    openHandle();
    flags.trunc = oldTrunc;

    if (good())
        return SUCCESS;
    else
        return FAIL;
}

void DFile::closeHandle()
{   if (!good())
        return;
	if (!bShouldCloseHandle) {
		handle = (t_FileHandle) INVALID_HANDLE_VALUE;
		return;
		}

    closeCount++;

#ifdef _WIN32
    if (!CloseHandle(handle))
        logAndSaveError("closeHandle", 0, true);
        
#else
    if (::close(handle))
    	logAndSaveError("closeHandle", 0, true);

#endif
    handle = (t_FileHandle) INVALID_HANDLE_VALUE;
	bShouldCloseHandle = false;
}

//
//   setLength truncates a DFile to a certain length.
//   If we have write access, it will truncate the
//   actual DFile on disk.  Otherwise it will
//   just set a limit on its own access to the DFile.
//
int DFile::setLength(DFilePosition len)
{
    if (!flags.write)
        return FAIL;

#ifdef _WIN32
    seek(len);
    if (!SetEndOfFile(handle))
        return logAndSaveError("setLength", len);
#elif defined(__UNIX__)
    #ifdef _ANDROID
    if (ftruncate(handle, len) != 0) {
		logAndSaveError("ftruncate", len);
            return FAIL;
            }
    #else
    if (ftruncate64(handle, len) != 0) {
		logAndSaveError("ftruncate64", len);
            return FAIL;
            }
    #endif
#else
    if (chsize(handle, len))
    	return FAIL;
#endif
    length = len;
    return SUCCESS;
}

// Close and reopen handle
void DFile::flush()
{	closeHandle();
	// Don't want to re-truncate the file when we reopen it
    int oldTrunc = flags.trunc;
	int oldCreate = flags.create;
	DFilePosition savePos = tell();
    flags.trunc = false;
	flags.create = false;
	openHandle();
    flags.trunc = oldTrunc;
	flags.create = oldCreate;
	seek(savePos);
}


#define LongNamePrefix		L"\\\\?\\"
#define LongNamePrefixUNC	L"\\\\?\\UNC\\"

void DFile::fixLongFilename(DWString& fn)
{	if (fn.getLength() < 255)
		return;
	if (!wcsncmp(fn, LongNamePrefix, 4))
		return;
	if (DFilename::isUnc(fn)) {
		fn.remove(0, 2);
		fn.insert(LongNamePrefixUNC, 0);
	}
	else
		fn.insert(LongNamePrefix, 0);
	}



#ifdef _WIN32



HANDLE DFile::CreateFileU8(
  const char *fileName,          // pointer to name of the file
  DWORD dwDesiredAccess,       // access (read-write) mode
  DWORD dwShareMode,           // share mode
  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                               // pointer to security attributes
  DWORD dwCreationDisposition,  // how to create
  DWORD dwFlagsAndAttributes,  // file attributes
  HANDLE hTemplateFile         // handle to file with attributes to
                               // copy
    )
{
    DWString wName;
    wName.storeUtf8AsUnicode(fileName);
    fixLongFilename(wName);
    return CreateFileW(wName, dwDesiredAccess, dwShareMode, lpSecurityAttributes,
        dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

void DFile::fixLongFilename(DString& name)
{	DWString ws;
	ws.storeUtf8AsUnicode(name);
	fixLongFilename(ws);
	name.clear();
	name.storeUnicodeAsUtf8(ws);
}


DWORD DFile::GetFileAttributesU8(const char *fn)
{   DWString uname;
    uname.storeUtf8AsUnicode(fn);
    fixLongFilename(uname);
    return GetFileAttributesW(uname);
}

BOOL DFile::SetFileAttributesU8(const char *fn, DWORD fa)
{   DWString uname;
    uname.storeUtf8AsUnicode(fn);
    fixLongFilename(uname);
    return SetFileAttributesW(uname, fa);
}

#ifdef HAVE_WINAPI
DWORD DFile::GetModuleFilenameU8(HMODULE hModule, DString& dest)
{	
	wchar_t buf[4*MAX_PATH];
	memset(buf, 0, sizeof buf);
	DWORD ret = GetModuleFileNameW(hModule, buf, 4*MAX_PATH);
	dest.storeUnicodeAsUtf8(buf);
	return ret;
}
#endif


#ifndef INVALID_FILE_ATTRIBUTES
#	define INVALID_FILE_ATTRIBUTES ((DWORD)-1)
#endif


BOOL DFile::CopyFileU8(const char *existingFile, const char *newFile, BOOL bFailIfExists)
{	DWString wExisting;
	wExisting.storeUtf8AsUnicode(existingFile);
	DWString wNew;
	wNew.storeUtf8AsUnicode(newFile);
	
	fixLongFilename(wExisting);
	fixLongFilename(wNew);
	BOOL ret = CopyFileW(wExisting, wNew, bFailIfExists);
	if (!ret && gfLoggingEnabled) {
		DString msg;
		msg << "CopyFile failed: " << "\"" << existingFile << "\" --> \"" << newFile << "\" ";
		debugLog(msg);
		}
	return ret;
}



#ifndef INVALID_SET_FILE_POINTER
#define INVALID_SET_FILE_POINTER ((DWORD)-1)
#endif

BOOL DFile::SetFilePointer64(HANDLE hFile, DFilePosition pos, DWORD moveMethod)
{
	LARGE_INTEGER li, newPos;
	li.QuadPart = pos;
    BOOL ret = SetFilePointerEx(hFile, li, &newPos, moveMethod);
	return ret;
}
#endif


BOOL DFile::DeleteFileU8(const char *fn)
{
#ifdef _WIN32
	DWString uname;
    uname.storeUtf8AsUnicode(fn);
    fixLongFilename(uname);
    return DeleteFileW(uname);
#else
	return unlink(fn);
#endif
}

bool DFile::ExistsU8(const char *fn)
{
#ifdef _WIN32
	DWORD attr = GetFileAttributesU8(fn);
#ifdef _WINRT
	debugLog("ExistsU8: ", fn);
	debugLog("GetFileAttributes returns:", attr);
#endif
	if (attr == INVALID_FILE_ATTRIBUTES)
		return false;
	else
		return true;
#else
	struct stat statbuf;
	if (lstat(fn, &statbuf) >= 0)
		return true;
	else
		return false;
#endif
}

bool DFile::IsFileU8(const char *fn)
{
#ifdef _WIN32
	DWORD attr = GetFileAttributesU8(fn);
	if (attr == INVALID_FILE_ATTRIBUTES)
		return false;
	if (attr & FILE_ATTRIBUTE_DIRECTORY)
		return false;
	return true;
#else
	struct stat statbuf;
	if (lstat(fn, &statbuf) >= 0) {
		if (!S_ISDIR(statbuf.st_mode))
			return true;
		}
	return true;
#endif
}

BOOL DFile::CreateDirectoryRecursiveU8(const char *dir)
{
	if (IsDirectoryU8(dir))
		return true;
    if (CreateDirectoryU8(dir))
		return true;

    DFilename fn = dir;
    fn.rmTail();
    if (!strcmp(fn, dir)) 
		return false;
	
    CreateDirectoryRecursiveU8(fn);
    return DFile::CreateDirectoryU8(dir);
  
}

BOOL DFile::CreateDirectoryU8(const char *fn)
{
#ifdef _WIN32
	DWString uname;
    uname.storeUtf8AsUnicode(fn);
    fixLongFilename(uname);
    return CreateDirectoryW(uname, 0);
#else
	int permissions = S_IXGRP | S_IXOTH | S_IRGRP | S_IROTH |S_IRUSR | S_IWUSR | S_IXUSR;
    int ret = ::mkdir(fn, permissions);
	if (ret)
		return false;
	else
		return true;
#endif
}

bool DFile::IsDirectoryU8(const char *fn)
{
#ifdef _WIN32
	DWORD attr = GetFileAttributesU8(fn);
	if (attr == INVALID_FILE_ATTRIBUTES)
		return false;
	if (attr & FILE_ATTRIBUTE_DIRECTORY)
		return true;
	return false;
#else
	struct stat statbuf;
	if (lstat(fn, &statbuf) >= 0) {
		if (S_ISDIR(statbuf.st_mode))
			return true;
		}
	return false;
#endif


}

#ifdef _WIN32
/*
	Copy a file, preserving the Last Access time on the original and the
	Last Access, Creation, and Last Modified time on the copy.
	
	This requires that we set the Last Access time on the original.
*/
BOOL DFile::CopyPreservingTimes(const char *existingFile, const char *newFile, BOOL bFailIfExists) 
{
	
	
    HANDLE hFile = DFile::CreateFileU8(existingFile, GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL );
    FileTimes savedFileTimes;
    if (hFile != INVALID_HANDLE_VALUE) {
		savedFileTimes.get(hFile);
		CloseHandle(hFile);
		}
	
	BOOL ret = CopyFileU8(existingFile, newFile, bFailIfExists);
	if (!ret)
		return ret;
	
	if (savedFileTimes.bHaveData) {

		SetFileTimes(newFile, savedFileTimes);
		}
	return ret;
}

#endif

#ifdef _WIN32
static void copyFileTime(FILETIME& dest, const FILETIME& source)
{	dest.dwHighDateTime = source.dwHighDateTime;
	dest.dwLowDateTime = source.dwLowDateTime;
}

void FileTimes::get(t_FileHandle fh)
{	clear();
	bHaveData = makeBool(GetFileTime(fh, &created, &lastAccess, &modified));
	bHaveCreated = true;
	bHaveModified = true;
	bHaveLastAccess = true;
}
void FileTimes::set(t_FileHandle fh)
{	FILETIME *pCreated = &created;
	FILETIME *pLastAccess = &lastAccess;
	FILETIME *pModified = &modified;
	if (!bHaveCreated)
		pCreated = 0;
	if (!bHaveLastAccess)
		pLastAccess = 0;
	if (!bHaveModified)
		pModified = 0;
	SetFileTime(fh, pCreated, pLastAccess, pModified);
}
void FileTimes::getFrom(const BY_HANDLE_FILE_INFORMATION& bhfi)
{	copyFileTime(created, bhfi.ftCreationTime);
	copyFileTime(modified, bhfi.ftLastWriteTime);
	copyFileTime(lastAccess, bhfi.ftLastAccessTime);
	bHaveCreated = true;
	bHaveLastAccess = true;
	bHaveModified = true;
	bHaveData = true;
}

#endif

