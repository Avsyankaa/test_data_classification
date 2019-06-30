#pragma once
#ifndef DStringProperty_H
#   define DStringProperty_H

#ifndef DSTRING_H
#   include <dstring.h>
#endif

#ifdef USE_DTSEARCH_NAMESPACE

namespace dtSearch {

#endif


#ifdef __AFX_H__
    #ifndef MfcAvailable
        #define MfcAvailable 
    #endif
#endif


/* Summary
   Manages conversion among character encodings (Ansi, Unicode, UTF-8).
   Remarks
   Set and get functions are provided for each encoding type. DStringProperty stores
   text internally in a DString in UTF-8, the same format that the dtSearch Engine
   uses.

   The dtSearch Engine's C++ support classes use DStringProperty to provide easy
   access to strings passed to or from the dtSearch Engine. For example, DSearchJob
   uses a DStringProperty, "Request" for the search request string. Any changes to
   Request are automatically reflected in dtsSearchJob.request. The following code
   sets up a search job using a Unicode string to initialize the "request":
   <code>
       DSearchJob searchJob;
       searchJob.Request.setW(L"Unicode search request");
   </code>

   "W", "A", "U8", and "Ui" versions of "set" and "get" functions are provided to
   get or set the string value as Unicode (W), Ansi (A), UTF-8 (U8), or TCHAR or
   CString (Ui).

   Implementation Details:

   A DStringProperty maintains a text buffer, value, with a UTF-8 representation of
   the text.

   DStringProperty is designed to be associated with a structure element, so that
   set*() or get*() calls will provide easy access to the contents of a string in a
   structure. A string member of a structure can be either a char * (a pointer to a
   memory location) or a char[N], a fixed-length buffer of N bytes. DStringProperty
   has constructors that will work with either variation.

   Each time the string is changed, potentially causing reallocation of the buffer,
   the const char * string pointer is updated to point to the new buffer.

   In this example, DStringProperty is declared to manage the dtsSearchJob.request
   string:
   <code>
       dtsSearchJob searchJob;
       DStringProperty xrequest(&searchJob.request);
       // Now any change to xrequest automatically causes searchJob.request to point to the
       // updated string
       \xrequest.setW(L"Unicode search request");
   </code>
   Each time a DStringProperty changes, it calls onValueChange, which sets *pValue
   to the new string buffer in value. This keeps the string pointer in the API
   structure consistent with the contents of the string buffer.                             */
class DStringProperty {
    public:
    	// Constructor initializing the DStringProperty with the address of a const char * pointer.
    	// The const char * pointer will always point to the string buffer in value.
        DStringProperty(const char **apValue = 0) :
			pValue(apValue),
			originalBuf(0),
			originalBufSize(0)
			{ };
		// Constructor initializing with a string value
        DStringProperty(const char *toCopy) :
            pValue(0), originalBuf(0), originalBufSize(0) {
            set(toCopy);
            onValueChange();
            }
		// Constructor initializing with a Unicode string value
        DStringProperty(const wchar_t *toCopy) :
            pValue(0), originalBuf(0), originalBufSize(0) {
            set(toCopy);
            onValueChange();
            }
		// Constructor attaching an existing string buffer
        DStringProperty(char *buf, ptrdiff_t maxLen) :
            pValue(0),
            value(buf, maxLen),
            originalBuf(buf), originalBufSize(maxLen)
            { }
		// Constructor attaching an existing string buffer AND a const char * pointer,
		// for values that are stored both ways, such as dtsOptions.userThesaurusFile and 
		// dtsOptions.userThesaurusFile2
        DStringProperty(char *buf, long maxLen, const char **apValue) :
            pValue(apValue),
            originalBuf(buf), originalBufSize(maxLen)
            { }
        // Copy constructor
        DStringProperty(const DStringProperty& other) : originalBuf(0), originalBufSize(0), pValue(0) {
            *this = other;
            }

        void setPVal(const char **apValue) {
			pValue = apValue;
			}
        ~DStringProperty() {
            pValue = 0;
            originalBuf = 0;
            }
        // Clear the string
        void clear() {
            value.clear();
			onValueChange();
            }
        void setNull() {
			*pValue = 0;
			}
		// Returns true if the string is empty or contains only whitespace
        int isBlank() {
            return value.isBlank();
            }

