
#ifndef XMLMAKER_H
#	define XMLMAKER_H

#ifndef DSTRING_H
#	include <dstring.h>
#endif
#ifndef DTSEARCH_H
#	include <dtsearch.h>
#endif
#ifndef StringBuilder_H
	#include <StringBuilder.h>
#endif

#if !defined(_MSC_VER) && !defined(_atoi64)
	#define _atoi64 atoll
#endif

#ifdef USE_DTSEARCH_NAMESPACE

namespace dtSearch {

#endif

// Minimal text obfuscation to make things like passwords in
// xml fields less obvious
class CTextHider {
	public:
		CTextHider() { }
		virtual ~CTextHider() { }
		virtual void scramble(DString& data);
		virtual void unscramble(DString& data);
	};

/*
	Lightweight classes for reading and writing XML-formatted configuration files.
	CXmlReader is *only* for reading files created by CXmlMaker -- it does not understand
	attributes or anything else but very basic XML data.

	These classes are used to create and read dtSearch configuration files such as the
	file segmentation rules file, text fields file, etc.
 */

inline int intval(const char *s)
{
	if ((s[0] == '0') && (s[1] == 'x'))
		return (int) strtol(s+2, 0, 16);
	else
		return atoi(s);
}


class CXmlMaker  {
	public:
		CXmlMaker();
		void beginField(const char *fieldName);
		void endField(const char *name);
		void storeValue(const char *value);
        void storeValue(const wchar_t *wvalue);
		void storeField(const char *name, int value);
		void storeField(const char *name, unsigned long value) {
			storeField(name, (int) value);
			}
		void storeField(const char *name, long value) {
			storeField(name, (int) value);
			}
        void storeField(const char *name, __int64 value);
#ifdef WIN32
        void storeField(const char *name, bool value) {
            storeField(name, (int) value);
            }
#endif
		void storeField(const char *name, const DStringSet& value);
		void storeField(const char *name, const DWStringSet& value);
		void storeField2(const char *name, const DStringSet& value, char delimiter = ';', char quote = '`');
		void storeFieldHidden(const char *name, const DStringSet& value);

		void storeField(const char *fieldName, const char *value);
		void storeField(const char *fieldName, const wchar_t *value) {
			DString tmp;
			if (value)
				tmp.storeUnicodeAsUtf8(value);
			storeField(fieldName, tmp);
			}
		void storeHexField(const char *fieldName, const void *value, long bytes) {
			DString tmp;
            if (value)
			    tmp.appendHexEncoded(value, bytes);
			storeField(fieldName, tmp);
			}
		void storeField(const char *fieldName, const dtsFileDate& value);
		void storeField(const char *fieldName, const DArrayAsVector<long>& value);
#ifdef GrowableIntArray_H		
		void storeField(const char *fieldName, const CGrowableIntArray<long>& value);
#endif		
		void storeField(const char *fieldName, const long *valueArray, long nItems);
		void storeRelativeField(const char *fieldName, const char *value);
		void storeNonblank(const char *fieldName, const char *value) {
			if (!strIsBlank(value))
				storeField(fieldName, value);
			}
		void storeNonzero(const char *fieldName, long value) {
			if (value)
				storeField(fieldName, value);
			}
		void storeFields(DStringSet& fieldSet);

		void makeComment(const char *s);
		void makeCommentTitle(const char *s, const char *a = 0, const char *b = 0);
		void clear();
		void clearNoHeader();
		bool getText(DString& s) {
			return m_text.copyToString(s);
			}
		bool getText(DStreamBase& sb) {
			return m_text.copyToStream(sb);
		}
		int writeU8(const char *fn);
		int write() {
			return writeU8(m_outputFile);
			}
		int write(const TCHAR *fn) {
			return writeU8(UiToUtf8(fn));
			}
		ptrdiff_t getLength() {
			return m_text.getLength();
			}
		void write(DFile& f) {
			m_text.copyToFile(f);
			}
		void setOutputU8(const char *fn) {
			m_outputFile = fn;
			}
		void setOutput(const TCHAR *fn) {
			m_outputFile = UiToUtf8(fn);
			}

		void insertXml(const char *xml) {
			m_text << xml;
			}
        void setAllowUtf8(int v) {
            m_fAllowUtf8 = v;
            }
        void setTextHider(CTextHider *p) {
			m_pHider = p;
			}
		static const char *fixFieldName(const char *name, DString& fixed);
		// Illegal XML Unicode characters: http://en.wikipedia.org/wiki/XML#Details_on_valid_characters
		// http://www.w3.org/TR/2006/REC-xml11-20060816/#NT-RestrictedChar
		static inline bool isLegalXmlChar(wchar_t ch) {
            if (ch > 0xfffd)
                return false;
			if (ch > 0x9f) 
				return (ch < 0xd800) || (ch >= 0xe000);
			if (ch >= 0x7f) 
				// 7f-84 and 86-9f are defined as "restricted"
				return (ch == 0x85);
			if (ch >= 32)
				return true;
			return (ch == 9) || (ch == 10) || (ch == 13);		
			}
		static inline bool isLegalXmlChar32(int ch) {
			if (ch < 0x10000)
				return isLegalXmlChar((wchar_t) ch);
			return (ch < 0x10FFFF);
			}
		static inline bool isLegalLowChars(const char *s, ptrdiff_t len = -1) {
			if (len < 0)
				len = strlen(s);
			const unsigned char *us = (const unsigned char *) s;		
			for (int i = 0; i < len; ++i) {
				unsigned char ch = us[i];
				if ((ch < 32) && !isLegalXmlChar(ch))
					return false;
				if (ch > 127)
					return false;
				}
			return true;
			}
				
