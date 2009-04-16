#pragma once

#include "resource.h"
// CConfigDlg dialog

class CConfigDlg : public CDialogBar
{
	DECLARE_DYNAMIC(CConfigDlg)

public:
	CConfigDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConfigDlg();

// Dialog Data
	enum { IDD = IDD_DIALOGBAR  };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	 afx_msg LONG OnInitDialog ( UINT, LONG );   

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeComboInterface();
public:
	afx_msg void OnBnClickedButtonInterface();
public:
	afx_msg void OnBnClickedButtonStart();
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
public:
	afx_msg void OnBnClickedButtonStart2();
	public:
	afx_msg void OnBnClickedButtonErrDataSave();
public:
	afx_msg void OnCbnSelchangeComboState();
public:
	afx_msg void OnCbnSelchangeComboScandelay();
public:
	afx_msg void OnCbnSelchangeComboLoctime();
public:
	afx_msg void OnBnClickedButtonUpload();
public:
	afx_msg void OnBnClickedButtonResult();
public:
	afx_msg void OnCbnSelchangeComboSort();
};
