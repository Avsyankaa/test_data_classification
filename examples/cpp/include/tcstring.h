#pragma once
#ifndef TCSTRING_H
	#define TCSTRING_H

#ifndef DSTRING_H
	#include <dstring.h>
#endif

// TCString is a string consisting of TCHAR
#ifdef UNICODE
	#define TCString 	DWString 
	#define TCStringSet DWStringSet
#else
	#define TCString 	DString 
	#define TCStringSet DStringSet
#endif

#ifdef USE_DTSEARCH_NAMESPACE
	namespace dtSearch {
#endif



class CUiStringSet {
    public:
        CUiStringSet() { }
        ~CUiStringSet() { }
        void append(const TCHAR *s) {
            m_imp.append(s);
            }
        int find(const TCHAR *s) const {
            return m_imp.find(s);
            }
        int ifind(const TCHAR *s) const {
            return m_imp.ifind(s);
            }
        int getCount() const {
            return m_imp.getCount();
            }
        const TCHAR *getString(int i) const {
            return m_imp.getString(i);
            }
		void copyFromBuf(const TCHAR *buf) {
			clear();
			m_imp.copyFromBuf(buf);
			}
        CUiStringSet& operator=(const CUiStringSet& other) {
            m_imp = other.m_imp;
            return *this;
            }
        void append(const CUiStringSet& other) {
            m_imp.append(other.m_imp);
            }
        void appendU8(const char *s) {
			m_imp.append(Utf8ToUi(s));
			}
        void iappendUnique(const TCHAR *s) {
            if (ifind(s) == FAIL)
                append(s);
            }
        void iunion(CUiStringSet& other) {
			m_imp.iunion(other.m_imp);
			}
        void tokenize(const TCHAR *s, char sepChar, int fAllowEmptyTokens = 0) {
            m_imp.tokenize(s, sepChar, fAllowEmptyTokens);
            }
        void tokenize(const TCHAR *s, const TCHAR *sepChars, int fAllowEmptyTokens = 0) {
            m_imp.tokenize(s, sepChars, fAllowEmptyTokens);
            }
        void tokenizeq(const TCHAR *s, const TCHAR *sepChars, char quoteChar = '\"') {
            m_imp.tokenizeq(s, sepChars, quoteChar);
            }
        long getInt(int i) const {
            return m_imp.getInt(i);
            }
        void clear() {
            m_imp.clear();
            }
        void deleteString(int iString) {
            m_imp.deleteString(iString);
            }
        const TCHAR *getBuffer() const {
            return m_imp.getBuffer();
            }
        void formatAsString(TCString& dest, const TCHAR *delimiter = _T(" "), char quote = '\"') const {
            m_imp.formatAsString(dest, delimiter, quote);
            }
        void formatAsSimpleString(TCString& dest, char delimiter = ' ', char quote = '\"') const {
            m_imp.formatAsSimpleString(dest, delimiter, quote);
            }
#if defined(MfcAvailable)            
        CString formatAsString(const TCHAR *delimiter = _T(" "), char quote = '\"') const {
            TCString tmp;
            m_imp.formatAsString(tmp, delimiter, quote);
            CString ret = tmp;
            return ret;
            }
        CString formatAsSimpleString(char delimiter = ' ', char quote = '\"') const {
            TCString tmp;
            m_imp.formatAsSimpleString(tmp, delimiter, quote);
            CString ret = tmp;
            return ret;
            }
#endif
            
#if defined(MfcAvailable) && defined(DStringProperty_H)
		void getFrom(DStringSetProperty& set) {
			clear();
			for (int i = 0; i < set.getCount(); ++i)
				append(set.getStringUi(i));
			}
#endif
		void copy(const CUiStringSet& other) {
			m_imp.copy(other.m_imp);
			}
		void copyTo(DWStringSet& other) {
	#ifdef UNICODE
			other.copy(m_imp);
	#else
			
			other.clear();
			for (int i = 0; i < m_imp.getCount(); ++i) {
				DString s;
				s = UiToUtf8(m_imp.getString(i));
				other.appendU8(s);
				}

	#endif
		}
    protected:
    	TCStringSet m_imp;
    };

#ifdef USE_DTSEARCH_NAMESPACE
}  // dtSearch namespace
#endif



#endif
