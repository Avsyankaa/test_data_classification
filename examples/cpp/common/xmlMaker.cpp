#include <stdafx.h>
#ifndef XMLMAKER_H
    #include <xmlmaker.h>
#endif
#include <dtconfig.h>

#ifdef USE_DTSEARCH_NAMESPACE
using namespace dtSearch;
#endif


#pragma warning(disable: 4706) // assignment within conditional

static CTextHider unsecureHider;

/*
    Usage:
        CXmlReader rdr;
        CXmlReader::FieldSet items;
        rdr.setXml(text);
        rdr.find("TextFieldItem", items);
        for (int i = 0; i < items.getCount(); ++i) {
            rdr.select(items, i);
            const char *name = rdr.getField("Name");
            const char *beginMark = rdr.getField("BeginMark"); 
            ...
            }
        fields = rdr.find("SynonymSet");
        for (int i = 0; i < fields.getCount(); ++i) {
            rdr.selectField(fields.get(i));
            const char *relation = rdr.getValue("Relation");
            }
*/

CXmlReader::CXmlReader()
{	m_pHider = &unsecureHider;
}

CXmlReader::CXmlReader(const char *s)
{	m_pHider = &unsecureHider;
	setXml(s);
}

#define EndComment      "-->"
#define BeginComment    "<!--"
#define BeginTextDecl   "<?"
#define BeginCData      "<![CDATA["
#define BeginCommentNoBracket    "!--"
#define BeginTextDeclNoBracket   "?"
#define BeginCDataNoBracket      "![CDATA["
#define EndTextDecl     "?>"
#define EndCData        "]]>"
#define crlf            "\r\n"
const char quote = '\"';

#define XmlHeader       "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" crlf

void CXmlReader::dump(DString& out)
{   out << "Count: " << m_fieldNameTable.getCount() << crlf;
    for (int i = 0; i < m_fieldNameTable.getCount(); ++i) {
        out << " Field: " << quote << m_fieldNameTable.getString(i) << quote
            << " Value: " << quote << m_fieldValueTable.getString(i) << quote
            << " Nest: " << m_nestLevel.get(i)
            << " Entries: " << m_entryCount.get(i)
            << crlf;
        }
    out << "Original data:" << crlf << m_xmlText << crlf << crlf;
}



void CXmlReader::clear()
{	m_xmlText.clear();
    m_nestLevel.clear();
    m_entryCount.clear();
    m_fieldNameTable.clear();
    m_fieldValueTable.clear();
}

bool CXmlReader::setXml(const char *s, const char *segmentName)
{
	clear();
	if (!s)
		s = "";
	m_xmlText.clear();
    if (!strIsBlank(segmentName) && (strlen(segmentName) < 120)) {
		DString80 beginTag, endTag;
		beginTag << "<" << segmentName << ">";
		endTag << "</" << segmentName << ">";
        const char *begin = stristr(s, beginTag);
        if (begin) {
            const char *end = stristr(begin, endTag);
            if (end) {
				size_t l = end - begin + strlen(endTag);
                if (!m_xmlText.tryStore(begin, l))
					return false;
				}
			else {
				if (!m_xmlText.tryStore(begin, strlen(begin)))
					return false;
				}
            }
        }
	else
        if (!m_xmlText.tryStore(s, strlen(s)))
			return false;

    buildFromText();
    return true;
}

int CXmlReader::getFromU8(const char *filename, const char *segmentName)
{
	clear();

    if (strIsBlank(filename))
        return FAIL;

    DString text;
    if (text.getFromU8(filename) != FAIL) {
		if (!text.isBlank()) {
			if (setXml(text, segmentName)) {
				m_filename = filename;
				return SUCCESS;
				}
			}
		}
	return FAIL;
}

void CXmlReader::storeField(const char *fieldName, const char * fieldValue, int nestLevel)
{   if (strIsBlank(fieldName))
        return;
    if (strIsBlank(fieldValue))
        fieldValue = " ";
    m_fieldNameTable.append(fieldName);
    svalue = fieldValue;
    svalue.trim();
    m_fieldValueTable.append(svalue);
    m_nestLevel.append(nestLevel);
    m_entryCount.append(1);
}

void CXmlReader::getFirstIn2(const Selection& subset, const char *fieldName, DStringSet& value, const char *delimiter, char quoteChar) {
	const char *v = getFirstIn(subset, fieldName);
	if (v) {
		value.clear();
		value.tokenizeq(v, delimiter, quoteChar);
		}
	}


