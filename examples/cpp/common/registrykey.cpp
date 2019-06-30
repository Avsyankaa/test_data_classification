#include <stdafx.h>
#include <RegistryKey.h>

#ifdef USE_DTSEARCH_NAMESPACE
    using namespace dtSearch;
#endif

#ifndef FAIL
#   define FAIL (-1)
#   define SUCCESS 0
#endif

static char backsl = '\\';
static char nl = '\n';
static char quote = '\"';


////////////////////////////////////////////////////////////////////
//
//  CRegistryKey handles creating and querying registry settings
//


CRegistryKey::CRegistryKey() :
    hKey(0),
    lastError(0)
{
}

CRegistryKey::~CRegistryKey()
{   close();
}

void CRegistryKey::close()
{   if (hKey) 
        RegCloseKey(hKey);
    hKey = 0;
    lastError = 0;
    
}

static HKEY CategoryToKey(RegistryCategory p)
{   switch(p) {
        case HKeyCurrentUser:
            return HKEY_CURRENT_USER;
        case HKeyLocalMachine:
            return HKEY_LOCAL_MACHINE;
        case HKeyNone:
        default:
            return 0;
        }
}

#ifdef _UNICODE
long __RegOpenKeyEx(HKEY hkParent, const char *subkey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult)
{   DWString ws(subkey);
    return RegOpenKeyEx(hkParent, ws, ulOptions, samDesired, phkResult);
}

LONG __RegCreateKeyEx(  HKEY hKey,                // handle to an open key
  const char *lpSubKey,         // address of subkey name
  DWORD Reserved,           // reserved
  const char *lpClass,           // address of class string
  DWORD dwOptions,          // special options flag
  REGSAM samDesired,        // desired security access
  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                            // address of key security structure
  PHKEY phkResult,          // address of buffer for opened handle
  LPDWORD lpdwDisposition   // address of disposition value buffer
    )
{   DWString usSubKey(lpSubKey);
    DWString usClass(lpClass);
    return RegCreateKeyEx(hKey, usSubKey, Reserved, usClass.str(), dwOptions, samDesired, lpSecurityAttributes,
        phkResult, lpdwDisposition);
}

#else
#   define __RegOpenKeyEx           RegOpenKeyEx
#   define __RegCreateKeyEx         RegCreateKeyEx
#endif

int CRegistryKey::openRead(RegistryCategory category, const char *subkey)
{   return openRead(CategoryToKey(category), subkey);
}

static const char *HKLM = "HKEY_LOCAL_MACHINE";
static const char *HKCU = "HKEY_CURRENT_USER";
static const char *HKCR = "HKEY_CLASSES_ROOT";

static HKEY NameToKey(const char *name, long& offset)
{   if (!_strnicmp(name, HKLM, strlen(HKLM))) {
        offset = (long) (strlen(HKLM)+1);
        return HKEY_LOCAL_MACHINE;
        }
    if (!_strnicmp(name, HKCU, strlen(HKCU))) {
        offset = (long) (strlen(HKCU)+1);
        return HKEY_CURRENT_USER;
        }
    if (!_strnicmp(name, HKCR, strlen(HKCR))) {
		offset = (long) (strlen(HKCR)+1);
		return HKEY_CLASSES_ROOT;
		}
    offset = 0;
    return 0;
}

int CRegistryKey::openRead(HKEY hkParent, const char *subkey, DWORD otherFlags)
{   close();
	if (strIsBlank(subkey))
		return FAIL;

    if (!hkParent) {
        long offset = 0;
        hkParent = NameToKey(subkey, offset);
        subkey += offset;
        }
    if ((lastError = __RegOpenKeyEx(hkParent, subkey, 0, KEY_READ | otherFlags, &hKey)) == ERROR_SUCCESS)
        return SUCCESS;
    else 
		return saveOpenError();
}


