#include "stdafx.h"
#include "Server.h"
#include <BuildNum.h>
#include "Constants.h"

using namespace System;
namespace SCS = System::Collections::Specialized;

extern int gfLoggingEnabled = 0;

#ifdef debugLog
	#undef debugLog
#endif

#define MyName "dtsnet"

void debugLog(const char *s)
{	if (gfLoggingEnabled && !strIsBlank(s)) 
		dtssAddToLog(s);
}

void debugLog(const char *s, const char *t)
{	if (gfLoggingEnabled) {
		DString tmp;
		tmp << s;
		if (!strIsBlank(t))
			tmp << " " << t;
		dtssAddToLog(tmp);
		} 
}

void debugLog(const char *s, long t)
{	if (gfLoggingEnabled) {
		DString tmp;
		tmp << s << " " << t;
		dtssAddToLog(tmp);
		}
}

void debugLog(const char *a, const char *b, const char *c)
{	if (gfLoggingEnabled) {
		DString tmp;
		tmp << a;
		if (!strIsBlank(b))
			tmp << " " << b;
		if (!strIsBlank(c))
			tmp << " " << c;
		dtssAddToLog(tmp);
		}
}

void debugLog(const char *a, long b, long c)
{	if (gfLoggingEnabled) {
		DString tmp;
		tmp << a << " " << b << " " << c;
		dtssAddToLog(tmp);
		}
}


namespace dtSearch {
namespace Engine {

const char *logSep = "=======================================================================";

static void logVersion()
{	debugLog(logSep);
	debugLog(MyName);
	debugLog("            build number  ", BuildNumber);
	debugLog("            build date:   ", __DATE__);
    HINSTANCE hi = GetModuleHandle(_T("MyName"));
	TCHAR moduleName[MAX_PATH];
	memset(moduleName, 0, sizeof moduleName);
    GetModuleFileName(hi, moduleName, MAX_PATH);
	debugLog("            file:         ", UiToUtf8(moduleName));
}

	Server::Server()
	{	
		CountCreate(otServer);
		
		dtsVersionInfo vi;
		dtssVersionEx(vi);
		m_majorVersion = vi.majorVersion;
		m_minorVersion = vi.minorVersion;
		m_build = vi.build;

	}

	Server::~Server()
	{	CountDelete(otServer);
	}
	
	void Server::SetCrashLogging(String^ logName)
	{	INIT
			dtsCrashHandlerInfo crashHandler;
			DStringProperty crashName;
			crashName.set(logName);
			strCopy(crashHandler.logName, crashName, sizeof crashHandler.logName);
			DFilename dmpName = crashName;
			dmpName.setExt(".dmp");
			crashHandler.miniDumpName = dmpName.str();
			strCopy(crashHandler.appName, "dtSearchNetApi2.dll", sizeof crashHandler.appName);
			strCopy(crashHandler.appVersion, __DATE__ " " __TIME__, sizeof crashHandler.appVersion);
			dtssInstallCrashDiagnostics(crashHandler);
	}

	void Server::SetDebugLogging (String^ logName, DebugLogFlags flags)
	{	INIT

	if (flags == DebugLogFlags::dtsCrashLog)
		{	SetCrashLogging(logName);
		}
		else
		{
			DStringProperty log;
			log.set(logName);
			dtssDebugLogEx(log, (long) flags);
			gfLoggingEnabled = !strIsBlank(log);
			debugLog("\r\ndtSearch .NET API version " dtSearchVersionStr " Build " sBuildNumber "\r\n", __DATE__);
		}
	}

	void Server::AddToLog(String^ text) 
	{	INIT
		DStringProperty sText;
		sText.set(text);
		debugLog(sText);
	}

	// Converts paths in web applications
	String^ Server::ConvertPath (String^ path, long siteId, ConvertPathFlags flags, String^ tableLocation)
	{	INIT

		DConvertPathRequest req;
		req.Path.set(path);
		req.TableRegistryKey.set(tableLocation);

		if (IsFlagSet(flags, ConvertPathFlags, dtsCpRefreshMappings))
			req.RefreshMappings();

		bool readFromRegistry = IsFlagSet(flags, ConvertPathFlags, dtsCpReadFromRegistry);

		if (IsFlagSet(flags, ConvertPathFlags, dtsCpMakeLocal))
			req.MakeLocal(siteId, readFromRegistry);
		else if (IsFlagSet(flags, ConvertPathFlags, dtsCpMakeVirtual))
			req.MakeVirtual(siteId, readFromRegistry);
		else if (IsFlagSet(flags, ConvertPathFlags, dtsCpMakeUNC))
			req.MakeUNC();

		// Need to convert to Unicode before creating a String
		String^ result = req.ConvertedPath.allocNetString();

		return result;
	}

	SCS::StringCollection ^Server::Tokenize(String ^input)
	{	INIT
		DStringSetProperty set;
		set.tokenize(input);
		SCS::StringCollection ^ret = gcnew SCS::StringCollection;
		for (int i = 0; i < set.getCount(); ++i) {
			ret->Add(DString::allocNetString(set.getString(i)));
			}
		return ret;
		}

	String ^Server::AllocatedObjectList() {
		INIT
		DString list;
		ApiObjectCounter::GetObjectCount(list);
		return list.allocNetString();
		}

}}
