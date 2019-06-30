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
	char moduleName[MAX_PATH];
	memset(moduleName, 0, sizeof moduleName);
    GetModuleFileName(hi, moduleName, MAX_PATH);
	debugLog("            file:         ", moduleName);
}

	Server::Server()
	{	Trace("Server::Server...1");
		CountCreate(otServer);
		Trace("Server::Server...2");
		dtsVersionInfo vi;
		dtssVersionEx(vi);
		m_majorVersion = vi.majorVersion;
		m_minorVersion = vi.minorVersion;
		m_build = vi.build;
		
	}
	
	Server::~Server()
	{	CountDelete(otServer);
	}

	void Server::SetDebugLogging (String* logName, DebugLogFlags flags)
	{	INIT
	
		if (flags == dtsCrashLog)
		{
			dtsCrashHandlerInfo crashHandler;
			DStringProperty crashName;
			crashName.set(logName);
			strncpy(crashHandler.logName, crashName, sizeof crashHandler.logName);
			strncpy(crashHandler.appName, "dtSearchEngine.Server", sizeof crashHandler.appName);
			strcpy(crashHandler.appVersion, __DATE__ " " __TIME__);
			dtssInstallCrashDiagnostics(crashHandler);
		}
		else
		{
			DStringProperty log;
			log.set(logName);
			dtssDebugLogEx(log, flags);
			gfLoggingEnabled = !strIsBlank(log);
			debugLog("\r\ndtSearch .NET 1.1 API version \r\n", __DATE__);
		}
	}

	// Converts paths in web applications
	String* Server::ConvertPath (String* path, long siteId, ConvertPathFlags flags, String* tableLocation)
	{	INIT
	
		DConvertPathRequest req;
		req.Path.set(path);
		req.TableRegistryKey.set(tableLocation);

		if (flags & dtsCpRefreshMappings)
			req.RefreshMappings();
		
		bool readFromRegistry = (flags & dtsCpReadFromRegistry)? true : false;
			
		if (flags & dtsCpMakeLocal)
			req.MakeLocal(siteId, readFromRegistry);
		else if (flags & dtsCpMakeVirtual)
			req.MakeVirtual(siteId, readFromRegistry);
		else if (flags & dtsCpMakeUNC)
			req.MakeUNC();

		// Need to convert to Unicode before creating a String
		String* result = req.ConvertedPath.allocNetString();

		return result;
	}

	SCS::StringCollection *Server::Tokenize(String *input)
	{	DStringSetProperty set;
		set.tokenize(input);
		SCS::StringCollection *ret = new SCS::StringCollection;
		for (int i = 0; i < set.getCount(); ++i) {
			ret->Add(DString::allocNetString(set.getString(i)));
			}
		return ret;
		}

	String *Server::AllocatedObjectList() {
		INIT
		DString list;
		ApiObjectCounter::GetObjectCount(list);
		return list.allocNetString();
		}
	
}}
