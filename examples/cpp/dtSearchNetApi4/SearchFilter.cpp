#include "stdafx.h"

#include "SearchFilter.h"

using namespace System;

namespace dtSearch { namespace Engine
{
	SearchFilter::SearchFilter ()
	{	CountCreate(otSearchFilter);
		m_searchFilter = new dtsSearchFilter ();
	}

	SearchFilter::SearchFilter (dtsSearchFilter *filter, bool bShouldCopy)
	{	CountCreate(otSearchFilter);
		if (filter) {
			if (bShouldCopy)
				m_searchFilter = new dtsSearchFilter (*filter);
			else 
				// Take ownership of the filter passed in
				m_searchFilter = filter;
			}
		else
			m_searchFilter = nullptr;
	}

	SearchFilter::~SearchFilter ()
	{	CountDelete(otSearchFilter);
		DisposeUnmanagedItems();
	}

	long SearchFilter::AddIndex (String^ indexPath)
	{
		DStringProperty temp;
		temp.set(indexPath);
		long index = m_searchFilter->addIndex (temp.str());
		return index;
	}

	void SearchFilter::SelectAll (long whichIndex)
	{
		m_searchFilter->selectAll (whichIndex);
	}

	void SearchFilter::SelectNone (long whichIndex)
	{
		m_searchFilter->selectNone (whichIndex);
	}

	void SearchFilter::SelectItems (long whichIndex, long firstItem, long lastItem, bool isSelected)
	{
		m_searchFilter->selectItems (whichIndex, firstItem, lastItem, isSelected);
	}

	void SearchFilter::SelectItems (long whichIndex, array<int> ^docIds, bool isSelected)
	{
		DArrayAsVector<unsigned long> ids;
		Converters::GetNetArray(&ids, docIds);
		m_searchFilter->setSelections(whichIndex, ids.c_vector(), ids.getCount(), isSelected);
	
	}

	long SearchFilter::SelectItemsBySearch (long whichIndex, String^ searchRequest, bool isSelected)
	{
		DSearchJob sj;
		sj.Request.set(searchRequest);
		sj.SetDelayDocInfo(true);
		int intIsSelected = isSelected;
		m_searchFilter->selectItemsBySearch(whichIndex, sj.GetSearchJob(), intIsSelected);

		if (sj.Failed())
			return -1;
		else
			return sj.GetSearchJob().totalFiles;
	}

	void SearchFilter::And (SearchFilter^ other)
	{
		m_searchFilter->andWith (*other->m_searchFilter);
	}

	void SearchFilter::AndNot (SearchFilter^ other)
	{
		m_searchFilter->andNot (*other->m_searchFilter);
	}


	void SearchFilter::Or (SearchFilter^ other)
	{
		m_searchFilter->orWith (*other->m_searchFilter);
	}

	bool SearchFilter::Equals (SearchFilter^ other)
	{
		if (m_searchFilter->equals (*other->m_searchFilter))
				return true;
		return false;
	}

	// True on success
	bool SearchFilter::Read (String^ filename)
	{
		DStringProperty temp;
		temp.set(filename);
		long result = m_searchFilter->read (temp.str());
		return (result == SUCCESS);
	}

	// True on success
	bool SearchFilter::Write (String^ filename)
	{
		DStringProperty temp;
		temp.set(filename);
		long result = m_searchFilter->write (temp.str());
		return (result == SUCCESS);
	}

	long SearchFilter::ReadMultiple (String^ filtersToRead)
	{
		DStringSetProperty filtersToReadSet;
	    filtersToReadSet.tokenize(filtersToRead);

		int ret = 0;
		for (int i = 0; i < filtersToReadSet.getCount(); ++i)
		{
	        const char *fn = filtersToReadSet.getString(i);
		    int v = FAIL;
			if (i == 0)
				v = m_searchFilter->read(fn);
			else
			{
				dtsSearchFilter *sf = new dtsSearchFilter ();
				v = sf->read(filtersToReadSet.getString(i));
				if (v != FAIL)
					m_searchFilter->orWith (*sf);
				delete sf;
	        }

			if (v != FAIL)
				ret++;
		}

		return ret;
	}
	
	array<long>^ SearchFilter::GetItems(long whichIndex) {
		return getIndexSelections(whichIndex);
	}


	array<long>^ SearchFilter::getIndexSelections (long whichIndex)
	{
		// Get items -- returns a handle to the array
		dtsIntArrayHandle arrHandle = m_searchFilter->getItems (whichIndex);

		// Convert the handle to an unmanaged array of long
		long arrLength = dtssIntArrayGetLength (arrHandle);
		if (arrLength < 0)
			return nullptr;

		// Copy unmanaged array to managed array
		array<long>^ managedArr = gcnew array<long>(arrLength);
		const int bufSize = 1000;
		long items[bufSize];
		for (long i = 0; i < arrLength; i += bufSize)
		{
			int ct = dtssIntArrayGetItemRange(arrHandle, i, min(i+bufSize-1, arrLength), items);
			ct = min(ct, arrLength-i);
			ct = min(ct, bufSize);
			for (int j = 0; j < ct; ++j) {
				managedArr [i+j] = items[j];
				}
		}
		dtssIntArrayDelete (arrHandle);

		return managedArr;
	}

	String^ SearchFilter::getIndexPath (long whichIndex)
	{
		dtsStringHandle stringHandle = m_searchFilter->getIndexPath (whichIndex);
		DString path;
		path.getFromStringHandle (stringHandle, TRUE);
		String^ result = path.allocNetString();
		return result;
	}

	dtsSearchFilter* SearchFilter::getImplementation ()
	{
		return m_searchFilter;
	}
	
	void SearchFilter::DisposeUnmanagedItems() {
		if (m_searchFilter) {
			dtsSearchFilter *toDelete = m_searchFilter;
			m_searchFilter = 0;
			delete toDelete;
			}
		}

}}
