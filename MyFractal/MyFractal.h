
// MyFractal.h : fichier d'en-tête principal de l'application MyFractal
//
#pragma once

#ifndef __AFXWIN_H__
	#error "incluez 'pch.h' avant d'inclure ce fichier pour PCH"
#endif

#include "resource.h"       // symboles principaux


// CMyFractalApp :
// Consultez MyFractal.cpp pour l'implémentation de cette classe
//

class CMyFractalApp : public CWinAppEx
{
public:
	CMyFractalApp() noexcept;


// Substitutions
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implémentation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMyFractalApp theApp;
