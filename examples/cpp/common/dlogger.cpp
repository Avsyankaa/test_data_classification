#include <stdafx.h>
#include <dlogger.h>
#include <xmlmaker.h>
#include <dthread.h>


#ifdef HAVE_WINAPI
	#include <oleauto.h>
    #include <registrykey.h>
#endif

#ifdef USE_DTSEARCH_NAMESPACE
    using namespace dtSearch;
    namespace dtSearch {
#endif

#define True  1
#define False 0

const char quote = '\"';
static const char *crlf = "\r\n";

static const char *checkString(const char *s)
{	if (s)
		return s;
	else
		return "";
}

static const wchar_t *checkString(const wchar_t *s)
{	if (s)
		return s;
	else
		return L"";
}

DLogger *DLogger::theDefaultLog = 0;

#ifdef _UNICODE
void __OutputDebugString(const char *s)
{
    DWString ws(s);
    OutputDebugString(ws);
}
#else
    #define __OutputDebugString OutputDebugString
#endif



static void appendInt(DString& buf, long v)
{
    buf.commaNum(v);

    buf << " (0x";
	buf.hex(v, " ");
	buf << ")";
}

static void appendInt(DString& buf, __int64 v)
{
    buf.commaNum(v);
    buf << " (0x";
    buf.hex(v, " ");
    buf << ")";
}

const char *eq = "=================================================================================";

void DLogger::trace(const char *s, __int64 v, __int64 v2)
{
	DString80 t;
	t << s << " ";
	t.commaNum(v);
	t << " ";
	t.commaNum(v2);
	trace(t);
}

void DLogger::trace(const char *s, __int64 v)
{
	DString80 t;
	t << s << " ";
	t.commaNum(v);
	trace(t);
}

void DLogger::trace(const char *s)
{
	#ifdef _WIN32
	__OutputDebugString(s);
	#endif
}

void DLogger::trace(const char *s, const char *t)
{	DString x;
	x << s << " " << t;
	trace(x);
}
	


DLogger::DLogger(const char *logFn, const char *dir, int flags, const char *env, const char *aHeaderString ) :
fCommit(flags & dlLogCommit ? true : false),
fAppend(flags & dlLogAppend ? true : false),
fLogging(false),
fLogTime(false),
outputFn(0),
outputFn2(0),
fOutputToDbwin(false),
fShowThreadId(false),
threadTable(250)
{
    	
	threadTable.setDefaultValue(0);
	
    timer.start();

    if (aHeaderString)
        headerString = aHeaderString;

    // Get path for log.  Can be a directory or environment variable
    // identifying a directory.  If blank, use Windows temp directory.
    if (!dir)
        dir = "";
    if (!logFn || !*logFn)
        logFn = "debug.log";

    const char *p = dir;
    if (dir[0] == '%') {
        char envDir[256];
        strCopy(envDir, dir+1, sizeof envDir);
        char *q = strchr(envDir, '%');
        if (q)
            *q = '\0';
		DString256 val;
        p = val.safeGetEnv(envDir);
        if (!p)
            p = dir;
    }
    if (!p || !*p)
        logName.makeTempName(logFn);
    else {
        logName = p;
        logName.addTail(logFn);
    }

    if (flags & dlLogStart)
        startLogging();
	else if ((flags & dlLogStartIfEnv) && !strIsBlank(env)) {
    	DString256 val;
        if (val.safeGetEnv(env)) {
			 trace("Starting debug logging due to environment variable", env);
             startLogging();
            }
    }
#ifdef HAVE_WINAPI
    else if (flags & dlLogStartIfReg) {
        CRegistryKey key;
        if (key.openRead(HKeyCurrentUser, env) == SUCCESS) {
            long fDebug = 0;
            key.getValue("Debug", fDebug);
            if (fDebug) {
				trace("Starting debug logging due to registry key", env);
                startLogging();
				}
        }
    }
#endif
    fOutputToDbwin = ((flags & dtsLogToDbwin) ? true : false);
    if (!theDefaultLog)
        theDefaultLog = this;
}


DLogger::~DLogger()
{   close();
    if (theDefaultLog == this)
        theDefaultLog = 0;
}

void DLogger::setOutput(void (*pOutputFn)(const char *))
{
    outputFn = pOutputFn;
    checkIfLogging();
}

void DLogger::checkIfLogging()
{
    if (outputFn || logFile.good() || outputFn2 || fOutputToDbwin) {
        fLogging = true;
#if defined(_WIN32)
        trace("dtSearch Engine: Logging enabled");
#endif

        }
    else
        fLogging = false;
}

void DLogger::setOutput2(void (*pOutputFn)(void *pData, const char *), void *aPData)
{
    outputFn2 = pOutputFn;
    pOutputData = aPData;
    checkIfLogging();
}

void DLogger::hexDump(DString& x, const char *msg, const void *pv, ptrdiff_t bytes)
{   const char *p = (const char *)pv;
    x << msg << ": ";
    for (int i = 0; i < bytes; ++i) {
        unsigned char c = (unsigned char) p[i];
        PadHexNum b(c, 2);
        x << b;
    }
}


void DLogger::hexDumpFormatted(DString &out, const char *data, ptrdiff_t len, int nIndent)
{    if (!data || (len < 0))
        return;

    DString80 indent;
    indent.padTo(nIndent, ' ');

    out << indent;
    DString256 s;
    out << "0000: ";
    for (ptrdiff_t i = 0; i < len; ++i) {

        unsigned char v = data[i];
        if ((v < 128) && (v >= 32))
            s << v;
        else
            s << '.';
        PadHexNum h(v, 2);
        out << h;
        if (i % 4 == 3)
            out << ' ';
        if (i % 32 == 31) {
            out << " " << s << crlf << indent;
            PadHexNum offset((int) i+1, 4);
            out << offset << ": ";
            s.clear();
            }
        }
    if (len > 31) {
        for (size_t i = len; i % 32 != 0; ++i) {
            out << "  ";
            if (i % 4 == 3)
                out << " ";
            }
        }
    out << " " << s << crlf;
}



//  Un-suspend logging if it was suspended, and try to open a log
//  file if logging is not on at all.
void DLogger::startLogging()
{	trace("StartLogging");
    checkIfLogging();

    if (!logFile.isOpen() && !strIsBlank(logName)) {
		trace("Opening log file", logName);
        open(logName, fAppend);
        if (logFile.good())
			trace("Log file opened OK");
		else 
			trace("Unable to open log file, error = ", logFile.getLastError());
        }
}

void DLogger::setLogName(const char *n)
{	if (DFilename::isRelative(n))
		logName.makeTempName(n);
	else
		logName = n;
	trace("SetLogName", n);
	bool wasOpen = fLogging;
	close();
	if (wasOpen)
		open(logName, false);
}


bool DLogger::startLogging(const char *filename, bool appendFlag)
{	trace("StartLogging", filename);
	
	close();
	setLogName(filename);
	bool ret = true;
	
    if (!strIsBlank(logName)) {
		trace("Opening log file", logName);
        open(logName, appendFlag);
        if (logFile.good()) 
			trace("Log file opened OK");
		else {
			trace("Unable to open log file, error = ", logFile.getLastError());
			ret = false;
			}
        }
       return ret;
}


//  Clear logging flag but do not close anything
void DLogger::stopLogging()
{   fLogging = False;
}

void DLogger::close()
{   logFile.close();
    fLogging = false;
}

void DLogger::suspend()
{	logFile.close();
}

void DLogger::resume()
{
	logFile.open(logName, F_WRITE | F_CREATE | F_CONCURRENT);
	logFile.seek(logFile.getLength());
}

int DLogger::open(const char *s, bool afAppend)
{   fAppend = afAppend;

    close();
    

    int f = F_WRITE | F_CREATE | F_CONCURRENT;
    if (!fAppend)
        f |= F_TRUNC;

    if (!strIsBlank(s))
        logName = s;

    if (logFile.isOpen())
        logFile.close();

    logFile.open(logName, f);

    if (!logFile.good()) {
		DString ext;
		logName.getExt(ext);
		logName.rmExt();
		__int64 t = (long) time(0);
		__int64 c = (long) clock();
		__int64 num = 10000 * (t % 10000) + (c % 10000);
		logName << (num);
		logName.setExt(ext);
		logFile.open(logName, f);
		}
	if (!logFile.good()) {
        fLogging = false;
        trace("Unable to open log file ", logName);
        trace(" error= ", GetLastError());
        return FAIL;
    }
    else if (fAppend)
        logFile.seek(logFile.getLength());
    else
		// Start with UTF-8 BOM
		logFile.write("\xef\xbb\xbf\r\n", 5);

    fLogging = true;
    return SUCCESS;
}

void DLogger::setTimeLogging(bool v)
{
    fLogTime = v;
}

void DLogger::setCommit(bool v)
{   fCommit = v;
}

void DLogger::log(const wchar_t *s)
{	if (s) {
		DString tmp = s;
		log(tmp);
		}

}

void DLogger::truncate()
{	close();
    open(logName, false);
}

void DLogger::log(const char *s)
{   if (!fLogging || !s)
        return;


    if (*s == '$') {
		if (!_stricmp(s, "$trunc")) {
			truncate();
			return;
			}
		}

    if (strchr(s, '\n')) {
        DStringSet lines;
        lines.tokenize(s, "\n\r", false);
        for (int i = 0; i < lines.getCount(); ++i)
            log(lines.getString(i));
        return;
    }

	DString256 tempBuf;
	

    if (fLogTime) {
		char t[80];
		timer.click();
        timer.deltaImage(t, sizeof t, 2);
        tempBuf << t << " -- ";
	    }

    int iThread = getThreadNum();
    if (threadCount > 1) 
        tempBuf << "[" << iThread << "] " << ' ';
		
	int indent = getIndent(iThread);
	if (indent > 10)
		indent = 10;
	for (int i = 0; i < indent; ++i)
        tempBuf << '\t';

    tempBuf << s;
    if (tempBuf.last() != '\n')
        tempBuf << crlf;
        
    SerializeBlock;

    writeText(tempBuf);

    EndSerializedBlock;
}

#ifdef __UNIX__
static __int64 GetCurrentThreadId()
{	return (__int64) pthread_self();
}
#endif


int DLogger::getThreadNum()
{
#if defined(__UNIX__) || defined( _MT)
    SerializeBlock

    __int64 ht = GetCurrentThreadId();
    int i = threadTable.find(ht);
    if (i > FAIL)
        return i+1;

    threadTable.append(ht);
    threadCount++;
	
	return threadCount;
    EndSerializedBlock;
#else
    return 1;
#endif
}

void DLogger::beginIndent() {
	indentTable.beginIndent(getThreadNum());
    }
void DLogger::endIndent() {
	indentTable.endIndent(getThreadNum());
    }

int DLogger::getIndent(int iThread)
{	return indentTable.getIndent(iThread);
}

void DLogger::writeText(const char *s)
{   if (!fLogging)
        return;

    if (outputFn)
        outputFn(s);
    if (outputFn2)
        outputFn2(pOutputData, s);
    if (logFile.good()) {
        logFile.write(s, (long) strlen(s));
        if (fCommit && strchr(s, '\n')) {
            close();
            open(logName, true);
        }
    }
#if defined(_WIN32)
    if (fOutputToDbwin)
        __OutputDebugString(s);
#endif
}

void DLogger::log(const char *s, const char *t)
{   if (fLogging) {
        DString256 buf;
        buf << checkString(s) << ' ' << checkString(t);
        log(buf);
    }
}


void DLogger::log(const char *s, void *p)
{   if (fLogging && s) {
        uintptr_t l = (uintptr_t) p;
        DString256 buf;
        buf << s << ' ' << l;
        log(buf);
    }
}

void DLogger::log(const char *s, const char *t, const char *u)
{   if (fLogging && s) {
        DString256 buf;
        buf << checkString(s) << ' ' << checkString(t) << ' ' << checkString(u);
        log(buf);
    }
}


void DLogger::log(const char *s, const char *t, const char *u, const char *v)
{   if (fLogging && s) {
        DString256 buf;
        buf << checkString(s) << ' ' << checkString(t) << ' ' << checkString(u) << ' ' << checkString(v);
        log(buf);
    }
}


void DLogger::log(const char *s, long v)
{   if (fLogging && s) {
        DString256 buf;
        buf << s << ' ';
        appendInt(buf, v);
        log(buf);
    }
}

void DLogger::log(const char *s, __int64 v)
{   if (fLogging && s) {
        DString256 buf;
        buf << s << ' ';
        appendInt(buf, v);
        log(buf);
    }
}

void DLogger::log(const char *s, __int64 a, __int64 b)
{   if (fLogging && s) {
        DString256 buf;
        buf << s << ' ';
        appendInt(buf, a);
        buf << ' ';
        appendInt(buf, b);
        log(buf);
    }
}

void DLogger::FormatLastError(DString& dest, long err)
{
#ifdef WIN32
    if (err == -1)
        err = GetLastError();
    if (err != 0) {
		TCHAR buf[1024];
		memset(buf, 0, sizeof(buf));
		FormatMessage(
					 FORMAT_MESSAGE_FROM_SYSTEM |
					 FORMAT_MESSAGE_IGNORE_INSERTS,
					 NULL,
					 err,
					 MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
					 buf, _countof(buf),
					 0);
		dest = UiToUtf8(buf);
		dest.rtwhite();
		dest << " (" << err << ")";

		}
#else
    if (err == -1)
        err = errno;
    dest << "Error: " << err;
    const char *s = strerror(err);
    if (s)
        dest << " = " << quote << s << quote;
#endif
}

void DLogger::logLastError(const char *aMsg, const char *a, const char  *b)
{   if (!fLogging)
        return;
#ifdef _WIN32
    long err = GetLastError();
#else
    long err = errno;
#endif

    DString msg;
    msg << checkString(aMsg) << ' ' << checkString(a) << ' ' << checkString(b);
    msg << " Error = [" << err << "] ";
    DString sysmsg;
    FormatLastError(sysmsg, err);
    msg << " = \"" << sysmsg << "\"";
    debugLog(msg);
}



void DLogger::logFileContents(const char *fn, const char *what)
{   if (!fLogging)
        return;

    if (!what)
        what = "";

    DString begin, end;
    begin << "<" << what << ": " << fn << ">";
    end << "</" << what << ": "  << fn << ">";
    log(begin);
    DString contents;
    contents.getFrom(Utf8ToUi(fn));
    if (contents.isBlank()) {
        long err = GetLastError();
        DString msg;
        FormatLastError(msg, err);
        debugLog("Error: ", err);
        debugLog("Message: ", msg);
    }
    else
        writeText(contents);
    log(end);
}

#ifdef HAVE_WINAPI

const char nl = '\n';

static void makeFullpath(HKEY hKey, const char *name, DString& fullpath)
{
    if (fullpath.isBlank()) { 
        if (hKey == HKEY_LOCAL_MACHINE) 
            fullpath << "HKEY_LOCAL_MACHINE"; 
        else if (hKey == HKEY_CURRENT_USER) 
            fullpath << "HKEY_CURRENT_USER"; 
        if (fullpath.getLength()) 
            fullpath << '\\'; 
        fullpath << name; 
        } 
		
}


void DLogger::listRegistryValue(HKEY hKey, const char *name, const char *valueName, DString& dest)
{ 	
	DString fullpath;
	makeFullpath(hKey, name, fullpath);

    CRegistryKey key; 
    if (key.openRead(hKey, name) == FAIL) 
        return; 
    DString val;
	key.getValue(valueName, val);
    dest << "[" << fullpath << "] "; 
    dest << valueName << " = " << val;
}

void DLogger::listRegistryValue(const char *name, const char *valueName, DString& dest)
{ 	

    CRegistryKey key; 
    if (key.openRead(name) == FAIL) 
        return; 
    DString val;
	key.getValue(valueName, val);
    dest << "[" << name << "] "; 
    dest << valueName << " = " << val;
}

	

void DLogger::listRegistryKeyRecursive(HKEY hStartKey, const char *name, DString& dest, const char *pFullPath) 
{
	if (strIsBlank(name))
        return; 

    CRegistryKey key; 
    if (key.openRead(hStartKey, name) == FAIL) 
        return; 
	
	DString fullpath;
	if (pFullPath)
		fullpath = pFullPath;
	makeFullpath(hStartKey, name, fullpath);

    DString def; 
    key.getValue("", def); 

    DStringSet names, values; 
    key.getValues(names, values); 
    int i; 
    dest << "[" << fullpath << "]"; 
    if (!def.isBlank()) 
        dest << " = " << quote << def << quote; 
    dest << nl; 
    for (i = 0; i < names.getCount(); ++i) { 
        dest << "    "; 
        dest << quote << names.getString(i) << quote << " = "; 
        const char *s = values.getString(i); 
        if (!_strnicmp(s, "dword:", 6)) 
            dest << s; 
        else { 
            dest << quote; 
            for (const char *val = values.getString(i); val && *val; ++val) { 
                if (*val == quote) 
                    dest << '\\' << quote; 
                else 
                    dest << *val; 
                } 
            dest << quote; 
            } 
        dest << nl; 
        } 
    dest << nl; 
    int ret = SUCCESS; 
    for (i = 0; ret != FAIL; ++i) { 
        DString subKey; 
        DString subFullpath; 
        ret = key.enumSubKey(i, subKey); 
        subFullpath << fullpath << "\\" << subKey; 
        if (ret != FAIL) 
            listRegistryKeyRecursive(key.getHandle(), subKey, dest, subFullpath); 
        } 
}

#endif

#if defined(MfcAvailable) && defined(_WIN32)
void DLogger::logFolder(const char *folder, const char *filter)
{
	DFilename toFind = folder;
    toFind.addTail(filter);
	Utf8ToUi csToFind(toFind);
    CFileFind finder;
    BOOL bWorking = finder.FindFile(csToFind);
    log("----------------------------------------------");
    log("Directory listing: ", folder);
    log("----------------------------------------------");
    while (bWorking) {
        bWorking = finder.FindNextFile();
        CString csname = finder.GetFileName();
        CString cspath = finder.GetFilePath();
        CString csItem;
        #ifdef CFileGetStatus_H
        CFileStatus fs;
        CFile_GetStatus(cspath, fs);
        CString csDate =	fs.m_mtime.Format(_T("%m/%d/%Y %H:%M:%S"));
        if (fs.m_attribute & CFile::directory)
            csItem.Format(_T("%24.24s  %10.10s  "), csDate, _T("<dir>"));
        else {
            csItem.Format(_T("%24.24s  %10d  "), csDate, fs.m_size);
            }
        #else

        #endif
        csItem += csname;
        log(csItem);
        }
    log("----------------------------------------------");
}

void DLogger::findFilesRecursive(const char *folder, const char *filter, DStringSet& dest)
{	
	DFilename toFind = folder;
    toFind.addTail("*");
	Utf8ToUi csToFind(toFind);
    CFileFind finder;
    BOOL bWorking = finder.FindFile(csToFind);
    while (bWorking) {
        bWorking = finder.FindNextFile();
        CString csname = finder.GetFileName();
        CString cspath = finder.GetFilePath();
        DString fn = UiToUtf8(csname);
        if (striMatches(fn, filter) && !finder.IsDots() && !finder.IsDirectory()) 
			dest.append(UiToUtf8(cspath));
		if (finder.IsDirectory() && !finder.IsDots()) 
			findFilesRecursive(UiToUtf8(finder.GetFilePath()), filter, dest);
			
        }
}




#else
void DLogger::logFolder(const char * /*folder*/, const char * /*filter*/)
{
}

#endif




void DLogger::logToDefault(const char *s, const char *a, const char *b)
{
    if (gfLoggingEnabled)
        theDefaultLog->log(s, a, b);
}

void DLogger::logToDefault(const char *s, const char *a, const char *b, const char *c)
{
    if (gfLoggingEnabled)
        theDefaultLog->log(s, a, b, c);
}

void DLogger::logToDefault(const char *s, const wchar_t *a, const wchar_t *b)
{
    if (gfLoggingEnabled)
        theDefaultLog->log(s, a, b);
}



void DLogger::logToDefault(const char *s, const char *t)
{
    if (gfLoggingEnabled)
        theDefaultLog->log(s, t);
}

void DLogger::logToDefault(const char *s, long v)
{
    if (gfLoggingEnabled)
        theDefaultLog->log(s, v);
}

void DLogger::logToDefault(const char *s, __int64 v)
{
	if (gfLoggingEnabled)
		theDefaultLog->log(s, v);
}

void DLogger::logToDefault(const char *s, unsigned __int64 v)
{
	if (gfLoggingEnabled)
		theDefaultLog->log(s, (__int64) v);
}

void DLogger::logToDefault(const char *s, unsigned long v)
{	__int64 t = v;
	logToDefault(s, t);
}

#if defined(_WIN32) && !defined(_X64)
void DLogger::logToDefault(const char *s, size_t v) 
{	__int64 t = v;
	logToDefault(s, t);
}

#else
void DLogger::logToDefault(const char *s, unsigned int v)
{	__int64 t = v;
	logToDefault(s, t);
}

#endif

void DLogger::logToDefault(const char *s, int v)
{	logToDefault(s, (long) v);
}



void DLogger::logToDefault(const char *s, void *ptr)
{
    if (gfLoggingEnabled)
        theDefaultLog->log(s, ptr);
}

void DLogger::logToDefault(const char *s)
{
    if (gfLoggingEnabled)
        theDefaultLog->log(s);
}

void DLogger::logToDefault(const char *s, __int64 a, __int64 b)
{
    if (gfLoggingEnabled)
        theDefaultLog->log(s, a, b);
}

void DLogger::logFileToDefault(const char *fn, const char *what)
{   if (gfLoggingEnabled)
        theDefaultLog->logFileContents(fn, what);
}

void DLogger::logFolderToDefault(const char *fn, const char *what)
{   if (gfLoggingEnabled)
        theDefaultLog->logFolder(fn, what);
}


void DLogger::logSystemErrorToDefault(const char *a, const char *b, const char *c)
{   if (gfLoggingEnabled)
        theDefaultLog->logLastError(a, b, c);
}


void DLogger::log(const wchar_t *s, const wchar_t *t)
{   if (fLogging) {
        DString256 buf;
        buf << checkString(s) << ' ' << checkString(t);
        log(buf);
    }
}

void DLogger::log(const wchar_t *s, void *p)
{   if (fLogging) {
        uintptr_t l = (uintptr_t) p;
        DString256 buf;
        buf << checkString(s) << ' ' << l;
        log(buf);
    }
}

void DLogger::log(const char *s, const wchar_t *t, const wchar_t *u)
{   if (fLogging) {
        DString256 buf;
        buf << checkString(s) << ' ' << checkString(t) << ' ' << checkString(u);
        log(buf);
    }
}

void DLogger::log(const wchar_t *s, const wchar_t *t, const wchar_t *u)
{   if (fLogging) {
        DString256 buf;
        buf << checkString(s) << ' ' << checkString(t) << ' ' << checkString(u);
        log(buf);
    }
}

void DLogger::log(const wchar_t *s, long v)
{   if (fLogging && s) {
        DString256 buf;
        buf << s;
        appendInt(buf,v);
        log(buf);
    }
}

void DLogger::log(const wchar_t *s, long a, long b)
{   if (fLogging && s) {
        DString256 buf;
        buf << s;
        appendInt(buf, a);
        buf << ' ';
        appendInt(buf, b);
        log(buf);
    }
}

        
#define debugTrace(s)

void DLogger::logToDefault(const wchar_t *s, const wchar_t *a, const wchar_t *b)
{
    if (gfLoggingEnabled)
        theDefaultLog->log(s, a, b);
}

void DLogger::logToDefault(const wchar_t *s, const wchar_t *t)
{
    if (gfLoggingEnabled)
        theDefaultLog->log(s, t);
}

void DLogger::logToDefault(const wchar_t *s, long v)
{
    if (gfLoggingEnabled)
        theDefaultLog->log(s, v);
}

void DLogger::logToDefault(const wchar_t *s, void *ptr)
{
    if (gfLoggingEnabled)
        theDefaultLog->log(s, ptr);
}

void DLogger::logToDefault(const wchar_t *s)
{
    if (gfLoggingEnabled)
        theDefaultLog->log(s);
}

void DLogger::logToDefault(const wchar_t *s, long a, long b)
{
    if (gfLoggingEnabled)
        theDefaultLog->log(s, a, b);
}

        
#ifdef _MAC
    #define LogPlatform "Mac OS X"
#elif defined(_ANDROID)
	#define LogPlatform "Android"
#elif defined(__UNIX__)
    #define LogPlatform "Linux"
#elif defined(_WINRT)
	#define LogPlatform "WinRT/UWP"
#else
    #define LogPlatform "Windows"
#endif

#ifdef _X64
    #define Bits "64-bit"
#else
    #define Bits "32-bit"
#endif

#ifdef VSVER
	#undef VSVER
#endif
#if (_MSC_VER > 1900) 
	#define VSVER "2015+"
#elif (_MSC_VER >= 1900) 
	#define VSVER "2015"
#elif (_MSC_VER >= 1800) 
    #define VSVER "2013"
#elif (_MSC_VER >= 1700) 
    #define VSVER "2012" 
#elif (_MSC_VER >= 1600) 
    #define VSVER "2010" 
#elif (_MSC_VER >= 1500) 
    #define VSVER "2008" 
#elif (_MSC_VER >= 1400) 
    #define VSVER "2005" 
#elif (_MSC_VER >= 1300) 
    #define VSVER "2003" 
#endif 


#define MAKESTR(x) #x

#if defined(__clang__)
    #define BuiltBy "clang " __clang_version__
#elif defined(_MSC_VER)
    #define BuiltBy      "VC " VSVER
#elif defined(__BORLANDC__)
    #define BuiltBy      "BC"
#elif defined(__GNUC__)
    #define BuiltBy      "GCC " __VERSION__
#else
    #define BuildBy "XC"
#endif

#define BuiltWhen    __DATE__ " " __TIME__

#ifdef UNICODE
    #define CharSet "Unicode"
#else
    #define CharSet "Ansi"
#endif

 
#if defined(StandardHeader) && defined(dtSearchVersionStr)
	#define BuildInfo StandardHeader
#else
	#define BuildInfo  LogPlatform " " Bits " " CharSet " " BuiltBy " (" BuiltWhen ")"
#endif

void DLogger::logHeader(const char *extra)
{ 	log(eq);
    DFileDate fd;
    fd.nowUTC();
    DString80 s, t;
    fd.formatISO(s);
    fd.formatLocal(t);
	log(BuildInfo);
	logOS();
	DString msg;
	msg << "Log started: " << s << " Local: " << t;
    debugLog(msg);

    log(eq);
	headerString.trim();
	if (!headerString.isBlank())
		log(headerString);
    if (extra)
        log(extra);

    log(eq);
}


void DLogger::getOsName(DString& s)
{
#ifdef __UNIX__
    s << "UNIX";
#elif defined(_WINRT)
	Platform::String ^ver = Windows::System::Profile::AnalyticsInfo::VersionInfo->DeviceFamilyVersion;
	DString80 t;
	t.storeUnicodeAsUtf8(ver->Data());
	__int64 h = _atoi64(t);
	s << "WinRT/UWP " << HexNum(h);
#else
    CWindowsVersionInfo vi;
    s << vi.getName() << " " << vi.getDetails();
#endif
}

void DLogger::logOS()
{   DString s;
    getOsName(s);
    log("Environment: ", s);
}

#ifdef _WIN32
	#if _MSC_VER >= 1700
		#include <Mapi.h>
		#include <WabCode.h>
	#else
		#include <MapiCode.h>
	#endif
	#include <filterr.h>

	#ifndef MAPI_E_INVALID_FLAGS
		#define MAPI_E_INVALID_FLAGS 0x80010106
	#endif
#endif

#define deferr(x) { x, #x }
struct ErrorTableItem {
    __int64 code;
    const char *text;
};


#ifdef WIN32

static ErrorTableItem ErrorTable[] = {

	// deferr(MAPI_E_CORRUPT_STORE), not supported in WinSDK 8
	#ifndef _WINRT_DLL
	deferr(MAPI_E_UNABLE_TO_COMPLETE),
	deferr(MAPI_E_TIMEOUT),
	deferr(MAPI_E_UNKNOWN_ENTRYID),
	deferr(MAPI_E_LOGON_FAILED),
	deferr(MAPI_E_VERSION),
	deferr(MAPI_E_INVALID_ENTRYID),
	deferr(MAPI_E_NO_SUPPORT),
	deferr(MAPI_E_UNKNOWN_FLAGS),
	deferr(MAPI_E_INVALID_FLAGS),
	#endif
    deferr(DISP_E_UNKNOWNINTERFACE),
    deferr(DISP_E_MEMBERNOTFOUND),
    deferr(DISP_E_PARAMNOTFOUND),
    deferr(DISP_E_TYPEMISMATCH),
    deferr(DISP_E_UNKNOWNNAME),
    deferr(DISP_E_NONAMEDARGS),
    deferr(DISP_E_BADVARTYPE),
    deferr(DISP_E_EXCEPTION),
    deferr(DISP_E_OVERFLOW),
    deferr(DISP_E_BADINDEX),
    deferr(DISP_E_UNKNOWNLCID),
    deferr(DISP_E_ARRAYISLOCKED),
    deferr(DISP_E_BADPARAMCOUNT),
    deferr(DISP_E_PARAMNOTOPTIONAL),
    deferr(DISP_E_BADCALLEE),
    deferr(DISP_E_NOTACOLLECTION),
    deferr(RPC_E_CHANGED_MODE),
    deferr(TYPE_E_BUFFERTOOSMALL),
    deferr(TYPE_E_INVDATAREAD),
    deferr(TYPE_E_UNSUPFORMAT),
    deferr(TYPE_E_REGISTRYACCESS),
    deferr(TYPE_E_LIBNOTREGISTERED),
    deferr(TYPE_E_UNDEFINEDTYPE),
    deferr(TYPE_E_QUALIFIEDNAMEDISALLOWED),
    deferr(TYPE_E_INVALIDSTATE),
    deferr(TYPE_E_WRONGTYPEKIND),
    deferr(TYPE_E_ELEMENTNOTFOUND),
    deferr(TYPE_E_AMBIGUOUSNAME),
    deferr(TYPE_E_NAMECONFLICT),
    deferr(TYPE_E_UNKNOWNLCID),
    deferr(TYPE_E_DLLFUNCTIONNOTFOUND),
    deferr(TYPE_E_BADMODULEKIND),
    deferr(TYPE_E_SIZETOOBIG),
    deferr(TYPE_E_DUPLICATEID),
    deferr(TYPE_E_TYPEMISMATCH),
    deferr(TYPE_E_OUTOFBOUNDS),
    deferr(TYPE_E_IOERROR),
    deferr(TYPE_E_CANTCREATETMPFILE),
    deferr(TYPE_E_CANTLOADLIBRARY),
    deferr(TYPE_E_INCONSISTENTPROPFUNCS),
    deferr(TYPE_E_CIRCULARTYPE),
    deferr(STG_E_INVALIDFUNCTION),
    deferr(STG_E_FILENOTFOUND),
    deferr(STG_E_PATHNOTFOUND),
    deferr(STG_E_TOOMANYOPENFILES),
    deferr(STG_E_ACCESSDENIED),
    deferr(STG_E_INVALIDHANDLE),
    deferr(STG_E_INSUFFICIENTMEMORY),
    deferr(STG_E_INVALIDPOINTER),
    deferr(STG_E_NOMOREFILES),
    deferr(STG_E_DISKISWRITEPROTECTED),
    deferr(STG_E_SEEKERROR),
    deferr(STG_E_WRITEFAULT),
    deferr(STG_E_READFAULT),
    deferr(STG_E_SHAREVIOLATION),
    deferr(STG_E_LOCKVIOLATION),
    deferr(STG_E_FILEALREADYEXISTS),
    deferr(STG_E_INVALIDPARAMETER),
    deferr(STG_E_MEDIUMFULL),
    deferr(STG_E_ABNORMALAPIEXIT),
    deferr(STG_E_INVALIDHEADER),
    deferr(STG_E_INVALIDNAME),
    deferr(STG_E_UNKNOWN),
    deferr(STG_E_UNIMPLEMENTEDFUNCTION),
    deferr(STG_E_INVALIDFLAG),
    deferr(STG_E_INUSE),
    deferr(STG_E_NOTCURRENT),
    deferr(STG_E_REVERTED),
    deferr(STG_E_CANTSAVE),
    deferr(STG_E_OLDFORMAT),
    deferr(STG_E_OLDDLL),
    deferr(STG_E_SHAREREQUIRED),
    deferr(STG_E_NOTFILEBASEDSTORAGE),
    deferr(STG_E_EXTANTMARSHALLINGS),
    deferr(OLE_E_FIRST),
    deferr(OLE_E_OLEVERB),
    deferr(OLE_E_ADVF),
    deferr(OLE_E_ENUM_NOMORE),
    deferr(OLE_E_ADVISENOTSUPPORTED),
    deferr(OLE_E_NOCONNECTION),
    deferr(OLE_E_NOTRUNNING),
    deferr(OLE_E_NOCACHE),
    deferr(OLE_E_BLANK),
    deferr(OLE_E_CLASSDIFF),
    deferr(OLE_E_CANT_GETMONIKER),
    deferr(OLE_E_CANT_BINDTOSOURCE),
    deferr(OLE_E_STATIC),
    deferr(OLE_E_PROMPTSAVECANCELLED),
    deferr(OLE_E_INVALIDRECT),
    deferr(OLE_E_WRONGCOMPOBJ),
    deferr(OLE_E_INVALIDHWND),
    deferr(OLE_E_NOT_INPLACEACTIVE),
    deferr(OLE_E_CANTCONVERT),
    deferr(OLE_E_NOSTORAGE),
    deferr(DV_E_FORMATETC),
    deferr(DV_E_DVTARGETDEVICE),
    deferr(DV_E_STGMEDIUM),
    deferr(DV_E_STATDATA),
    deferr(DV_E_LINDEX),
    deferr(DV_E_TYMED),
    deferr(DV_E_CLIPFORMAT),
    deferr(DV_E_DVASPECT),
    deferr(DV_E_DVTARGETDEVICE_SIZE),
    deferr(DV_E_NOIVIEWOBJECT),
    deferr(OLE_E_LAST),
    deferr(DRAGDROP_E_FIRST),
    deferr(DRAGDROP_S_FIRST),
    deferr(DRAGDROP_E_INVALIDHWND),
    deferr(DRAGDROP_E_ALREADYREGISTERED),
    deferr(DRAGDROP_E_NOTREGISTERED),
    deferr(DRAGDROP_E_LAST),
    deferr(DRAGDROP_S_LAST),
    deferr(CLASS_E_NOAGGREGATION),
    deferr(CLASSFACTORY_E_FIRST),
    deferr(CLASS_E_CLASSNOTAVAILABLE),
    deferr(CLASSFACTORY_E_LAST),
    deferr(MARSHAL_E_FIRST),
    deferr(MARSHAL_E_LAST),
    deferr(DATA_E_FIRST),
    deferr(DATA_E_LAST),
    deferr(VIEW_E_DRAW),
    deferr(VIEW_E_FIRST),
    deferr(VIEW_E_LAST),
    deferr(REGDB_E_FIRST),
    deferr(REGDB_E_READREGDB),
    deferr(REGDB_E_WRITEREGDB),
    deferr(REGDB_E_KEYMISSING),
    deferr(REGDB_E_INVALIDVALUE),
    deferr(REGDB_E_CLASSNOTREG),
    deferr(REGDB_E_IIDNOTREG),
    deferr(REGDB_E_LAST),
    deferr(CACHE_E_NOCACHE_UPDATED),
    deferr(CACHE_E_FIRST),
    deferr(CACHE_E_LAST),
    deferr(OLEOBJ_E_NOVERBS),
    deferr(OLEOBJ_S_INVALIDVERB),
    deferr(OLEOBJ_E_FIRST),
    deferr(OLEOBJ_E_INVALIDVERB),
    deferr(OLEOBJ_E_LAST),
    deferr(CLIENTSITE_E_FIRST),
    deferr(CLIENTSITE_E_LAST),
    deferr(INPLACE_E_FIRST),
    deferr(INPLACE_E_NOTUNDOABLE),
    deferr(INPLACE_E_NOTOOLSPACE),
    deferr(INPLACE_E_LAST),
    deferr(ENUM_E_FIRST),
    deferr(ENUM_E_LAST),
    deferr(CONVERT10_E_FIRST),
    deferr(CONVERT10_E_OLESTREAM_GET),
    deferr(CONVERT10_E_OLESTREAM_PUT),
    deferr(CONVERT10_E_OLESTREAM_FMT),
    deferr(CONVERT10_E_OLESTREAM_BITMAP_TO_DIB),
    deferr(CONVERT10_E_STG_FMT),
    deferr(CONVERT10_E_STG_NO_STD_STREAM),
    deferr(CONVERT10_E_STG_DIB_TO_BITMAP),
    deferr(CONVERT10_E_LAST),
    deferr(CLIPBRD_E_FIRST),
    deferr(CLIPBRD_E_CANT_OPEN),
    deferr(CLIPBRD_E_CANT_EMPTY),
    deferr(CLIPBRD_E_CANT_SET),
    deferr(CLIPBRD_E_BAD_DATA),
    deferr(CLIPBRD_E_CANT_CLOSE),
    deferr(CLIPBRD_E_LAST),
    deferr(MK_E_CONNECTMANUALLY),
    deferr(MK_E_FIRST),
    deferr(MK_E_EXCEEDEDDEADLINE),
    deferr(MK_E_NEEDGENERIC),
    deferr(MK_E_UNAVAILABLE),
    deferr(MK_E_SYNTAX),
    deferr(MK_E_NOOBJECT),
    deferr(MK_E_INVALIDEXTENSION),
    deferr(MK_E_INTERMEDIATEINTERFACENOTSUPPORTED),
    deferr(MK_E_NOTBINDABLE),
    deferr(MK_E_NOTBOUND),
    deferr(MK_E_CANTOPENFILE),
    deferr(MK_E_MUSTBOTHERUSER),
    deferr(MK_E_NOINVERSE),
    deferr(MK_E_NOSTORAGE),
    deferr(MK_E_NOPREFIX),
    deferr(MK_E_LAST),
    deferr(CO_E_FIRST),
    deferr(CO_E_NOTINITIALIZED),
    deferr(CO_E_ALREADYINITIALIZED),
    deferr(CO_E_CANTDETERMINECLASS),
    deferr(CO_E_CLASSSTRING),
    deferr(CO_E_IIDSTRING),
    deferr(CO_E_APPNOTFOUND),
    deferr(CO_E_APPSINGLEUSE),
    deferr(CO_E_ERRORINAPP),
    deferr(CO_E_DLLNOTFOUND),
    deferr(CO_E_ERRORINDLL),
    deferr(CO_E_WRONGOSFORAPP),
    deferr(CO_E_OBJNOTREG),
    deferr(CO_E_OBJISREG),
    deferr(CO_E_OBJNOTCONNECTED),
    deferr(CO_E_APPDIDNTREG),
    deferr(CO_E_LAST),
    deferr(E_FAIL),
    deferr(E_NOTIMPL),
    deferr(S_OK),
    deferr(S_FALSE),
    deferr(E_PENDING),
    deferr(E_INVALIDARG),
    deferr(E_ACCESSDENIED),
    deferr(E_HANDLE),
    deferr(E_OUTOFMEMORY),
    deferr(FILTER_E_ACCESS),
    deferr(FILTER_E_END_OF_CHUNKS),
    deferr(FILTER_E_EMBEDDING_UNAVAILABLE),
    deferr(FILTER_E_LINK_UNAVAILABLE),
    deferr(FILTER_E_NO_MORE_VALUES),
    deferr(FILTER_E_NO_MORE_TEXT),
    deferr(FILTER_E_NO_TEXT),
    deferr(FILTER_E_NO_VALUES),
    deferr(FILTER_E_PASSWORD),
    deferr(FILTER_E_UNKNOWNFORMAT),
    deferr(FILTER_S_LAST_TEXT),
    deferr(FILTER_S_LAST_VALUES),

    { 0, 0}
};
        
#endif

const char *DLogger::HRESULTtoMessage(HRESULT hr)
{
#ifdef WIN32
     switch (hr) {
        case S_OK:
            return "S_OK";
        case S_FALSE:
            return "S_FALSE";
        default:
            break;
    }

    for (int i = 0; ErrorTable[i].code; ++i) {
        if ((long) hr == ErrorTable[i].code)
            return ErrorTable[i].text;
    }

    if (HRESULT_CODE(hr) != hr)
        return HRESULTtoMessage(HRESULT_CODE(hr));
#endif

    return "Unknown";
}

void DLogger::getCurrentUserEnv(DString& dest)
{   const char *vars[] = { "USERDOMAIN", "USERNAME", "USERPROFILE", "SystemRoot", "COMPUTERNAME", "TEMP", 0};
	DString ebuf;
    for (int i = 0; vars[i]; ++i) {
        const char *v = ebuf.safeGetEnv(vars[i]);
        if (!strIsBlank(v)) {
            if (!dest.isBlank())
                dest << "; ";
            dest << vars[i] << " = " << quote << v << quote;
        }
    }
}


void DLogger::logFunctionStart(const char *s, const char *t)
{	if (!fLogging) 
		return;
    DString x;
    x << "<" << checkString(s) << ' ' << checkString(t) <<  ">";
    log(x);
    beginIndent();
}

void DLogger::logFunctionEnd(const char *s, const char *t)
{	if (!fLogging)
		return;
    endIndent();
    DString x;
    x << "</" << checkString(s) << ' ' << checkString(t) << ">";
    log(x);
}

#ifndef SUCCEEDED
#   define SUCCEEDED(x) (x == 0)
#endif

HRESULT DLogger::logHRESULT(const char *msg, HRESULT hr)
{	if (!SUCCEEDED(hr)) {
		if (gfLoggingEnabled) {
			DString x;
			x << "Call Failed: " << msg << " " << HRESULTtoMessage(hr);
			x << " HRESULT: ";
			x.hex((unsigned long) hr);
			debugLog(x);
			}
		}
	return hr;
}


void DLogger::escapeString(DString& dest, const char *source, ptrdiff_t len, bool bEscapeNewline)
{	if (len < 0)
		len = strlen(source);
	for (int i = 0; i < len; ++i) {
		unsigned char ch = (unsigned char) source[i];

		if ((ch >= 32) && (ch < 128)) {
			if (ch == '%') {
				if (::Ascii_IsXDigit(source[i+1]))
					dest << "%%";
				else
					dest << "%";
				}
			else
				dest << ch;
			}
		else if (!bEscapeNewline && ((ch == '\n') || (ch == '\r')))
			dest << ch;
		else
            dest << '%' << PadHexNum((unsigned int) ch, 2, true);
        }
}	


static bool canShowUnicodeChar(wchar_t ch)
{	if (ch < 32)
		return false;
	if (IsUnicodeSpace(ch))
		return true;
	if (!CXmlMaker::isLegalXmlChar(ch))
		return false;
	if ((ch >= 0x2000) && (ch <= 0x200f))
		return false;
	if (ch > 0xd800)
		return false;
	return true;
}


void DLogger::escapeString(DWString& dest, const wchar_t *source, ptrdiff_t len, bool bEscapeNewline)
{	if (len < 0)
		len = wcslen(source);
	for (int i = 0; i < len; ++i) {
		wchar_t ch = source[i];

		if (canShowUnicodeChar(ch)) {
			if (ch == '%') {
				if (::Ascii_IsXDigit(source[i+1]))
					dest << "%%";
				else
					dest << "%";
				}
			else 
				dest << ch;
			}
		else if (!bEscapeNewline && ((ch == '\n') || (ch == '\r')))
			dest << ch;
		else if (ch < 256)
            dest << '%' << PadHexNum((int) ch, 2);
		else 
            dest << "U+" << PadHexNum((int) ch, 4);
			
        }
}	



#ifdef HAVE_WINAPI
        
static int makeVersion(int major, int minor)
{	if ((minor >= 0) && (minor < 100))
       return major*100 + minor;
    else
       return major + 99;
}
        
#pragma warning(disable:4996) // GetVersionExW deprecated
CWindowsVersionInfo::CWindowsVersionInfo()
{
	memset(&m_versionInfo, 0, sizeof m_versionInfo);
    m_versionInfo.dwOSVersionInfoSize = sizeof m_versionInfo;
    GetVersionEx(&m_versionInfo);

    m_version = makeVersion(m_versionInfo.dwMajorVersion, m_versionInfo.dwMinorVersion);

    memset(&m_versionInfoEx, 0, sizeof m_versionInfoEx);
    m_versionInfoEx.dwOSVersionInfoSize = sizeof m_versionInfoEx;
    GetVersionEx((OSVERSIONINFO *) &m_versionInfoEx);
}

bool CWindowsVersionInfo::isAtLeast(int major, int minor, int sp)
{	if (m_versionInfoEx.dwMajorVersion > (unsigned int) major)
		return true;
	if (m_versionInfoEx.dwMajorVersion < (unsigned int) major)
		return false;
	if (m_versionInfoEx.dwMinorVersion > (unsigned int) minor)
		return true;
	if (m_versionInfoEx.dwMinorVersion < (unsigned int) minor)
		return true;
	if (m_versionInfoEx.wServicePackMajor >= (unsigned int) sp)
		return true;
	return false;
}

const char *CWindowsVersionInfo::getName()
{   const char *ret = "Unknown";
    switch(m_versionInfo.dwPlatformId) {
        case VER_PLATFORM_WIN32_NT:
            if (m_versionInfo.dwMajorVersion < 5)
                ret = "Windows NT";
            else if (m_versionInfo.dwMajorVersion >= 7)
				ret = "Windows 8+";
			else if (m_versionInfo.dwMajorVersion == 6) {
				if (m_versionInfo.dwMinorVersion == 0) {
					if (m_versionInfoEx.wProductType == VER_NT_WORKSTATION)
						ret = "Windows Vista";
					else
						ret = "Windows Server 2008";
					}
					
				else if (m_versionInfo.dwMinorVersion == 1) {
					if (m_versionInfoEx.wProductType == VER_NT_WORKSTATION)
						ret = "Windows 7";
					else
						ret = "Windows Server 2008 R2";
					}
				else if (m_versionInfo.dwMinorVersion == 2) {
					if (m_versionInfoEx.wProductType == VER_NT_WORKSTATION)
						ret = "Windows 8";
					else
						ret = "Windows Server 2012";
					}
				else
					ret = "Windows 8+";
					
				}
			// Windows version 5
            else if (m_versionInfo.dwMinorVersion == 0)
                ret = "Windows 2000";
            else if (m_versionInfo.dwMinorVersion == 1)
                ret = "Windows XP";
            else if (m_versionInfo.dwMinorVersion == 2)
				ret = "Windows 2003 Server";
            break;
        case VER_PLATFORM_WIN32_WINDOWS:
			if (m_versionInfo.dwMinorVersion >= 90)
				ret = "Windows ME";
			else if (m_versionInfo.dwMinorVersion > 0)
                ret = "Windows 98";
            else
                ret = "Windows 95";
            break;
        default:
            ret = "Windows ?";
            break;
        }
    return ret;
}

const char *CWindowsVersionInfo::getDetails()
{	if (m_osDetails.isBlank()) {
		if (!strIsBlank(m_versionInfo.szCSDVersion))
			m_osDetails << m_versionInfo.szCSDVersion << " ";

		m_osDetails << "[" << m_versionInfo.dwMajorVersion << "."
			<< PadNum(m_versionInfo.dwMinorVersion, 2)
			<< " Build " << m_versionInfo.dwBuildNumber;


		if (m_versionInfoEx.wServicePackMajor > 0) {
			m_osDetails << " SP " << m_versionInfoEx.wServicePackMajor << "."
						<< PadNum(m_versionInfoEx.wServicePackMinor, 2);
			}
		m_osDetails << "]";
		}
    return m_osDetails;
}

#endif

#ifdef USE_DTSEARCH_NAMESPACE
    } //    namespace dtSearch
#endif
