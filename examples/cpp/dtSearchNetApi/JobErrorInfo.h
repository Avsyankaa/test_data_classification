#pragma once

#include "stdafx.h"
#include "Macros.h"
#include "Utils.h"

//#include <dtsearch.h>
//#include <dtsfc.h>
//#include <dstring.h>

using namespace System;

namespace dtSearch
{

namespace Engine
{

		/// <summary>
		/// Records error messages after an IndexJob, SearchJob, FileConverter, etc.
		/// </summary>
		/// <remarks>
		/// For each error, a code and a string message are returned.  A single job
		/// may return more than one error.  Use the ErrorCodes enumeration to
		/// interpret the value of ErrorCode.
		/// </remarks>
public __gc class JobErrorInfo : public DisposableObject
	{
	public:

		JobErrorInfo();
		~JobErrorInfo();
			/// <summary>
			/// Number of errors
			/// </summary>
		__property long get_Count ();
			//P:Count

		String* Message (long index);

		// Returns the message code for message with a given index
		long Code (long index);

		// Returns the first message argument for message with a given index
		String* Arg1 (long index);

		// Returns the second message argument for message with a given index
		String* Arg2 (long index);
		
		void Copy(const dtsErrorInfo *pInfo);
		
		void Copy(JobErrorInfo *other) {
			Copy(other->m_errorHandler);
			}
		void AppendError(int code, const char *msg) {
			m_errorHandler->handleMessage(code, msg, 0, 0);
			}

	protected:
		dtsErrorInfo* m_errorHandler;

		virtual void DisposeUnmanagedItems()  {
			if (m_errorHandler) {
				debugLog("JobErrorInfo::DisposeUnmanagedItems");
				delete m_errorHandler;
				m_errorHandler  = 0;
				}
			}

	};
}
}
