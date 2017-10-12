// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__E204F3C2_DFB4_41B6_A679_35B42388C8D2__INCLUDED_)
#define AFX_STDAFX_H__E204F3C2_DFB4_41B6_A679_35B42388C8D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#pragma comment(linker,"/ENTRY:WinMainCRTStartup")
#if !defined(_DEBUG) && !defined(DEBUG)
//如果是RELEASE选项

#if !defined(_UNICODE) && !defined(UNICODE)
//如果是RELEASE选项下的ANSI选项
#pragma comment(linker,"/ENTRY:WinMainCRTStartup")
#pragma comment(lib, "nafxcw.lib")
#else
//如果是RELEASE选项下的UNICODE选项
#pragma comment(linker,"/ENTRY:WinMainCRTStartup")
#pragma comment(lib, "uafxcw.lib")
#endif

#pragma comment(lib, "libcmt.lib")

#else
//如果是DEBUG选项

#if !defined(_UNICODE) && !defined(UNICODE)
//如果是DEBUG选项下的ANSI选项
#pragma comment(linker,"/ENTRY:WinMainCRTStartup")
#pragma comment(lib, "nafxcwd.lib")
#else
//如果是DEBUG选项下的UNICODE选项
//#pragma comment(linker, "/nodefaultlib:libcmtd.lib")
#pragma comment(linker,"/ENTRY:wWinMainCRTStartup")
#pragma comment(lib, "uafxcwd.lib")
#endif

#pragma comment(lib, "libcmtd.lib")

#endif

//#pragma comment(linker,"/verbose:lib")
//#pragma comment(linker, "/nodefaultlib")

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__E204F3C2_DFB4_41B6_A679_35B42388C8D2__INCLUDED_)
#include <afxcontrolbars.h>
