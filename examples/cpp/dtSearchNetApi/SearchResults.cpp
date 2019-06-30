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
		Dispose(false);
		CountDelete(otSearchResults);
	}

	void SearchResults::AddDoc (String* indexPath, long docId, long hits __gc[])
	{
		// Better to _pin or make a copy? 
		DArrayAsVector<long> array;
		Converters::GetNetArray(&array, hits); 

		DStringProperty sIndexPath;
		sIndexPath.set(indexPath);
		m_results->addItem(sIndexPath, docId, hits->Count, array.c_vector() );
	}

	void SearchResults::SetSortKey (String* newKey)
	{
		DStringProperty sKey;
		sKey.set(newKey);
		m_results->setSortKey(m_results->getSelectedDoc(), sKey);
	}

	void SearchResults::SerializeFromXml (String* val)
	{
		DStringProperty sXml;
		sXml.set(val);
		m_results->serializeFromXml(sXml);
	}

	String* SearchResults::SerializeAsXml ()
	{
		DString sXml;
		dtsStringHandle hStr = m_results->serializeAsXml(-1);
		sXml.getFromStringHandle(hStr, true);
		return sXml.allocNetString();
	}

	void SearchResults::UrlDecodeItem (String* val)
	{
		DStringProperty sUrl;
		sUrl.set(val);
		debugLog("URL: ", sUrl);
		m_results->urlDecode(sUrl);
	}

	String* SearchResults::UrlEncodeItem ()
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

	String* SearchResults::MakePdfWebHighlightFile ()
	{
		DString temp;
		m_results->MakePdfWebHighlightFile(temp);
		return temp.allocNetString();
	}

	void SearchResults::MakePdfHighlightFile ()
	{
		m_results->makePdfHighlightFile (m_results->getSelectedDoc());
	}

	SearchReportJob* SearchResults::NewSearchReportJob ()
	{
		SearchReportJob *srj = new SearchReportJob ();
		srj->SetResults (this);
		return srj;
	}

	void SearchResults::Sort (SortFlags flags, String* fieldName)
	{
		DStringProperty sFieldName;
		sFieldName.set(fieldName);
		m_results->sort(flags, sFieldName);
	}

	bool SearchResults::GetNthDoc (long iDoc)
	{	if (iDoc != m_results->getSelectedDoc())
			m_currentItem = NULL;
		return (m_results->GetNthDoc (iDoc) < 0) ? false : true;
	}
	
	SearchResultsItem *SearchResults::get_CurrentItem() {
		if (m_currentItem == NULL)
			m_currentItem = Converters::MakeResultsItem(&m_results->DocInfo);
		return m_currentItem;
		}


	String* SearchResults::get_DocDetailItem (String* name)
	{
		DStringProperty toGet;
		toGet.set(name);
		DString result;
		m_results->getDocInfoItem(toGet, result);

		return result.allocNetString();
	}

	String* SearchResults::GetHitByteOffsets ()
	{
		DWString* output = new DWString ();
		dtsSearchNotificationItem currDoc;
		m_results->getDocInfo(m_results->getSelectedDoc(), currDoc);

		dtsMapHitsJob job(currDoc);
		short result = 0;
		dtssMapHitsInFile(job, &currDoc, result);
		for (int i = 0; i < job.nTableEntries; ++i) {
			dtsHitLocation& loc = job.table[i];
			*output << loc.wordOffset << ' '
				<< loc.byteOffset << ' '
				<< loc.page << ' '
				<< loc.paragraph;
			if (i < job.nTableEntries-1)
				*output << "; ";
			}

		String* outStr = new String (output->str());
		delete output;
		return outStr;
	}

	long SearchResults::get_TotalHitCount ()
	{
		return m_results->getTotalHits ();
	}

	String* SearchResults::get_Request ()
	{
		return m_results->Request.allocNetString();
	}

	int SearchResults::get_DocHits () __gc[]
	{
		return Converters::MakeNetArray(m_results->GetDocHits (), m_results->DocHitCount);
	}

	String* SearchResults::get_DocDetails ()
	{	
		return m_results->getDocInfoString().allocNetString();
	}

	long SearchResults::get_DocHitCount ()
	{
		return m_results->DocHitCount;
	}

	String* SearchResults::get_DocName ()
	{
		return m_results->DocName.allocNetString();
	}

	long SearchResults::get_Count ()
	{
		return m_results->getCount();
	}
	
	void SearchResults::DisposeUnmanagedItems()
	{	debugLog("SearchResults::DisposeUnmangedItems");
		deleteResults();
	}

	void SearchResults::IntersectionOf(SearchResults *a, SearchResults *b)
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
	
	void SearchResults::UnionWith(SearchResults *other)
	{	if (other) {
			dtsSearchResults *res = other->getImplementation();
			if (res) 
				m_results->unionWith(*res);
			else
				debugLog("Ignored, other results are null");
			}
	
	}

}}
