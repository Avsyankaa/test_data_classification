#include "stdafx.h"
#include "utils.h"
#include "server.h"
#include "IndexJob.h"
#include "DataSource.h"
#include <ApiIndexJob.h>


using namespace System;
using namespace System::Collections;
using namespace System::Collections::Specialized;
using namespace System::Text;
using namespace dtSearch::Engine;

private __gc class InternalIndexJobUtil {
	public:
		static void CopyInfo(IndexProgressInfo *info, dtsMergeStatusInfo& source) {
				info->Step = dtSearch::Engine::IndexingStep::ixStepMergingIndexes;
				info->PercentDone = source.percentDone;
				}
		static void CopyInfo(IndexProgressInfo *info, dtsVerifyStatusInfo& source) {
				info->Step = dtSearch::Engine::IndexingStep::ixStepVerifyingIndex;
				info->PercentDone = source.percentDone;
				}
		static void CopyInfo(IndexProgressInfo *info,
				dtsIndexProgressInfo& source) {
				IndexFileInfo *fi = info->File;
				if (fi) {
					fi->BytesRead = source.file.bytesRead;
					fi->DisplayName = Converters::MakeNetString(source.file.displayName);
					fi->DocId = source.file.docId;
					fi->Fullname = Converters::MakeNetString(source.file.fullname);
					fi->Location = Converters::MakeNetString(source.file.location);
					fi->Name = Converters::MakeNetString(source.file.name);
					fi->OpenFailMessage = Converters::MakeNetString(source.file.openFailMessage);
					fi->PercentDone = source.file.percentDone;
					fi->Size = source.file.size;
					fi->Type = Converters::MakeNetString(source.file.type);
					fi->TypeId = (dtSearch::Engine::TypeId) source.file.typeId;
					fi->WordCount = source.file.wordCount;
					}
				info->BinaryCount = source.binaryCount;
				info->BytesReadKB = source.bytesReadKB;

				info->BytesToIndexKB = source.bytesToIndexKB;
				info->DocBytesReadKB = source.docBytesReadKB;
				info->DocsInIndex = source.docsInIndex;
				info->EncryptedCount = source.encryptedCount;
				info->IndexPath = Converters::MakeNetString(source.indexPath);
				info->IndexPercentFull = source.indexPercentFull;
				info->OpenFailures = source.openFailures;
				info->PercentDone = source.getPercentDone();
				info->Step = (dtSearch::Engine::IndexingStep) source.step;
				info->UpdateType = (dtSearch::Engine::MessageCode) source.updateType;
				info->WordsInIndex = source.wordsInIndex;
				}
	};

class CNetDataSource : public DDataSourceBase {
    public:
        CNetDataSource();
        void attach(gcroot<DataSource*> ds);
        virtual int getNextDoc(dtsInputStream& dest);
        virtual int rewind();
    protected:
        bool m_fDone;
        bool m_fUseDocBytes;
	    DStringProperty text;
	    DStringProperty fields;
	    DStringProperty name;
	    DStringProperty displayName;
	    DStringSet fieldSet;
		gcroot<DataSource *> m_pDS;
		static void ConvertDate(dtsFileDate& dest, DateTime source);
    };

CNetDataSource::CNetDataSource() :
	m_pDS(0),
	m_fDone(false),
	m_fUseDocBytes(false)
	{ }

void CNetDataSource::attach(gcroot<DataSource*> ds)
{	m_pDS = ds;
}

int CNetDataSource::rewind()
{   debugLog("CNetDataSource::rewind");
    m_pDS->Rewind();
    m_fDone = false;
    return SUCCESS;
}

void CNetDataSource::ConvertDate(dtsFileDate& dest, System::DateTime source)
{	dest.hour = source.Hour;
	dest.minute = source.Minute;
	dest.second = source.Second;
	dest.year = source.Year;
	dest.month = source.Month;
	dest.day = source.Day;
}