        int operator==(const DStringProperty& other) const {
            return !strcmp(value, other.value);
            }
        // Copy a DStringProperty
        DStringProperty& operator=(const DStringProperty& other) {
            value = other.value;
            onValueChange();
            return *this;
            }
        // Copy a string
        DStringProperty& operator=(const char *other) {
            set(other);
            onValueChange();
            return *this;
            }
        // Get const char * pointer to the string buffer
        operator const char *() const {
            return value;
            }

        // Get char * pointer to the string buffer
        char *str() const {
            return value.str();
            }
        // Returns the length of the string
        ptrdiff_t getLength() const {
            return value.getLength();
        }
        // Store a Unicode string
        DStringProperty& operator=(const wchar_t *other) {
            set(other);
            onValueChange();
            return *this;
            }
            
		void copy(const DStringProperty& other) {
			setU8(other.str());
			onValueChange();
			}            
		// Store a Unicode string
        void setW(const wchar_t *ws) {
            if (ws) {
                value.storeUnicodeAsUtf8(ws);
                onValueChange();
                }
            else
                clear();
            }
        void setW(const wchar_t *ws, ptrdiff_t len) {
            if (ws) {
                value.storeUnicodeAsUtf8(ws, len);
                onValueChange();
                }
            else
                clear();
            }
        void set(const wchar_t *ws) {
            setW(ws);
            }
       	// Store a UTF-8 string
        void setU8(const char *utf8String) {
            if (utf8String) {
                value = utf8String;
                onValueChange();
                }
            else
                clear();
            }
        // Store an Ansi string
        void setA(const char *ansiString) {
            if (ansiString) {
                DWString temp;
                temp.storeAnsiAsUnicode(ansiString);
                value.storeUnicodeAsUtf8(temp);
                onValueChange();
                }
            else
                clear();
            }
        void set(const char *s) {
            setU8(s);
            }
        void get(DWString& dest) const {
            dest.storeUtf8AsUnicode(value);
            }
		// Get the string as Unicode
        void getW(DWString& dest) const {
            dest.storeUtf8AsUnicode(value);
            }
		// Get the string as UTF-8
        void getU8(DString& utf8Dest) const {
            utf8Dest = value;
            }
		// Get the string as Ansi
        void getA(DString& dest) const {
            DWString uTemp(value);
            dest.storeUnicodeAsAnsi(uTemp);
            }
#   ifdef _UNICODE
        void setUi(const wchar_t *s) {
            setW(s);
            }
#   else   /* Store a const TCHAR * string */
        void setUi(const char *s) {
            setA(s);
            }
#   endif  // Unicode
#if defined(MfcAvailable)
        CString getUi() const {
            CString ret;
            getUi(ret);
            return ret;
            }
#   ifdef _UNICODE
        void getUi(CString &cs) const {
            DWString uTemp(value);
            cs = uTemp;
            }
#   else   // Unicode
		// Get the string as TCHAR
        void getUi(CString &cs) const {
            DString tmp;
            getA(tmp);
            cs = tmp;
            }
#   endif  // Unicode
#endif // MfcAvailable

#if defined(_OLEAUTO_H_)
		// Convert the string to a COM BSTR and return it
        BSTR allocSysString() {
            return value.allocSysString();
            }
#endif
#if defined(DTENGINE_IMPORTS) && !defined(NO_DTSEARCH_LIB)
		// Convert the string from a dtsStringHandle, optionally deleting the handle after the
		// string has been converted
        void getFromStringHandle(dtsStringHandle hStr, int fReleaseHandle) {
            value.getFromStringHandle(hStr, fReleaseHandle);
            onValueChange();
            }
#endif

#if defined(VS2005) /* Store a .NET System::String */
		void set(System::String ^str, bool bFixNulls = false) {
			if (str) {
				CConvertedDotNetString cstr(str);
				setW(cstr, cstr.getLength());
				if (bFixNulls)
					value.replace(0, ' ');
				}
			else
				set("");
			}
		// Return the string as a .NET System::String
		System::String ^allocNetString() const {
			return value.allocNetString();
			}
#elif defined(VS2003)
		void set(System::String *str) {
			if (str) {
				CConvertedDotNetString cstr(str);
				set(cstr);
				}
			else
				set("");
			}
		// Return the string as a .NET System::String
		System::String *allocNetString() const {
			return value.allocNetString();
			}
#endif

