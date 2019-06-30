#ifndef MemAllocator_H
	#define MemAllocator_H
	
	
#ifdef USE_DTSEARCH_NAMESPACE

namespace dtSearch {

#endif

class overwrite_exception : public std::exception {
	public:
		overwrite_exception(void *badData, ptrdiff_t nBytes) {
			setData(badData, nBytes);
			}
		void setData(void *badData, ptrdiff_t nBytes) {
			clear();
			bad_data_bytes = min(nBytes, sizeof bad_data);
			memmove(&bad_data, badData, bad_data_bytes);
			}
		overwrite_exception(const overwrite_exception& other) {
			copy(other);
			}
		overwrite_exception& operator=(const overwrite_exception& other) {
			copy(other);
			return *this;
			}
		void clear() {
			memset(&bad_data, 0, sizeof bad_data);
			bad_data_bytes = 0;
			}
		void copy(const overwrite_exception& other) {
			memmove(&bad_data, &other.bad_data, sizeof bad_data);
			bad_data_bytes = other.bad_data_bytes;
			}
		char bad_data[16];
		ptrdiff_t bad_data_bytes;
	};



const unsigned char SentinelPrefix = '\x21';
const unsigned char SentinelSuffix = '\x22';
const int SentinelSize = 8;
const int PointerAlignment = sizeof(void *);	
const int DefaultBlockSize = 65536-2*SentinelSize-32;

class CAllocatedBlockTable {
	public:
		CAllocatedBlockTable() :
			usedBlockCount(0),
			sentinelSize(8)
		{
			blockTable.setDefaultValue(0);
			sizeTable.setDefaultValue(0);
			allocatedSize = 0;
			}
		~CAllocatedBlockTable() {
			dealloc();
			}
		void dealloc() {
			for (int i = 0; i < blockTable.getCount(); ++i) 
				delete[]  blockTable.getValue(i);
			blockTable.clear();
			sizeTable.clear();
			usedBlockCount = 0;
			allocatedSize = 0;
			usedSize = 0;
			}
		void clear() {
			usedBlockCount = 0;
			usedSize = 0;
			}
		char *allocBlock(ptrdiff_t size) {
			if (usedBlockCount < blockTable.getCount()) {
				if (sizeTable.getValue(usedBlockCount) >= size) {
					char *ret = blockTable.get(usedBlockCount);
					usedBlockCount++;
					usedSize += size;
					memset(ret + sentinelSize, 0, size);
					return addSentinelBytes(ret, sizeTable.get(usedBlockCount));
					}
				}
			if (!tryAddBlock(size))
				throw std::bad_alloc();
			usedBlockCount = blockTable.getCount();
			usedSize += size;
			char *ret = addSentinelBytes(blockTable.last(), size);
			checkThrow();
			return ret;
			}
		bool tryAddBlock(ptrdiff_t size) {
			char *b = new(std::nothrow) char[size + 2*sentinelSize];
			if (b) {
				blockTable.append(b);
				sizeTable.append(size);
				addSentinelBytes(blockTable.last(), size);
				allocatedSize += size;
				return true;
				}
			return false;
			}
		void checkThrow() {
			for (int i = 0; i < blockTable.getCount(); ++i) {
				char *block = blockTable.get(i);
				ptrdiff_t size = sizeTable.get(i);
				if (!hasGoodSentinelPrefix(block)) {
					overwrite_exception ex(block, size);
					throw ex;
					}
				if (!hasGoodSentinelSuffix(block, size)) {
					overwrite_exception ex(block + size + sentinelSize, sentinelSize);
					throw ex;
					}
				}
			}	
		ptrdiff_t getUsedSize() const {
			return usedSize;
			}
		ptrdiff_t getAllocatedSize() const {
			return allocatedSize;
			}
	protected:	
		DArrayAsVector<char *> blockTable;
		DArrayAsVector<ptrdiff_t> sizeTable;
		int usedBlockCount;
		ptrdiff_t allocatedSize;
		ptrdiff_t usedSize;
		int sentinelSize;
		bool hasGoodSentinelPrefix(char *block) {
			for (int i = 0; i < sentinelSize; ++i) {
				if (block[i] != SentinelPrefix)
					return false;
				}
			return true;
			}
		bool hasGoodSentinelSuffix(char *block, ptrdiff_t size) {
			block += (size + sentinelSize);
			for (int i = 0; i < sentinelSize; ++i) {
				if (block[i] != SentinelSuffix)
					return false;
				}
			return true;
			}
		char *addSentinelBytes(char *block, ptrdiff_t size) {
			memset(block, SentinelPrefix, sentinelSize);
			memset(block + size + sentinelSize, SentinelSuffix, sentinelSize);
			return block + sentinelSize;
			}
	};


class CMemAllocator {
    protected:

	

