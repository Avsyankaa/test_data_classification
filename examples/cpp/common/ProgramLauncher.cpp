#include <stdafx.h>
#include <ProgramLauncher.h>
#include <shellapi.h>
#if WINVER >= 0x500
#include <psapi.h>
#endif

const char quote = '\"';

#ifdef _DEBUG
	#define EnableTrace
#endif

#ifdef Trace
	#undef Trace
#endif


static CString AddQuotes(const TCHAR *s)
{	CString ret;
	ret += _T("\"");
	ret += s;
	ret += _T("\"");
	return ret;
}	


CProgramLauncher::CProgramLauncher()
{	m_hProcess = 0; 
	m_hThread = 0; 
	m_processID = 0; 
	m_dwCreationFlags = 0; 
    m_fHidden = 0;
    m_lastError = 0;
    m_hWindow = 0;
    m_fLastError = 0;
    m_fLaunchSucceeded = 0;
    m_bMinimize = false;
	m_waitResult = 0;
	m_waitTimeMillis = INFINITE;
    

	TCHAR buf[MAX_PATH];
	memset(buf, 0, sizeof buf);
	GetModuleFileName(0, buf, MAX_PATH);
	m_directory = buf;
	m_directory.rmTail();
    m_returnValue = (DWORD) -1;
}

CString CProgramLauncher::GetThisFolder()
{
	TCHAR buf[MAX_PATH];
	memset(buf, 0, sizeof buf);
	GetModuleFileName(0, buf, MAX_PATH);
	CSFilename f = buf;
	f.rmTail();
	return f; 

}

void CProgramLauncher::SetProgramInThisFolder(const TCHAR *s)
{	m_program = s;
	m_program.makeAbsoluteFrom(m_directory);
}

void CProgramLauncher::ExecImp(int fWait, CStdioRedirector *pRedirector)
{    m_commandLine.Empty();

	
	if (!m_program.isType(_T(".exe")) && !m_program.isType(_T(".bat")))
		m_program.setExt(_T(".exe"));

	m_commandLine = AddQuotes(m_program);
	m_commandLine += _T(" ");
	m_commandLine += m_options;

    STARTUPINFO startupInfo;
    memset(&startupInfo, 0, sizeof startupInfo);
    startupInfo.cb = sizeof startupInfo;
    if (m_fHidden) {
        startupInfo.dwFlags = STARTF_USESHOWWINDOW;
        startupInfo.wShowWindow = SW_HIDE;
        m_dwCreationFlags = CREATE_NO_WINDOW;
        }
	if (m_bMinimize) {
		startupInfo.dwFlags = STARTF_USESHOWWINDOW;
		startupInfo.wShowWindow = SW_MINIMIZE;
		}
	if (pRedirector) {
		if (!pRedirector->SetupProcessInfo(startupInfo)) {
			m_lastError = -2;
			return;
			}
		}
		
    PROCESS_INFORMATION processInfo;
    memset(&processInfo, 0, sizeof processInfo);

	const TCHAR *pEnvironment = 0;
	m_launchEnvironment.clear();
	if (Environment.getCount()) {
		TCHAR *pCurrentEnv = GetEnvironmentStrings();
		CUiStringSet externalEnv;
		externalEnv.copyFromBuf(pCurrentEnv);
		FreeEnvironmentStrings(pCurrentEnv);
		m_launchEnvironment.append(externalEnv);
		for (int i = 0; i < Environment.getCount(); ++i) {
			m_launchEnvironment.appendU8(Environment.getString(i));
			}

		pEnvironment = m_launchEnvironment.getBuffer();
		if (gfLoggingEnabled) {
			debugLog("Launch environment");
			for (int i = 0; i < m_launchEnvironment.getCount(); i++ ) {
				debugLog(m_launchEnvironment.getString(i));
				}
			}
			
#ifdef _UNICODE		
		m_dwCreationFlags |= CREATE_UNICODE_ENVIRONMENT;
#endif
		}
		
	BOOL bInherit = (pRedirector? TRUE : FALSE);
	BOOL ok = 
     CreateProcess(
        0,                            // if we put csProgram here, it won't check the path
        m_commandLine.GetBuffer(0),
        0,                            //  LPSECURITY_ATTRIBUTES lpProcessAttributes,
        0,                            // LPSECURITY_ATTRIBUTES lpThreadAttributes,
        bInherit,		              // BOOL bInheritHandles,       // handle inheritance flag
        m_dwCreationFlags,            //   DWORD dwCreationFlags,      // creation flags
        //(LPVOID) Environment.getBuffer(), 
        (LPVOID) pEnvironment,				  // new environment block
        m_directory.GetBuffer(0),	  // LPCTSTR lpCurrentDirectory, // current directory name
        &startupInfo,                 // LPSTARTUPINFO lpStartupInfo,
        &processInfo                  //LPPROCESS_INFORMATION lpProcessInformation
    );
    m_fLaunchSucceeded = makeBool(ok);
    if (!m_fLaunchSucceeded) 
        m_lastError = GetLastError();
    else
        m_lastError = 0;
	m_commandLine.ReleaseBuffer();
	m_directory.ReleaseBuffer();
    m_processID = processInfo.dwProcessId;
	m_hProcess = processInfo.hProcess;
	m_hThread = processInfo.hThread;

	
    if (m_fLaunchSucceeded) {
		if (pRedirector)
			pRedirector->RunProcess();
		if (fWait) 
			WaitForExit();
		}
}

