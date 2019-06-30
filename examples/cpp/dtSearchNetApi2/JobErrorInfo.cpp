#include "stdafx.h"

#include "JobErrorInfo.h"

using namespace System;

namespace dtSearch { namespace Engine 
{
	JobErrorInfo::JobErrorInfo ()
	{
	}
	


	// Returns the message text for message with a given index
	String^ JobErrorInfo::Message (long index)
	{	if (index < Count)
			return MessageTable[index];
		else
			return nullptr;
	}

	// Returns the message code for message with a given index
	long JobErrorInfo::Code (long index)
	{	if (index < Count)
			return MessageCodeTable[index];
		else
			return 0;
	}

	// Returns the first message argument for message with a given index
	String^ JobErrorInfo::Arg1 (long index)
	{	if (index < Count)
			return Arg1Table[index];
		else
			return nullptr;
	}

	// Returns the second message argument for message with a given index
	String^ JobErrorInfo::Arg2 (long index)
	{
		if (index < Count)
			return Arg2Table[index];
		else
			return nullptr;
	}
	
	void JobErrorInfo::Copy(const dtsErrorInfo *pInfo) {
		if (!pInfo) {
			Count = 0;
			return;
			}
		
		Count = pInfo->getCount();
		MessageCodeTable = gcnew array<int>(Count);
		MessageTable = gcnew StringCollection;
		Arg1Table = gcnew StringCollection;
		Arg2Table = gcnew StringCollection;
		for (int i = 0; i < Count; ++i) {
			MessageTable->Add(DString::allocNetString(pInfo->getMessage(i)));
			Arg1Table->Add(DString::allocNetString(pInfo->getMessageArg1(i)));
			Arg2Table->Add(DString::allocNetString(pInfo->getMessageArg2(i)));
			MessageCodeTable[i] = pInfo->getMessageCode(i);
			}
		}
}}
