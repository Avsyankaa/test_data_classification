#include "stdafx.h"
#include "ListIndexJob.h"

using namespace System;
using namespace System::Collections;
using namespace System::Collections::Specialized;
using namespace System::Text;
using namespace dtSearch::Engine;


namespace dtSearch { namespace Engine {

ListIndexJob::ListIndexJob() :
	NetJobBase("ListIndexJob")
{	CountCreate(otListIndexJob);
}

ListIndexJob::~ListIndexJob()
{

}

void ListIndexJob::DisposeUnmanagedItems()
{	NetJobBase::DisposeUnmanagedItems();
}

bool ListIndexJob::Execute()
{	LogFunction("ListIndexJob::Execute()");

	DListIndexJob *job = new DListIndexJob;
	job->IndexPath.set(IndexPath);
	job->SetListFlags((long) ListIndexFlags);
	if (OutputToString)
		job->SetOutputToString();
	job->ToMatch.set(ToMatch);
	job->OutputFile.set(OutputFile);
	if (OutputStringMaxSize > 0)
		job->SetMaxSize(OutputStringMaxSize);
		
	bool ret = ExecuteJob(job);
	if (ret && OutputToString)
		OutputString = job->OutputString.allocNetString();
	
	// ExecuteJob will take care of deleting the job
	
	return ret;
}

void ListIndexJob::ExecuteInThread ()
{	LogFunction("ListIndexJob::ExecuteInThread", "");
	NetJobBase::ExecuteInThreadImp();
}

bool ListIndexJob::IsThreadDoneWait (long millisecondsToWait)
{	return NetJobBase::IsThreadDoneWaitImp(millisecondsToWait);
}

void ListIndexJob::AbortThread ()
{	NetJobBase::AbortThread(AbortValue::Cancel);
}


}

}