#define TIMEOUT_RETURN 0xD0000001

void CProgramLauncher::WaitForExit()
{	m_waitResult = WaitForSingleObject(m_hProcess, m_waitTimeMillis);
	m_returnValue = (DWORD) -1;
	if ((m_waitTimeMillis != INFINITE) && (m_waitResult == WAIT_TIMEOUT))
		m_returnValue = TIMEOUT_RETURN;
	else {
		DWORD ret = 0;
		if (GetExitCodeProcess(m_hProcess, &ret)) 
			m_returnValue = ret;
		else {
			int err = GetLastError();
			if (err == STILL_ACTIVE) 
				m_returnValue = TIMEOUT_RETURN;
			}
		}
    CloseHandle(m_hProcess);
    CloseHandle(m_hThread);

	m_hThread = 0;
	m_hProcess = 0;
}

int CProgramLauncher::IsRunning()
{	if (!m_processID) {

		return false;
		}
	if (!m_hWindow) {
		m_hWindow = GetWindowForProcess(m_processID);
		}
	if (!m_hWindow) {

		return false;
		}
	BOOL val = IsWindow(m_hWindow);

	return val;
}

void CProgramLauncher::KillProcess() 
{
	TerminateProcess(m_hProcess, 1);

}

void CProgramLauncher::ShellLaunch(const TCHAR *what)
{	SHELLEXECUTEINFO info;	
	memset(&info, 0, sizeof info);
	info.cbSize = sizeof info;
	info.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_FLAG_DDEWAIT;
	info.lpFile = what;
	info.lpVerb = _T("open");
	info.nShow = SW_SHOW;

	m_fLaunchSucceeded = makeBool(ShellExecuteEx(&info));
	m_hProcess = info.hProcess;
    if (!m_fLaunchSucceeded) 
        m_lastError = GetLastError();
    else 
        m_lastError = 0;
}


void CProgramLauncher::CloseProgram(int timeoutMillis, int fTerminate)
{   // Get hWindow for the process
    HWND hWindow = GetProcessWindow();
    if (hWindow) {
        PostMessage(hWindow, WM_CLOSE, 0, 0);
        WaitForSingleObject(hWindow, timeoutMillis);
        if (fTerminate && IsWindow(hWindow))
            TerminateProcess(m_hProcess, 1);
        }
}

#if WINVER >= 0x500
#pragma comment(lib, "psapi.lib")
static BOOL CALLBACK EnumWindowsProcToEnumerateProcesses(HWND hWnd, LPARAM lParam) {
	
    if (!hWnd)
	    return TRUE;		// Not a window
    DWORD dwProcessId = 0;
	DWORD dwThreadId = GetWindowThreadProcessId(hWnd, &dwProcessId);
	if (dwProcessId) {
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessId);
		if (hProcess) {
			TCHAR filename[MAX_PATH];
			DWORD len = GetModuleFileNameEx(hProcess, 0, filename, MAX_PATH);
			if ((len > 0) && (len < MAX_PATH)) {
				filename[len] = 0;
				DString tmp = UiToUtf8(filename);
				DStringSet *set = (DStringSet *) lParam;
				if (set)
					set->append(tmp);
				}
			}
		}
    
    return TRUE;
}

void CProgramLauncher::ListProcessExecutables(DStringSet& dest)
{	EnumWindows(EnumWindowsProcToEnumerateProcesses, (LPARAM) &dest);
}

#endif

