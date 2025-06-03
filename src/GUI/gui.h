
// GUI.h: Hauptheaderdatei für die GUI-Anwendung
//
#pragma once

#ifndef __AFXWIN_H__
	#error "'pch.h' vor dieser Datei für PCH einschließen"
#endif

#include "resource.h"       // Hauptsymbole


// MyEmulatorGUI:
// Siehe GUI.cpp für die Implementierung dieser Klasse
//

class MyEmulatorGUI : public CWinAppEx
{
public:
	MyEmulatorGUI() noexcept;


// Überschreibungen
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementierung
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern MyEmulatorGUI theApp;