static const char *translateEntity(const char *s, wchar_t& ch)
{   ch = '&';
    const char *ret = s + 1;

    char entity[10];
    memset(entity, 0, sizeof entity);
    strCopy(entity, s+1, sizeof entity);
    entity[9] = '\0';
    char *semicolon = strchr(entity, ';');
    if (!semicolon)
        return ret;
    *semicolon = '\0';
    if ((entity[0] == '#') && Ascii_IsDigit(entity[1])) {
        ch = (wchar_t) atoi(&entity[1]);
        return strchr(ret, ';')+1;
        }
    static const char *entities[] = {
        "&amp", "\"quot", ">gt", "<lt"
        };
    static const int entityCount = 4;
    for (int i = 0; i < entityCount; ++i) {
        if (!strcmp(entity, entities[i] + 1)) {
            ch = *entities[i];
            ret += strlen(entities[i]);
            return ret;
            }
        }
    return ret;
}

static int startsWith(const char *s, const char *text)
{   while(*s && *text) {
        if (*s != *text)
            return false;
        s++;
        text++;
        }
    return (*s == 0);
}

void CXmlReader::resolveRelativePath(DString& fn)
{
	if (m_filename.isBlank() || fn.isBlank() || !DFilename::isRelative(fn))
		return;
    DFilename base = UiToUtf8(m_filename).str();
    base.rmTail();
    DFilename ret = fn.str();
    ret.makeAbsoluteFrom(base);
    fn = ret;
}

int CXmlReader::getFirstInRelative(const Selection& subset, const char *fieldName, DString& value) const
{   const char *v = getFirstIn(subset, fieldName);
    if (!strIsBlank(v) && !m_filename.isBlank()) {
        DFilename base = UiToUtf8(m_filename).str();
        base.rmTail();
        DFilename ret = v;
        ret.makeAbsoluteFrom(base);
        value = ret;
        return SUCCESS;
        }
    value.clear();
    return FAIL;
}

void CXmlReader::buildFromText()
{    m_nestLevel.clear();
    m_entryCount.clear();
    m_fieldNameTable.clear();
    m_fieldValueTable.clear();

    const char *p = m_xmlText;
    DString fieldName;
    DString fieldValue;
    int nestLevel = 0;
    const int maxNestLevel = 100;
    int firstEntryInLevel[maxNestLevel];
    memset(&firstEntryInLevel, 0, sizeof firstEntryInLevel);
    DString utf8;

    while(p && *p) {
        char c = *p;
        if (c == '<') {
            p++;
            // Comment
            if (startsWith(BeginCommentNoBracket, p)) {
                p = strstr(p, EndComment);
                if (p)
                    p += strlen(EndComment);
                }
            else if (startsWith(BeginTextDeclNoBracket, p)) {
                p = strstr(p, EndTextDecl);
                if (p)
                    p += strlen(EndTextDecl);
                }
            else if (startsWith(BeginCDataNoBracket, p)) {
                p += 8;
                const char *q = strstr(p, EndCData);
                if (!q)
                    q = p + strlen(p);
                fieldValue.store(p, q - p);
                p = q;
                if (startsWith(EndCData, p))
                    p += strlen(EndCData);
                }
            // Begin or end field
            else {
                if (!fieldName.isBlank()) {
                    storeField(fieldName, fieldValue, nestLevel);
                    firstEntryInLevel[nestLevel] = getCount()-1;
                    }

                fieldValue.clear();
                fieldName.clear();
                const char *q = strchr(p, '>');
                if (q) {
                    int fIsEnd = false;
                    if (*p == '/') {
                        p++;
                        fIsEnd = true;
                        if (nestLevel < 0)
                            return;
                        int iField = firstEntryInLevel[nestLevel];
                        if (iField >= 0)
                            m_entryCount[iField] = getCount() - firstEntryInLevel[nestLevel];
                        nestLevel--;
                        }
                    else if (*(q-1) != '/') {
                        fieldName.store(p, (q-p));
                        nestLevel++;
                        if (nestLevel >= maxNestLevel)
                            return;
                        }
                    }
                if (q)
                    p = q + 1;
                else
                    p = 0;
                }
            }
        else if (c == '&') {
            // entity
            wchar_t ch;
            p = translateEntity(p, ch);
            if (ch < 128)
                fieldValue << (char) ch;
            else {
                utf8.clear();
                utf8.storeUnicodeAsUtf8(&ch, 1);
                fieldValue << utf8;
                }
            }

        else{
            if ((c < 0) || !_IsSpace(c) || fieldValue.getLength())
                fieldValue << c;
            p++;
            }
        }
}

CXmlReader::Selection CXmlReader::selectAll() const
{   Selection result;
    result.m_first = 0;
    result.m_last = m_fieldNameTable.getCount();
    return result;
}

