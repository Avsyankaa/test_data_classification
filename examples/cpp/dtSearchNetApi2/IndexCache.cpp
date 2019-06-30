#include "stdafx.h"
#include "IndexCache.h"
#include <dtsIndexCache.h>

using namespace System;

namespace dtSearch { namespace Engine 
{
	IndexCache::IndexCache(int aMaxCount) :
		m_inUseCount(0),
		m_hitCount(0),
		m_openIndexCount(0),
		m_autoCloseTime(0),
		m_autoReopenTime(0) {
		CountCreate(otIndexCache);
		pCache = new dtsIndexCache(aMaxCount, 0);
		m_maxCount = aMaxCount;
		}
	
	IndexCache::~IndexCache() {
		DisposeUnmanagedItems();
		}
		
	void IndexCache::DisposeUnmanagedItems()
	{	if (pCache) {
			delete pCache;
			pCache = 0;
			}
	}

	void IndexCache::updateOptions() {
		dtsIndexCacheOptions opts;
		opts.autoCloseTime = m_autoCloseTime;
		opts.autoReopenTime = m_autoReopenTime;
		opts.maxCount = m_maxCount;
		pCache->setOptions(opts);
		}
	void IndexCache::updateStatus() {
		dtsIndexCacheStatus status;
		pCache->getStatus(status);
		m_inUseCount = status.inUseCount;
		m_hitCount = status.cacheHitCount;
		m_openIndexCount = status.openIndexCount;
		}
	bool IndexCache::CloseIndex(String ^indexPath) {
		DStringProperty sPath;
		sPath.set(indexPath);
		return pCache->closeIndex(sPath);
		}
		
	String ^IndexCache::GetOpenIndexes() {
		DString s;
		dtsStringHandle hres = pCache->getOpenIndexes();
		s.getFromStringHandle(hres, true);
		return s.allocNetString();
		}
		
} } 
