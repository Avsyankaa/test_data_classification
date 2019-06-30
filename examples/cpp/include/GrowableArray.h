#ifndef GrowableArray_H
    #define GrowableArray_H

// need for strcmp
#ifndef __STRING_H
    #include <string.h>
#endif
#ifndef DARRAY_H
#	include <darray.h>
#endif

#ifndef FAIL
#   define FAIL -1
#endif


#ifdef USE_DTSEARCH_NAMESPACE

namespace dtSearch {

#endif


//  Class defines an array of any type of objects as a vector that
//  automatically resizes itself.  The type must have operator=
//  defined.

template <class Type>
class CGrowableArray {
    public:
        CGrowableArray(int aSize = 64, int aIncrement = 0);
        CGrowableArray(const CGrowableArray& other);
        ~CGrowableArray();
        CGrowableArray& operator=(const CGrowableArray& other);
		void copyArray(const CGrowableArray& other);
		void copyArray(const DArrayAsVector<Type>& other);
		void appendArray(const CGrowableArray& other);
		void appendArray(Type *pTable, int count);

        inline Type& operator[](int i);

        inline Type& get(int i) const;

        inline Type& getValue(int i) const {
			return get(i);
			}
        inline Type& getValue(unsigned int i) const {
			return get((int) i);
			}
        inline Type& getValue(__int64 i) const {
			return get((int) i);
			}

        void fill(const Type& v);

        int find(const Type& v);
        
        int findLTE(const Type& v) const;
        int findExact(const Type& v) const;

        void pack(const Type& v);

        void remove(int i);
		void removePreservingOrder(int i);

        void insertAt(const Type& v, int pos);

		void storeAt(const Type& v, int pos);


        inline int getCount() const {
            return count;
            }
        inline int isEmpty() const {
            return count == 0;
            }
        void clear() {
            count = 0;
            if (bClearToNulls)
    			clearAllToNulls();
            }
        inline Type& first() {
            return firstChunk[0];
            }
        inline Type& last() {
            return get(count-1);
            }
        int append(const Type& v);
		bool tryAppendMultiple(const Type& v, int nTimes);
        Type& append(); 
        bool tryAppend(const Type& v);
        void store(const Type *v, int count);
        void reverse();
        bool extend(int newSize);
		void push(const Type& v);
		int pop(Type& v);
		bool setCount(int newCount) {
			if (extend(newCount)) {
				count = newCount;
				return true;
				}
			return false;
		}
		void shrink(int aSize);
		void setClearToNulls(bool v) {
			bClearToNulls = v;
			clear();
		}
		bool set(const Type& v, int pos);

		Type *makeVector();

		bool isVector() const {
			return (chunkTable.getCount() == 1);
			}

		void clearAllToNulls() {
			clearAllTo(0);
			}
		void clearAllTo(unsigned char ch);
		int getChunkCount() const {
			return chunkTable.getCount();
			}
		Type *getNthChunk(int i) const {
			return chunkTable.getValue(i);
			}
		int getIncrement() const {
			return increment;
			}
		ptrdiff_t getAllocatedSizeInBytes() const {
				return sizeof(Type) * increment * chunkTable.getCount();
			}
		ptrdiff_t getSize() const {
			return increment * chunkTable.getCount();
			}
		inline bool goodIndex(int i) const {
			return (i >= 0) && (i < count);
		}
		void swapItems(int i, int j) {
			Type t = get(i);
			set(get(j), i);
			set(t, j);
			}
		void swap(CGrowableArray<Type>& other) {
			Swap(firstChunk, other.firstChunk);
			Swap(size, other.size);
			Swap(increment, other.increment);
			Swap(count, other.count);
			Swap(bThrowOnFailedAlloc, other.bThrowOnFailedAlloc);
			chunkTable.swap(other.chunkTable);
			Swap(bClearToNulls, other.bClearToNulls);
			} 
    protected:


        bool addChunks(int nChunks);

        Type *firstChunk;
        int size;
        int increment;
        int count;
        bool bThrowOnFailedAlloc;
        DArrayAsVector<Type *> chunkTable;
		bool bClearToNulls;
		
