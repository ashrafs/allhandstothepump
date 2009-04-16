// WifiLocView.h : interface of the CWifiLocView class
//


#pragma once


#define PATH_INIT	0
#define	PATH_DOING	1
#define PATH_END	2

class CWifiLocView : public CView
{
protected: // create from serialization only
	CWifiLocView();
	DECLARE_DYNCREATE(CWifiLocView)

// Attributes
public:
	CWifiLocDoc* GetDocument() const;
	int m_nPathStep;
	int m_bInitDC;
	int m_bBgDC;
	CDC m_memDC,m_BgDC;
	CBitmap m_Bitmap , *m_pOldBitmap ,m_BgBitmap;
	HBITMAP m_hBitmap;
	LPVOID m_pBitmap;
// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CWifiLocView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
public:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
	afx_msg void OnDestroy();
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
public:
//	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
public:
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
public:
	void UpdateCursorRSSI();
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
public:
	void CompareRSSI(RSSIINFO *r1 , RSSIINFO *r2);
public:
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
public:
	void DrawArrow(CDC* pDC, int nWidth, COLORREF nColorIn, COLORREF nColorOut, int X1, int Y1, int X2, int Y2);
};

#ifndef _DEBUG  // debug version in WifiLocView.cpp
inline CWifiLocDoc* CWifiLocView::GetDocument() const
   { return reinterpret_cast<CWifiLocDoc*>(m_pDocument); }
#endif