int CNetDataSource::getNextDoc(dtsInputStream& dest)
{	LogFunction(".NetDataSource::getNextDoc", "");
    debugLog("CNetDataSource::getNextDoc");
    dtsIndexFileInfo *fi = dataSourceInterface.pFileInfo;
    if (fi) {
		m_pDS->DocId = fi->docId;
		m_pDS->DocWordCount = fi->wordCount;
		m_pDS->DocTypeId = fi->typeId;
		m_pDS->DocError = fi->openFailMessage;
		m_pDS->WasDocError = !strIsBlank(fi->openFailMessage);
		}
	debugLog("Calling GetNextDoc() in data source...");
    m_fDone = !m_pDS->GetNextDoc();
	debugLog("   GetNextDoc returned ", m_fDone);

    if (m_fDone)
        return FAIL;

    dtsFileInfo fileInfo;
    text.set(m_pDS->DocText);
    fields.set(m_pDS->DocFields);
    name.set(m_pDS->DocName);
    displayName.set(m_pDS->DocDisplayName);

    ConvertDate(fileInfo.modified, m_pDS->DocModifiedDate);
    ConvertDate(fileInfo.created, m_pDS->DocCreatedDate);

    bool fDocIsFile = m_pDS->DocIsFile;
    bool fHaveDocBytes = m_pDS->HaveDocBytes;

    debugLog("Values returned from DataSource:");

    strCopy(fileInfo.displayName, displayName, sizeof fileInfo.displayName);
    strCopy(fileInfo.filename, name, sizeof fileInfo.filename);

        debugLog("DocName:        ", fileInfo.filename);
        debugLog("DocIsFile:      ", fDocIsFile);
        debugLog("DocDisplayName: ", fileInfo.displayName);
        debugLog("DocText:        ", text);
        debugLog("DocFields:      ", fields);
        debugLog("HaveDocBytes:   ", fHaveDocBytes);

	fieldSet.clear();
    fieldSet.tokenize(fields, '\t', true);
    if (gfLoggingEnabled) {
		DString fs;
		debugLog("Parsed fields:  ");
		for (int i = 0; i < fieldSet.getCount(); i += 2) {
			fs.clear();
			fs << fieldSet.getString(i) << "=" << '\"' << fieldSet.getString(i+1) << '\"' << "\n";
			debugLog("    ", fs);
			}
		}
	DInputSourceBase *is = 0;
	if (fHaveDocBytes) {
		debugLog("Accessing docBytes");
		byte bytes __gc[] = m_pDS->DocBytes;
		int len = bytes->Length;
		if (len > 0) {
			debugLog("DocBytes size: ", len);
			DMemoryInputStream *ms = new DMemoryInputStream;
			if (!ms->allocate(len)) {
				debugLog("Allocation failed");
				delete ms;
				return FAIL;
				}
			Marshal::Copy(bytes, 0, (IntPtr)ms->getBuffer(), len);
			debugLog("docBytes accessed");
			ms->setDataSize(len);
			fileInfo.setSize(len);
			ms->setFileInfo(fileInfo);
			ms->Fields = fieldSet;
			if (!text.isBlank())
				ms->Fields.appendField(" ", text);
			is = ms;
			}
		}
	if (!is)
		is = DInputSourceBase::makeInputSource(fileInfo, text, fieldSet, fDocIsFile);

    is->makeInterface(dest);
    return SUCCESS;
}


class CApiIndexJobForNet : public InternalApiIndexJob::CApiIndexJob {
	public:
		CApiIndexJobForNet() {
			debugLog("CApiIndexJobForNet::CApiIndexJobForNet");
			m_indexProgressInfo = new IndexProgressInfo;
			m_indexFileInfo = new IndexFileInfo;
			m_indexProgressInfo->File = m_indexFileInfo;
			}
		~CApiIndexJobForNet() {
			debugLog("CApiIndexJobForNet::~CApiIndexJobForNet");
			m_statusHandler = 0;
			m_callerDataSource = 0;
			m_indexProgressInfo = 0;
			m_indexFileInfo = 0;
			}
		void setStatusHandler(gcroot<IIndexStatusHandler*> handler) {
			m_statusHandler = handler;
			}
		void attachDataSource(gcroot<DataSource*> ds) {
			m_callerDataSource = ds;
			m_dataSourceToIndex.attach(ds);
			UpdateJob.AttachDataSource(&m_dataSourceToIndex);
			}
		virtual void OnIndexStatusUpdate(dtsIndexProgressInfo& info);
		virtual void OnMergeStatusUpdate(dtsMergeStatusInfo& info);
		virtual void OnVerifyStatusUpdate(dtsVerifyStatusInfo& info);
		virtual void OnCheckAbort();
		void make(IndexJob& netjob);
		void makeProgressInfo(dtsIndexProgressInfo& info);

    protected:
		gcroot <IIndexStatusHandler*> m_statusHandler;
		gcroot <DataSource*> m_callerDataSource;
		gcroot <IndexProgressInfo *> m_indexProgressInfo;
		gcroot <IndexFileInfo *>m_indexFileInfo;

		CNetDataSource m_dataSourceToIndex;
    };