		Type *newChunk(int nItems);
		void dealloc();
		inline bool indexWithinSize(int i) const {
			return (i >= 0) && (i < size);
		}
    };


template <class Type>
CGrowableArray<Type>::CGrowableArray(int aSize, int aIncrement) :
	size(0), count(0), firstChunk(0), bClearToNulls(false), bThrowOnFailedAlloc(false)
{   if (aSize < 2)
        aSize = 2;
	increment = aIncrement;
	if (increment < 2)
		increment = 8192/sizeof(Type);
    if (increment < 16)
        increment = 16;
    extend(aSize);
}


template <class Type>
CGrowableArray<Type>::CGrowableArray(const CGrowableArray& other) :
    size(0),
    increment(0),
    count(0),
    bClearToNulls(false),
    bThrowOnFailedAlloc(other.bThrowOnFailedAlloc)
{   increment = other.increment;
    if (increment < 16)
        increment = 16;
	copyArray(other);
}

template <class Type>
CGrowableArray<Type>& CGrowableArray<Type>::operator=(const CGrowableArray<Type>& other)
{	copyArray(other);
	return *this;
}

template <class Type>
void CGrowableArray<Type>::copyArray(const CGrowableArray<Type>& other)
{	bClearToNulls = other.bClearToNulls;
	clear();
	
	if (extend(other.count)) {
		for (int i = 0; i < other.count; ++i)
			append(other.get(i));
		}
}

template <class Type>
void CGrowableArray<Type>::copyArray(const DArrayAsVector<Type>& other)
{	clear();
	
	if (extend(other.getCount())) {
		for (int i = 0; i < other.getCount(); ++i)
			append(other.get(i));
		}
}


template <class Type>
void CGrowableArray<Type>::appendArray(const CGrowableArray<Type>& other)
{	if (extend(other.count + count)) {
		for (int i = 0; i < other.count; ++i)
			append(other.get(i));
		}
}

template <class Type>
void CGrowableArray<Type>::appendArray(Type *pTable, int otherCount)
{	if (extend(otherCount)) {
		for (int i = 0; i < otherCount; ++i) {
			append(pTable[i]);
			}
		}

}



template <class Type>
CGrowableArray<Type>::~CGrowableArray()
{ 	dealloc();
}

template <class Type>
void CGrowableArray<Type>::dealloc()
{   for (int i = 0; i < chunkTable.getCount(); ++i) {
		Type *toDelete = chunkTable.getValue(i);
		delete[] toDelete;
		chunkTable[i] = 0;
	}

    firstChunk = 0;
    chunkTable.clear();
    size = 0;
    count = 0;
}

template <class Type>
int CGrowableArray<Type>::append(const Type& v)
{   if (!extend(count+1))
		return FAIL;

    count++;
    get(count-1) = v;

    return count-1;
}

template <class Type>
bool CGrowableArray<Type>::tryAppendMultiple(const Type& v, int nTimes)
{   if (!extend(count+nTimes))
		return false;

	for (int i = 0; i < nTimes; ++i) {
		append(v);
		}

    return true;
}


template <class Type>
Type& CGrowableArray<Type>::append()
{   if (!extend(count+1))
		throw std::bad_alloc();

    count++;
    return get(count-1);
}


template <class Type>
bool CGrowableArray<Type>::tryAppend(const Type& v)
{	if (!extend(count+1))
		return false;

    count++;
    get(count-1) = v;

    return true;
}



template <class Type>
Type *CGrowableArray<Type>::newChunk(int nItems)
{	Type *t = new Type[nItems];
		
	if (bClearToNulls && t)
		memset((void*)t, 0, sizeof(Type) * nItems);
	return t;
}

template <class Type>
bool CGrowableArray<Type>::addChunks(int nChunks)
{
    // Add new chunks to the array
    for (int i = 0; i < nChunks; ++i) {
		Type *x = newChunk(increment);
		if (!x)
			return false;
        chunkTable.append(x);
		}
    size = chunkTable.getCount() * increment;
    firstChunk = chunkTable[0];
    return true;
}

// Indexing with automatic extension
template <class Type>
Type& CGrowableArray<Type>::operator[](int i)
{
	if (goodIndex(i)) {
		if (i < increment)
			return firstChunk[i];
		return get(i);
		}

    if (i >= size)
        extend(i + 1);
    if (i >= count)
        count = i + 1;
	return get(i);
}

template<class Type>
bool CGrowableArray<Type>::set(const Type& v, int i)
{
	if (i < 0)
		return false;

    if (indexWithinSize(i) && (i < increment)) {
        firstChunk[i] = v;
		if (i >= count)
		    count = i + 1;
        return true;
        }

	if (!extend(i+1))
		throw std::bad_alloc();

    int chunk = i/increment;
    int offset = i % increment;

    Type *p = chunkTable.getValue(chunk);
	p[offset] = v;

	if (i >= count)
        count = i + 1;
	return true;
}


// Indexing without automatic extension
template <class Type>
Type& CGrowableArray<Type>::get(int i) const
{ 
    if (indexWithinSize(i) && (i < increment))
        return firstChunk[i];

    if ((i >= size) || (i < 0))
		i = 0;

    int chunk = i/increment;
    int offset = i % increment;

    Type *p = chunkTable.getValue(chunk);
	if (p)
		return p[offset];
	else
		return firstChunk[0];
}

template <class Type>
void CGrowableArray<Type>::clearAllTo(unsigned char ch)
{	int blockSize = increment * sizeof(Type);
	for (int i = 0; i < chunkTable.getCount(); ++i) {
		Type *p = chunkTable.get(i);
		if (p)
			memset(p, ch, blockSize);
	}
}





template <class Type>
void CGrowableArray<Type>::storeAt(const Type& v, int pos)
{	extend(pos+1);
	get(pos) = v;
}

template <class Type>
void CGrowableArray<Type>::remove(int i)
{   if ((i < 0) || (i >= count))
        return;

    if (i < count-1)
        get(i) = get(count-1);
    count--;

}

template <class Type>
void CGrowableArray<Type>::removePreservingOrder(int iItem)
{   if ((iItem < 0) || (iItem >= count))
        return;
    for (int i = iItem; i < count-1; ++i) {
		get(i) = get(i+1);
		}
    count--;
}


template <class Type>
bool CGrowableArray<Type>::extend(int newSize)
{   if (newSize < 2)
        newSize = 2;
    if (newSize <= size)
        return true;


    int nChunksToAdd = (newSize + increment - 1)/increment - chunkTable.getCount();

    if (!addChunks(nChunksToAdd))
		return false;
		
	return newSize <= size;
}


template <class Type>
void CGrowableArray<Type>::shrink(int aSize)
{	if (aSize >= size)
		return;
	dealloc();
	if (aSize > 0)
		extend(aSize);
}


template <class Type>
void CGrowableArray<Type>::store(const Type *v, int ct)
{	for (int i = 0; i < ct; ++i)
		append(v[i]);
}

template <class Type>
void CGrowableArray<Type>::reverse()
{   int s = 0, e = count-1;
    Type tmp;
    while( s < e) {
        tmp = get(s);
        get(s) = get(e);
        get(e) = tmp;
        s++;
        e--;
        }
}

template <class Type>
void CGrowableArray<Type>::fill(const Type& v)
{   count = size;
    for (int i = 0; i < size; ++i)
        get(i) = v;
}

template <class Type>
int CGrowableArray<Type>::find(const Type& v)
{   for (int i = 0; i < count; ++i) {
        if (get(i) == v)
            return i;
        }
    return -1;
}

template <class Type>
void CGrowableArray<Type>::insertAt(const Type& v, int pos)
{   if (pos < 0)
        return;
    if (pos >= count)
        count = pos+1;
    else
        count++;
    extend(count+1);
    if (count >= size) {
        count--;
        return;
        }
    for (int i = count - 2; (i >= pos); --i)
        get(i+1) = get(i);

    get(pos) = v;
}

template <class Type>
void CGrowableArray<Type>::pack(const Type& nullValue)
{   int dest = 0, source = 0;
    while(source < count) {
        if (!(get(source) == nullValue)) {
            if (dest != source)
                get(dest) = get(source);
            dest++;
            }
        source++;
        }
    count = dest;
    }

template <class Type>
void CGrowableArray<Type>::push(const Type& v)
{   append(v);
}

template <class Type>
int CGrowableArray<Type>::pop(Type& v)
{   if (count > 0) {
        v = get(count-1);
        count--;
        return SUCCESS;
        }
    return FAIL;
}

template <class Type> 		
Type *CGrowableArray<Type>::makeVector()
{
	if (chunkTable.getCount() == 1)
		return chunkTable.getValue(0);
	Type *newArray = new Type[count];
		
	if (!newArray)
		return 0;
	for (int i = 0; i < count; ++i) {
		newArray[i] = get(i);
	}
	int saveCount = count;
	dealloc();
	count = saveCount;
	chunkTable.append(newArray);
	increment = count;
	size = count;
	firstChunk = newArray;
	return newArray;
} 

template <class Type>
int CGrowableArray<Type>::findLTE(const Type& v) const
{   int x, y;
    y = getCount();
    x = 0;
    while(y > x) {
        int p = (x + y)/2;
        if (getValue(p) > v)
            y = p-1;
        else if (v > getValue(p))
            x = p+1;
        else {
            x = p;
            y = p;
            }
        }

    // find LTE position
    if (x >= getCount())
        return getCount()-1;
    int i = x;
    // Back up to the *first* item in the array that is less than or
    // equal to the search item
    while((i > 0) && !(v > getValue(i)))
        i--;
    while((i < getCount()-1) && (v > getValue(i)) && (!(getValue(i+1) > v)))
        i++;
    return i;
}


template <class Type>
int CGrowableArray<Type>::findExact(const Type& v) const
{   int pos = findLTE(v);
    if ((pos >= 0) && (getValue(pos) == v))
        return pos;
    else
        return FAIL;
}


#ifdef USE_DTSEARCH_NAMESPACE
}  // dtSearch namespace
#endif


#endif