    protected:
        DString value;
        // Used if the value is associated with a const char * pointer that should be
        // updated each time string contents change
        const char **pValue;
        // Used if the value is associated with a fixed-length buffer
        char *originalBuf;
        ptrdiff_t originalBufSize;

        // If this property is linked to an API structure
        // string pointer or buffer, update the pointer or buffer every time its value
        // changes.
        void onValueChange() {
            if (pValue)
                *pValue = value.str();
            if (originalBuf) {
                if (originalBuf != value.str())
                    strCopy(originalBuf, value.str(), originalBufSize);
                }
            }
    };

#define DssTokenSeparators "\n\t\r ,;"

/* \ \
   Summary
   Manages conversion among character encodings (Ansi, Unicode, UTF-8) for an
   \ordered list of strings.
   Remarks
   Append and get functions are provided for each encoding type.

   The dtSearch Engine's <link C++ Support Classes, C++ support classes> use
   DStringSetProperty to provide easy access to string sets passed to or from the
   dtSearch Engine. For example, DSearchJob uses a DStringSetProperty for the list
   \of indexes to search.

   "W", "A", "U8", and "Ui" versions of "append" and "getString" functions are
   provided to append or or retrieve the string values as Unicode (W), Ansi (A),
   UTF-8 (U8), or TCHAR or CString (Ui).

   DStringSetProperty stores text internally in a DStringSet in UTF-8, the same
   format that the dtSearch Engine uses.

   DStringSetProperty, like DStringProperty, is designed to be associated with a
   structure element, so append*() or getString^() calls will provide easy access to
   the contents of a string set attached to a structure.                             */

class DStringSetProperty {
    public:
    	// Constructor attaching a const char * pointer that will be made
    	// to point to the start of the string set
        DStringSetProperty(const char **apValue = 0): pValue(apValue){ };
        ~DStringSetProperty() {
            value.clear();
            pValue = 0;
            }
        // Copy constructor
        DStringSetProperty& operator=(const DStringSetProperty& other) {
            value = other.value;
            onValueChange();
            return *this;
            }
        // Store a DStringSet
        DStringSetProperty& operator=(const DStringSet& other) {
            copy(other);
            return *this;
            }
        void copy(const DStringSet& other) {
			value.copy(other);
            onValueChange();
            }
        DStringSet& getImp() {
			return value;
			}
        int operator==(const DStringSetProperty& other) const {
            return value == other.value;
            }
        int operator!=(const DStringSetProperty& other) const {
            return !(value == other.value);
            }
        // Get a const char * pointer to the start of the buffer
        operator const char *() {
            return value.getBuf();
            }
        // Clear the string set
        void clear() {
            value.clear();
            onValueChange();
            }
        // Returns the number of strings in the set
        int getCount() const {
            return value.getCount();
            }
        //! For use in properties that are sets of name,value pairs
        int FindFieldName(const char *fn) const {
            for (int i = 0; i < value.getCount(); i += 2) {
                if (!strcmp(fn, value.getString(i)))
                    return i;
                }
            return -1;
            }
        //! For use in properties that are sets of name,value pairs (case-insensitive)
        int FindFieldNameI(const char *fn) const {
            for (int i = 0; i < value.getCount(); i += 2) {
                if (!_stricmp(fn, value.getString(i)))
                    return i;
                }
            return -1;
            }
        //! Find a string in the set
        int Find(const char *s) const {
            for (int i = 0; i < value.getCount(); i ++) {
                if (!strcmp(s, value.getString(i)))
                    return i;
                }
            return -1;
            }

