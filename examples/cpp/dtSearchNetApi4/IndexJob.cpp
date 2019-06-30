#include "stdafx.h"
#include "utils.h"
#include "server.h"
#include "IndexJob.h"
#include "DataSource.h"



using namespace System;
using namespace System::IO;
using namespace System::Collections;
using namespace System::Collections::Specialized;
using namespace System::Text;
using namespace dtSearch::Engine;

private ref class InternalIndexJobUtil {
	public:
		static void CopyInfo(IndexProgressInfo ^info, dtsMergeStatusInfo& source) {
				info->Step = dtSearch::Engine::IndexingStep::ixStepMergingIndexes;
				info->UpdateType = dtSearch::Engine::MessageCode::dtsnIndexMergeJobProgress;
				info->PercentDone = source.percentDone;
				}
		static void CopyInfo(IndexProgressInfo ^info, dtsVerifyStatusInfo& source) {
				info->Step = dtSearch::Engine::IndexingStep::ixStepVerifyingIndex;
				info->UpdateType = dtSearch::Engine::MessageCode::dtsnIndexVerifyProgress;
				info->PercentDone = source.percentDone;
				}
		static void CopyInfo(IndexProgressInfo ^info,
				dtsIndexProgressInfo& source, const char *userFields) {
				IndexFileInfo ^fi = info->File;
				if (fi) {
					fi->BytesRead = source.file.bytesRead;
					fi->DisplayName = Converters::MakeNetString(source.file.displayName);
					fi->DocId = source.file.docId;
					fi->Fullname = Converters::MakeNetString(source.file.fullname);
					fi->Location = Converters::MakeNetString(source.file.location);
					fi->Name = Converters::MakeNetString(source.file.name);
					fi->OpenFailMessage = Converters::MakeNetString(source.file.openFailMessage);
					fi->PercentDone = source.getPercentDone();
					fi->Size = source.file.size;
					fi->Type = Converters::MakeNetString(source.file.type);
					fi->TypeId = (dtSearch::Engine::TypeId) source.file.typeId;
					fi->WordCount = source.file.wordCount;
					fi->Flags = (dtSearch::Engine::FileInfoFlags) source.fileInfoFlags;
					if (!strIsBlank(userFields))
						fi->UserFields = Converters::MakeNetString(userFields);
					else
						fi->UserFields = "";
					}
				info->BinaryCount = source.binaryCount;
				info->BytesReadKB = source.bytesReadKB;

				info->BytesToIndexKB = source.bytesToIndexKB;
				info->BytesToIndex64 = source.bytesToIndex64;
				info->DocBytesReadKB = source.docBytesReadKB;
				info->DocBytesRead64 = source.docBytesRead64;
				info->DocsInIndex = source.docsInIndex;
				info->EncryptedCount = source.encryptedCount;
				info->PartiallyEncryptedCount = source.partiallyEncryptedCount;
				info->PartiallyCorruptCount = source.partiallyCorruptCount;
				info->IndexPath = Converters::MakeNetString(source.indexPath);
				info->IndexPercentFull = source.indexPercentFull;
				info->OpenFailures = source.openFailures;
				info->PercentDone = source.percentDone;
				info->Step = (dtSearch::Engine::IndexingStep) source.step;
				info->UpdateType = (dtSearch::Engine::MessageCode) source.updateType;
				info->WordsInIndex = source.wordsInIndex;
				info->BytesReadKB = source.bytesReadKB;
				info->BytesRead64 = source.bytesRead64;
				info->FilesToIndex = source.filesToIndex;
				info->FilesRead = source.filesRead;
				info->DocsRead = source.docsRead;
				info->FilesToCheck = source.filesToCheck;
				info->FilesChecked = source.filesChecked;
				info->ElapsedSeconds = source.elapsedSeconds;
				info->EstRemainingSeconds = source.estRemaining;
				info->CurrMergePercent = source.currMergePercent;
			}
	};
	
#define BufSize 4096

CNetInputStream::CNetInputStream()
{	Buffer = gcnew array<byte>(BufSize);
	m_pStream = 0;
	// Should not automatically delete 
	m_bSuppressDeleteOnReleaseCB = true;
}

void CNetInputStream::attach(gcroot<Stream ^> s)
{	m_pStream = 0;
	fileInfo.setSize(0);
	if (s) {
		if (!s->CanRead)
			setOpenFailInfo(dtsErApiError, ".NET Stream class lacks Read() support");
		else if (!s->CanSeek)
			setOpenFailInfo(dtsErApiError, ".NET Stream class lacks Seek() support");
		else {
			fileInfo.setSize(s->Length);
			m_pStream = s;
			}
		}
}

