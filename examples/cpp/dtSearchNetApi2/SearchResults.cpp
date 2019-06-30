#include "stdafx.h"

#include "SearchResults.h"
#include "SearchReportJob.h"
#include "Utils.h"

using namespace System;
using namespace System::Collections;
using namespace System::Collections::Specialized;
using namespace System::Text;


namespace dtSearch { namespace Engine 
{
	// Constructor
	SearchResults::SearchResults ()
	{
		// Ensures that ApiInitializer loads
		INIT

		m_results = new DSearchResults ();
		CountCreate(otSearchResults);
	}

	SearchResults::SearchResults (DSearchResults* results, bool bAttach)
	{	if (results) {
			if (bAttach)
				m_results = results;
			else
				m_results = new DSearchResults (*results);
			}
		else
			m_results = 0;
		CountCreate(otSearchResults);
	}

	// Destructor 
	SearchResults::~SearchResults()
	{
		CountDelete(otSearchResults);
	}

	void SearchResults::AddDoc (String^ indexPath, long docId, array<int> ^hits)
	{
		// Better to pin or make a copy? 
		DArrayAsVector<long> vec;
		Converters::GetNetArray(&vec, hits); 

		DStringProperty sIndexPath;
		sIndexPath.set(indexPath);
		m_results->addItem(sIndexPath, docId, vec.getCount(), vec.c_vector() );
	}

	void SearchResults::AddDocs (String^ indexPath, array<long> ^docIds)
	{
		// Better to pin or make a copy? 
		DArrayAsVector<long> vec;
		Converters::GetNetArrayL(&vec, docIds); 

		DStringProperty sIndexPath;
		sIndexPath.set(indexPath);
		m_results->addItems(sIndexPath, vec.getCount(), vec.c_vector() );
	}

	void SearchResults::SetSortKey (String^ newKey)
	{
		DStringProperty sKey;
		sKey.set(newKey);
		m_results->setSortKey(m_results->getSelectedDoc(), sKey);
	}

	void SearchResults::SerializeFromXml (String^ val)
	{
		DStringProperty sXml;
		sXml.set(val);
		m_results->serializeFromXml(sXml);
	}

	String^ SearchResults::SerializeAsXml ()
	{
		DString sXml;
		dtsStringHandle hStr = m_results->serializeAsXml(-1);
		if (hStr && sXml.getFromStringHandle(hStr, true)) {
			String^ ret = sXml.allocNetString();
			if (ret != nullptr)
				return ret;
			}
		
		System::OutOfMemoryException ^ex = gcnew System::OutOfMemoryException("Unable to allocate memory for serialized search results");
		throw ex;
		return nullptr;
	}

	void SearchResults::UrlDecodeItem (String^ val)
	{
		DStringProperty sUrl;
		sUrl.set(val);
		debugLog("URL: ", sUrl);
		m_results->urlDecode(sUrl);
	}

	String^ SearchResults::UrlEncodeItem ()
	{
		DString destStr;
		destStr.extend(1024);
	
		int i = m_results->getSelectedDoc();
		if (i >= 0) 
		{
			m_results->urlEncode(i, destStr.str(), destStr.getMaxLength());
			destStr.updateLength();
		}

		return destStr.allocNetString();
	}

	String^ SearchResults::MakePdfWebHighlightFile ()
	{
		DString temp;
		m_results->MakePdfWebHighlightFile(temp);
		return temp.allocNetString();
	}

	void SearchResults::MakePdfHighlightFile ()
	{
		m_results->makePdfHighlightFile (m_results->getSelectedDoc());
	}

	SearchReportJob^ SearchResults::NewSearchReportJob ()
	{
		SearchReportJob ^srj = gcnew SearchReportJob ();
		srj->SetResults (this);
		return srj;
	}

	void SearchResults::Sort (SortFlags flags, String^ fieldName)
	{
		DStringProperty sFieldName;
		sFieldName.set(fieldName);
		m_results->sort((int) flags, sFieldName);
	}

	bool SearchResults::GetNthDoc (long iDoc)
	{	if (iDoc != m_results->getSelectedDoc())
			m_currentItem = nullptr;
		return (m_results->GetNthDoc (iDoc) < 0) ? false : true;
	}
	

	String^ SearchResults::GetHitByteOffsets ()
	{
		DWString output;
		dtsSearchNotificationItem currDoc;
		m_results->getDocInfo(m_results->getSelectedDoc(), currDoc);

		dtsMapHitsJob job(currDoc);
		short result = 0;
		dtssMapHitsInFile(job, &currDoc, result);
		for (int i = 0; i < job.nTableEntries; ++i) {
			dtsHitLocation& loc = job.table[i];
			output << loc.wordOffset << ' '
				<< loc.byteOffset << ' '
				<< loc.page << ' '
				<< loc.paragraph;
			if (i < job.nTableEntries-1)
				output << "; ";
			}
		String^ outStr = gcnew String (output.str());
		return outStr;
	}


	
	void SearchResults::DisposeUnmanagedItems()
	{	debugLog("SearchResults::DisposeUnmangedItems");
		deleteResults();
	}

	void SearchResults::IntersectionOf(SearchResults ^a, SearchResults ^b)
	{	LogFunction("SearchResults::IntersectionOf");
		if (a && b) {
			dtsSearchResults *resA = a->getImplementation();
			dtsSearchResults *resB = b->getImplementation();
			debugLog("Items in A: ", resA->getCount());
			debugLog("Items in B: ", resB->getCount());
			if (resA && resB) {
				m_results->intersectionOf(*resA, *resB);
				debugLog("Result:  ", m_results->getCount());
				}
			else
				debugLog("Ignored, invalid input (one or more arguments is null)");
			}
		else
			Empty();
	}
	
	void SearchResults::UnionWith(SearchResults ^other)
	{	if (other) {
			dtsSearchResults *res = other->getImplementation();
			if (res) 
				m_results->unionWith(*res);
			else
				debugLog("Ignored, other results are null");
			}
	
	}

}}
