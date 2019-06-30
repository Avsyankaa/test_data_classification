#ifndef StringBuilder_H
	#define StringBuilder_H

#include <darray.h>
#include <dfile.h>
#include <GrowableArray.h>
#include <MemAllocator.h>

#ifdef USE_DTSEARCH_NAMESPACE

namespace dtSearch {

#endif


const ptrdiff_t SegmentSize = 8192;

class DStringBuilder {
	public:

		DStringBuilder() {
			m_segmentTable.setDefaultValue(0);
			}
		~DStringBuilder() {
			}

		DStringBuilder& operator=(const DStringBuilder& other) {
			clear();
			other.copyToStringBuilder(*this);
			return *this;
		}
		DStringBuilder& operator=(const char *s) {
			clear();
			append(s);
			return *this;
		}
		bool write(const char *s, ptrdiff_t len) {
			if (len < SegmentSize) {
				m_inputBuffer.store(s, (int) len);
				return onWrite();
				}
			else
				return bigWrite(s, len);
			}
		bool append(const char *s) {
			return write(s, strlen(s));
			}
		bool append(const char *s, ptrdiff_t len) {
			return write(s, len);
			}
		void clear() {
			m_allocator.clear();
			m_segmentTable.clear();
			m_inputBuffer.clear();
		}
		ptrdiff_t getLength() const {
			return m_segmentTable.getCount()*SegmentSize + m_inputBuffer.getLength();
		}
		DStringBuilder& operator<<(const DString& s) {
			write(s, s.getLength());
			return *this;
		}
		DStringBuilder& operator<<(const wchar_t *ws) {
			append(ws, wcslen(ws));
			return *this;
		}
		void appendUcs16(const unsigned short *s, ptrdiff_t len, bool bInvertByteOrder = false) {
			DWString tmp;
			while(len > 0) {
				ptrdiff_t toAppend = _Min(len, SegmentSize*4);
				tmp.clear();
				tmp.storeUcs16AsUnicode(s, toAppend);
				if (bInvertByteOrder)
					tmp.invertByteOrder();
				append(tmp, tmp.getLength());
				len -= toAppend;
				}
			}

		void append(const wchar_t *ws, ptrdiff_t len) {
			DString tmp;
			while(len > 0) {
				ptrdiff_t toAppend = _Min(len, SegmentSize*4);
				tmp.clear();
				tmp.storeUnicodeAsUtf8(ws, toAppend);
				write(tmp, tmp.getLength());
				len -= toAppend;
				}
		}
		void appendChar(char c) {
			m_inputBuffer.appendChar(c);
			onWrite();
			}
		DStringBuilder& operator<<(const char *s) {
			write(s, strlen(s));
			return *this;
		}
		DStringBuilder& operator<<(int i) {
			m_inputBuffer << i;
			onWrite();
			return *this;
			}
		DStringBuilder& operator<<(char c) {
			m_inputBuffer << c;
			onWrite();
			return *this;
			}
		DStringBuilder& operator<<(unsigned char c) {
			m_inputBuffer << c;
			onWrite();
			return *this;
			}
		DStringBuilder& operator<<(wchar_t c) {
			m_inputBuffer << c;
			onWrite();
			return *this;
			}
		DStringBuilder& operator<<(unsigned int i) {
			m_inputBuffer << i;
			onWrite();
			return *this;
			}
		DStringBuilder& operator<<(__int64 i) {
			m_inputBuffer << i;
			onWrite();
			return *this;
			}
		bool copyToStringBuilder(DStringBuilder& other) const {
			for (int i = 0; i < m_segmentTable.getCount(); ++i) {
				other.write(m_segmentTable.get(i), SegmentSize);
			}
			return other.write(m_inputBuffer, m_inputBuffer.getLength());
			}
		bool copyToBuffer(void *pBuf, ptrdiff_t bufSize) const {
			if (bufSize < getLength())
				return false;
			char *p = (char *) pBuf;
			for (int i = 0; i < m_segmentTable.getCount(); ++i) {
				memmove(p, m_segmentTable.get(i), SegmentSize);
				p += SegmentSize;
				}
			memmove(p, m_inputBuffer, m_inputBuffer.getLength());
			p += m_inputBuffer.getLength();
			if (bufSize > getLength())
				*p++ = 0; // null terminate if we can
			return true;
		}
		bool copyToString(DString& s) const {
			s.clear();
			if (!s.tryExtend(getLength()+1))
				return false;
			for (int i = 0; i < m_segmentTable.getCount(); ++i)
				s.store(m_segmentTable.get(i), SegmentSize);

			s.store(m_inputBuffer, m_inputBuffer.getLength());
			return true;
		}
		bool copyToFile(DFile& f) const {
			for (int i = 0; i < m_segmentTable.getCount(); ++i) {
				if (f.write(m_segmentTable.get(i), SegmentSize) == FAIL)
					return false;
			}
			if (f.write(m_inputBuffer, m_inputBuffer.getLength()) == FAIL)
				return false;
			return true;
		}
		bool copyToStream(DStreamBase& f) const {
			for (int i = 0; i < m_segmentTable.getCount(); ++i) {
				if (f.write(m_segmentTable.get(i), SegmentSize) == FAIL)
					return false;
			}
			if (f.write(m_inputBuffer, m_inputBuffer.getLength()) == FAIL)
				return false;
			return true;
		}

