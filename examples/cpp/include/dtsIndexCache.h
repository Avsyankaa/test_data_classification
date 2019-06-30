#ifndef dtsIndexCache_H
	#define dtsIndexCache_H

#ifdef USE_DTSEARCH_NAMESPACE

namespace dtSearch {

#endif

#include <dt_util.h>

// Options to control the behavior of an index cache (dtsIndexCache)
struct dtsIndexCacheOptions {
	// dtsIndexCacheOptions constructor
	dtsIndexCacheOptions() {
		initStruct<dtsIndexCacheOptions>(*this);
		}
	dtsIndexCacheOptions& operator=(const dtsIndexCacheOptions& other) {
		copy(other);
		return *this;
		}
	void copy(const dtsIndexCacheOptions& other) {
		copyStruct<dtsIndexCacheOptions>(*this, other);
		}
	// The constructor will initialize to sizeof(dtsIndexCacheOptions)
	int structSize;

	/* Summary
	   Maximum number of indexes to keep in the cache
	   Remarks
	   The maximum number indexes to keep in the cache cannot be changed unless the
	   cache does not contain any indexes that are in use (being used to for a search in
	   progress). If any indexes are currently in use, an attempt to change maxCount
	   will be ignored.

	   The actual number of open indexes in a cache will depend on the level of
	   activity. For example, if the cache has maxCount set to 10 but only one index is
	   being searched on a single thread, the cache will never contain more than one
	   index. On the other hand, if the a single index is being searched on three
	   threads, the cache may have up to three instances of that index.
	                                                                                     */
	int maxCount;

	/* Summary
	   Number of seconds an index can be left open before it is automatically closed and
	   reopened.
	   Remarks
	   When an index is opened, its view of the index contents is not changed by
	   subsequent index updates. To ensure that indexes in the cache do not get out of
	   date, use autoReopenTime to specify a maximum number of seconds before an open
	   index is closed and reopened.
	   
	   On each access to the cache, the last-opened time for each index is compared to
	   autoReopenTime and, if the index has not been opened for a sufficient period of
	   time, the index is closed and reopened.

	   The cache does not have its own thread to implement autoReopenTime, so it only
	   checks autoReopenTime when the cache is accessed for any reason, including a call
	   to dtsIndexCache.GetStatus().                                                     

	   Set autoReopenTime to -1 to have dtSearch check the modification date
	   of the index on every search to see if the index should be closed and
	   reopened reflect a recent update.
	   */
	int autoReopenTime;

	/* Summary
	   Number of seconds before an unused index can be closed automatically.
	   Remarks
	   Use autoCloseTime to have the cache automatically shrink when idle. On each
	   access to the cache, the last-used time for each index is compared to
	   autoCloseTime and, if the index has not been used for a sufficient period of
	   time, the index is closed and removed from the cache.

	   The cache does not have its own thread to implement autoCloseTime, so it only
	   checks autoCloseTime when the cache is accessed for any reason, including a call
	   to dtsIndexCache.GetStatus().                                                    */
	int autoCloseTime;

	// begin-internal
	// reserved
	int reserved[8];
	int flags;
	// end-internal
	};

/* Status of an index cache (dtsIndexCache), obtained from
   dtsIndexCache.getStatus().                              */
struct dtsIndexCacheStatus {
	// dtsIndexCacheStatus constructor
	dtsIndexCacheStatus() {
		initStruct<dtsIndexCacheStatus>(*this);
		}
	dtsIndexCacheStatus& operator=(const dtsIndexCacheStatus& other) {
		copy(other);
		return *this;
		}
	void copy(const dtsIndexCacheStatus& other) {
		copyStruct<dtsIndexCacheStatus>(*this, other);
		}