		struct MemBlock {
			char *ptr;
			ptrdiff_t avail;
	
			MemBlock() : ptr(0), avail(0) { }
			void clear() {
				ptr = 0;
				avail = 0;
				}		
			inline char *allocUnaligned(ptrdiff_t bytes) {
				if (bytes <= avail) {
					char *ret = ptr;
					useBytes(bytes);
					return ret;
					}
				return 0;
				}
			inline char *allocAligned(ptrdiff_t bytes) {
					const int alignment = PointerAlignment;
					uintptr_t x = (uintptr_t) ((void *)ptr);
					uintptr_t offset = x % alignment;
					if (offset > 0) 
						useBytes(alignment - offset);
					return allocUnaligned(bytes);
					}
			// Allocate a block guaranteed to be aligned to the provided power-of-two alignment
			inline char *allocAligned(ptrdiff_t bytes, int alignment) {
					uintptr_t x = (uintptr_t) ((void *)ptr);
					uintptr_t offset = (x & (alignment-1));
					if (offset > 0) 
						useBytes(alignment - offset);
					return allocUnaligned(bytes);
				}
			inline void useBytes(ptrdiff_t ct) {
				ptr += ct;
				avail -= ct;
				}
			};

		ptrdiff_t m_blockSize;
		ptrdiff_t m_totalSize;
		ptrdiff_t m_preallocatedSize;
		MemBlock unalignedBlock;
		MemBlock pointerAlignedBlock;
		
		CAllocatedBlockTable m_blockTable;
		DHeapPtr<CAllocatedBlockTable> m_oversizedBlockTable;
        void newBlock(MemBlock& dest) {
			char *p = m_blockTable.allocBlock(m_blockSize);
			if (p) {
				dest.ptr = p;
				dest.avail = m_blockSize;
				m_totalSize += m_blockSize;
				onBlockAlloc();
				}
			else
				throw std::bad_alloc();
            }
		virtual void onBlockAlloc() { }
        char *allocOversized(ptrdiff_t size) {
			m_oversizedBlockTable.allocOnce();
            char *s = m_oversizedBlockTable->allocBlock(size);
            if (s) 
				m_totalSize += size;
				
            return s;
            }

        void dealloc() {
			m_totalSize = 0;
			m_preallocatedSize = 0;
			m_blockTable.dealloc();
			unalignedBlock.clear();
			pointerAlignedBlock.clear();
			if (!m_oversizedBlockTable.isEmpty())
				m_oversizedBlockTable->dealloc();
            }
    public:
    
        CMemAllocator(ptrdiff_t aSize = DefaultBlockSize) :
            m_blockSize(0), m_totalSize(0)
            {
            setBlockSize(aSize);
            }
        virtual ~CMemAllocator() {
            dealloc();
            }
        void setBlockSize(ptrdiff_t v) {
            dealloc();
            m_blockSize = v;
            }
		void *getAlignedBlock() {
			return pointerAlignedBlock.ptr;
			}
		void *getUnalignedBlock() {
			return unalignedBlock.ptr;
			}
		bool preallocate(ptrdiff_t totalSize) {
			dealloc();
			m_preallocatedSize = 0;
			if (m_blockSize == 0)
				m_blockSize = DefaultBlockSize;
			ptrdiff_t nBlocks = (totalSize + m_blockSize - 1)/m_blockSize;
			for (ptrdiff_t i = 0;  i < nBlocks; ++i) {
				if (!m_blockTable.tryAddBlock(m_blockSize))
					return false;
				m_preallocatedSize += m_blockSize;
				}
			return true;
			}
        virtual void clear() {
            m_blockTable.clear();
            m_oversizedBlockTable.dealloc();
            unalignedBlock.clear();
            pointerAlignedBlock.clear();
			m_totalSize = 0;
            }
		// Check for memory overwrites around the allocated blocks, and throw overwrite_exception if any are found
		void checkThrow() {
			m_blockTable.checkThrow();
			if (!m_oversizedBlockTable.isEmpty())
				m_oversizedBlockTable->checkThrow();
			}
		ptrdiff_t getAllocatedSize() const {
			return m_blockTable.getAllocatedSize();
		}
		ptrdiff_t getUsedSize() const {
			return m_blockTable.getUsedSize() - unalignedBlock.avail - pointerAlignedBlock.avail;
			}
			