		bool copyToFileU8(const char *fn, bool bAppend = false) const {
			DFile file;
			int flags = F_ANY;
			if (!bAppend)
				flags |= F_TRUNC;
			if (file.open(fn, flags) == SUCCESS) {
				if (bAppend)
					file.seek(file.getLength());
				return copyToFile(file);
				}
			else
				return false;
		}


		char last() const {
			if (m_inputBuffer.getLength())
				return m_inputBuffer.last();
			if (m_segmentTable.getCount()) {
				const char *ret = m_segmentTable.get(m_segmentTable.getCount()-1);
				return ret[SegmentSize-1];
				}
			return 0;
			}
		int getBlockCount() {
			return m_segmentTable.getCount() + 1;
		}
		const char *getNthBlock(int iBlock) {
			if (iBlock < m_segmentTable.getCount())
				return m_segmentTable.get(iBlock);
			else if (iBlock == m_segmentTable.getCount())
				return m_inputBuffer;
			else
				return 0;
		}
		ptrdiff_t getNthBlockSize(int iBlock) {
			if (iBlock < m_segmentTable.getCount())
				return SegmentSize;
			else if (iBlock == m_segmentTable.getCount())
				return m_inputBuffer.getLength();
			else
				return 0;
		}
		ptrdiff_t getStandardBlockSize() {
			return SegmentSize;
		}
	protected:
		DArrayAsVector<const char *> m_segmentTable;
		CMemAllocator m_allocator;
		DString m_inputBuffer;
		bool onWrite() {
			while(m_inputBuffer.getLength() >= SegmentSize) {
				if (!addSegment(m_inputBuffer))
					return false;
				m_inputBuffer.remove(0, SegmentSize);
				}
			return true;
			}


		bool addSegment(const char *s) {
			const char *text = m_allocator.binaryCopy(s, SegmentSize);
			if (text) {
				m_segmentTable.append(text);
				return true;
				}
			else
				return false;
		}


		bool bigWrite(const char *s, ptrdiff_t len) {
			while(len >= SegmentSize) {
				if (m_inputBuffer.getLength() > 0) {
					ptrdiff_t chunkSize = SegmentSize - m_inputBuffer.getLength();
					if (!write(s, chunkSize))
						return false;
					s += chunkSize;
					len -= chunkSize;
				}
				else {
					addSegment(s);
					s += SegmentSize;
					len -= SegmentSize;
				}
			}
			return write(s, len);
		}


	};


#ifdef USE_DTSEARCH_NAMESPACE

} // namespace dtSearch

#endif

#endif

