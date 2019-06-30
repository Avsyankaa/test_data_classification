#include "StdAfx.h"
#include "netjobbase.h"

using namespace System;
using namespace System::Threading;

using namespace dtSearch::Engine;

// Any DJobBase-based class (DIndexJob, DSearchJob, etc.) can be executed by calling Execute, and
// will return errors from GetErrors(), so these common aspects of executing a job can be
// handled in the base class.
bool NetJobBase::ExecuteJob(DJobBase *aJob)
{	debugLog("NetJobBase::ExecuteJob...");
	bDone = false;
	if (!SetExecuting(true)) {
		reportError("Error:  Job is already running");
		return false;
		}
	
	freeJob();
	pJob = aJob;
	if (pJob == NULL) {
		debugLog("Null job, returning false");
		return false;
		}
	
	
	debugLog("ExecuteJob calling Execute...");
	pJob->Execute();
	
	debugLog("Execute() returned");
	
	m_Errors->Copy(pJob->GetErrors());
	
	bool ret = (pJob->Failed() == FALSE);
	
	SetExecuting(false);
	bDone = true;
	debugLog("NetJobBase::ExecuteJob... done");
	return ret;
}

NetJobBase::NetJobBase(const char *aName) : 
	bExecuting(0)
	{	name = aName;
		m_Errors = new JobErrorInfo();
		pJob = 0;
		pOptions = 0;
	};


void NetJobBase::freeJob() {
	if (pJob) {
		debugLog("NetJobBase::freeJob...", (int) pJob);
		DJobBase *toDelete = pJob;
		pJob = 0;
		if (toDelete)
			delete toDelete;
		debugLog("NetJobBase::freeJob...done");
		}
	}

void NetJobBase::DisposeUnmanagedItems() {
	debugLog("NetJobBase::DisposeUnmanagedItems");
	freeJob();
	if (pOptions) {
		debugLog("Deleting options");
		delete pOptions;
		pOptions = 0;
		}
	debugLog("NetJobBase::DisposeUnmanagedItems...done");
	}


bool NetJobBase::CallExecuteThroughDelegate() {
	if (pOptions) {
		// Makes sure options are consistent with thread called from
		DOptions thisThreadOptions;
		if (!thisThreadOptions.equals(*pOptions)) {
			debugLog("Conforming options to those of calling thread");
			pOptions->Save();
			}
		else
			debugLog("Thread options are consistent");
		}
	return Execute();
}

	
// This invokes the Execute() method of the derived class through a delegate.
// The Execute() method will create a DIndexJob, DSearchJob, or other DJobBase-derived class,
// and call ExecuteJob() to execute it.	
void NetJobBase::ExecuteInThreadImp()
{	debugLog("<ExecuteInThreadImp", name, ">");
	bDone = false;		
	if ((asrExecutingResult != NULL) || (executingDelegate != NULL))
	{
		reportError("SearchJob is already running -- cannot execute the same SearchJob twice");
		return;
	}
	Monitor::Enter(this);	
			
	if (pOptions) 
		delete pOptions;
	pOptions = new DOptions;

	executingDelegate = new ExecuteDelegate(this, &NetJobBase::CallExecuteThroughDelegate);
	
	AsyncCallback *acb = new AsyncCallback(this, &NetJobBase::OnThreadDone);
		
	asrExecutingResult = executingDelegate->BeginInvoke(acb, 0);
	Monitor::Exit(this);
	debugLog("</ExecuteInThreadImp", name, ">");
}

// AbortThread sets the cancel flag for the job and returns immediately.  The job will then
// exit as soon as possible and return.
void NetJobBase::AbortThread(AbortValue v)
{	debugLog("AbortThread", name);
	if (v == AbortValue::CancelImmediately)
		pJob->CancelImmediate();
	else
		pJob->Cancel();
}

// Ensures that EndInvoke is always called
void NetJobBase::OnThreadDone(IAsyncResult *ar)
{	debugLog("NetJobBase::OnThreadDone", name);
	if (asrExecutingResult)
		EndThread();
}

// Calls EndInvoke on the thread that was started by ExecuteInThreadImp.  The
// saved delegate and AsyncResult pointers are set to null to ensure this will be
// done exactly once for each thread invocation.
void NetJobBase::EndThread()
{	debugLog("NetJobBase::EndThread", name);

	if (executingDelegate == NULL) {
		debugLog("No delegate");
		return;
		}

	Monitor::Enter(this);
	ExecuteDelegate *del = executingDelegate;
	executingDelegate = NULL;
	IAsyncResult *asr = asrExecutingResult;
	asrExecutingResult = NULL;
	Monitor::Exit(this);
	if (del) {
		debugLog("EndInvoke", name);
		del->EndInvoke(asr);
		debugLog("EndInvoke done", name);
		}
}
	

bool NetJobBase::IsThreadDoneWaitImp(long millisecondsToWait)
{	debugLog("NetJobBase::IsThreadDoneWaitImp", millisecondsToWait);
	
	Monitor::Enter(this);
	IAsyncResult *asr = asrExecutingResult;
	Monitor::Exit(this);

	if (!asr)
		return bDone;

	bool ret = asr->AsyncWaitHandle->WaitOne(millisecondsToWait, false);
	debugLog("IsDone: ", (ret? "Yes" : "No"));
	return ret;
}	
	
bool NetJobBase::IsThreadDoneImp()
{	
	return IsThreadDoneWaitImp(0);
}

bool NetJobBase::SetExecuting(bool v) {
	bool ret = true;
	Monitor::Enter(this);
	if (v && bExecuting)
		ret = false;
	else 
		bExecuting = v;
	Monitor::Exit(this);
	return ret;
	}
