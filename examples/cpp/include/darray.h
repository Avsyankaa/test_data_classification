#pragma once
#ifndef DARRAY_H
    #define DARRAY_H


#ifndef FAIL
    #define FAIL (-1)
#endif
#ifndef SUCCESS
    #define SUCCESS (0)
#endif
#include <new>
#ifdef USE_DTSEARCH_NAMESPACE

namespace dtSearch {

#endif

template <typename Type> void Swap(Type& a, Type& b) {
	Type t;
	t = a;
	a = b;
	b = t;
	}

				

//  Class defines an array of any type of objects as a vector that
//  automatically resizes itself.  The type must have operator=
//  defined.

template <class Type>
class DArrayAsVector {
    public:
        DArrayAsVector(int aSize = 32);
        DArrayAsVector(const DArrayAsVector<Type>& other);
        ~DArrayAsVector();
        DArrayAsVector& operator=(const DArrayAsVector<Type>& other);
        bool operator==(const DArrayAsVector<Type>& other);
        bool copyArray(const DArrayAsVector<Type>& other);
        // Returns a const reference, so it is OK to return defaultValue
        inline const Type& get(int i) const {
            if ((i >= 0) && (i < count))
                return array[i];
            else
				return defaultValue;
            }

        // Returns a reference, so it cannot return defaultValue
        inline Type& getRef(int i) const {
            if ((i >= 0) && (i < count))
                return array[i];
            else 
				return array[0];
            }
        // Returns a const reference, so it is OK to return defaultValue
        inline const Type& getValue(int i) const {
            if ((i >= 0) && (i < count))
                return array[i];
            else
				return defaultValue;
            }

        Type& operator[](int i);

        void fill(const Type& v);

        void setDefaultValue(const Type& v);

        int find(const Type& v);

        void pack(const Type& v);
        void reverse();

        void insertAt(const Type& v, int pos);
        void replace(const Type& v, int pos);
        void set(const Type& v, int pos);


        void remove(int i);
        
        void removeLast() {
        		if (count > 0) 
        			remove(count-1);
        		}

        void removePreservingOrder(int i);

        Type *c_vector() const {
            return array;
            }
        int getCount() const {
            return count;
            }
        bool isEmpty() const {
            return count == 0;
            }
        void clear() {
			clearImp();
			}
        int append(const Type& v);
        void appendMultiple(const Type& v, int n) {
			extend(count+n);
			for (int i = count; i < n+count; ++i) 
				array[i] = v;
			count += n;
			}
        inline bool tryAppend(const Type& v) {
			if (count+1 < size) {
				array[count++] = v;
				return true;
				}
		    if (!tryExtend(count+1))
				return false;
			array[count] = v;
			 count++;
			return true;
		}
        inline bool tryAppendV(const Type v) {
			if (count+1 < size) {
				array[count++] = v;
				return true;
				}
		    if (!tryExtend(count+1))
				return false;
			array[count] = v;
			 count++;
			return true;
		}

        bool appendArray(const DArrayAsVector<Type>& other);
        bool store(const Type *p, int ct);
        void setCount(int aCount) {
            if (aCount <= size)
                count = aCount;
            }
        Type& last() {
            if (count > 0)
                return array[count-1];
            else
                return array[0];
            }
        void setLast(const Type& v) {
			if (count > 0)
				array[count-1] = v;
			else
				append(v);
			}
        Type& first() {
            return array[0];
            }
        Type& append() {
			if (count < size) {
				return array[count++];
				}
			extend(count+1);
			return array[count++];
			}
        void terminate(const Type terminalValue);
        // Try to extend array to new size and throw exception on failure
        void extend(int newSize);
        // Try to extend array to new size and return false on failure
        bool tryExtend(int newSize);
        Type *detachBuffer() {
			Type *ret = array;
			array = 0;
			size = 0;
			count = 0;
			return ret;
			}
		bool isBadIndex(int i) const {
			return (i < 0) || (i >= count);
			}
		bool isGoodIndex(int i) const {
			return ((i >= 0) && (i < count));
			}
		bool isGoodIndexAndCount(int i, int ct) const {
			if (ct == 0)
				return true;
			return isGoodIndex(i) && isGoodIndex(ct-1) && isGoodIndex(i+ct-1);
			}
		void push(const Type& v) {
				append(v);
			}
		int pop(Type& v) {
		if (count > 0) {
			 v = last();
			 count--;
			return SUCCESS;
			}
			return FAIL;
			}
		int getAllocatedCount() const {
			return size;
			}
		void shrink(int newSize) {
			clear();
			if (size > newSize) {
				Type *tempNewArray = new Type[newSize];
				useNewArray(newSize, tempNewArray);
				}
			}
		// Find the value that is less than or equal to a value using binary search, assuming that the array
		// is sorted.
        int findLTE(const Type& v) const;
		// Find the value that is equal to a value using binary search, assuming that the array is sorted.
        int findExact(const Type& v) const;