        const char *getString(int i ) {
            return value.getString(i);
            }
        // Format the set as a delimited string (Utf-8)
        void formatAsString(DString& dest) const {
                value.formatAsString(dest, " ", '\"');
            }
#if defined(MfcAvailable)
        // Format the set as a delimited string (CString)
        CString formatAsStringUi() const {
                DString tmp;
                value.formatAsString(tmp);
                CString ret = Utf8ToUi(tmp);
                return ret;
                }
#endif
        // Format the set as a delimited string (Utf-8, minimizing use of quotation marks)
        void formatAsSimpleString(DString& dest) const {
                value.formatAsSimpleString(dest, ' ', '\"');
            }
        // Format the set as a delimited string (Utf-8, minimizing use of quotation marks)
        void formatAsSimpleString(DString& dest, char sep, char quote) const {
                value.formatAsSimpleString(dest, sep, quote);
            }
        // Remove a string from the set
        void remove(int i ) {
            value.deleteString(i);
            }
        // Append a Unicode string to the set
        int appendW(const wchar_t *ws, int fCheckUnique = false) {
            DString t;
            t.storeUnicodeAsUtf8(ws);
            return appendU8(t, fCheckUnique);
            }
        int append(const wchar_t *ws, int fCheckUnique = false) {
            return appendW(ws, fCheckUnique);
            }
		// Append a UTF-8 string to the set
        int appendU8(const char *utf8String, int fCheckUnique = false) {
            if ((fCheckUnique == false) || (value.find(utf8String) == FAIL)) {
                value.append(utf8String);
                onValueChange();
                return true;
                }
            else
                return false;
            }
        // Append an Ansi string to the set
        int appendA(const char *ansiString, int fCheckUnique = false) {
            DWString temp;
            temp.storeAnsiAsUnicode(ansiString);
            DString temp2;
            temp2.storeUnicodeAsUtf8(temp);
            return appendU8(temp2, fCheckUnique);
            }
        int append(const char *s, int fCheckUnique = false) {
            return appendU8(s, fCheckUnique);
            }
        void append(long v) {
            value.append(v);
            onValueChange();
            }
		// Tokenize a Unicode string to make a string set, using whitespace, comma, and semicolon as delimiters
        void tokenizeW(const wchar_t *ws) {
			tokenizeW(ws, DssTokenSeparators);
            }
		// Tokenize a Unicode string to make a string set
        void tokenizeW(const wchar_t *ws, const char *delim) {
			DString t;
			if (!strIsBlank(ws))
				t.storeUnicodeAsUtf8(ws);
			tokenizeU8(t, delim);
            }
        
        void tokenize(const wchar_t *ws) {
            tokenizeW(ws);
            }
		// Tokenize a UTF-8 string to make a string set, using whitespace, comma, and semicolon as delimiters
        void tokenizeU8(const char *utf8String, const char *delim = 0) {
			if (!delim)
				delim = DssTokenSeparators;
			if (strIsBlank(utf8String))
				value.clear();
			else 
				value.tokenizeq(utf8String, delim);
            onValueChange();
            }
		// Tokenize an Ansi string to make a string set, using whitespace, comma, and semicolon as delimiters
        void tokenizeA(const char *ansiString) {
            DWString temp;
            temp.storeAnsiAsUnicode(ansiString);
			tokenizeW(temp);
            }
        void tokenize(const char *s) {
            tokenizeU8(s);
            onValueChange();
            }
#if defined(VS2005)
		// Tokenize a .NET System::String to make a string set, using whitespace, comma, and semicolon as delimiters
		void tokenize(System::String ^str, const char *sep = 0) {
			if (!sep)
				sep = DssTokenSeparators;
			if (!str) {
				clear();
				return;
				}
			CConvertedDotNetString cstr(str);
			DString t;
			t.storeUnicodeAsUtf8(cstr, cstr.getLength());
			value.tokenizeq(t, sep);
			onValueChange();
			}
		// Append a .NET System::String
		void append(System::String ^str) {
			if (str) {
				CConvertedDotNetString cstr(str);
				appendW(cstr);
				}
			}
		// Make a string set from a .NET string array
		void copy(array<System::String ^> ^stringArray) {
			clear();
			if (!stringArray)
				return;
			for (int i = 0; i < stringArray->Length; ++i) {
				System::String ^s = stringArray[i];
				append(s);
				}
			}
		// Make a string set from a .NET System::Collections::Specialized::StringCollection
		void copy(System::Collections::Specialized::StringCollection ^column) {
			clear();
			if (!column)
				return;
			for (int i = 0; i < column->Count; ++i) {
				System::String ^s = column[i];
				append(s);
				}
			}
#elif defined(VS2003)
		// Tokenize a .NET System::String to make a string set, using whitespace, comma, and semicolon as delimiters
		void tokenize(System::String *str, const char *sep = 0) {
			if (!sep)
				sep = DssTokenSeparators;
			if (!str) {
				clear();
				return;
				}
			CConvertedDotNetString cstr(str);
			DString t;
			t.storeUnicodeAsUtf8(cstr);
			value.tokenizeq(t, sep);
			onValueChange();
			}
		// Append a .NET System::String
		void append(System::String *str) {
			if (str) {
				CConvertedDotNetString cstr(str);
				appendW(cstr);
				}
			}
		// Make a string set from a .NET System::Collections::Specialized::StringCollection
		void copy(System::Collections::Specialized::StringCollection *column) {
			clear();
			if (!column)
				return;
			for (int i = 0; i < column->Count; ++i) {
				System::String *s = column->get_Item(i);
				append(s);
				}
			}
#endif

