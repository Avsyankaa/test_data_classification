#pragma once
#ifndef CRegistryKey_H
    #define CRegistryKey_H

#if !defined(__WIN32__)
    #include <shellapi.h>
#endif
#ifndef DSTRING_H
    #include <dstring.h>
#endif
#ifndef TCSTRING_H
	#include <tcstring.h>
#endif

#ifdef USE_DTSEARCH_NAMESPACE
    namespace dtSearch {
#endif

enum RegistryCategory {
    HKeyCurrentUser, HKeyLocalMachine, HKeyNone };

class CRegistryKey {
    public:
        CRegistryKey();
        ~CRegistryKey();
        int openRead(RegistryCategory parent, const char *subkey);
        int openrw(HKEY parent, const char *subKey, DWORD otherFlags = 0);
		int openrw32(HKEY parent, const char *subKey, DWORD otherFlags = 0) {
			otherFlags |= KEY_WOW64_32KEY;
			return openrw(parent, subKey, otherFlags);
			}
		int openrw64(HKEY parent, const char *subKey, DWORD otherFlags = 0) {
			otherFlags |= KEY_WOW64_64KEY;
			return openrw(parent, subKey, otherFlags);
			}
		int openrw(RegistryCategory parent, const char *subkey);
        int openrw(const char *key) {
            return openrw(0, key);
            }
        int openRead(HKEY parent, const char *subKey, DWORD otherFlags = 0);
        int openRead(const CRegistryKey& parent, const char *subKey) {
            return openRead(parent.hKey, subKey);
            }
        int openRead(const char *key) {
            return openRead(0, key);
            }
        void close();
        int create(HKEY parent, const char *subkey, DWORD otherFlags = 0);
		int create(const char *key, DWORD otherFlags = 0);
        
        int create(RegistryCategory parent, const char *keyPath);
        int createTemp(RegistryCategory parent, const char *keyPath);
        void setValue(const char *valueName, const char *text);
        DWORD setValue(const char *valueName, long l);
        DWORD setDwordValue(const char *valueName, DWORD v);
        DWORD setBoolValue(const char *valueName, bool v) {
			long t = (v? 1 : 0);
			return setValue(valueName, t);
			}
        void setValue(const char *valueName, DStringSet& set);
#if defined(MfcAvailable) 
        void setValueUi(const char *valueName, const TCHAR *text);
        CString getValueUi(const char *valueName);
        int getValueUi(const char *valueName, CString& dest);
#endif
        int getValue(const char *valueName, DString& dest);
        int getDefaultValue(DString& dest) {
			return getValue("", dest);
			}
		int getMultiStringValue(const char *valueName, TCStringSet& dest);
		void setMultiStringValue(const char *valueName, const TCStringSet& source);
        int getValue(const char *valueName, long& dest);
        int getDwordValue(const char *valueName, DWORD& dest);
        int getValue(const char *valueName, DStringSet& set);
        int getValues(DStringSet& names, DStringSet& values);
        int getBoolValue(const char *valueName, bool& dest) {
			long x = 0;
			if (getValue(valueName, x) == FAIL)	
				return FAIL;
			dest = makeBool(x);
			return SUCCESS;
			}
        int getBoolValue(const char *valueName, bool& dest, bool defaultValue) {
			if (getBoolValue(valueName, dest) == FAIL) {
				dest = defaultValue;
				return FAIL;
				}
			return SUCCESS;
			}
		
        void setValues(const DStringSet& names, const DStringSet& values);
        int good();
        int getSubKeys(DStringSet& subKeys);
        int enumSubKey(int iSubKey, DString& subKeyName);
        HKEY getHandle() { 
            return hKey;
            }
        void deleteValue(const TCHAR *val);

        static int getValue(const char *name, const char *valueName, DString& dest);
        static int getValue(HKEY parent, const char *name, const char *valueName, DString& dest);
		static int setStringValue_3264(HKEY parent, const char *name, const char *valueName, const char *val);
        static int getDwordValue(HKEY parent, const char *name, const char *valueName, DWORD& dest);
        static int setDwordValue(HKEY parent, const char *name, const char *valueName, DWORD val);
        int getChildValue(const char *child, const char *valueName, DString& dest);
        static DWORD deleteKeyRecursive(HKEY parent, const TCHAR *subkey);
		static void dumpRegistryKeyRecursive(HKEY hStartKey, const char *name, DString& dest) ;
        DWORD getError() {
			return lastError;
			}
    protected:
        HKEY hKey;
        DWORD lastError;
        int saveOpenError() {
			DWORD err = GetLastError();
			if (err)
				lastError = err;
			hKey = 0;
			return FAIL;
			}
    };
#ifdef USE_DTSEARCH_NAMESPACE
    } // namespace dtSearch
#endif


#endif