void CNetInputStream::logException(const char *msg, Exception^ ex)
{	DStringProperty sp;
	sp.set(ex->Message);
	debugLog(msg, sp);

}

void CNetInputStream::seek(DFilePosition where)
{
	try {
		if (m_pStream)
			m_pStream->Seek(where, SeekOrigin::Begin);
		}
	catch(Exception^ ex) {
		logException("Seek", ex);
		}
}

DFilePosition CNetInputStream::tell()
{	try {
		if (m_pStream)
			return m_pStream->Position;
		else
			return 0;
		}
	catch(Exception^ ex) {
		logException("Tell", ex);
		return 0;
		}
}

long CNetInputStream::read(void *dest, long bytes)
{	try {
		if (!m_pStream)
			return 0;
		int totalBytesRead = 0;
		int offset = 0;
		while(bytes > 0) {
			int toRead = min(bytes, BufSize);
			int bytesRead = m_pStream->Read(Buffer, 0, toRead);
			if (bytesRead > 0) {
				IntPtr ipDest = (IntPtr)((char *) dest + offset);
				Marshal::Copy(Buffer, 0, ipDest, bytesRead);
				totalBytesRead += bytesRead;
				bytes -= bytesRead;
				offset += bytesRead;
				}
			else
				break;
			}
		return totalBytesRead;
		}
	catch(Exception ^ex) {
		logException("Read", ex);
		return 0;
		}
			
}



CNetDataSource::CNetDataSource() :
	m_pDS(0),
	m_fDone(false),
	m_fUseDocBytes(false),
	m_fDataSourceError(false)
	{ }

void CNetDataSource::attach(gcroot<DataSource^> ds)
{	m_pDS = ds;
}

int CNetDataSource::rewind()
{   debugLog("CNetDataSource::rewind");
	int ret = FAIL;
	try {
		m_pDS->Rewind();
		ret = SUCCESS;
		}
	catch(Exception^ ex) {
		reportError(ex->Message, "Rewind");
		}
	catch(...) {
		reportError("Unknown exception", "Rewind");
		}
    m_fDone = false;
    return ret;
}

void CNetDataSource::ConvertDate(dtsFileDate& dest, System::DateTime ^source)
{	dest.hour = source->Hour;
	dest.minute = source->Minute;
	dest.second = source->Second;
	dest.year = source->Year;
	dest.month = source->Month;
	dest.day = source->Day;
}

int CNetDataSource::getNextDoc(dtsInputStream& dest)
{	int ret = FAIL;

	try {
		ret = getNextDocImp(dest);
		}
	catch(std::bad_alloc) {
		reportError("Out of memory", "");
		}
	catch(Exception^ ex) {
		reportError(ex->Message, "GetNextDoc");
		}
	catch(...) {
		reportError("Unknown exception", "GetNextDoc");
		}

	return ret;

}

	

int CNetDataSource::getNextDocImp(dtsInputStream& dest)
{	LogFunction(".NetDataSource::getNextDoc", "");
    debugLog("CNetDataSource::getNextDoc");
    dtsIndexFileInfo *fi = dataSourceInterface.pFileInfo;
    if (fi) {
		m_pDS->DocId = fi->docId;
		m_pDS->DocWordCount = fi->wordCount;
		m_pDS->DocTypeId = fi->typeId;
		m_pDS->DocError = Converters::MakeNetString(fi->openFailMessage);
		m_pDS->WasDocError = !strIsBlank(fi->openFailMessage);
		}
	debugLog("Calling GetNextDoc() in data source...");
    m_fDone = !m_pDS->GetNextDoc();
	debugLog("   GetNextDoc returned ", m_fDone);

	if (m_fDone) {
		debugLog("Data source is done");
        return FAIL;
	}
	
	inputStreamMaker.Clear();
	inputStreamMaker.Text.set(m_pDS->DocText);
    inputStreamMaker.Name.set(m_pDS->DocName);
    inputStreamMaker.DisplayName.set(m_pDS->DocDisplayName);

	if (m_pDS->DocModifiedDate)
		ConvertDate(inputStreamMaker.ModifiedDate, m_pDS->DocModifiedDate);
	if (m_pDS->DocCreatedDate)
		ConvertDate(inputStreamMaker.CreatedDate, m_pDS->DocCreatedDate);

    inputStreamMaker.DocIsFile = m_pDS->DocIsFile;
    
    DStringProperty fields;
    fields.set(m_pDS->DocFields, true);
	
    
	inputStreamMaker.SetFields(fields);
    
    bool haveDocBytes = m_pDS->HaveDocBytes && m_pDS->DocBytes;
    if (haveDocBytes) {
	    inputStreamMaker.HaveDocBytes = true;
	    inputStreamMaker.DocBytesCount = m_pDS->DocBytes->Length;
	    }
    
    if (m_pDS->DocStream) {
		netInputStream.attach(m_pDS->DocStream);
		inputStreamMaker.MakeInputStream(&netInputStream);
		}
	else
		inputStreamMaker.MakeInputStream();
	
	if (haveDocBytes) {
		debugLog("Accessing docBytes");
		array<byte> ^bytes = m_pDS->DocBytes;
		int len = bytes->Length;
		void *pDest = inputStreamMaker.GetDocBytesBuffer();
		if (pDest) 
			Marshal::Copy(bytes, 0, (IntPtr)pDest, len);
		else
			debugLog("ERROR! No DocBytes pointer found");
		debugLog("docBytes accessed");
		}
    inputStreamMaker.ReturnInputStream(dest);
    return SUCCESS;
}