void CApiIndexJobForNet::OnCheckAbort()
{   int v = dtsContinue;
    if (m_statusHandler != 0)
        v = m_statusHandler->CheckForAbort();
    if (v == dtsAbort)
        Cancel();
    else if (v == dtsAbortImmediate)
        CancelImmediate();
}

void CApiIndexJobForNet::OnIndexStatusUpdate(dtsIndexProgressInfo& info)
{	if (!m_statusHandler)
		return;
	makeProgressInfo(info);
	m_statusHandler->OnProgressUpdate(m_indexProgressInfo);
}

void CApiIndexJobForNet::makeProgressInfo(dtsIndexProgressInfo& info)
{	InternalIndexJobUtil::CopyInfo(m_indexProgressInfo, info);
}

void CApiIndexJobForNet::OnMergeStatusUpdate(dtsMergeStatusInfo& info)
{	if (!m_statusHandler)
		return;

	InternalIndexJobUtil::CopyInfo(m_indexProgressInfo, info);
	m_statusHandler->OnProgressUpdate(m_indexProgressInfo);
}

void CApiIndexJobForNet::OnVerifyStatusUpdate(dtsVerifyStatusInfo& info)
{	if (!m_statusHandler)
		return;

	InternalIndexJobUtil::CopyInfo(m_indexProgressInfo, info);
	m_statusHandler->OnProgressUpdate(m_indexProgressInfo);
}




// ------------------ Managed (.NET) code -----------------------