		static bool deleteIllegalXml(DWString& ws);
			
	protected:
		CTextHider *m_pHider;
		DString m_rootField;
		DStringBuilder m_text;
		DString svalue; //temp
        DWString wvalue; //temp
        DString fieldNameTemp;
        const char *fixFieldName(const char *originalName);
		int m_indent;
		DFilename m_outputFile;
        int m_fAllowUtf8;

		void putIndent();
		void breakLine();
		void storeLegalUtf8(const char *value);
	};


class CXmlReader {
	public:
	typedef int FieldId;
	class FieldSet {
		friend class CXmlReader;
		public:
			FieldSet() { }
			FieldSet(const FieldSet& other) {
				m_items = other.m_items;
				}
			FieldSet& operator=(const FieldSet& other) {
				m_items = other.m_items;
				return *this;
				}
			int isEmpty() const {
				return getCount() == 0;
				}
			int operator!() const {
				return isEmpty();
				}
			void clear() {
				m_items.clear();
				}
			int getCount() const {
				return m_items.getCount();
				}
			int get(int i) const {
				return (int) m_items.get(i);
				}
			void append(FieldId i) {
				m_items.append(i);
				}
		protected:
			DArrayAsVector<FieldId> m_items;
		};

	class Selection {
		friend class CXmlReader;
		public:
			Selection() :
				m_first(0), m_last(-1) { }
			Selection(const Selection& other) {
				*this = other;
				}
			Selection& operator=(const Selection& other) {
				m_first = other.m_first;
				m_last = other.m_last;
				return *this;
				};
			int isEmpty() const {
				return (m_first > m_last);
				}
			int operator!() const {
				return isEmpty();
				}
			void clear() {
				m_first = 0;
				m_last = -1;
				}
		protected:
			int m_first;
			int m_last;
		};

		CXmlReader();
 		CXmlReader(const char *s);
		bool setXml(const char *s, const char *segmentName = 0);
		void setFilenameU8(const char *s) {
			m_filename = s;
			}
		const char *getXml() {
			return m_xmlText;
			}
		int getFromU8(const char *filename, const char *segmentName = 0);
		int getFrom(const TCHAR *filename, const char *segmentName = 0) {
			return getFromU8(UiToUtf8(filename), segmentName);
			}
		FieldSet find(const char *fieldName, const Selection& subset) const;
		FieldSet find(const char *fieldName) const;
		FieldId findFirstIn(const char *fieldName, const Selection& subset) const;

		void getFieldsInSelection(Selection subset, DStringSet& dest);
		void getValuesInSelection(const CXmlReader::Selection subset, const char *fieldName, DStringSet& dest);
		const char *getWithDefault(const char *fieldName, const char *def) const {
			const char *v = getFirst(fieldName);
			if (strIsBlank(v))
				return def;
			else
				return v;
			}
		const char *getFirst(const char *fieldName) const {
			Selection sel = selectAll();
			FieldId f = findFirstIn(fieldName, sel);
			if (f < 0)
				return 0;
			else
				return m_fieldValueTable.getString((int)f);
			}
        int getFirstIn(const Selection& subset, const char *name, __int64& value) const {
            const char *v = getFirstIn(subset, name);
            if (v) {
                value = _atoi64(v);
                return SUCCESS;
                }
            else
                return FAIL;
            }
#ifdef WIN32
        int getFirstIn(const Selection& subset, const char *name, bool& value) const {
            const char *v = getFirstIn(subset, name);
            if (v) {
                value = (intval(v)? true : false);
                return SUCCESS;
                }
            else
                return FAIL;
            }

#endif
        int getFirstIn(const Selection& subset, const char *name, int& value) const {
            const char *v = getFirstIn(subset, name);
            if (v) {
                value = intval(v);
                return SUCCESS;
            }
            else
                return FAIL;
            }
        int getFirstIn(const Selection& subset, const char *fieldName, long& value) const {
			const char *v = getFirstIn(subset, fieldName);
			if (v) {
				value = atol(v);
				return SUCCESS;
				}
			else
				return FAIL;
			}
		int getFirstIn(const Selection& subset, const char *fieldName, unsigned long& value) const {
			const char *v = getFirstIn(subset, fieldName);
			if (v) {
				value = atol(v);
				return SUCCESS;
				}
			else
				return FAIL;
			}
		int getFirstIn(const Selection& subset, const char *fieldName, DStringSet& value) const {
			value.clear();
			const char *v = getFirstIn(subset, fieldName);
			if (v) {
				value.tokenizeq(v, " \t\r\n");
				return SUCCESS;
				}
			else
				return FAIL;
			}