class CApiIndexJobForNet : public InternalApiIndexJob::CApiIndexJob {
	public:
		CApiIndexJobForNet() {
			debugLog("CApiIndexJobForNet::CApiIndexJobForNet");
			m_indexProgressInfo = gcnew IndexProgressInfo;
			m_indexFileInfo = gcnew IndexFileInfo;
			m_indexProgressInfo->File = m_indexFileInfo;
			}
		~CApiIndexJobForNet() {
			debugLog("CApiIndexJobForNet::~CApiIndexJobForNet");
			m_statusHandler = 0;
			m_callerDataSource = 0;
			m_indexProgressInfo = 0;
			m_indexFileInfo = 0;
			}
		void setStatusHandler(gcroot<IIndexStatusHandler^> handler) {
			m_statusHandler = handler;
			}
		void attachDataSource(gcroot<DataSource^> ds) {
			m_callerDataSource = ds;
			m_dataSourceToIndex.attach(ds);
			UpdateJob.AttachDataSource(&m_dataSourceToIndex);
			}
		virtual void OnIndexStatusUpdate(dtsIndexProgressInfo& info);
		virtual void OnMergeStatusUpdate(dtsMergeStatusInfo& info);
		virtual void OnVerifyStatusUpdate(dtsVerifyStatusInfo& info);
		virtual void OnCheckAbort();
		void make(IndexJob^ netjob);
		void makeProgressInfo(dtsIndexProgressInfo& info);
		bool getDataSourceErrors(DString& dest) {
			return m_dataSourceToIndex.getErrors(dest);
			}
		virtual void Execute();			
    protected:
		gcroot <IIndexStatusHandler^> m_statusHandler;
		gcroot <DataSource^> m_callerDataSource;
		gcroot <IndexProgressInfo ^> m_indexProgressInfo;
		gcroot <IndexFileInfo ^>m_indexFileInfo;

		CNetDataSource m_dataSourceToIndex;
		DStringSet userFieldsSet;
		DString userFieldsFormatted;
    };
    
void CApiIndexJobForNet::Execute()
{
	CApiIndexJob::Execute();
	DString dataSourceErrors;
	if (getDataSourceErrors(dataSourceErrors)) {
		errorHandler.handleMessage(dtsErDataSourceException, dataSourceErrors, 0, 0);
		}

}    

void CApiIndexJobForNet::OnCheckAbort()
{   AbortValue v = AbortValue::Continue;
    if (m_statusHandler)
        v = m_statusHandler->CheckForAbort();
	if (v == AbortValue::Cancel)
        Cancel();
	else if (v == AbortValue::CancelImmediately)
        CancelImmediate();
}

void CApiIndexJobForNet::OnIndexStatusUpdate(dtsIndexProgressInfo& info)
{	if (!m_statusHandler)
		return;
	makeProgressInfo(info);
	m_statusHandler->OnProgressUpdate(m_indexProgressInfo);
}

void CApiIndexJobForNet::makeProgressInfo(dtsIndexProgressInfo& info)
{	userFieldsFormatted.clear();
	if (info.updateType == dtsnIndexFileDone) {
		if (!strIsBlank(info.userFields)) {
			userFieldsSet.copyFromBuf(info.userFields);
			userFieldsSet.formatAsString(userFieldsFormatted);
			}
		}
	InternalIndexJobUtil::CopyInfo(m_indexProgressInfo, info, userFieldsFormatted);
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

	MaxMemToUseMB = 64;

	FoldersToIndex = gcnew StringCollection;
	IncludeFilters = gcnew StringCollection;
	ExcludeFilters = gcnew StringCollection;
	IndexesToMerge = gcnew StringCollection;
	StoredFields = gcnew StringCollection;
	EnumerableFields = gcnew StringCollection;

}


