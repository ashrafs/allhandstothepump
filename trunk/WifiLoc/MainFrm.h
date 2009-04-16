// MainFrm.h : interface of the CMainFrame class
//


#pragma once


#include "ConfigDlg.h"
#include "VirtualINS.h"

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
public:
	CConfigDlg m_wndConfigDlg;
	CVirtualINS m_VirtualINS;
	HWND	m_wndINS;
	int m_nLocCount;

// Generated message map functions
protected:
	afx_msg int	 OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdateDialogBarButtonInterface(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDialogBarButtonStart(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDialogBarButtonStart2(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDialogBarButtonErrDataSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDialogBarButtonUploadData(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDialogBarButtonResult(CCmdUI* pCmdUI);
    afx_msg void OnDialogBarButtonInterface();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	int IPC_SendStartMessage(int x , int y);
	int IPC_SendWifiData(int x , int y , double trust );
	void UpdateINSInfo();
	void ShowRSSI(CListCtrl *pList , void *r );
};


