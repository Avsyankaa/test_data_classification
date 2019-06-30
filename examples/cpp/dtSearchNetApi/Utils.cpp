#include "stdafx.h"

#include "Utils.h"
#include "SearchJob.h"
#include <Registrykey.h>

using namespace System;
using namespace System::Collections::Specialized;

namespace dtSearch { namespace Engine {

DateTime Converters::MakeNetDate(dtsFileDate& dtsDate)
{	
	try {
		if (dtsDate.year > 1) {
			DateTime ret(dtsDate.year, dtsDate.month, dtsDate.day,
				dtsDate.hour, dtsDate.minute, dtsDate.second, 0);
			return ret;
			}
		}
	catch(ArgumentOutOfRangeException *) {
		}
	catch(ArgumentException *) {
		}
	DateTime empty(1,1,1,0,0,0,0);
	return empty;
}

int Converters::MakeNetArray(long *data, int count) __gc[]
{	int ret __gc[] = new int __gc[count];
	for (int i = 0; i < count; ++i) {
		ret[i] = data[i];
		}
	return ret;
}

void Converters::GetNetArray(DArrayAsVector<long> *darray, int netArray __gc[])
{	if (!darray)
		return;
		
	darray->clear();

	try {
		if (netArray->Length > 0) {
			darray->extend(netArray->Length);
			for (int i = 0; i < netArray->Length; ++i) {
				long v = netArray[i];
				darray->append(v);
				}
			}
		}
	catch(System::NullReferenceException *) {
		darray->clear();
		}

}

String *Converters::MakeNetArray(const char *buf) __gc[]
{	if (!buf)
		buf = "";
	int ct = 0;
	for (const char *p = buf; *p; p += (strlen(p)+1)) 
		ct++;
		
	String *ret __gc[] = new String * __gc[ct];
	int i = 0;
	for (const char *p = buf; *p; p += (strlen(p)+1)) {
		ret[i] = MakeNetString(p);
		i = i + 1;
		}
	return ret;
}

StringDictionary *Converters::MakeDictionary(const char *s)
{	StringDictionary *ret = new StringDictionary;
	while(s && *s) {
		const char *name = s;
		s += (strlen(s) + 1);
		if (*s) {
			const char *value = s;
			s += (strlen(s) + 1);
			ret->Add(MakeNetString(name), MakeNetString(value));
			}
		}
	
	return ret;
}

String *Converters::MakeNetString(const char *s)
{	String *ret;
	if (!s)
		return ret;
	else
		return DString::allocNetString(s);
}

SearchResultsItem *Converters::MakeResultsItem(dtsSearchResultsItem *item)
{	SearchResultsItem *ret = new SearchResultsItem;
	if (!strIsBlank(item->filename)) {
		ret->Filename = MakeNetString(item->filename);
		ret->DisplayName = MakeNetString(item->displayName);
		ret->Location = MakeNetString(item->location);
		ret->ShortName = MakeNetString(item->shortName);
		ret->Title = MakeNetString(item->title);
		ret->WordCount = item->wordCount;
		ret->Size = item->size;
		ret->IndexedBy = item->indexedBy;
		ret->ModifiedDate = MakeNetDate(item->modified);
		ret->CreatedDate = MakeNetDate(item->created);
		ret->TypeId = (dtSearch::Engine::TypeId) item->typeId;
		}
	ret->HitCount = item->hitCount;
	ret->WhichIndex = item->iIndex;
	ret->Score = item->score;
	ret->ScorePercent = item->scorePercent;
	ret->DocId = item->docId;
	ret->Hits = MakeNetArray(item->hits, item->hitCount);
	ret->IndexRetrievedFrom = MakeNetString(item->indexRetrievedFrom);	
	ret->Synopsis = MakeNetString(item->synopsis);
	if (!strIsBlank(item->hitDetails))
		ret->HitDetails = MakeNetArray(item->hitDetails);
	if (!strIsBlank(item->hitsByWord))
		ret->HitsByWord = MakeNetArray(item->hitsByWord);
	if (!strIsBlank(item->userFields))
		ret->UserFields = MakeDictionary(item->userFields);
	ret->PhraseCount = item->phraseCount;
	return ret;
}


}}  // namespace dtSearch.Engine



#define EngineName "dten600.dll"

void TriggerDelayLoad()
{	dtsVersionInfo vi;
	dtssVersionEx(vi);
}

bool TryLoad(const char *n)
{	// Make sure the module is named dten600.dll
	DFilename fn = n;
	DFilename tail;
	fn.getTail(tail);
	if (stricmp(tail, EngineName)) {
		DString msg;
		msg << "Wrong module name: " << n;
		Trace(msg);
		return false;
		}
		
	HINSTANCE hi = LoadLibrary(n);
	if (hi) {
		OutputDebugString("Found dtSearch Engine");
		OutputDebugString(n);
		TriggerDelayLoad();
		FreeLibrary(hi);
		return true;
		}
	return false;
}

bool LoadEngine()
{
	// Load the dtSearch Engine DLL, dten600.dll, into memory 
	//
	// This .NET wrapper DLL is linked with the dtSearch Engine DLL delay-loaded, so it will
	// not be loaded until one of the dtSearch Engine API functions is called.
	// This code attempts to locate it, first using LoadLibrary, and then using CreateObject,
	// so the DLL will already be in memory when the delay-load implementation looks for it.
	
	// (1) Check if the dll is already loaded
	Trace("Calling GetModuleHandle");
	if (GetModuleHandle(EngineName)) {
		Trace("Found using GetModuleHandle");
		TriggerDelayLoad();
		return true;
		}

	Trace("Calling LoadLibrary");
	// (2) Try to load it using LoadLibrary
	if (TryLoad("dten600.dll")) {
		Trace("Found using Load Library");
		return true;
		}

	Trace("LoadLibrary failed, looking in registry");
	// This key is created when the dtSearch Engine is registered
	const TCHAR *typelibkey = "CLSID\\{A967E5D2-B0E1-11D3-B57C-00105AA461D0}\\InprocServer32";
	// (3) Try to load it using CreateObject()
	
	HKEY hk = 0;
	bool ret = false;
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, typelibkey, 0, KEY_READ, &hk) == ERROR_SUCCESS) {
		OutputDebugString("Registry key found");
		TCHAR buf[MAX_PATH];
		DWORD cb = MAX_PATH, type = 0;
		memset(&buf, 0, sizeof buf);
		if (RegQueryValueEx(hk, 0, 0, &type, (LPBYTE) buf, &cb) == ERROR_SUCCESS) {
			OutputDebugString(buf);
			if (ret = TryLoad(buf)) {
				OutputDebugString("Loaded with LoadLibrary from type lib");
				}
			}
		RegCloseKey(hk);
		}			
	else {
		Trace("Could not find dten600.dll");
		System::IO::FileNotFoundException __gc *ex = new System::IO::FileNotFoundException("dten600.dll");
		throw ex;
		}
	return ret;
}

