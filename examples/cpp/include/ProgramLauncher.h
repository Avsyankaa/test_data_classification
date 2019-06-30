#ifndef ProgramLauncher_H
#   define ProgramLauncher_H

#include <StringTools.h>
#include <dstring.h>

#ifdef USE_DTSEARCH_NAMESPACE
	using namespace dtSearch;
#endif

class CStdioRedirector {
	public:
		CStdioRedirector();
		~CStdioRedirector() {
			CloseHandles();
			}
		
		DString Input;
		DString Output;
		
		bool SetupProcessInfo(STARTUPINFO& startupInfo);
		bool RunProcess();
	protected:
		bool CreatePipes();
		void CloseHandles();
		void SendInputToProcess();
		void GetOutputFromProcess();
		
		SECURITY_ATTRIBUTES securityAttributes;
		HANDLE hChildStdinRd;
		HANDLE hChildStdinWr;
		HANDLE hChildStdinWrDup;
		HANDLE hChildStdoutRd;
		HANDLE hChildStdoutWr;
		HANDLE hChildStdoutRdDup;
	};		


class CProgramLauncher {
    public:
		
        CProgramLauncher();
        void EnvironmentAppend(const char *s) {
			if (!strIsBlank(s) && strchr(s, '='))
				Environment.append(s);
			}
		void EnvironmentClear() {
			Environment.clear();
			}
        void Exec() {
            ExecImp(false);
            }
        void ExecWait() {
            ExecImp(true);
            }
        void ExecRedirected(DString& input, DString& output) {
			CStdioRedirector redir;
			redir.Input = input;
			ExecImp(true, &redir);
			output = redir.Output;
			}
        void SetProgram(const TCHAR *s) {
            m_program = s;
            }
        void SetProgramU8(const char *s) {
			m_program = Utf8ToUi(s);
			}
		void SetProgramInThisFolder(const TCHAR *s);
        void SetOptions(const TCHAR  *s) {
            m_options = s;
            }
        void SetOptionsU8(const char *s) {
			SetOptions(Utf8ToUi(s));
			}
        void SetDirectory(const TCHAR * s) {
            m_directory = s;
            }
		const TCHAR *GetProgram() {
			return m_program;
			}
        CSFilename m_program;
        CString m_commandLine;
        HINSTANCE GetProcess() {
              return (HINSTANCE) m_hProcess;
               }
		void ShellLaunch(const TCHAR *what);
		void WaitForExit();
		bool WaitTimeout() {
			return m_waitResult == WAIT_TIMEOUT;
			}
		void SetCreationFlags(DWORD f) {
			m_dwCreationFlags = f;
			}
        long GetReturnValue() {
            return m_returnValue;
            }
        void SetHidden(bool v) {
            m_fHidden = v;
            }
		void SetMinimized(bool v = true) {
			m_bMinimize = v;
			}           
        bool Succeeded() {   
            return m_fLaunchSucceeded;
            }
        long GetError() {
            return m_lastError;
            }
		void SetMaxWaitTime(DWORD v) {
			m_waitTimeMillis = v;
			}
		void AddFileOption(const TCHAR *opt, const TCHAR *filename) {
			if (!strIsBlank(m_options))
				m_options += _T(" ");
			m_options += opt;
			m_options += _T(" \"");
			m_options += filename;
			m_options += _T("\"");
			}
		void AddOption(const TCHAR *opt) {
			if (!strIsBlank(m_options))
				m_options += _T(" ");
			m_options += opt;
			}
		void KillProcess();
        HWND GetProcessWindow();
        int IsRunning();
        void CloseProgram(int timeoutMillis, int fTerminate);
		static void ListProcessExecutables(DStringSet& dest);
        static HWND GetWindowForProcess(DWORD processId);
        static HWND FindWindowLike(CFilenameFilter& filter);
		static HWND FindWindowByExe(const TCHAR *exeName);
		static void FindWindowsByExe(const TCHAR *exeName, DArrayAsVector<HWND>& result);
        static void FindWindowsLike(CFilenameFilter& filter, DArrayAsVector<HWND>& result);

        static CString GetThisFolder();
    protected:
		DStringSet Environment;
        CString m_options;
        CSFilename m_directory;
        CUiStringSet m_launchEnvironment;
        bool m_fHidden;
        bool m_bMinimize;
        bool m_fLaunchSucceeded;
        int m_lastError;
        int m_fLastError;
        HANDLE m_hProcess;
		HANDLE m_hThread;
		DWORD m_processID;
		DWORD m_dwCreationFlags;
        HWND m_hWindow;
        DWORD m_returnValue;
		DWORD m_waitResult;
		DWORD m_waitTimeMillis;

        void ExecImp(int fWait, CStdioRedirector *pRedirector = 0);
    };


class CProgramLauncherInThread : public CProgramLauncher {
    public:
        CProgramLauncherInThread();
        int IsDone() {
            return m_fDone;
            }
        int IsRunning() {
            return m_fStarted && !m_fDone;
            }
        void ExecInThread();
#ifdef __AFXWIN_H__
        void ExecModal(CWnd *parent);
#endif
    protected:
        int m_fStarted;
        int m_fDone;
        static UINT StartFn(void *pData);
        static void StartFn2(void *pData);

    };


#endif
