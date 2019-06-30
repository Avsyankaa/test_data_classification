#pragma once

#include "stdafx.h"
#include "Macros.h"
#include "Utils.h"
#include "Constants.h"
#include "NetJobBase.h"

using namespace System;
using namespace System::Collections;
using namespace System::Runtime::InteropServices;
using namespace System::Threading;

#include <dtsIndexCache.h>

#pragma managed

namespace dtSearch { namespace Engine
{

	/// <summary>
	/// An index cache keeps a pool of indexes open for faster
	/// searching
	/// </summary>
	/// <remarks>
	/// An index cache can make searching substantially faster when a
	/// series of searches must be executed against a small number of
	/// indexes. The index cache maintains a pool of open indexes
	/// that will be available for searching on any thread. This
	/// eliminates the need to open and close the indexes being
	/// searched for every search request.
	/// * How to use an index cache *
	///   1. Create an IndexCache object; and
	///   2. Call SearchJob.SetIndexCache() to assign the cache to a
	///      SearchJob.
	/// * Cache Lifetime *
	/// An IndexCache uses unmanaged resources (the implementation of
	/// the index cache inside the dtSearch Engine) and therefore
	/// implements the IDisposable pattern. Either a using{} clause
	/// \or a call to Dispose() is needed to ensure that these
	/// resources will be freed when the cache is no longer needed.
	///
	/// An application should generally create a single index cache
	/// before executing its first search, and should delete the
	/// index cache only after the last search is done and the last
	/// search results object has been deleted.
	///
	/// Deleting an IndexCache object while a search is executing can
	/// cause a crash or a memory leak. Additionally, a search
	/// results object inherits the cache used in a search, so all
	/// search results objects must also be deleted before deleting
	/// the index cache.
	/// * Index Lifetime *
	/// To prevent indexes from remaining in the cache indefinitely,
	/// use AutoCloseTime to specify a maximum number of seconds
	/// before unused indexes are automatically closed. Another
	/// setting, AutoReopenTime, can be used to ensure that even
	/// heavily-used indexes are periodically closed and reopened to
	/// refresh their view of the index contents.
	///
	/// The cache does not have its own thread to implement
	/// AutoCloseTime or AutoReopenTime, so it only checks
	/// AutoCloseTime and AutoReopenTime when the cache is accessed
	/// for any reason, including a calls to read the OpenIndexCount
	/// and InUseCount properties.
	/// * Threading *
	/// Index caches are thread-safe, so any number of concurrent
	/// searches can share a single IndexCache.
	///
	/// The same index may be open any number of times in an index
	/// cache. For example, if you have three concurrent threads
	/// searching c:\\SampleIndex, and if the cache size is set to
	/// three indexes, then the cache will contain three open indexes
	/// all referencing c:\\SampleIndex.
	/// * Performance *
	/// A search job will try to obtain each index to search from the
	/// cache. If an open index is available, the search job will use
	/// it for the duration of the search and return it to the cache
	/// when the search is done.
	///
	/// If an index cache does not have any available indexes for a
	/// search, the search will create its own private open index to
	/// process the search, just as it would have done if there was
	/// no index cache. Therefore, attaching an index cache to a
	/// search job will not cause the search to fail due to an
	/// insufficient cache size, nor can it make searching slower
	/// than it would be without a cache.
	/// </remarks>
	public ref class IndexCache : public DisposableObject
	{
	public:

		// Create an index cache
		IndexCache(int aSize);

		~IndexCache();

	/// <summary>
	/// Maximum number of indexes to keep in the cache
	/// </summary>
	/// <remarks>
	/// The maximum number indexes to keep in the cache cannot be
	/// changed unless the cache does not contain any indexes that
	/// are in use (being used to for a search in progress). If any
	/// indexes are currently in use, an attempt to change maxCount
	/// will be ignored.
	///
	/// The actual number of open indexes in a cache will depend on
	/// the level of activity. For example, if the cache has maxCount
	/// set to 10 but only one index is being searched on a single
	/// thread, the cache will never contain more than one index. On
	/// the other hand, if the a single index is being searched on
	/// three threads, the cache may have up to three instances of
	/// that index.
	/// </remarks>
		property int MaxCount {
			int get() {
				return m_maxCount;
				}
			}
	/// <summary>
	/// Number of seconds an index can be left open before it is
	/// automatically closed and reopened.
	/// </summary>
	/// <remarks>
	/// When an index is opened, its view of the index contents is
	/// not changed by subsequent index updates. To ensure that
	/// indexes in the cache do not get out of date, use
	/// autoReopenTime to specify a maximum number of seconds before
	/// an open index is closed and reopened.
	///
	/// On each access to the cache, the last-opened time for each
	/// index is compared to autoReopenTime and, if the index has not
	/// been opened for a sufficient period of time, the index is
	/// closed and reopened.
	///
	/// The cache does not have its own thread to implement
	/// autoReopenTime, so it only checks autoReopenTime when the
	/// cache is accessed for any reason.
	///
	/// Set autoReopenTime to -1 to have dtSearch check the 
	/// modification date of the index on every search to see if the 
	/// index should be closed and reopened reflect a recent update.
	/// </remarks>

		property int AutoReopenTime {
			int get() {
				return m_autoReopenTime;
				}
			void set(int newVal) {
				m_autoReopenTime = newVal;
				updateOptions();
				}
			}



	/// <summary>
	/// Number of seconds before an unused index can be closed
	/// automatically.
	/// </summary>
	/// <remarks>
	/// Use autoCloseTime to have the cache automatically shrink when
	/// idle. On each access to the cache, the last-used time for
	/// each index is compared to autoCloseTime and, if the index has
	/// not been used for a sufficient period of time, the index is
	/// closed and removed from the cache.
	///
	/// The cache does not have its own thread to implement
	/// autoCloseTime, so it only checks autoCloseTime when the cache
	/// is accessed for any reason, including a call to
	/// dtsIndexCache.GetStatus().
	/// </remarks>

		property int AutoCloseTime {
			int get() {
				return m_autoCloseTime;
				}
			void set(int newVal) {
				m_autoCloseTime = newVal;
				updateOptions();
				}
			}

		/// Number of open indexes in the cache
		property int OpenIndexCount {
			int get() {
				updateStatus();
				return m_openIndexCount;
				}
			}

		/// Number of open indexes that are currently in use
		property int InUseCount {
			int get() {
				updateStatus();
				return m_inUseCount;
				}
			}
			
		/// Close an index specified by filename or filename pattern
		bool CloseIndex(String ^indexPath);
		
		// Returns a list of the open indexes in a string.
		// Index paths will be quoted and separated with spaces
		String^ GetOpenIndexes();

		/// For internal use.
		dtsIndexCache* getImplementation () {
			return pCache;
			}

	protected:


		virtual void DisposeUnmanagedItems() override;
		int m_maxCount;
		int m_autoReopenTime;
		int m_autoCloseTime;
		int m_inUseCount;
		int m_openIndexCount;
		dtsIndexCache *pCache;

		void updateOptions();
		void updateStatus();


	};
}
}