namespace dtSearch { namespace Engine
{
// Constructor sets up error handling and non-zero constants
IndexJob::IndexJob () :
	NetJobBase("IndexJob")
{
	CountCreate(otIndexJob);

	m_MaxMemToUseMB = 64;

	m_FoldersToIndex = new StringCollection;
	m_IncludeFilters = new StringCollection;
	m_ExcludeFilters = new StringCollection;
	m_IndexesToMerge = new StringCollection;
	m_StoredFields = new StringCollection;

}

// Destructor aborts thread if running
IndexJob::~IndexJob ()
{
	m_DataSourceToIndex = NULL;
	Dispose(false);
	CountDelete(otIndexJob);
}

void IndexJob::DisposeUnmanagedItems()
{
	Monitor::Enter(this);
	indexJob = 0;  // NetJobBase will delete it
	Monitor::Exit(this);

	NetJobBase::DisposeUnmanagedItems();
}

// Execute an index job
bool IndexJob::Execute ()
{
	debugLog(".NET IndexJob::Execute");

	Monitor::Enter(this);
	indexJob = new CApiIndexJobForNet;
	Monitor::Exit(this);

	makeJob(*indexJob);
	bool ret = ExecuteJob(indexJob);
	debugLog(".NET IndexJob::Execute done,", (ret? "returning true" : "returning false"));
	return ret;
}

// Start execution of an index job in a separate thread
void IndexJob::ExecuteInThread ()
{	NetJobBase::ExecuteInThreadImp();
}

// Called periodically by main thread during asynchronous (separate thread) execution
// Gets the done/not-done status as well as updates progress data
bool IndexJob::IsThreadDone (int waitTimeMillis, IndexProgressInfo *status)
{	debugLog("IndexJob::IsThreadDone");
	bool ret = NetJobBase::IsThreadDoneWaitImp(waitTimeMillis);
	if (status && indexJob)
		makeIndexProgressInfo(status);
	return ret;
}

// Abort the index job
void IndexJob::AbortThread ()
{
	NetJobBase::AbortThread(AbortValue::Cancel);
}

// Abort the index job immediately
void IndexJob::AbortThreadImmediate ()
{	NetJobBase::AbortThread(AbortValue::CancelImmediately);
}

// Retrieves index information
IndexInfo* IndexJob::GetIndexInfo(String* indexPath)
{	IndexInfo *ret = new IndexInfo;

	DIndexInfo info;
	info.IndexPath.set(indexPath);
	if (info.Get() == FAIL)
		return ret;
	ret->CompressedDate = Converters::MakeNetDate(info.compressed);
	ret->UpdatedDate = Converters::MakeNetDate(info.updated);
	ret->CreatedDate = Converters::MakeNetDate(info.created);

	ret->Fragmentation = info.fragmentation;
	ret->DocCount = info.docCount;
	ret->Flags = (dtSearch::Engine::IndexingFlags)(info.indexFlags);
	ret->IndexSize = info.indexSize64;
	ret->WordCount = info.wordCount64;
	ret->PercentFull = info.percentFull;
	ret->ObsoleteCount = info.obsoleteCount;
	ret->StructureVersion = info.structureVersion;
	return ret;
}

void IndexJob::makeJob(CApiIndexJobForNet& ij)
{   ij.ClearJobs();
	ij.UpdateJob.IndexPath.set(m_IndexPath);
	ij.UpdateJob.SetIndexingFlags(m_IndexingFlags);
	ij.UpdateJob.indexJob.autoCommitIntervalMB = m_AutoCommitIntervalMB;
	ij.UpdateJob.FoldersToIndex.copy(m_FoldersToIndex);
	ij.UpdateJob.IncludeFilters.copy(m_IncludeFilters);
	ij.UpdateJob.ExcludeFilters.copy(m_ExcludeFilters);
	ij.UpdateJob.ToRemoveListName.set(m_ToRemoveListName);
	ij.UpdateJob.TempFileDir.set(m_TempFileDir);
	ij.UpdateJob.StoredFields.copy(m_StoredFields);
	if (m_DataSourceToIndex)
		ij.attachDataSource(m_DataSourceToIndex);

	if (m_StatusHandler)
		ij.setStatusHandler(m_StatusHandler);
	ij.UpdateJob.SetActionCreate(m_ActionCreate);
	ij.UpdateJob.SetActionAdd(m_ActionAdd);
	ij.UpdateJob.SetActionRemoveListed(m_ActionRemoveListed);
	ij.UpdateJob.SetActionRemoveDeleted(m_ActionRemoveDeleted);
	ij.UpdateJob.SetActionCompress(m_ActionCompress);
	ij.UpdateJob.SetFlagCreateCaseSensitive(m_CreateCaseSensitive);
	ij.UpdateJob.SetFlagCreateRelativePaths(m_CreateRelativePaths);
	ij.UpdateJob.SetFlagCreateAccentSensitive(m_CreateAccentSensitive);
	ij.UpdateJob.SetMaxMemToUseMB(m_MaxMemToUseMB);

	if (m_ActionVerify)
		ij.VerifyJob.IndexPath.set(m_IndexPath);
	if (m_ActionMerge) {
		ij.MergeJob.IndexesToMerge.copy(m_IndexesToMerge);
		int iTarget = 0;
		if (m_IndexPath && m_IndexPath->Length > 0) {
			if (ij.MergeJob.IndexesToMerge.Find(ij.UpdateJob.IndexPath) == FAIL) {
				ij.MergeJob.IndexesToMerge.append(m_IndexPath);
				iTarget = ij.MergeJob.IndexesToMerge.getCount()-1;
				}
			}
		ij.MergeJob.SetTarget(iTarget, false);
		}
}




// Converts a dtsFileDate to a .NET DateTime object
DateTime IndexJob::ConvertToDateTime (dtsFileDate fileDate)
{
	// If date is not valid, initialize to 1/1/0001
	try
	{
		return DateTime (fileDate.year, fileDate.month, fileDate.day);
	}
	catch (Exception*)
	{
		return DateTime (0);
	}
}

// Copies index update state from m_jobStatus to the caller-supplied
// IndexProgressInfo.   m_jobStatus is only updated during IsThreadDone,
// so it is safe to access it here (this is only called from IsThreadDone).
void IndexJob::makeIndexProgressInfo(IndexProgressInfo *netStatus)
{
	debugLog("IndexJob::makeIndexProgressInfo...");
	if (!indexJob) {
		debugLog("No job");
		return;
		}
	DHeapPtr<InternalApiIndexJob::CJobStatus> pStatus = new InternalApiIndexJob::CJobStatus;

	// Guard against indexJob being replaced while getting status, if another Execute is called
	Monitor::Enter(this);
	if (indexJob)
		indexJob->GetStatus(*pStatus, false);
	Monitor::Exit(this);

	switch(pStatus->WhichIsActive) {
		case InternalApiIndexJob::CJobStatus::UpdateStatusIsActive:
			InternalIndexJobUtil::CopyInfo(netStatus, pStatus->UpdateStatus);
			break;
		case InternalApiIndexJob::CJobStatus::MergeStatusIsActive:
			InternalIndexJobUtil::CopyInfo(netStatus, pStatus->MergeStatus);
			break;
		case InternalApiIndexJob::CJobStatus::VerifyStatusIsActive:
			InternalIndexJobUtil::CopyInfo(netStatus, pStatus->VerifyStatus);
			break;
		default:
			break;
		}
	debugLog("IndexJob::makeIndexProgressInfo...Done");

}

}}
