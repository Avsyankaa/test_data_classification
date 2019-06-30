#ifndef StringTools_H
    #define StringTools_H

#ifndef DSTRING_H
#   include <dstring.h>
#endif
#ifndef TCSTRING_H
#	include <tcstring.h>
#endif


class CDialog;

#ifdef USE_DTSEARCH_NAMESPACE
	namespace dtSearch {
#endif

CString Str(long n);
CString StrHex(long n);
CString QuoteString(const TCHAR *s);


class CFilenameFilter {
    public:
        CFilenameFilter();
		CFilenameFilter(const CFilenameFilter& other) {
			copy(other);
			}

        CFilenameFilter& operator=(const CFilenameFilter& other) {
			copy(other);
			return *this;
			}
		void copy(const CFilenameFilter& other) {
            m_include.copy(other.m_include);
            m_exclude.copy(other.m_exclude);
            m_excludeExceptions.copy(other.m_excludeExceptions);
            fCaseInsensitive = other.fCaseInsensitive;
            }
        void SetInclude(const char *includes);
        void SetInclude(const DStringSet& includes);
        void SetExcludeExceptions(const char *includes);
        void SetExcludeExceptions(const DStringSet& includes);
        void AddExcludeException(const char *ex);
        void SetExclude(const char *excludes);
        void SetExclude(const DStringSet& excludes);
        void AddInclude(const char *filter);
        void AddExclude(const char *filter);
        int Matches(const char *s);
        void Image(DString& dest) {
            DString incl,excl;
            if (m_include.getCount()) {
                m_include.formatAsSimpleString(incl);
                dest << "Include: " << incl;
                }
            if (m_exclude.getCount()) {
                m_exclude.formatAsSimpleString(excl);
                dest << " Exclude: " << excl;
                }
            }
        void Clear() {
            m_include.clear();
            m_exclude.clear();
            m_excludeExceptions.clear();
            }
        int IsEmpty() {
			return (m_include.getCount() == 0) && (m_exclude.getCount() == 0);
			}
        static int matchFilter(const char *s, const char *filter);
    protected:
        DStringSet m_include;
        DStringSet m_exclude;
        DStringSet m_excludeExceptions;
        DFilename tmp;
        int fCaseInsensitive;

        const char *preprocess(const char *s);

        static int matchSet(const char *fullName, const char *tail, DStringSet& set);
    };

//
//  These string classes handle conversions between
//  The user interface strings (Ansi or Unicode)
//  and utf8/ansi/Unicode
//


class CResourceString : public DString {
    public:
        CResourceString(int rc) {
            CString tmp;
            tmp.LoadString(rc);
            DWStringFromUiString wTmp = tmp;
            storeUnicodeAsUtf8(wTmp);
            }
    };


class CSFilename : public CString {
    public:
        CSFilename() { }
        CSFilename(const TCHAR *s) :
            CString(s) { }
        CSFilename(const CString& s) :
            CString(s) { }
        CSFilename& operator=(const CSFilename& other) {
            CString::operator =(other);
            return *this;
            }
        CSFilename& operator=(const TCHAR *s) {
            CString::operator=(s);
            return *this;
            }
        CSFilename& operator=(const CString& s) {
            CString::operator=(s);
            return *this;
            }
        int operator==(const CSFilename& other) const {
            return !_tcscmp(*this, other);
            }
        void rmTail() {
            int p = lastSlash();
            if (p <= 0)
                return;
            *this = Left(p);
            }
        void rmPath() {
            int p = lastSlash();
            if (p <= 0)
                return;
            *this = Mid(p+1);
            }
        void setPath(const TCHAR *s) {
			rmPath();
			CString tail = *this;
			*this = s;
			addTail(tail);
		}

        void rmTrailingSlash() {
            TrimRight();
            if (!IsEmpty() && (lastSlash() == GetLength()-1))
                *this = Left(GetLength()-1);
            }
        void addTrailingSlash() {
            if (lastSlash() == GetLength()-1)
                return;
            int iLastSlash = lastSlash();
            if (iLastSlash >= 0)
                *this += GetAt(iLastSlash);
            else
                *this += "\\";
            }
        void addTail(const TCHAR *s) {
			if ((*s != '/') && (*s != '\\'))
				addTrailingSlash();
            *this += s;
            }
        void addTailU8(const char *s) {
			addTail(Utf8ToUi(s));
			}
        int lastSlash() {
            int a = CString::ReverseFind('\\');
            int b = CString::ReverseFind('/');
            return max(a, b);
            };
        void getTail(CString& s) {
            int p = lastSlash();
			if ((p > 0) && (p < GetLength() - 1))
				s = Mid(p + 1);
			else
				s = *this;
            }
        CString getTail() {
			CString csTail;
			getTail(csTail);
			return csTail;
			}
        void getPath(CString& s) {
            int p = lastSlash();
            if (p > 0)
                s = Left(p);
            }
        CString getPath() {
			CString csPath;
			getPath(csPath);
			return csPath;
			}
        void getExt(CString& s) {
            s.Empty();
            int p = CString::ReverseFind('.');
            if (p > lastSlash())
                s = Mid(p+1);
            if (_tcschr(s, '?'))
				s.Empty();
            }
        int isType(const TCHAR *s) {
            CString csExt;
            getExt(csExt);
            if (*s == '.')
                s++;
            return !csExt.CompareNoCase(s);
            }
        int isHtml() {
            return isType(_T("HTML")) || isType(_T("HTM"));
            }
        void setExt(const TCHAR *ext) {
            if (*ext == '.')
                ext++;
            int p = CString::ReverseFind('.');
            if (p > lastSlash())
                *this = Left(p+1) + ext;
            else {
                *this += _T(".");
                *this += ext;
                }
            }
        void rmExt() {
            int p = CString::ReverseFind('.');
            if (p > lastSlash())
                *this = Left(p);
            }
        int isRelative() {
            return DFilename::isRelative(*this);
            }
        int isUnc() {
            return DFilename::isUnc(*this);
            }
        void makeTempName(const TCHAR *s) {
			if (!s || !*s)
				s = _T("TempFile.tmp");
			DFilename fn;
			fn.makeTempName(UiToUtf8(s));
			*this = Utf8ToUi(fn);
            }
        void makeUniqueTempName(const TCHAR *prefix) {
			CString name;
			name.Format(_T("%s_%4x%x.TMP"), prefix, time(0) & 0xffff, (int) clock());
			makeTempName(name);
			}
        void makeAbsoluteFrom(const TCHAR *s) {
            DFilename fn = UiToUtf8(*this);
            DFilename base = UiToUtf8(s);
            fn.makeAbsoluteFrom(base);
            *this = Utf8ToUi(fn);
            }
        void makeRelativeTo(const TCHAR *s) {
            DFilename fn = UiToUtf8(*this);
            DFilename base = UiToUtf8(s);
            fn.makeRelativeTo(base);
            *this = Utf8ToUi(fn);
            }
        int isContainerName() {
            return Find(_T(">")) > 0;
            }
        CString getContainer() {
            CString ret = *this;
            if (isContainerName())
                ret = ret.Left(ret.Find(_T(">")));
            return ret;
            }
		void fullpath(const TCHAR *relPath) {
			TCHAR buf[1024];
			memset(buf, 0, sizeof buf);
			_tfullpath(buf, relPath, 1024);
			*this = buf;
			}
		void quoteIfNeeded() {
			if (Find(' ') < 0)
				return;
			Insert(0, _T("\""));
			*this += _T("\"");
			}
		bool exists() {
			DWORD attr = GetFileAttributes(*this);
			if (attr == INVALID_FILE_ATTRIBUTES)
				return false;
			else
				return true;    
			};
		bool isFile() {
			DWORD attr = GetFileAttributes(*this);
			if (attr == INVALID_FILE_ATTRIBUTES)
				return false;
			if (attr & FILE_ATTRIBUTE_DIRECTORY)
				return false;
			return true;
			}
		};
#ifdef USE_DTSEARCH_NAMESPACE
}  // dtSearch namespace
#endif

#endif