CXmlReader::Selection CXmlReader::select(const FieldSet& set, int iItem) const
{   Selection result;
    if (set.getCount() > iItem) {
        int iField = set.get(iItem);
        result.m_first = iField;
        result.m_last = iField + m_entryCount.get(iField) - 1;
        }
    return result;
}

CXmlReader::Selection CXmlReader::select(const char *fieldName, const CXmlReader::Selection& subset) const
{   FieldSet matchingFields = find(fieldName, subset);
    return select(matchingFields, 0);
}

CXmlReader::Selection CXmlReader::select(const char *fieldName) const
{   return select(fieldName, selectAll());
}

void CXmlReader::getFieldsInSelection(const CXmlReader::Selection subset, DStringSet& dest)
{   for (int i = subset.m_first; i <= subset.m_last; ++i) {
        const char *n = m_fieldNameTable.getString(i);
        const char *v = m_fieldValueTable.getString(i);
        if (!strIsBlank(n) && !strIsBlank(v)) {
            dest.append(n);
            dest.append(v);
            }
        }
}

void CXmlReader::getValuesInSelection(const CXmlReader::Selection subset, const char *fieldName, DStringSet& dest)
{   for (int i = subset.m_first; i <= subset.m_last; ++i) {
        const char *n = m_fieldNameTable.getString(i);
        const char *v = m_fieldValueTable.getString(i);
        if (!strIsBlank(v) && !_stricmp(fieldName, n)) {
            dest.append(v);
            }
        }
}


CXmlReader::FieldSet CXmlReader::find(const char *fieldName, const CXmlReader::Selection& sel) const
{   FieldSet result;
    for (int i = sel.m_first; i <= sel.m_last; ++i) {
        if (!_stricmp(fieldName, m_fieldNameTable.getString(i)))
            result.append(i);
        }
    return result;
}

CXmlReader::FieldId CXmlReader::findFirstIn(const char *fieldName, const CXmlReader::Selection& sel) const
{   for (int i = sel.m_first; i <= sel.m_last; ++i) {
        if (!_stricmp(fieldName, m_fieldNameTable.getString(i)))
            return i;
        }
    return FAIL;
}

CXmlReader::FieldSet CXmlReader::find(const char *fieldName) const
{   Selection all = selectAll();
    return find(fieldName, all);
}

CXmlMaker::CXmlMaker()
{   m_indent = 0;
    m_text << XmlHeader;
    m_fAllowUtf8 = true;
    m_pHider = &unsecureHider;
}

void CXmlMaker::clear()
{   m_text.clear();
    m_indent = 0;

    m_text.clear();
    m_text << XmlHeader;
}

void CXmlMaker::clearNoHeader()
{   m_text.clear();
    m_indent = 0;
}

void CXmlMaker::putIndent()
{   breakLine();
    for (int i = 0; i < m_indent; ++i)
        m_text << '\t';
}

void CXmlMaker::breakLine()
{   if (m_text.last() != '\n')
        m_text << crlf;
}
// http://www.w3.org/TR/REC-xml/#NT-NameChar
// 	NameStartChar	   ::=   	":" | [A-Z] | "_" | [a-z] | [#xC0-#xD6] | [#xD8-#xF6] | [#xF8-#x2FF] | [#x370-#x37D] | [#x37F-#x1FFF] | [#x200C-#x200D] | [#x2070-#x218F] | [#x2C00-#x2FEF] | [#x3001-#xD7FF] | [#xF900-#xFDCF] | [#xFDF0-#xFFFD] | [#x10000-#xEFFFF]
// [4a]   	NameChar	   ::=   	NameStartChar | "-" | "." | [0-9] | #xB7 | [#x0300-#x036F] | [#x203F-#x2040]
static bool isLegalXmlNameStartChar(wchar_t ch)
{
	if (Ascii_IsAlpha(ch) || (ch == ':') || (ch == '_'))
		return true;
	static wchar_t legalRanges[] = {
		0xc0, 0xd6,
		0xd8, 0xf6,
		0xf8, 0x2ff,
		0x370, 0x37d,
		0x37f, 0x1fff,
		0x200c, 0x200d,
		0x2070, 0x218f,
		0x2c00, 0x2fef,
		0x3001, 0xd7ff,
		0xf900, 0xfdcf,
		0,0 };
	for (int i = 0; legalRanges[i]; i += 2) {
		if ((ch >= legalRanges[i]) && (ch <= legalRanges[i+1]))
			return true;
		}
	return false;
}

