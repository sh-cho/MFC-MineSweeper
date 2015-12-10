
// Mine.h : main header file for the Mine application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMineApp:
// See Mine.cpp for the implementation of this class
//

class CMineApp : public CWinApp
{
public:
	CMineApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMineApp theApp;