int CRegistryKey::openrw(RegistryCategory parent, const char *subkey)
{
    HKEY parentKey = CategoryToKey(parent);
    return openrw(parentKey, subkey);
}

int CRegistryKey::openrw(HKEY parentKey, const char *subkey, DWORD otherFlags)
{   
	if (strIsBlank(subkey))
		return FAIL;

    if (!parentKey) {
        long offset = 0;
        parentKey = NameToKey(subkey, offset);
        subkey += offset;
        }

    close();

    if ((lastError = __RegOpenKeyEx(parentKey, subkey, 0, KEY_ALL_ACCESS | otherFlags, &hKey)) == ERROR_SUCCESS)
        return SUCCESS;
    else if (create(parentKey, subkey) == SUCCESS)
        return SUCCESS;
    else
		return saveOpenError();
}



int CRegistryKey::create(RegistryCategory p, const char *subkey)
{   HKEY parentKey = CategoryToKey(p);
    return create(parentKey, subkey);
}

int CRegistryKey::create(const char *key, DWORD otherFlags)
{
    long offset = 0;
    HKEY parentKey = NameToKey(key, offset);
    key += offset;
	return create(parentKey, key, otherFlags);

}

int CRegistryKey::create(HKEY parentKey, const char *subkey, DWORD otherFlags)
{   close();
    lastError = __RegCreateKeyEx(
        parentKey,
        subkey,     // Name of the key
        0,          // DWORD reserved
        "",         // Object class
        REG_OPTION_NON_VOLATILE,
        otherFlags | KEY_ALL_ACCESS,  // sam desired
        NULL,       // security attributes
        &hKey,      // returned handle
        NULL        // disposition value buffer
        );
    if (lastError == ERROR_SUCCESS)
        return SUCCESS;
    return saveOpenError();
}

void CRegistryKey::deleteValue(const TCHAR *value)
{   RegDeleteValue(hKey, value);
}


int CRegistryKey::createTemp(RegistryCategory p, const char *subkey)
{   HKEY parentKey = CategoryToKey(p);

    close();
    if (__RegCreateKeyEx(
        parentKey,
        subkey,     // Name of the key
        0,          // DWORD reserved
        "",         // Object class
        REG_OPTION_VOLATILE,
        KEY_ALL_ACCESS,
        NULL,       // security attributes
        &hKey,
        NULL) == ERROR_SUCCESS)
            return SUCCESS;
    else 
		return saveOpenError();
}


int CRegistryKey::good()
{   return (hKey != 0);
}

#if defined(MfcAvailable) 
void CRegistryKey::setValueUi(const char *aValueName, const TCHAR *text)
{   CUiStringFromUtf8 valueName = aValueName;

    RegSetValueEx(
        hKey,               // handle of key to set value for
        valueName,          // address of value to set
        0,                  // reserved
        REG_SZ,             // flag for value type
        (const unsigned char *) text,               // address of value data
        (DWORD)(_tcslen(text) * sizeof(TCHAR)));    // size of value data
}

CString CRegistryKey::getValueUi(const char *valueName)
{   DString tmp;
    getValue(valueName, tmp);
    CString ret;
    ret = Utf8ToUi(tmp);
    return ret;
}

int CRegistryKey::getValueUi(const char *valueName, CString& dest)
{   DString tmp;
    int ret = getValue(valueName, tmp);
    dest = Utf8ToUi(tmp);
    return ret;
}


#endif



void CRegistryKey::setValue(const char *aValueName, const char *aText)
{   if (!good())
        return;

    CUiStringFromUtf8 valueName = aValueName;
    CUiStringFromUtf8 text = aText;

	DWORD valueDataSize = (DWORD) ((text.getLength() + 1) * sizeof(TCHAR));
    RegSetValueEx(
        hKey,               // handle of key to set value for
        valueName,          // address of value to set
        0,                  // reserved
        REG_SZ,             // flag for value type
        (const unsigned char *) text.str(),               // address of value data
        valueDataSize);    // size of value data
}

DWORD CRegistryKey::setValue(const char *aValueName, long l)
{   if (!good())
        return (DWORD)-1;

    CUiStringFromUtf8 valueName = aValueName;

    return RegSetValueEx(
        hKey,               // handle of key to set value for
        valueName,          // address of value to set
        0,                  // reserved
        REG_DWORD,          // flag for value type
        (unsigned char *) &l,                // address of value data
        sizeof l);  // size of value data

}

DWORD CRegistryKey::setDwordValue(const char *aValueName, DWORD l)
{   if (!good())
        return (DWORD)-1;

    CUiStringFromUtf8 valueName = aValueName;

    return RegSetValueEx(
        hKey,               // handle of key to set value for
        valueName,          // address of value to set
        0,                  // reserved
        REG_DWORD,          // flag for value type
        (unsigned char *) &l,                // address of value data
        sizeof l);  // size of value data

}


int CRegistryKey::getSubKeys(DStringSet& subKeys)
{   int ret = SUCCESS;
    for (int i = 0; ret != FAIL; ++i) {
        DString subKey;
        ret = enumSubKey(i, subKey);
        if (ret != FAIL)
            subKeys.append(subKey);
        }
    return SUCCESS;
}

int CRegistryKey::enumSubKey(int iSubKey, DString& subKeyName)
{	TCFilenameBuf buf;

    long ret = RegEnumKey(hKey, iSubKey, buf, buf.size());
    if (ret == ERROR_SUCCESS) {
        subKeyName = buf;
        return SUCCESS;
        }
    else
        return FAIL;
}

int CRegistryKey::getMultiStringValue(const char *aValueName, TCStringSet& dest)
{	dest.clear();
    if (!good())
        return FAIL;

    TCHAR *tmp = 0;
    unsigned long cb = 0;
    unsigned long type = 0
    ;
    CUiStringFromUtf8 valueName = aValueName;
    long v = RegQueryValueEx(hKey, valueName.str(), 0, &type, 0, &cb);
    if (v != ERROR_SUCCESS)
        return FAIL;
        
    switch(type) {
        case REG_SZ: {
            tmp = new TCHAR[cb+1];
            memset(tmp, 0, sizeof(TCHAR) * (cb+1));
            v = RegQueryValueEx(hKey, valueName.str(), 0, &type, (unsigned char *) tmp, &cb);
            if (v == ERROR_SUCCESS) 
                dest.append(tmp);
            delete [] tmp;
            return (v == ERROR_SUCCESS? SUCCESS : FAIL);
            }
        case REG_MULTI_SZ: {	
			// RegQueryValueEx does not always add terminating null characters
            tmp = new TCHAR[cb+2];
            memset(tmp, 0, sizeof(TCHAR) * (cb+2));
            v = RegQueryValueEx(hKey, valueName.str(), 0, &type, (unsigned char *) tmp, &cb);
            if (v == ERROR_SUCCESS) {
				dest.copyFromBuf(tmp);
				}
            delete [] tmp;
            return (v == ERROR_SUCCESS? SUCCESS : FAIL);
            }

        default:
			break;
        }
    return FAIL;
}

#ifndef _T
    #define _T(x) x
    #define _tcscpy strcpy
#endif



void CRegistryKey::setMultiStringValue(const char *aValueName, const TCStringSet& source)
{   CUiStringFromUtf8 valueName = aValueName;

	DWORD valueDataSize = (DWORD) (source.getLength() * sizeof(TCHAR));
    RegSetValueEx(
        hKey,               // handle of key to set value for
        valueName,          // address of value to set
        0,                  // reserved
        REG_MULTI_SZ,             // flag for value type
        (const BYTE *) source.getBuffer(),  //
        valueDataSize);    // size of value data

}

