#include "stdafx.h"

#include "JobErrorInfo.h"

using namespace System;

namespace dtSearch { namespace Engine 
{
	JobErrorInfo::JobErrorInfo ()
	{
		CountCreate(otJobErrorInfo);
		m_errorHandler = new dtsErrorInfo;
	}
	
	JobErrorInfo::~JobErrorInfo() {
		delete m_errorHandler;
		m_errorHandler = 0;
		
		CountDelete(otJobErrorInfo);
		}

	// Returns the number of errors
	long JobErrorInfo::get_Count ()
	{	return m_errorHandler->getCount ();
	}								

	// Returns the message text for message with a given index
	String* JobErrorInfo::Message (long index)
	{
		// Check range
		if (index >= get_Count())
		{
			debugLog ("Accessing out-of-range error", "");
			return NULL;
		}

		
		return DString::allocNetString(m_errorHandler->getMessage(index));
	}

	// Returns the message code for message with a given index
	long JobErrorInfo::Code (long index)
	{
		// Check range
		if (index >= get_Count())
		{
			debugLog ("Accessing out-of-range error", "");
			return -1;
		}

		return m_errorHandler->getMessageCode (index);
	}

	// Returns the first message argument for message with a given index
	String* JobErrorInfo::Arg1 (long index)
	{
		// Check range
		if (index >= get_Count())
		{
			debugLog ("Accessing out-of-range error", "");
			return NULL;
		}
		return DString::allocNetString(m_errorHandler->getMessageArg1 (index));
	}

	// Returns the second message argument for message with a given index
	String* JobErrorInfo::Arg2 (long index)
	{
		// Check range
		if (index >= get_Count())
		{
			debugLog ("Accessing out-of-range error", "");
			return NULL;
		}

		return DString::allocNetString(m_errorHandler->getMessageArg2(index));
	}
	
	void JobErrorInfo::Copy(const dtsErrorInfo *pInfo) {
		if (pInfo)
			m_errorHandler->copy(*pInfo);
		else
			m_errorHandler->clear();
		}
}}
