
// MFC_OpenGL_Sample.h : main header file for the MFC_OpenGL_Sample application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMFCOpenGLSampleApp:
// See MFC_OpenGL_Sample.cpp for the implementation of this class
//

class CMFCOpenGLSampleApp : public CWinAppEx
{
public:
	CMFCOpenGLSampleApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMFCOpenGLSampleApp theApp;
