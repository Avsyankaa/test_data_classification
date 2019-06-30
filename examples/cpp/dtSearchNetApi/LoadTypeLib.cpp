#include <stdlib.h>
#include <windows.h>


#import <dten600.dll> 
#include <dtsearch.h>

void LoadEngineFromTypelib()
{	CoInitialize(0);
	OutputDebugString("LoadEngineFromTypeLib...");
	dtengine::IServerPtr svr;
	HRESULT hr = svr.CreateInstance(__uuidof(dtengine::IServer));
	if (!SUCCEEDED(hr)) {
		OutputDebugString("CreateInstance failed");
		char ret[80];
		ultoa(hr & 0x7fffffff, ret, 16);
		OutputDebugString(ret);
		}
	else {		
		dtsVersionInfo vi;
		dtssVersionEx(vi);
		OutputDebugString("LoadEngineFromTypeLib... Done");
		}

}
