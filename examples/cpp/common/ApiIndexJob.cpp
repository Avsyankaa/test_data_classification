#include <stdafx.h>
#include <ApiIndexJob.h>

using namespace InternalApiIndexJob;

 
void CJobQueue::Execute()
{
	beginExecution();
	int didFail = false;
	for (int i = 0; (i < jobs.getCount()) && !IsCancelled() && !didFail; ++i) {
		DJobBase *j = jobs.getValue(i);
		j->Execute();
		if (j->Failed())
			didFail = true;
		}
	endExecution(didFail);
} 

void CApiIndexJob::Execute()
{	ClearJobs();
	if (!UpdateJob.IsEmpty())
		Add(UpdateJob);
	if (!MergeJob.IsEmpty())
		Add(MergeJob);
	if (!VerifyJob.IsEmpty())
		Add(VerifyJob);
	CJobQueue::Execute();
}

void CApiIndexJob::handleMessage(dtsMessage& msg)
{
    switch(msg.command) {
        case dtsnCheckForAbort:
            OnCheckAbort();
            break;
        case dtsnIndexMergeJobProgress: {
            dtsMergeStatusInfo *prog = (dtsMergeStatusInfo *) msg.ptrParam;
            if (prog) {
				Status.update(*prog);
                OnMergeStatusUpdate(*prog);
                }
            break;
            }
        case dtsnIndexVerifyProgress: {
			dtsVerifyStatusInfo *prog = (dtsVerifyStatusInfo *) msg.ptrParam;
			if (prog) {
				Status.update(*prog);
				OnVerifyStatusUpdate(*prog);
				}
			break;
			}
		default: {
			if ((msg.command > dtsnFirstIndexStatusMessage) && (msg.command < dtsnLastIndexStatusMessage)) {
	            dtsIndexProgressInfo *prog = (dtsIndexProgressInfo *) msg.ptrParam;
	            if (prog) {
					Status.update(*prog);
		            OnIndexStatusUpdate(*prog);
		            }
		        }
            break;
			}
        }
}

void CInputStreamMaker::initFileInfo()
{
    strCopy(FileInfo.displayName, DisplayName, sizeof FileInfo.displayName);
    strCopy(FileInfo.filename, Name, sizeof FileInfo.filename);
    FileInfo.modified.copy(ModifiedDate);
    if (CreatedDate.year)
		FileInfo.created.copy(CreatedDate);
	else
		FileInfo.created.copy(ModifiedDate);
}

void CInputStreamMaker::logInfo()
{	if (!gfLoggingEnabled)
		return;
		
    debugLog("Values returned from DataSource:");

    debugLog("DocName:        ", FileInfo.filename);
    debugLog("DocIsFile:      ", DocIsFile);
    debugLog("DocDisplayName: ", FileInfo.displayName);
    debugLog("DocText:        ", Text);

    debugLog("HaveDocBytes:   ", HaveDocBytes);
    if (HaveDocBytes)
		debugLog("DocBytes size:  ", (int) DocBytesCount);

	DString fs;
	debugLog("Parsed fields:  ");
	for (int i = 0; i < Fields.getCount(); i += 2) {
		fs.clear();
		fs << Fields.getString(i) << "=" << '\"' << Fields.getString(i+1) << '\"' << "\n";
		debugLog("    ", fs);
		}
}

void CInputStreamMaker::SetFields(const char *s)
{	debugLog("SetFields: ", s);
	Fields.clear();
    Fields.tokenize(s, '\t', true);
    if (Fields.getCount() % 2 == 1)
		Fields.append(" ");
}

bool CInputStreamMaker::MakeInputStream(DInputStreamBase *streamToUse)
{	initFileInfo();
	logInfo();
	
	try {
		if (HaveDocBytes || streamToUse) {
			if (streamToUse) {
				pInputStream.attachWithoutOwnership(streamToUse);
				FileInfo.setSize(streamToUse->getSize());
				}
			else {
				DMemoryInputStream *ms = new DMemoryInputStream;
				pInputStream = ms;
				if (ms->allocate(DocBytesCount)) {
					docBytesBuffer = ms->getBuffer();
					ms->setDataSize(DocBytesCount);
					}
				else {
					bOutOfMemory = true;
					ms->setOpenFailInfo(dtsErOutOfMemory, "Out of memory allocating DocBytes");
					}
				FileInfo.setSize(DocBytesCount);
				}
			
			pInputStream->setFileInfo(FileInfo);
			pInputStream->Fields = Fields;
			if (!Text.isBlank()) {
				pInputStream->Fields.append(" ");
				pInputStream->Fields.append(Text);
				}
			}
		else
			pInputStream = DInputSourceBase::makeInputSource(FileInfo, Text, Fields, DocIsFile);
		return true;
		}
	catch(std::bad_alloc ) {
		bOutOfMemory = true;
		}

	return false;
}

void CInputStreamMaker::SetDocBytes(void *p, size_t size)
{
	if (!HaveDocBytes || (size > DocBytesCount) || !docBytesBuffer) {
		debugLog("Error -- SetDocBytes called incorrectly");
		return;
		}
	memmove(docBytesBuffer, p, size);		
}

int CInputStreamMaker::ReturnInputStream(dtsInputStream& dest)
{	if (pInputStream.isEmpty()) {
		debugLog("No input stream");
		return FAIL;
		}
	if (bOutOfMemory)
		return FAIL;
		
	pInputStream->makeInterface(dest);
	pInputStream.detach();
	return SUCCESS;
}

void CInputStreamMaker::Clear()
{
	Text.clear();
	DisplayName.clear();
	Name.clear();
	HaveDocBytes = false;
	DocIsFile = false;
	DocBytesCount = 0;
	ModifiedDate.clear();
	CreatedDate.clear();
	FileInfo.clear();
	docBytesBuffer = 0;
	pInputStream.dealloc();
	bOutOfMemory = false;
}