static void ExpandEnvironmentStringsU8(DString& s)
{
	if (s.isBlank())
		return;
	TCString input = Utf8ToUi(s);
	TCString output;
	ptrdiff_t size = ExpandEnvironmentStrings(input, output.str(), (DWORD) output.getMaxLength()-1);
	if (size > 0)
		output.setLength(size);
	s = UiToUtf8(output);
}



int CRegistryKey::getValue(const char *aValueName, DString& dest)
{   dest.clear();
    if (!good())
        return FAIL;

    TCHAR *tmp = 0;
    unsigned long cb = 0;
    unsigned long type = 0
    ;
    CUiStringFromUtf8 valueName = aValueName;
    long v = RegQueryValueEx(hKey, valueName.str(), 0, &type, 0, &cb);
    if (v != ERROR_SUCCESS)
        return FAIL;

    switch(type) {
        case REG_DWORD: {
            long intVal;
            v = RegQueryValueEx(hKey, valueName.str(), 0, &type, (unsigned char *) &intVal, &cb);
            if (v == ERROR_SUCCESS)
                dest << intVal;
            return v;
            }
		case REG_EXPAND_SZ:
        case REG_SZ: {
            tmp = new TCHAR[cb+1];
            memset(tmp, 0, sizeof(TCHAR) * (cb+1));
            v = RegQueryValueEx(hKey, valueName.str(), 0, &type, (unsigned char *) tmp, &cb);
            if (v == ERROR_SUCCESS) 
                dest = UiToUtf8(tmp);
            delete [] tmp;
			if (type == REG_EXPAND_SZ)
				ExpandEnvironmentStringsU8(dest);
            return (v == ERROR_SUCCESS? SUCCESS : FAIL);
            }
        case REG_MULTI_SZ: {	
			// RegQueryValueEx does not always add terminating null characters
            tmp = new TCHAR[cb+2];
            memset(tmp, 0, sizeof(TCHAR) * (cb+2));
            v = RegQueryValueEx(hKey, valueName.str(), 0, &type, (unsigned char *) tmp, &cb);
            if (v == ERROR_SUCCESS) {
				TCStringSet set;
				set.copyFromBuf(tmp, cb);
				TCString ts;
				set.formatAsSimpleString(ts, '|');
				dest = UiToUtf8(ts);
				}
            delete [] tmp;
            return (v == ERROR_SUCCESS? SUCCESS : FAIL);
            }

        default:
			break;
        }
    return FAIL;
}

#ifndef _T
    #define _T(x) x
    #define _tcscpy strcpy
#endif

int CRegistryKey::getValues(DStringSet& names, DStringSet& values)
{   names.clear();
    values.clear();
    if (!good())
        return FAIL;

    TCHAR valueName[512];
    TCHAR data[512];

    DString val;
    names.clear();
    values.clear();
    unsigned long type;
    long v = ERROR_SUCCESS;
    for (int iVal = 0; v == ERROR_SUCCESS; iVal++) {
        unsigned long dwSize = sizeof valueName;
        unsigned long dwSize2 = sizeof data;
		valueName[0] = 0;
		data[0] = 0;
        v = RegEnumValue(
                    hKey,       // Key handle
                    iVal,       // Value to get
                    valueName,  // buffer for name
                    &dwSize,    // buffer size (out = bytes stored)
                    NULL,
                    &type,
                    (unsigned char * )data,     // buffer for data
                    &dwSize2    // buffer size
                    );
        if (v == ERROR_SUCCESS && dwSize && dwSize2) {
            DString n = UiToUtf8(valueName);
            names.append(n);
            switch(type) {
                case REG_SZ: {  
                    DUtf8StringFromUiString s = data;
                    values.append(s);
                    break;
                    }
                case REG_DWORD: {
                    unsigned int intVal;
                    memmove(&intVal, data, sizeof intVal);
					PadHexNum hex(intVal, 8);
					DString80 t;
					t << "dword:" << hex;
                    values.append(t);
                    break;
                    }
                default:
                    values.append(" ");
                    break;
                }
            }
        }
    return names.getCount();
}