struct FindHWInfo {
    DWORD processId;
    HWND hWindow;
    CFilenameFilter *filter;
    DArrayAsVector<HWND> hWindowList;
    int fFindAll;
    const TCHAR *exeName;
    FindHWInfo() {
		processId = 0;
		hWindow = 0;
		filter = 0;
		fFindAll = 0;
		}
    };

// Look for a window that matches lParam
static BOOL CALLBACK EnumWindowsProcToFindProcess(HWND hWnd, LPARAM lParam) {
	
    if (!hWnd)
	    return TRUE;		// Not a window
    DWORD dwProcessId = 0;
    GetWindowThreadProcessId(hWnd, &dwProcessId);
    
    FindHWInfo *info = (FindHWInfo *) lParam;
    if (dwProcessId == info->processId) {
        info->hWindow = hWnd;
        return FALSE;
        }
    return TRUE;
}

HWND CProgramLauncher::GetWindowForProcess(DWORD processId)
{   FindHWInfo info;
    info.processId = processId;
    info.hWindow = 0;
    EnumWindows(EnumWindowsProcToFindProcess, (LPARAM) &info);
    
    return info.hWindow;
}

HWND CProgramLauncher::GetProcessWindow()
{   m_hWindow = GetWindowForProcess(m_processID);
    return m_hWindow;
}

static BOOL CALLBACK EnumWindowsProcToFindByName(HWND hWnd, LPARAM lParam) {
    if (!hWnd)
	    return TRUE;		// Not a window
	TCHAR caption[MAX_PATH+20];
	memset(caption, 0, sizeof caption);
	GetWindowText(hWnd, caption, MAX_PATH);
	DString dsCaption = UiToUtf8(caption);
	
    FindHWInfo *info = (FindHWInfo *) lParam;
    if (info && info->filter) {
		if (info->filter->Matches(dsCaption)) {
			info->hWindow = hWnd;
			info->hWindowList.append(hWnd);
			if (info->fFindAll)
				return TRUE;
			else
				return FALSE;
			}
        }
    return TRUE;
}

HWND CProgramLauncher::FindWindowLike(CFilenameFilter& filter)
{	FindHWInfo info;
	info.filter = &filter;
	EnumWindows(EnumWindowsProcToFindByName, (LPARAM) &info);
    return info.hWindow;
}

void CProgramLauncher::FindWindowsLike(CFilenameFilter& filter, DArrayAsVector<HWND>& result)
{	FindHWInfo info;
	info.filter = &filter;
	info.fFindAll = true;
	EnumWindows(EnumWindowsProcToFindByName, (LPARAM) &info);
	result = info.hWindowList;
}

CProgramLauncherInThread::CProgramLauncherInThread()
{	m_fDone = false;
	m_fStarted = false;
}

UINT CProgramLauncherInThread::StartFn(void *pData)
{   CProgramLauncherInThread *pl = (CProgramLauncherInThread *) pData;
    pl->ExecWait();
    pl->m_fDone = true;
    return 0;
}

void CProgramLauncherInThread::StartFn2(void *pData)
{   CProgramLauncherInThread *pl = (CProgramLauncherInThread *) pData;
    pl->ExecWait();
    pl->m_fDone = true;
}


void CProgramLauncherInThread::ExecInThread()
{	m_fDone = false;
	m_fStarted = true;
#ifdef __AFXWIN_H__
    AfxBeginThread(StartFn, this);
#else
	_beginthread(StartFn2, 0, this);
#endif
}


#ifdef __AFXWIN_H__ 
static void MakeForeground(CWnd *wnd)
{	CWnd *p = wnd->GetTopLevelParent();

	DWORD thisThread = GetCurrentThreadId();
	HWND topWindow = ::GetForegroundWindow();
	DWORD topThread = GetWindowThreadProcessId(topWindow, NULL);
	
	if (topThread != thisThread)
		AttachThreadInput(topThread, thisThread,TRUE);
	p->SetForegroundWindow();
	wnd->SetActiveWindow();
	wnd->SetFocus();
	if (topThread != thisThread)
		AttachThreadInput(topThread, thisThread, FALSE);

}

#endif

// return false when PumpMessage() returns false, which indicates
// WM_QUIT received
static int PumpMessages()
{
#if defined(MfcAvailable)
    MSG msg;
    while (::PeekMessage (&msg, NULL, 0, 0, PM_NOREMOVE))
        if (!AfxGetThread ()->PumpMessage ())
			return false;
#endif
	return true;
}