static bool isLegalXmlNameChar(wchar_t ch)
{
	if (isLegalXmlNameStartChar(ch))
		return true;
	if (Ascii_IsDigit(ch))
		return true;
	if ((ch == '-') || (ch == '.') || (ch == 0xb7) || ((ch >= 0x300) && (ch <= 0x36f)) || ((ch >= 0x203f) && (ch <= 0x2040)))
		return true;
	return false;
}


const char *CXmlMaker::fixFieldName(const char *orig, DString& fixed)
{	fixed.clear();
	DWString f;
	f.storeUtf8AsUnicode(orig);
	DWString f2;
	if (!isLegalXmlNameStartChar(f.first()))
		f2 << "_";
	else if (!_strnicmp(orig, "xml", 3))
		f2 << "_";
	for (int i = 0; i < f.getLength(); ++i) {
		wchar_t ch = f.getChar(i);
		if (isLegalXmlNameChar(ch))
			f2 << ch;
		else if (ch == '/')
			f2 << ".";
		else
			f2 << "_";
		}
	fixed.clear();
	fixed.storeUnicodeAsUtf8(f2);
	fixed.replace(':', '_');
	return fixed;
}

const char *CXmlMaker::fixFieldName(const char *orig)
{	return fixFieldName(orig, fieldNameTemp);
}

void CXmlMaker::beginField(const char *fieldName)
{   putIndent();
    m_text << '<';
	m_text << fixFieldName(fieldName) << '>';
    m_indent++;
    if (m_rootField.isBlank())
        m_rootField = fieldName;
}

void CXmlMaker::endField(const char *fieldName)
{   m_indent--;
    if (m_text.last() == '\n')
        putIndent();
    m_text << "</";
    m_text << fixFieldName(fieldName);
    m_text << ">" << crlf;
}


void CXmlMaker::storeValue(const wchar_t *ws)
{   wchar_t ch;
    while((ch = *ws++)) {
        switch(ch) {
            case '<':
                m_text << "&lt;";
                break;
            case '>':
                m_text << "&gt;";
                break;
            case '&':
                m_text << "&amp;";
                break;
            default:
                if ((ch > 127) && (ch < 65534)) {
					if (isLegalXmlChar(ch)) {
						char tmp[10];
						I32_to_A(ch, tmp, sizeof tmp, 10);
						m_text << "&#" << ch << ";";
						}
					else
						m_text << '?';
					}
                else if (ch > 31)
                    m_text << (char)ch;
                else
					m_text << ' ';
                break;
            }
        }
}

bool CXmlMaker::deleteIllegalXml(DWString& ws)
{	bool ret = false;
	for (int i = 0; i < ws.getLength(); ++i) {
		if (!CXmlMaker::isLegalXmlChar(ws.getChar(i))) {
			ws.setChar(i, '?');
			ret = true;
			}
		}
	return ret;
}

void CXmlMaker::storeValue(const char *value)
{   if (!value)
        value = "";
    if (isLegalLowChars(value))
		storeLegalUtf8(value);
	else {
        wvalue.storeUtf8AsUnicode(value);
		if (!m_fAllowUtf8) 
			storeValue(wvalue);
		else {
			if (deleteIllegalXml(wvalue)) {
				DString tmp;
				tmp.storeUnicodeAsUtf8(wvalue);
				storeLegalUtf8(tmp);
				}
			else
				storeLegalUtf8(value);
			}
		}
}

// Only called internally after we filter out illegal XML characters in storeValue()
void CXmlMaker::storeLegalUtf8(const char *value)
{
    int fNeedCData = ((strchr(value, '\n') || strchr(value, '\r')) && !strstr(value, "]]>"));
    if (fNeedCData) {
        m_text << BeginCData << crlf;
        char ch;
        while((ch = *value++)) {
			if ((ch >= 32) || (ch < 0) || (ch == 0x09) || (ch == 0x0a) || (ch == 0xd))
				m_text << ch;
			else
				m_text << ' ';
			}
		m_text << EndCData << crlf;
        }
    else {
		char ch;
        while((ch = *value)) {
            switch(*value) {
                case '<':
                    m_text << "&lt;";
                    break;
                case '>':
                    m_text << "&gt;";
                    break;
                case '&':
                    m_text << "&amp;";
                    break;
                default:
					if ((ch >= 0) && (ch < 32))
						m_text << " ";
					else
						m_text << ch;
                    break;
                }
            value++;
            }
        }
}

void CXmlMaker::storeField(const char *name, const char *value)
{   beginField(name);
    storeValue(value);
    endField(name);
}