		// Swap internal contents with another array
		void swap(DArrayAsVector<Type>& other) {
			Swap(size, other.size);
			Swap(count, other.count);
			Swap(fHaveDefaultValue, other.fHaveDefaultValue);
			Swap(array, other.array);
			Swap(defaultValue, other.defaultValue);
		}
		bool good() {
			return array != 0;
		}
    protected:
        int size;
        int count;
        Type *array;
        Type defaultValue;
        int fHaveDefaultValue;

		
        void dealloc();
        void useNewArray(int aSize, Type *tempNewArray) {
			dealloc();
			array = tempNewArray;
			size = aSize;
			}
		void clearImp() {
		   count = 0;
		   if (fHaveDefaultValue) {
				for (int i = 0; i < size; ++i)
					array[i] = defaultValue;
				}
			}
		void clearUnused() {
		   if (fHaveDefaultValue) {
				for (int i = count; i < size; ++i)
					array[i] = defaultValue;
				}
			}			
    };

template <class Type>
DArrayAsVector<Type>::DArrayAsVector(int aSize) :
    size(0),
    array(0),
    count(0),
    fHaveDefaultValue(0)
{   if (aSize < 1)
        aSize = 1;
    extend(aSize);

}




template <class Type>
void DArrayAsVector<Type>::dealloc()
{	if (array) {
		delete[] array;
		array = 0;
	}
}

template <class Type>
void DArrayAsVector<Type>::extend(int newSize)
{
	#if defined(_MSC_VER) && !defined(_EXCEPTION_)
		tryExtend(newSize);
	#else
	   if (!tryExtend(newSize)) {
			throw std::bad_alloc();
		}
	#endif
}

template <class Type>
bool DArrayAsVector<Type>::tryExtend(int newSize)
{   if (newSize <= size)
        return true;

    if (newSize < size * 2)
        newSize = size * 2;

	// std::nothrow may not work, so we have to catch the bad_alloc just in case
	// See http://msdn.microsoft.com/msdnmag/issues/03/09/LegacySTLFix/default.aspx
	// James Hebben, "Don't Let Memory Allocation Failures Crash Your Legacy STL Application"
    Type *tempNewArray = 0;
    try {
    	tempNewArray = new(std::nothrow) Type[newSize];
	}
	catch(std::bad_alloc& ) {
		return false;
	}
    if (!tempNewArray)
    	return false;
    for (int i = 0; i < count; ++i)
        tempNewArray[i] = array[i];

    useNewArray(newSize, tempNewArray);

	clearUnused();
    return true;
}

template <class Type>
void DArrayAsVector<Type>::setDefaultValue(const Type& v)
{   defaultValue = v;
    fHaveDefaultValue = true;
    clear();
    for (int i = 0; i < size; ++i)
		array[i] = defaultValue;
}

template <class Type>
bool DArrayAsVector<Type>::store(const Type *p, int nItems)
{   if (!tryExtend(nItems+count)) 
		return false;
    for (long i = 0; i < nItems; ++i)
        array[i+count] = p[i];
    count += nItems;
    return true;
}

template <class Type>
Type& DArrayAsVector<Type>::operator[](int i)
{   if (i >= size)
        extend(i + 1);
    if (i >= count)
        count = i+1;
    return array[i];
}

    
    
template <class Type>
bool DArrayAsVector<Type>::appendArray(const DArrayAsVector<Type>& other)
{	return store(other.c_vector(), other.getCount());
}


template <class Type>
DArrayAsVector<Type>::DArrayAsVector(const DArrayAsVector<Type>& other) :
    size(other.size),
    array(new Type[other.size]),
    count(other.count),
    fHaveDefaultValue(0)
{   if (other.fHaveDefaultValue)
        setDefaultValue(other.defaultValue);
    for (int i = 0; i < other.count; ++i)
        array[i] = other.array[i];
    count = other.count;
}

template <class Type>
DArrayAsVector<Type>& DArrayAsVector<Type>::operator=(const DArrayAsVector<Type>& other)
{   copyArray(other);
    return *this;
}

template <class Type>
bool DArrayAsVector<Type>::operator==(const DArrayAsVector<Type>& other)
{   if (count != other.count)
        return false;
    for (int i = 0; i < count; ++i)
        if (!(get(i) == other.get(i)))
            return false;
    return true;
}

template <class Type>
bool DArrayAsVector<Type>::copyArray(const DArrayAsVector<Type> &other)
{   if (this == &other)
        return true;

    if (other.fHaveDefaultValue) {
        defaultValue = other.defaultValue;
        fHaveDefaultValue = true;
        }

    clear();
    if (!tryExtend(other.count+1))
		return false;
    for (int i = 0; i < other.count; ++i)
        array[i] = other.array[i];
    count = other.count;
    return true;
}

template <class Type>
DArrayAsVector<Type>::~DArrayAsVector()
{  dealloc();
    size = 0;
    count = 0;
}

template <class Type>
int DArrayAsVector<Type>::append(const Type& v)
{   extend(count+1);
    array[count] = v;
    int ct = count;
    count++;
    return ct;
}

template <class Type>
void DArrayAsVector<Type>::terminate(const Type terminalValue)
{   extend(count+1);
    array[count] = terminalValue;
}

template <class Type>
void DArrayAsVector<Type>::remove(int i)
{   if ((i < 0) || (i >= count))
        return;
    if (i < count-1)
        array[i] = array[count-1];
    count--;
}

template <class Type>
void DArrayAsVector<Type>::removePreservingOrder(int iItem)
{   if ((iItem < 0) || (iItem >= count))
        return;
    for (int i = iItem; i < count-1; ++i) {
		array[i] = array[i+1];
		}
    count--;
}


template <class Type>
void DArrayAsVector<Type>::fill(const Type& v)
{   for (int i = 0; i < size; ++i)
        array[i] = v;
}

template <class Type>
int DArrayAsVector<Type>::find(const Type& v)
{   for (int i = 0; i < count; ++i) {
        if (array[i] == v)
            return i;
        }
    return -1;
}

template <class Type>
void DArrayAsVector<Type>::insertAt(const Type& v, int pos)
{
	int newCount;
    if (pos >= count)
        newCount = pos+1;
    else
        newCount = count + 1;

    extend(newCount);

    for (int i = count - 1; (i >= pos); --i)
        array[i+1] = array[i];
    array[pos] = v;

	count = newCount;
}

template <class Type>
void DArrayAsVector<Type>::replace(const Type& v, int pos)
{   if ((pos < 0) || (pos >= count))
        return;
    array[pos] = v;
}

template <class Type>
void DArrayAsVector<Type>::set(const Type& v, int pos)
{   if (pos < 0)
        return;
    extend(pos+1);
    if (pos >= count)
        count = pos + 1;
    array[pos] = v;
}

template <class Type>
void DArrayAsVector<Type>::pack(const Type& nullValue)
{   int dest = 0, source = 0;
    while(source < count) {
        if (!(array[source] == nullValue)) {
            if (dest != source)
                array[dest] = array[source];
            dest++;
            }
        source++;
        }
    count = dest;
}

template <class Type>
void DArrayAsVector<Type>::reverse()
{   long s = 0, e = count-1;
    Type tmp;
    while( s < e) {
        tmp = array[s];
        array[s] = array[e];
        array[e] = tmp;
        s++;
        e--;
        }
}

// Was a separate class prior to version 7.2; merged into base class due to GCC 4.0 template behavior (see below)
#define DStackAsArray DArrayAsVector

// the this-> syntax used here is required due to an obscure C++ template rule that GCC 4.0 enforces
// See: http://gcc.gnu.org/onlinedocs/gcc-4.0.2/gcc/Name-lookup.html#Name-lookup
// (This is section 10.8.2 of the GCC online documentation)
template <class Type>
class DSortedArray : public DArrayAsVector<Type> {
    public:
		DSortedArray() { }
		DSortedArray(int aSize) : DArrayAsVector<Type>(aSize) { }
        long insert(const Type& v, int fCheckUnique = 0);
        long insertUnique(const Type& v);
    };


template <class Type>
long DSortedArray<Type>::insert(const Type& v, int fCheckUnique)
{   if (!this->getCount() || (v > this->last())) {
        this->append(v);
        return this->getCount()-1;
        }
    int pos = this->findLTE(v);
    if (pos < 0)
        pos = 0;
    // convert LTE to GTE
    if (v > this->get(pos))
        pos++;
    if (fCheckUnique && (this->get(pos) == v))
        return pos;
    this->insertAt(v, pos);
    return pos;
}

template <class Type>
long DSortedArray<Type>::insertUnique(const Type& v)
{   return insert(v, true);
}

// This assumes the array is sorted
template <class Type>
int DArrayAsVector<Type>::findLTE(const Type& v) const
{   int x, y;
    y = this->getCount();
    x = 0;
    while(y > x) {
        int p = (x + y)/2;
        if (this->get(p) > v)
            y = p-1;
        else if (v > this->get(p))
            x = p+1;
        else {
            x = p;
            y = p;
            }
        }

    // find LTE position
    if (x >= this->getCount())
        return this->getCount()-1;
    int i = x;
    // Back up to the *first* item in the array that is less than or
    // equal to the search item
    while((i > 0) && !(v > this->get(i)))
        i--;
    while((i < this->getCount()-1) && (v > this->get(i)) && (!(this->get(i+1) > v)))
        i++;
    return i;
}


template <class Type>
int DArrayAsVector<Type>::findExact(const Type& v) const
{   int pos = findLTE(v);
    if ((pos >= 0) && (this->get(pos) == v))
        return pos;
    else
        return FAIL;
}

class DIntArray : public DArrayAsVector<long> {
    public:
        DIntArray()  {
                setDefaultValue(0);
                }
        void plus(long value, int iItem) {
                if (iItem >= count)
                    set(0, iItem);
                long v = getValue(iItem);
                v += value;
                replace(v, iItem);
                }
        void minus(long value, int iItem) {
                if (iItem >= count)
                    set(0, iItem);
                long v = getValue(iItem);
                v  -= value;
                replace(v, iItem);
               }
        void times(long value, int iItem) {
                if (iItem >= count)
                    set(0, iItem);
                long v = getValue(iItem);
                v  *= value;
                replace(v, iItem);
                }
#ifdef DTENGINE_IMPORTS
        void getFromIntArrayHandle(dtsIntArrayHandle hArray, int fReleaseHandle) {
			clear();
			int nItems = (int) dtssIntArrayGetLength(hArray);
			if (nItems > 0) {
				extend(nItems);
				dtssIntArrayGetItems(hArray, array, nItems);
				count = nItems;
				}
            if (fReleaseHandle)
				dtssIntArrayDelete(hArray);
           }
#endif

    };

class DBitVector {
    public:
        DBitVector(long ct) :
            count(0),
            bits(0)
            { alloc(ct);
            }
        ~DBitVector() {
			if (bits) {
				delete [] bits;
				bits = 0;
				}
            count = 0;
            }
        DBitVector& operator=(const DBitVector& other) {
            alloc(other.count);
            if (other.count)
				memmove(bits, other.bits, other.count/32+1);
            return *this;
            }
        void setBit(long iBit) {
            if (iBit < count) {
                long iArrayElement, mask;
                bitToOffsets(iBit, iArrayElement, mask);
                bits[iArrayElement] |= mask;
                }
            }
        void clearBit(long iBit) {
            if (iBit < count) {
                long iArrayElement, mask;
                bitToOffsets(iBit, iArrayElement, mask);
                bits[iArrayElement] &= (~mask);
                }
            }
        void clearAll() {
			if (bits)
				memset(bits, 0, sizeof(long) * (count/32+1));
            }
        int checkBit(long iBit) {
            if (iBit < count) {
                long iArrayElement, mask;
                bitToOffsets(iBit, iArrayElement, mask);
                return (bits[iArrayElement] & mask) ? 1 : 0;
                }
            return 0;
            }
        void alloc(long len) {
            if (len <= count)
                return;
            if (bits)
                delete[] bits;
            bits = new long[len/32+1];
            count = len;
            clearAll();
            }
		long getSize() {
			return count;
			}
    protected:
        long *bits;
        long count;
        void bitToOffsets(long iBit, long& iArrayElement, long& mask) {
            if ((iBit >= count) || (iBit < 0))
                iBit = 0;
            iArrayElement = iBit / 32;
            long whichBit = iBit % 32;
            if (whichBit == 0)
                mask = 1;
            else if (whichBit == 1)
                mask = 2;
            else
                mask = 2L << (whichBit-1);
            }
    };



#ifdef USE_DTSEARCH_NAMESPACE
}  // dtSearch namespace
#endif


#endif
