// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__F0C48F05_F06B_4764_A4EA_D22E2947A83B__INCLUDED_)
#define AFX_STDAFX_H__F0C48F05_F06B_4764_A4EA_D22E2947A83B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#define NEW_STREAMS

#if _MSC_VER >= 1600
#define WINVER 0x0501
#endif

#include <afx.h>
#include <afxwin.h>
#include <afxdlgs.h>
#ifdef NEW_STREAMS
#include <fstream>
#include <iostream>
using namespace std;
#else
#include <fstream.h>
#include <io.h>
#endif

#include <dtsfc.h>
#include <stringtools.h>

// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__F0C48F05_F06B_4764_A4EA_D22E2947A83B__INCLUDED_)