void CXmlMaker::storeFields(DStringSet& fieldSet)
{   for (int i = 0; i+1 < fieldSet.getCount(); i += 2)
        storeField(fieldSet.getString(i), fieldSet.getString(i+1));
}

void CXmlMaker::storeField(const char *name, const dtsFileDate& date)
{   DString80 s;
	s.ymd( date.year, date.month, date.day, '-');
	s << ' ';
	s.hms(date.hour, date.minute, date.second);
	s << "Z";
    storeField(name, s);
}

void CXmlMaker::storeField(const char *name, const DArrayAsVector<long>& value)
{   svalue.clear();
    for (int i = 0; i < value.getCount(); ++i)
        svalue << value.get(i) << ' ';
    storeField(name, svalue);
}

#ifdef GrowableIntArray_H
void CXmlMaker::storeField(const char *name, const CGrowableIntArray<long>& value)
{   svalue.clear();
    for (int i = 0; i < value.getCount(); ++i)
        svalue << value.get(i) << ' ';
    storeField(name, svalue);
}
#endif


void CXmlMaker::storeField(const char *name, const long *valueArray, long nItems)
{   svalue.clear();
    for (int i = 0; i < nItems; ++i)
        svalue << valueArray[i] << ' ';
    storeField(name, svalue);
}

void CXmlMaker::storeRelativeField(const char *fieldName, const char *value)
{   DFilename v;
    v = value;
    if (!v.isBlank() && !strIsBlank(m_outputFile)) {
        DFilename baseFolder;
        baseFolder = UiToUtf8(m_outputFile);
        baseFolder.rmTail();
        v.makeRelativeTo(baseFolder);
        }
    storeField(fieldName, v);
}

void CXmlMaker::storeField(const char *name, int value)
{   svalue.clear();
    svalue << value;
    storeField(name, svalue);
}

void CXmlMaker::storeField(const char *name, __int64 value)
{   char buf[80];
    I64_to_A(value, buf, sizeof buf, 10);
    storeField(name, buf);
}

void CXmlMaker::storeField(const char *name, const DStringSet& value)
{   svalue.clear();
    value.formatAsSimpleString(svalue);
    storeField(name, svalue);
}

void CXmlMaker::storeField(const char *name, const DWStringSet& value)
{
	DWString wValue;
	value.formatAsSimpleString(wValue);
	storeField(name, wValue);
}


void CXmlMaker::storeFieldHidden(const char *name, const DStringSet& value)
{   svalue.clear();
    value.formatAsSimpleString(svalue);
    if (m_pHider)
		m_pHider->scramble(svalue);
    storeField(name, svalue);
}

void CXmlMaker::storeField2(const char *name, const DStringSet& value, char delimiter, char quoteChar)
{	svalue.clear();

	value.formatAsSimpleString(svalue, delimiter, quoteChar);
	storeField(name, svalue);
}

void CXmlMaker::makeCommentTitle(const char *text, const char *a, const char *b)
{   DString s;
    DString t;
    t << BeginComment << ' ';
    t.padTo(80, '=');
    t << ' ' << EndComment;

    s << text;
    if (!strIsBlank(a))
        s << ' ' << a;
    if (!strIsBlank(b))
        s << ' ' << b;
    s.padTo(75, ' ');
    s.replace('-', '=');
    m_text << crlf << t << crlf << BeginComment << ' ' << s << ' ' << EndComment << crlf << t << crlf;
}

int CXmlMaker::writeU8(const char *fn)
{	DFile out;
	int ret = FAIL;
	if (out.openU8(fn, F_ANY | F_TRUNC) == SUCCESS) {
		if (m_text.copyToFile(out))
			ret = SUCCESS;
		out.close();
		}
	return ret;
}



// Use simple xor encryption to make things like passwords
// stored in XML fields less obvious.  This can easily be cracked,
// so it is not secure at all.
static const char *key = "ThisIsNotSecure";

static void xxor(DString& data, DString& scrambled)
{	ptrdiff_t keyLen = strlen(key);
	size_t iKey = 0;
	for (int i = 0; i < data.getLength(); ++i) {
		char ch = data.getChar(i);
		char keych = key[iKey];
		char xch = ch ^ keych;
		scrambled << xch;
		iKey = (iKey + 1) % keyLen;
		}
}

void CTextHider::scramble(DString& data)
{	DString ret;
	xxor(data, ret);
	data.clear();
	data << '?';
	data.appendHexEncoded(ret, ret.getLength());
}

void CTextHider::unscramble(DString& data)
{	if (data.first() != '?')
		return;
	data.remove(0, 1);
	DString tmp;
	tmp.hexDecode(data);
	data.clear();
	xxor(tmp, data);
}