#ifdef __AFXWIN_H__
void CProgramLauncherInThread::ExecModal(CWnd *parent)
{	parent->EnableWindow(false);
	ExecInThread();
	while(IsRunning()) {
		PumpMessages();
		Sleep(200);
		}
	MakeForeground(parent);
	parent->EnableWindow(true);
	
}

#endif

const int BUFSIZE = 512;

static BOOL SafeCloseHandle(HANDLE *pHandle)
{	BOOL ret = true;
	if (*pHandle) {
		ret = CloseHandle(*pHandle);
		*pHandle = 0;
		}
	return ret;
}

bool CStdioRedirector::RunProcess()
{	
	SendInputToProcess();
	GetOutputFromProcess();
	
	return true;
}

// Write the data from Input in chunks, then close the handle so the child process stops reading
void CStdioRedirector::SendInputToProcess()
{   
	const char *data = Input;
    ptrdiff_t nBytes = Input.getLength();
 
	while(nBytes > 0) { 
		DWORD toWrite = min((int) nBytes, BUFSIZE);
		DWORD dwWritten = 0;
		if (!WriteFile(hChildStdinWrDup, data, toWrite, &dwWritten, 0))
			break; 
		
		nBytes -= dwWritten;
		data += dwWritten;
		}
 
    SafeCloseHandle(&hChildStdinWrDup);
    
} 


void CStdioRedirector::GetOutputFromProcess()
{	
	// Close the write end of the pipe before reading from the read end of the pipe. 
    if (!SafeCloseHandle(&hChildStdoutWr))
		return;
 
	// Read output from the child process and save in Output
    while(1)  { 
		DWORD dwRead = 0; 
		char buf[BUFSIZE]; 
        if( !ReadFile( hChildStdoutRdDup, buf, BUFSIZE, &dwRead, NULL)) 
			break; 
		if (dwRead == 0)
			break;
		Output.store(buf, (int) dwRead);
		} 
	
}


CStdioRedirector::CStdioRedirector()
{
	 hChildStdinRd = 0;
	 hChildStdinWr = 0;
	 hChildStdinWrDup = 0;
	 hChildStdoutRd = 0;
	 hChildStdoutWr = 0;
	 hChildStdoutRdDup = 0;
	 securityAttributes.bInheritHandle = TRUE;
	 securityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
	 securityAttributes.lpSecurityDescriptor = NULL;
}


void CStdioRedirector::CloseHandles()
{	
	SafeCloseHandle(&hChildStdinRd);
	SafeCloseHandle(&hChildStdinWr);
	SafeCloseHandle(&hChildStdinWrDup);
	SafeCloseHandle(&hChildStdoutRd);
	SafeCloseHandle(&hChildStdoutWr);
	SafeCloseHandle(&hChildStdoutRdDup);
}

bool CStdioRedirector::CreatePipes()
{	// Create child output pipe
	if (!CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &securityAttributes, 65536))
		return false;
	// Create noninheritable read handle and close the inheritable read handle. 
	if (!DuplicateHandle(
		GetCurrentProcess(),    // Source process
		hChildStdoutRd,			// Source handle
		GetCurrentProcess(),	// Target process
		&hChildStdoutRdDup,		// New handle
		0,						// Desired access
		FALSE,					// bInheritHandle
		DUPLICATE_SAME_ACCESS   	// Flags
			))
		return false;
	SafeCloseHandle(&hChildStdoutRd);
	
	// Create child input pipe
	if (!CreatePipe(&hChildStdinRd, &hChildStdinWr, &securityAttributes, 65536))
		return false;
	// Create noninheritable write handle and close the inheritable write handle. 
	if (!DuplicateHandle(
		GetCurrentProcess(),    // Source process
		hChildStdinWr,			// Source handle
		GetCurrentProcess(),	// Target process
		&hChildStdinWrDup,		// New handle
		0,						// Desired access
		FALSE,					// bInheritHandle
		DUPLICATE_SAME_ACCESS))	// Flags
		return false;
	SafeCloseHandle(&hChildStdinWr);
	
	return true;
}
    
bool CStdioRedirector::SetupProcessInfo(STARTUPINFO& startupInfo)
{	if (!CreatePipes())
		return false;
		
	startupInfo.hStdError = hChildStdoutWr;
	startupInfo.hStdOutput = hChildStdoutWr;
	startupInfo.hStdInput = hChildStdinRd;
	startupInfo.dwFlags |= STARTF_USESTDHANDLES;
	return true;
}