IndexJob::~IndexJob ()
{
	DataSourceToIndex = nullptr;

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
bool IndexJob::IsThreadDone (int waitTimeMillis, IndexProgressInfo ^status)
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
IndexInfo^ IndexJob::GetIndexInfo(String^ indexPath)
{	// Ensures that ApiInitializer loads
	INIT

	IndexInfo ^ret = gcnew IndexInfo;

	DIndexInfo info;
	info.IndexPath.set(indexPath);
	if (info.Get() == FAIL)
		return ret;
	ret->CompressedDate = Converters::MakeNetDate(info.compressed);
	ret->UpdatedDate = Converters::MakeNetDate(info.updated);
	ret->CreatedDate = Converters::MakeNetDate(info.created);

	ret->Fragmentation = info.fragmentation;
	ret->DocCount = info.docCount;
	ret->LastDocId = info.lastDocId;
	ret->StartingDocId = info.startingDocId;
	ret->Flags = (dtSearch::Engine::IndexingFlags)(info.indexFlags);
	ret->IndexSize = info.indexSize64;
	ret->WordCount = info.wordCount64;
	ret->PercentFull = info.percentFull;
	ret->ObsoleteCount = info.obsoleteCount;
	ret->StructureVersion = info.structureVersion;
	ret->TotalDataSize = info.totalDataSize;
	return ret;
}

void IndexJob::makeJob(CApiIndexJobForNet& ij)
{   ij.ClearJobs();
	ij.UpdateJob.IndexPath.set(IndexPath);
	ij.UpdateJob.SetIndexingFlags((long) IndexingFlags);
	ij.UpdateJob.indexJob.autoCommitIntervalMB = AutoCommitIntervalMB;
	ij.UpdateJob.FoldersToIndex.copy(FoldersToIndex);
	ij.UpdateJob.IncludeFilters.copy(IncludeFilters);
	ij.UpdateJob.ExcludeFilters.copy(ExcludeFilters);
	ij.UpdateJob.ToRemoveListName.set(ToRemoveListName);
	ij.UpdateJob.FileListName.set(ToAddFileListName);
	ij.UpdateJob.TempFileDir.set(TempFileDir);
	ij.UpdateJob.StoredFields.copy(StoredFields);
	ij.UpdateJob.EnumerableFields.copy(EnumerableFields);
	if (DataSourceToIndex != nullptr)
		ij.attachDataSource(DataSourceToIndex);

	if (StatusHandler != nullptr)
		ij.setStatusHandler(StatusHandler);
	ij.UpdateJob.SetActionCreate(ActionCreate);
	ij.UpdateJob.SetActionAdd(ActionAdd);
	ij.UpdateJob.SetActionRemoveListed(ActionRemoveListed);
	ij.UpdateJob.SetActionRemoveDeleted(ActionRemoveDeleted);
	ij.UpdateJob.SetActionCompress(ActionCompress);
	ij.UpdateJob.SetFlagCreateCaseSensitive(CreateCaseSensitive);
	ij.UpdateJob.SetFlagCreateRelativePaths(CreateRelativePaths);
	ij.UpdateJob.SetFlagCreateAccentSensitive(CreateAccentSensitive);
	ij.UpdateJob.SetMaxMemToUseMB(MaxMemToUseMB);
	ij.UpdateJob.indexJob.startingDocId = StartingDocId;
	if (ActionVerify)
		ij.VerifyJob.IndexPath.set(IndexPath);
	if (ActionMerge) {
		ij.MergeJob.IndexesToMerge.copy(IndexesToMerge);
		ij.MergeJob.SetFlags((int) IndexingFlags);
		int iTarget = 0;
		if (IndexPath && IndexPath->Length > 0) {
			if (ij.MergeJob.IndexesToMerge.Find(ij.UpdateJob.IndexPath) == FAIL) {
				ij.MergeJob.IndexesToMerge.append(IndexPath);
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
	catch (Exception^)
	{
		return DateTime (0);
	}
}

// Copies index update state from m_jobStatus to the caller-supplied
// IndexProgressInfo.   m_jobStatus is only updated during IsThreadDone,
// so it is safe to access it here (this is only called from IsThreadDone).
void IndexJob::makeIndexProgressInfo(IndexProgressInfo ^netStatus)
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
			InternalIndexJobUtil::CopyInfo(netStatus, pStatus->UpdateStatus, "");
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