void CRegistryKey::setValues(const DStringSet& names, const DStringSet& values)
{   if (!good())
        return;
    for (int i = 0; i < names.getCount(); ++i)
        setValue(names.getString(i), values.getString(i));
}


int CRegistryKey::getValue(const char *aValueName, long& dest)
{   if (!good()) 
        return FAIL;

    unsigned long cb = sizeof(long);
    unsigned long type = 0;
    CUiStringFromUtf8 valueName = aValueName;
    long v = RegQueryValueEx(hKey, valueName.str(), 0, &type, (unsigned char *) &dest, &cb); 
    if ((v == ERROR_SUCCESS) && (type == REG_DWORD))
        return SUCCESS;
    return FAIL;
}

int CRegistryKey::getDwordValue(const char *aValueName, DWORD& dest)
{   if (!good()) 
        return FAIL;

    unsigned long cb = sizeof(DWORD);
    unsigned long type = 0;
    CUiStringFromUtf8 valueName = aValueName;
    long v = RegQueryValueEx(hKey, valueName.str(), 0, &type, (unsigned char *) &dest, &cb); 
    if ((v == ERROR_SUCCESS) && (type == REG_DWORD))
        return SUCCESS;
    return FAIL;
}


void CRegistryKey::setValue(const char *valueName, DStringSet& set)
{   if (!good())
        return;

    DString val;
    set.formatAsString(val, ", ", '\"');
    setValue(valueName, val);
}

int CRegistryKey::getValue(const char *valueName, DStringSet& set)
{   set.clear();
    if (!good())
        return FAIL;
    DString val;
    if (getValue(valueName, val) == SUCCESS) {
        set.tokenizeq(val, ", ", '\"');
        return SUCCESS;
        }
    return FAIL;
}

DWORD CRegistryKey::deleteKeyRecursive(HKEY hStartKey, const TCHAR *pKeyName)
{
// Q142491
      DWORD   dwRtn, dwSubKeyLength;
      TCFilenameBuf szSubKey;
      HKEY    hKey;

      // Do not allow NULL or empty key name
      if ( pKeyName &&  lstrlen(pKeyName))
      {
         if( (dwRtn=RegOpenKeyEx(hStartKey,pKeyName,
            0, KEY_ENUMERATE_SUB_KEYS | DELETE, &hKey )) == ERROR_SUCCESS)
         {
            while (dwRtn == ERROR_SUCCESS )
            {
               dwSubKeyLength = szSubKey.size();
               dwRtn=RegEnumKeyEx(
                              hKey,
                              0,       // always index zero
                              szSubKey,
                              &dwSubKeyLength,
                              NULL,
                              NULL,
                              NULL,
                              NULL
                            );

               if(dwRtn == ERROR_NO_MORE_ITEMS)
               {
                  dwRtn = RegDeleteKey(hStartKey, pKeyName);
                  break;
               }
               else if(dwRtn == ERROR_SUCCESS)
                  dwRtn= deleteKeyRecursive(hKey, szSubKey);
            }
            RegCloseKey(hKey);
            // Do not save return code because error
            // has already occurred
         }
      }
      else
         dwRtn = ERROR_BADKEY;

      return dwRtn;
}


// Exported functions that implement the ISAPI interface
// BOOL WINAPI TerminateExtension(DWORD dwFlags);
// DWORD WINAPI HttpExtensionProc(LPEXTENSION_CONTROL_BLOCK lpEcb);
// BOOL WINAPI GetExtensionVersion(HSE_VERSION_INFO *pVer);

