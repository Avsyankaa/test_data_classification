#ifndef DLOGGER_H
    #define DLOGGER_H

#include <itimer.h>
#include <dstring.h>
#include <dfile.h>
#include <darray.h>
#include <dtsearch.h>


#ifdef USE_DTSEARCH_NAMESPACE
    namespace dtSearch {
#endif


const int dlLogCommit       = dtsLogCommit;
const int dlLogAppend       = dtsLogAppend;
const int dlLogTime         = dtsLogTime;
const int dlLogToDbwin      = dtsLogToDbwin;
const int dlLogStart        = 0x2000;
const int dlLogStartIfEnv   = 0x4000;
const int dlLogStartIfReg   = 0x8000;

#ifdef gfLoggingEnabled
	#undef gfLoggingEnabled
#endif

#define gfLoggingEnabled    DLogger::isEnabled()

#define MaxIndentThreads 300

class CIndentTable {
	public:
		CIndentTable() {
			for (int i = 0; i < MaxIndentThreads; ++i) 
				indent[i] = 0;
			}
		int getIndent(int iThread) {
			if ((iThread >= 0) && (iThread < MaxIndentThreads)) {
				int ret = (int)indent[iThread];
				if ((ret < 0) || (ret > 20))
					return 0;
				return ret;
				}
			return 0;
			}
		void beginIndent(int iThread) {
			if ((iThread >= 0) && (iThread < MaxIndentThreads))
				InterlockedIncrement(&indent[iThread]);
			}
		void endIndent(int iThread) {
			if ((iThread >= 0) && (iThread < MaxIndentThreads))
				InterlockedDecrement(&indent[iThread]);
			}
			
	protected:
#ifdef _WIN32
		volatile LONG indent[MaxIndentThreads];
#else
		LONG indent[MaxIndentThreads];
#endif
	};		

#ifdef _WIN32
class CWindowsVersionInfo {
	public:
		CWindowsVersionInfo(); 
        const char *getName();
        const char *getDetails();
        bool isAtLeast(int major, int minor, int sp);
	protected:
	
        OSVERSIONINFO m_versionInfo;
        OSVERSIONINFOEX m_versionInfoEx;
        int m_version;  // 100 * minorVersion + majorVersion
        DString80 m_osDetails;
		};
		
#endif	


class DLogger {
    public:
    	static void trace(const char *s);
    	static void trace(const char *s, const char *t);
    	static void trace(const char *s, __int64 v);
    	
    	static void trace(const char *s, __int64 v, __int64 v2);
        static DLogger *theDefaultLog;
        DLogger(const char *logFn = 0, const char *dir = 0, int flags = 0, const char *env = 0,
            const char *headerString = 0);
        ~DLogger();
        void setShowThreadId(bool v) {
			fShowThreadId = v;
			}
		bool startLogging(const char *logName, bool fAppend = false);
        void startLogging();    // start logging using default options
        void stopLogging();     // suspend logging
        void close();           // close log file
		void suspend();         // temporarily stop logging to unlock log file (call resume to resume)
		void resume();
        void truncate();
                                // open, or re-open, a log file
		void log(const char *s, const char *a, const char *b, const char *c);
	    void log(const char *s, const char *a, const char *b);
        void log(const char *s, const char *t);
        void log(const char *s, long v);
        void log(const char *s, __int64 v);
        void log(const char *s, __int64 a, __int64 b);
        void log(const char *s, void *ptr);
        void log(const char *s);

        void log(const char *s, const wchar_t *a, const wchar_t *b);
        void log(const wchar_t *s, const wchar_t *a, const wchar_t *b);
        void log(const wchar_t *s, const wchar_t *t);
        void log(const wchar_t *s, long v);
        void log(const wchar_t *s, long a, long b);
        void log(const wchar_t *s, void *ptr);
        void log(const wchar_t *s);

        void logLastError(const char *aMsg, const char *a = 0, const char  *b = 0);

        bool logging() {
            return fLogging;
            };
        void setTimeLogging(bool v = true);
        bool getTimeLogging() {
            return fLogTime;
            }
        void setDbwin(bool v = true) {
            fOutputToDbwin = v;
            }
        const char *getLogName() {
            return logName;
            }
        void setCommit(bool v = true);
        void logHeader(const char *extra = 0);
        void logOS();
        void setOutput(void (*pOutputFn)(const char *s));
        void setOutput2(void (*pOutputFn)(void *pData, const char *s), void *aPData);
        void setAsDefault() {
                theDefaultLog = this;
                }
        void beginIndent();
        void endIndent();
        void checkIfLogging();
		void logFolder(const char *folder, const char *filter);
		void setLogName(const char *n);
		void startLoggingInTempFolder(const char *n);
        void logFunctionStart(const char *f, const char *arg = 0);
        void logFunctionEnd(const char *f, const char *arg = 0);
        void logFileContents(const char *fn, const char *what = 0);
        static bool isEnabled() {
            return theDefaultLog && theDefaultLog->fLogging;
            }
        static void logToDefault(const char *s, const char *a, const char *b, const char *c);
        static void logToDefault(const char *s, const char *a, const char *b);
        static void logToDefault(const char *s, const char *t);
        static void logToDefault(const char *s, long v);
        static void logToDefault(const char *s, __int64 v);
		static void logToDefault(const char *s, unsigned __int64 v);
        static void logToDefault(const char *s, unsigned long v);
#if defined(_WIN32) && !defined(_X64)
		static void logToDefault(const char *s, size_t v);
#else		
	    static void logToDefault(const char *s, unsigned int v);
#endif		

        static void logToDefault(const char *s, int v);
        static void logToDefault(const char *s, __int64 a, __int64 b);
        static void logToDefault(const char *s, void *ptr);
        static void logToDefault(const char *s);
        static void logToDefault(const char *s, const wchar_t *a, const wchar_t *b = 0);
        static void logFileToDefault(const char *fn, const char *what = 0);
		static void logFolderToDefault(const char *fn, const char *filter);

        static void logToDefault(const wchar_t *s, const wchar_t *a, const wchar_t *b);
        static void logToDefault(const wchar_t *s, const wchar_t *t);
        static void logToDefault(const wchar_t *s, long v);
        static void logToDefault(const wchar_t *s, long a, long b);
        static void logToDefault(const wchar_t *s, void *ptr);
        static void logToDefault(const wchar_t *s);

        static void logSystemErrorToDefault(const char *a, const char *b, const char *c);

        static void hexDump(DString& x, const char *msg, const void *p, ptrdiff_t bytes);
        static void hexDumpFormatted(DString &out, const char *data, ptrdiff_t len, int nIndent = 4);

        static const char *HRESULTtoMessage(HRESULT hr);
        static void getCurrentUserEnv(DString& dest);
        static void getOsName(DString& s);
        static HRESULT logHRESULT(const char *msg, HRESULT hr);
        static void FormatLastError(DString& dest, long err = -1);
		static void escapeString(DString& dest, const char *source, ptrdiff_t len = -1, bool bEscapeNewline = true);
		static void escapeString(DWString& dest, const wchar_t *source, ptrdiff_t len = -1, bool bEscapeNewline = true);
		static void escapeString(DString& dest, DString& source, bool bEscapeNewline = true) {
			escapeString(dest, source, source.getLength(), bEscapeNewline);
			}
#ifdef _WIN32
		static void listRegistryKeyRecursive(HKEY hStartKey, const char *name, DString& dest, const char *pFullPath = 0);
		static void listRegistryValue(HKEY hKey, const char *name, const char *valueName, DString& dest);
		static void listRegistryValue(const char *name, const char *valueName, DString& dest);
#ifdef MfcAvailable
		static void findFilesRecursive(const char *folder, const char *filter, DStringSet& dest);
#endif
#endif
        int open(const char *logName, bool fAppend = false);
    protected:

		CIndentTable indentTable;
        DFile logFile;
        DString headerString;
        DFilename logName;
        CSimpleTimer timer;
        bool fCommit;
        bool fAppend;
        bool fLogging;
        bool fLogTime;

        bool fOutputToDbwin;
        bool fShowThreadId;

        long reserved[16];
        int threadCount;

        DArrayAsVector<__int64> threadTable;

        void open();
        void (*outputFn)(const char *s);
        void (*outputFn2)(void *pData, const char *s);
        int getIndent(int iThread);
        void *pOutputData;
        void writeText(const char *s);
        int getThreadNum();
    };

struct CLogFunction {
    CLogFunction(const char *s, const char *t = 0) : m_funcName(0) {
		m_funcName = s;
		if (gfLoggingEnabled) {
			if (t)
	            m_arg = t;
		    doStart();
		    }
        }
   CLogFunction(const char *s, const char *t, const char *t2) : m_funcName(0) {
		m_funcName = s;
		if (gfLoggingEnabled) {
            m_arg << t << " " << t2;
		    doStart();
		    }
        }
    void doStart() {
        if (gfLoggingEnabled)
            DLogger::theDefaultLog->logFunctionStart(m_funcName, m_arg);
        }
    CLogFunction(const char *s, int t) {
		m_funcName = s;
		if (gfLoggingEnabled) {
			m_arg << t;
			doStart();
			}
        }
    CLogFunction(const char *s, __int64 t) {
		m_funcName = s;
		if (gfLoggingEnabled) {
			m_arg << t;
	        doStart();
	        }
        }
    CLogFunction(const char *s, const wchar_t *t) {
		m_funcName = s;
		if (gfLoggingEnabled) {
			if (t)
	            m_arg.storeUnicodeAsUtf8(t);
		    doStart();
		    }
        }

    ~CLogFunction() {
        if (gfLoggingEnabled)
            DLogger::theDefaultLog->logFunctionEnd(m_funcName, m_arg);
        }
    const char *m_funcName;
    DString m_arg;

    };

struct CLogFunctionHR : public CLogFunction {
	CLogFunctionHR(const char *name, HRESULT *pHR) :
		CLogFunction(name)
		{
			m_pHR = pHR;
		}
	~CLogFunctionHR() {
		if (m_pHR) {
			m_arg << " Returns HR=";
			m_arg.hex(*m_pHR);
			m_arg << " ";
			m_arg << DLogger::HRESULTtoMessage(*m_pHR);
			}
		}

    HRESULT *m_pHR;
    };

struct CLogFunctionUi {
    CLogFunctionUi(const TCHAR *s, const TCHAR *t = 0) {
        if (gfLoggingEnabled) {
	        m_funcName = UiToUtf8(s);
		    if (t)
			    m_arg = UiToUtf8(t);
            DLogger::theDefaultLog->logFunctionStart(m_funcName, m_arg);
            }
        }
    CLogFunctionUi(const TCHAR *s, int arg) {
        if (gfLoggingEnabled) {
			m_funcName = UiToUtf8(s);
			m_arg << arg;
	        DLogger::theDefaultLog->logFunctionStart(m_funcName, m_arg);
	        }
        }
    CLogFunctionUi(const TCHAR *s, long arg) {
		if (gfLoggingEnabled) {
			m_funcName = UiToUtf8(s);
			m_arg << arg;
            DLogger::theDefaultLog->logFunctionStart(m_funcName, m_arg);
            }
        }

    ~CLogFunctionUi() {
        if (gfLoggingEnabled)
            DLogger::theDefaultLog->logFunctionEnd(m_funcName, m_arg);
        }
    DString m_funcName;
    DString m_arg;
    };

 

// dtconfig.h may have been included before dlogger.h
#ifdef debugLog
	#undef debugLog
#endif
#ifdef LogFunction
	#undef LogFunction
#endif

#define debugLog            DLogger::logToDefault
#define debugLogFile        DLogger::logFileToDefault
#define debugLogLastError   DLogger::logSystemErrorToDefault
#define logEvent            DLogger::logToDefault
#define debugLogFolder		DLogger::logFolderToDefault

#define LogHR(x)			DLogger::logHRESULT(#x, x);

#define LogFunction CLogFunction __clogfunction__
#define LogFunctionHR(n) CLogFunctionHR __clogfunction__(n, &hr)
#define LogFunctionUi CLogFunctionUi __clogfunction__
#define LogFunctionReturn(n)  __clogfunction__.m_arg << " Returns = " << n;


#define logMethod  LogFunction

#define HaveDebugLog

#ifdef USE_DTSEARCH_NAMESPACE
    } // namespace dtSearch
#endif

#endif