		int getFirstIn(const Selection& subset, const char *fieldName, DWStringSet& value) const {
			value.clear();
			const char *v = getFirstIn(subset, fieldName);
			if (v) {
				DWString wVal;
				wVal.storeUtf8AsUnicode(v);
				value.tokenizeq(wVal, L" \t\r\n");
				return SUCCESS;
			}
			else
				return FAIL;
		}

		int getFirstInHidden(const Selection& subset, const char *fieldName, DStringSet& value) const {
			const char *v = getFirstIn(subset, fieldName);
			if (v) {
				DString tmp = v;
				if (m_pHider)
					m_pHider->unscramble(tmp);
				value.clear();
				value.tokenizeq(tmp, " \t\r\n");
				return SUCCESS;
				}
			else
				return FAIL;
			}
		void getFirstIn2(const Selection& subset, const char *fieldName, DStringSet& value, const char *delimiter = ";", char quote = '`');
		int getFirstIn(const Selection& subset, const char *fieldName, dtsFileDate& value) const {
			const char *v = getFirstIn(subset, fieldName);
			if (v) {
				DStringSet temp;
				temp.tokenize(v, " /:-Z");
				if (temp.getCount() == 6) {
					value.year = (unsigned short) atol(temp.getString(0));
					value.month = (char) atol(temp.getString(1));
					value.day = (char) atol(temp.getString(2));
					value.hour = (char) atol(temp.getString(3));
					value.minute = (char) atol(temp.getString(4));
					value.second = (char) atol(temp.getString(5));
					return SUCCESS;
					}
				}
			return FAIL;
			}

		int getFirstIn(const Selection& subset, const char *fieldName, DString& value) const {
			const char *v = getFirstIn(subset, fieldName);
			if (v) {
				if (strIsBlank(v))
					value.clear();
				else
					value = v;
				return SUCCESS;
				}
			else
				return FAIL;
			}
		int getFirstInRelative(const Selection& subset, const char *fieldName, DString& value) const;
		void resolveRelativePath(DString& fn);

		int getFirstIn(const Selection& subset, const char *fieldName, DWString& value) const {
			const char *v = getFirstIn(subset, fieldName);
			if (v) {
				value.storeUtf8AsUnicode(v);
				return SUCCESS;
				}
			else
				return FAIL;
			}

		int getFirstIn(const Selection& subset, const char *fieldName, DArrayAsVector<long>& value) const {
			const char *v = getFirstIn(subset, fieldName);
			if (v) {
				value.clear();
				DStringSet temp;
				temp.tokenize(v, " ,");
				for (int i = 0; i < temp.getCount(); ++i)
					value.append(atol(temp.getString(i)));
				return SUCCESS;
				}
			return FAIL;
			}
#ifdef GrowableIntArray_H			
		int getFirstIn(const Selection& subset, const char *fieldName, CGrowableIntArray<long>& value) const {
			const char *v = getFirstIn(subset, fieldName);
			if (v) {
				value.clear();
				DStringSet temp;
				temp.tokenize(v, " ,");
				for (int i = 0; i < temp.getCount(); ++i)
					value.append(atol(temp.getString(i)));
				return SUCCESS;
				}
			return FAIL;
			}
#endif			
		const char *getFirstIn(const Selection& subset, const char *fieldName) const {
			FieldId f = findFirstIn(fieldName, subset);
			if (f < 0)
				return 0;
			else
				return m_fieldValueTable.getString(f);
			}
		const char *getFirstIn(const char *fieldName, const char *parentField) const {
			Selection sel = select(parentField);
			return getFirstIn(sel, fieldName);
			}
		int getFirstInHex(const Selection& subset, const char *fieldName, DString& dest) const {
			const char *v = getFirstIn(subset, fieldName);
			if (v) {
				dest.hexDecode(v);
				return SUCCESS;
				}
			else
				return FAIL;
			}
		Selection select(const FieldSet& set, int iItem) const;
		Selection select(const char *fieldName, const Selection& subset) const;
		Selection select(const char *fieldName) const;
		Selection selectAll() const;
		int getCount() const {
			return m_fieldNameTable.getCount();
			}
		void dump(DString& out);
		void clear();
		void setTextHider(CTextHider *p) {
			m_pHider = p;
			}
	protected:
		DString m_xmlText;
		DString svalue; // temp
		DArrayAsVector<int> m_nestLevel;
		DArrayAsVector<int> m_entryCount;
		DStringSet m_fieldNameTable;
		DStringSet m_fieldValueTable;
		DString m_filename;
		CTextHider *m_pHider;

		void buildFromText();
		void storeField(const char *fieldName, const char * fieldValue, int nestLevel);
	};

#ifdef USE_DTSEARCH_NAMESPACE
}  // dtSearch namespace
#endif
#endif
