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
		/// <para>
		/// For each error, a code and a string message are returned.  A single job
		/// may return more than one error.  Use the ErrorCodes enumeration to
		/// interpret the value of ErrorCode.
		/// </para>
		/// <para>
		/// Previous versions implemented the IDisposable interface because of the 
		/// need to release unmanaged resources.  JobErrorInfo no longer uses any
		/// unmanaged resources, but the IDisposable interface was retained for
		/// backward compatibility.
		/// </para>
		/// </remarks>
	public ref class JobErrorInfo : public DisposableObject
	{
	public:

		JobErrorInfo();

			/// <summary>
			/// Number of errors
			/// </summary>
		property long Count; 

		/// <summary>
		/// \Returns the message string for an error.
		/// </summary>
		/// <param name="iWhichError">Value between zero and Count\-1
		///                           indicating the error to return</param>
		String^ Message (long iWhichError);

		/// <summary>
		/// \Returns the message code (an ErrorCodes value) for an error.
		/// </summary>
		/// <param name="iWhichError">Value between zero and Count\-1
		///                           indicating the error code to
		///                           return</param>
		long Code (long iWhichError);

		/// Returns the first message argument for message with a given index
		String^ Arg1 (long iWhichError);

		/// Returns the second message argument for message with a given index
		String^ Arg2 (long iWhichError);

		void Copy(const dtsErrorInfo *pInfo);

	protected:
		System::Collections::Specialized::StringCollection ^MessageTable;
		System::Collections::Specialized::StringCollection ^Arg1Table;
		System::Collections::Specialized::StringCollection ^Arg2Table;
		array<int>^ MessageCodeTable;
	};
}
}