	// The constructor will initialize to sizeof(dtsIndexCacheStatus)
	int structSize;
	// Number of open indexes
	int openIndexCount;
	// Number of open indexes that are currently in use
	int inUseCount;
	// Number of times cache returned an open index when requested
	int cacheHitCount;
	// begin-internal
	int reserved[7];
	int reservedICSParam1;
	// end-internal
	};

// begin-internal
extern "C" {
    dtsIndexCacheHandle DLLFUNC dtsIndexCache_Alloc(int maxCount, int flags);
    void DLLFUNC dtsIndexCache_Free(dtsIndexCacheHandle h);
    void DLLFUNC dtsIndexCache_SetOptions(dtsIndexCacheHandle h, dtsIndexCacheOptions& opts);
    void DLLFUNC dtsIndexCache_GetStatus(dtsIndexCacheHandle h, dtsIndexCacheStatus& status);
    void DLLFUNC dtsIndexCache_GetOptions(dtsIndexCacheHandle h, dtsIndexCacheOptions& options);
    BOOL DLLFUNC dtsIndexCache_CloseIndex(dtsIndexCacheHandle h, const char *filenamePattern);
    dtsStringHandle DLLFUNC dtsIndexCache_GetOpenIndexes(dtsIndexCacheHandle h);
};
// end-internal

/* Summary
   An index cache keeps a pool of indexes open for faster searching
   Remarks
   An index cache can make searching substantially faster when a series of searches
   must be executed against a small number of indexes. The index cache maintains a
   pool of open indexes that will be available for searching on any thread. This
   eliminates the need to open and close the indexes being searched for every search
   request.
   * How to use an index cache *
     1. Create a dtsIndexCache object (you will need to #include \<dtsIndexCache.h\>);
     2. Use dtsIndexCache::getHandle() to get a handle to the cache, and assign this
        handle to dtsSearchJob.indexCacheHandle; and
     3. Set up the dtsSearchJob and execute the search.
   * Cache Lifetime *
   An application should generally create a single index cache before executing its
   first search, and should delete the index cache only after the last search is
   done and the last search results object has been deleted.

   Deleting a dtsIndexCache object while a search is executing can cause a crash or
   a memory leak. Additionally, a search results object inherits the cache used in a
   search, so all search results objects must also be deleted before deleting the
   index cache.
   * Index Lifetime *
   To prevent indexes from remaining in the cache indefinitely, use
   dtsIndexCacheOptions.autoCloseTime to specify a maximum number of seconds before
   unused indexes are automatically closed. Another setting,
   dtsIndexCacheOptions.autoReopenTime, can be used to ensure that even heavily-used
   indexes are periodically closed and reopened to refresh their view of the index
   contents.

   The cache does not have its own thread to implement autoCloseTime or
   autoReopenTime, so it only checks autoCloseTime and autoReopenTime when the cache
   is accessed for any reason, including a call to dtsIndexCache.GetStatus().
   * Threading *
   Index caches are thread-safe, so any number of concurrent searches can share a
   single dtsIndexCache.

   The same index may be open any number of times in an index cache. For example, if
   you have three concurrent threads searching c:\\SampleIndex, and if the cache
   size is set to three indexes, then the cache will contain three open indexes all
   referencing c:\\SampleIndex.
   * Performance *
   A search job will try to obtain each index to search from the cache. If an open
   index is available, the search job will use it for the duration of the search and
   return it to the cache when the search is done.

   If an index cache does not have any available indexes for a search, the search
   will create its own private open index to process the search, just as it would
   have done if there was no index cache. Therefore, attaching an index cache to a
   search job will not cause the search to fail due to an insufficient cache size,
   nor can it make searching slower than it would be without a cache.                  */
class dtsIndexCache {
	public:
		// Create an index cache
		dtsIndexCache(int maxCount, int flags = 0) {
			init();
			allocCache(maxCount, flags);
			}
		dtsIndexCache() {
			init();
			allocCache(3, 0);
			}
		~dtsIndexCache() {
			freeCache();
			}
		// Get the current state of the index cache
		void getStatus(dtsIndexCacheStatus& status) {
			if (hCache)
				dtsIndexCache_GetStatus(hCache, status);
			}
		// Get options for this index cache
		void getOptions(dtsIndexCacheOptions& options) {
			if (hCache)
				dtsIndexCache_GetOptions(hCache, options);
			}
		// Set options for this index cache
		void setOptions(dtsIndexCacheOptions& options) {
			if (hCache)
				dtsIndexCache_SetOptions(hCache, options);
			}
		// Returns true if one or more indexes in this cache are currently in use
		bool inUse() {
			dtsIndexCacheStatus status;
			getStatus(status);
			return status.inUseCount > 0;
			}
		// Returns a handle to the cache, which can be assigned to dtsSearchJob.indexCacheHandle
		dtsIndexCacheHandle getHandle() const {
			return hCache;
			}

		operator dtsIndexCacheHandle() const {
			return hCache;
			}
		/* Summary
		   Close all indexes in a cache that match a filename or filename pattern including
		   wildcards
		   Parameters
		   filenamePattern :  Filename or filename pattern to match. The filename pattern may
		                      include the wildcards * and ?. The match is case\-insensitive.
		   
		   Returns
		   \Returns true if either (a) the index was not present in the cache, or (b) all
		   instances of the index in the cache could be closed
		   Remarks
		   CloseIndex() will attempt to close all instances of an index in the cache. If an
		   instance is currently being used in a search, the attempt will fail and
		   closeIndex() will return false. After closeIndex() returns, the index may be
		   reopened if a search that uses the cache references the index. Therefore, before
		   calling closeIndex(), an application should ensure that the code that initiates
		   searches will not cause the index to be reopened.                                  */
		bool closeIndex(const char *filenamePattern) {
			if (hCache) 
				return dtsIndexCache_CloseIndex(hCache, filenamePattern)? true : false;
			else
				return false;
			}
		// Returns a list of the indexes that are open in the cache.
		// The index paths will be quoted and separated by spaces.
		dtsStringHandle getOpenIndexes() {
			if (hCache) 
				return dtsIndexCache_GetOpenIndexes(hCache);
			return 0;
			}
			
		// begin-internal
	protected:
		void init() {
			bOwnsHandle = false;
			hCache = 0;
			}
		void allocCache(int aMaxCount, int aFlags) {
			freeCache();

			hCache = dtsIndexCache_Alloc(aMaxCount, aFlags);
			if (hCache) {
				dtsIndexCacheOptions options;
				options.maxCount = aMaxCount;
				options.flags = aFlags;
				setOptions(options);
				bOwnsHandle = true;
				}
			}
		void freeCache() {
			if (hCache) {
				if (bOwnsHandle)
					dtsIndexCache_Free(hCache);
				hCache = 0;
				bOwnsHandle = 0;
				}
			}
		bool bOwnsHandle;
		dtsIndexCacheHandle hCache;
		// end-internal
	};

#ifdef USE_DTSEARCH_NAMESPACE
};  // namespace dtSearch
#endif

#endif // !defined(dtsIndexCache_H)