		inline char *alloc64(DFilePosition aSize) {
			return alloc(static_cast<ptrdiff_t>(aSize));
			}
		inline char *alloc(ptrdiff_t size) {
			return allocAligned(size);
			}
        inline char *allocUnaligned(ptrdiff_t aSize) {
        	char *r = unalignedBlock.allocUnaligned(aSize);
        	if (r)
        		return r;
        	
            if (aSize >= m_blockSize) 
				return allocOversized(aSize);
            newBlock(unalignedBlock);
            return unalignedBlock.allocUnaligned(aSize);
            }
        inline char *allocAligned(ptrdiff_t aSize) {
        	char *r = pointerAlignedBlock.allocAligned(aSize);
        	if (r)
        		return r;
        	
            if (aSize >= m_blockSize) 
				return allocOversized(aSize);
            newBlock(pointerAlignedBlock);
            return pointerAlignedBlock.allocAligned(aSize);
            }
		inline char *allocVarAligned(ptrdiff_t aSize, int alignment) {
			// Aligned from the unaligned block because the aligned block
			// has to assume pointer size alignment
        	char *r = unalignedBlock.allocAligned(aSize, alignment);
        	if (r)
        		return r;
        	
            if (aSize >= m_blockSize) 
				return allocOversized(aSize);
            newBlock(unalignedBlock);
            return unalignedBlock.allocAligned(aSize, alignment);
            }
        inline char *allocClear(int aSize) {
            char *ret = alloc(aSize);
            if (ret)
                memset(ret, 0, aSize);
            return ret;
            }
        inline wchar_t *copy(const wchar_t *ws, ptrdiff_t len = -1) {
			if (!ws)
				return 0;
			if (len < 0)
				len = wcslen(ws);
			ptrdiff_t stringSize = (sizeof(wchar_t)) * (len+1);
			wchar_t *ret = (wchar_t *) allocVarAligned(stringSize, sizeof(wchar_t));
			if (!ret)
				return 0;
			memmove(ret, ws, len * sizeof(wchar_t));
			ret[len] = 0;
            return ret;
            }

        inline char *copy(const char *s, ptrdiff_t len = -1) {
            if (!s)
                return copy("");
			if (len < 0)
				len = strlen(s);
            char *ret = allocUnaligned(len+1);
            if (ret) {
                memmove(ret, s, len);
                ret[len] = '\0';
                }
            return ret;
            }
        inline char *binaryCopy(const char *s, ptrdiff_t len, int alignment = 0) {
			if (!s || !len)
				return 0;

			char *ret;
			if (alignment > 0)
				ret = allocVarAligned(len, alignment);
			else
				ret = allocUnaligned(len);
			if (ret)
				memmove(ret, s, len);
			return ret;
			}
        inline void *binaryCopyV(const void *s, ptrdiff_t bytes) {
			return binaryCopy((const char *) s, bytes);
			}

        inline char *copyStringSet(const char *s) {
			return binaryCopy(s, DStringSet::calcSetLength(s), 0);
            }
        inline long *copyIntArray(const long *array, int count, int fNullTerminate) {
            ptrdiff_t nbytes = count * sizeof(long);
            if (fNullTerminate)
            	nbytes += (fNullTerminate? sizeof(long) : 0);
            long *ret = (long *) allocVarAligned(nbytes, sizeof(long));
            memmove(ret, array, nbytes);
            if (fNullTerminate)
                ret[count] = 0;
            return ret;
            }
    };


#ifdef USE_DTSEARCH_NAMESPACE

}; // namespace dtSearch

#endif

#endif