        void getString(int i, DWString& dest) const {
            dest.storeUtf8AsUnicode(value.getString(i));
            }
        // Get one string from the set, as Unicode
        void getStringW(int i, DWString& dest) const {
            dest.storeUtf8AsUnicode(value.getString(i));
            }
        // Get one string from the set, as UTF-8
        void getStringU8(int i, DString& dest) const {
            dest = value.getString(i);
            }
        // Get one string from the set, as Ansi
        void getStringA(int i, DString& dest) const {
            DWString uTemp(value.getString(i));
            dest.storeUnicodeAsAnsi(uTemp);
            }
        void getString(int i, DStringProperty& dest) const{
            dest.setU8(value.getString(i));
            }
        void getString(int i, DString& dest) const {
            dest = value.getString(i);
            }
		// Get all values in the string set
        void getStringSetU8(DStringSet& s) const {
            s = value;
            }
        // Append all values from a string set
        void appendU8(const DStringSet& other) {
            value.append(other);
            onValueChange();
            }
        // Append all values from a string set
        void append(const DStringSetProperty& other) {
            value.append(other.value);
            onValueChange();
		}
		// Append values from a string set that do not already exist in this string set
        void appendUnique(const DStringSetProperty& other) {
			appendUnique(other.value);
            }
		// Append values from a string set that do not already exist in this string set
        void appendUnique(const DStringSet& other) {
            for (int i = 0; i < other.getCount(); ++i) {
                DString tmp;
                other.getString(i, tmp);
                appendU8(tmp, true);
                }
            onValueChange();
            }
		void appendField(const char *fieldName, const char *fieldValue) {
			value.appendField(fieldName, fieldValue);
			onValueChange();
			}
#if defined(MfcAvailable)
        CString getStringUi(int i) {
            CString ret;
            getStringUi(i, ret);
            return ret;
            }
#   ifdef _UNICODE
		// Append a TCHAR or CString string to the set
        int appendUi(const wchar_t *s, int fCheckUnique = false) {
            return append(s, fCheckUnique);
            }
		// Get the value of a string as a CString
        void getStringUi(long i, CString &cs) const {
            DWString uTemp;
            getString(i, uTemp);
            cs = uTemp;
            }
        void tokenizeUi(const wchar_t *s) {
            tokenize(s);
            onValueChange();
            }
#   else   // Unicode
        int appendUi(const char *s, int fCheckUnique = false) {
            return appendA(s, fCheckUnique);
            }
        void getStringUi(long i, CString &cs) {
            DString tmp;
            getStringA(i, tmp);
            cs = tmp;
            }
        void tokenizeUi(const char *s) {
            tokenizeA(s);
            onValueChange();
            }
#   endif  // Unicode
#endif // MfcAvailable
		void copyFromBuf(const char *s) {
			value.copyFromBuf(s);
			onValueChange();
			}
    protected:
        DStringSet value;
        const char **pValue;

        // If this property is linked to a dtSearch API structure
        // string pointer, update the pointer every time its value
        // changes.
        void onValueChange() {
            if (pValue)
                *pValue = value.getBuf();
            }
    };
#ifdef USE_DTSEARCH_NAMESPACE
}  // dtSearch namespace
#endif

#endif