static void DumpRegistryKeyRecursive(HKEY hStartKey, const char *name, DString& dest, DString& fullpath)
{	if (!name || !*name)
		return;

	if (fullpath.isBlank()) {
		if (hStartKey == HKEY_LOCAL_MACHINE)
			fullpath << "HKEY_LOCAL_MACHINE";
		else if (hStartKey == HKEY_CURRENT_USER)
			fullpath << "HKEY_CURRENT_USER";
		if (fullpath.getLength())
			fullpath << backsl;
		fullpath << name;
		}

	dest << "[" << fullpath << "]" << nl;

	CRegistryKey key;
	if (key.openRead(hStartKey, name) == FAIL) {
		dest << "    Open failed, error = " << key.getError() << nl;
		return;
		}

	DStringSet names, values;
	key.getValues(names, values);
	int i;

	for (i = 0; i < names.getCount(); ++i) {
		dest << "    ";
		dest << quote << names.getString(i) << quote << " = ";
		const char *s = values.getString(i);
		if (!_strnicmp(s, "dword:", 6))
			dest << s;
		else {
			dest << quote;
			for (const char *v = values.getString(i); v && *v; ++v) {
				if (*v == quote)
					dest << '\\' << quote;
				else
					dest << *v;
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
			DumpRegistryKeyRecursive(key.getHandle(), subKey, dest, subFullpath);
		}
}


void CRegistryKey::dumpRegistryKeyRecursive(HKEY hStartKey, const char *name, DString& dest) 
{	DString dummy;
	DumpRegistryKeyRecursive(hStartKey, name, dest, dummy);
}

int CRegistryKey::getValue(const char *name, const char *valueName, DString& dest)
{	CRegistryKey rk;
	if (rk.openRead(name) == FAIL)
		return FAIL;
	if (rk.getValue(valueName, dest) == FAIL)
		return FAIL;
	return SUCCESS;
}

int CRegistryKey::getValue(HKEY parent, const char *name, const char *valueName, DString& dest)
{	CRegistryKey rk;
	if (rk.openRead(parent, name) == FAIL)
		return FAIL;
	if (rk.getValue(valueName, dest) == FAIL)
		return FAIL;
	return SUCCESS;
}

int CRegistryKey::getDwordValue(HKEY parent, const char *name, const char *valueName, DWORD& dest)
{	CRegistryKey rk;
	if (rk.openRead(parent, name) == FAIL)
		return FAIL;
	if (rk.getDwordValue(valueName, dest) == FAIL)
		return FAIL;
	return SUCCESS;
}

int CRegistryKey::setDwordValue(HKEY parent, const char *name, const char *valueName, DWORD val)
{	CRegistryKey rk;
	if (rk.openrw(parent, name) == FAIL)
		return FAIL;
	if (rk.setValue(valueName, val) == FAIL)
		return FAIL;
	return SUCCESS;
}

#ifdef _WIN64
static bool IsWin64()
	{
	return true;
	}
#else
typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

static bool IsWin64()
	{
	LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(_T("kernel32")), "IsWow64Process");
	BOOL bIsWow64 = FALSE;

	if (NULL != fnIsWow64Process) {
		fnIsWow64Process(GetCurrentProcess(), &bIsWow64);
		}
	return bIsWow64 ? true : false;
	}
#endif


int CRegistryKey::setStringValue_3264(HKEY parent, const char *name, const char *valueName, const char *val)
	{
	
	int ret = FAIL;
	CRegistryKey rk32;
	if (rk32.openrw32(parent, name) == SUCCESS) {
		rk32.setValue(valueName, val);
		ret = SUCCESS;
		}

	if (IsWin64()) {
		CRegistryKey rk64;
		if (rk64.openrw64(parent, name) == SUCCESS)
			rk64.setValue(valueName, val);
		}

	return ret;
	}



int CRegistryKey::getChildValue(const char *child, const char *valueName, DString& dest) {
	CRegistryKey rkChild;
	if (rkChild.openRead(*this, child) == FAIL)
		return FAIL;
	if (rkChild.getValue(valueName, dest) == FAIL)
		return FAIL;
	return SUCCESS;
	}

