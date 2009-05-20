// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "WifiLoc.h"

#include "MainFrm.h"
#include "WifiLocDoc.h"
#include "WifiLocView.h"
#include <stdlib.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_INTERFACE, OnUpdateDialogBarButtonInterface)
	ON_BN_CLICKED(IDC_BUTTON_INTERFACE, OnDialogBarButtonInterface)
	//ON_BN_CLICKED(IDC_BUTTON_START, OnDialogBarButtonStart)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_START, OnUpdateDialogBarButtonStart)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_START2, OnUpdateDialogBarButtonStart2)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_ERR, OnUpdateDialogBarButtonErrDataSave)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_UPLOAD, OnUpdateDialogBarButtonUploadData)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_RESULT, OnUpdateDialogBarButtonResult)
	ON_WM_TIMER()
	ON_WM_COPYDATA()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_nLocCount = 0;
	m_wndINS = NULL;
}

CMainFrame::~CMainFrame()
{
}

void CMainFrame::OnUpdateDialogBarButtonInterface(CCmdUI* pCmdUI)
{

    pCmdUI->Enable(TRUE);

}

void CMainFrame::OnUpdateDialogBarButtonStart(CCmdUI* pCmdUI)
{

    pCmdUI->Enable(TRUE);

}

void CMainFrame::OnUpdateDialogBarButtonStart2(CCmdUI* pCmdUI)
{

    pCmdUI->Enable(TRUE);

}
 
void CMainFrame::OnUpdateDialogBarButtonErrDataSave(CCmdUI* pCmdUI)
{

    pCmdUI->Enable(TRUE);

}

void CMainFrame::OnUpdateDialogBarButtonUploadData(CCmdUI* pCmdUI)
{

    pCmdUI->Enable(TRUE);

}
 
void CMainFrame::OnUpdateDialogBarButtonResult(CCmdUI* pCmdUI)
{

    pCmdUI->Enable(TRUE);

}

void CMainFrame::OnDialogBarButtonInterface()

{

    AfxMessageBox((LPCTSTR)"OnDialogBarButton1");

}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	if(!m_wndConfigDlg.Create(this,IDD_DIALOGBAR,CBRS_LEFT|CBRS_GRIPPER,IDD_DIALOGBAR))
	{
		TRACE0("Failed to create CONFIG DLG\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	WNDCLASS wc;
	::GetClassInfo( AfxGetInstanceHandle(),cs.lpszClass,&wc);
	wc.lpszClassName = cs.lpszClass = _T("WifiLoc");
	::RegisterClass(&wc);

	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers




void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	CWifiLocDoc *pDoc = (CWifiLocDoc*)GetActiveDocument();
	
	//WCHAR text[100];
	//wcscpy( text , _T("WifiLoc") );
	//::SetWindowText( this->GetSafeHwnd() , text );
	static int count = 0;
	if( pDoc->m_nState == ST_STDEV )
	{
		

		CListCtrl *pList1 = (CListCtrl*)this->m_wndConfigDlg.GetDlgItem(IDC_LIST_FINGERPRINT );
		CListCtrl *pList2 = (CListCtrl*)this->m_wndConfigDlg.GetDlgItem(IDC_LIST_RSSI );
		
		ShowRSSI( pList1 , &pDoc->m_Scanner.m_devMin );
		ShowRSSI( pList2 , &pDoc->m_Scanner.m_devMax );
		pDoc->m_Scanner.m_bStDev = 0;
	}
	else if( pDoc->m_nState == ST_TRAINING )
	{
#ifdef VIRTUAL_TRAINING			//Virtual Training//
		/*if( m_VirtualINS.Move() )
		{
			pDoc->m_nState = ST_CHECK;
			KillTimer(0);

		}
		CPoint index = m_VirtualINS.GetGrid();
		pDoc->m_Scanner.GetRSSIfromAirPCap((CListCtrl*)m_wndConfigDlg.GetDlgItem(IDC_LIST_RSSI) , pDoc->m_RssiDB[index.y][index.x] );
		if( pDoc->m_nState == ST_CHECK )
			AfxMessageBox(_T("Training Complete!!") , MB_OK );
		//if( count % 2 )
		//	pDoc->m_Scanner.GetVisibleNetworkList((CListCtrl*)m_wndConfigDlg.GetDlgItem(IDC_LIST_RSSI) , &pDoc->m_RssiDB[index.y][index.x] );
		//else
		//	pDoc->m_Scanner.Scan();
		//pDoc->m_Scanner.GetBssList(&pDoc->m_RssiDB[index.y][index.x]);*/
		CPoint prev = m_VirtualINS.GetGrid();
		if( m_VirtualINS.Move() )
		{
			pDoc->m_nState = ST_CHECK;
			KillTimer(0);
			
		}
		if( pDoc->m_nState == ST_CHECK )
			AfxMessageBox(_T("Training Complete!!") , MB_OK );
		CPoint cur = m_VirtualINS.GetGrid();
		if( prev.x != cur.x || prev.y != cur.y )
		{
			pDoc->m_Scanner.GetRSSIfromAirPCap((CListCtrl*)m_wndConfigDlg.GetDlgItem(IDC_LIST_RSSI) , pDoc->m_RssiDB[prev.y][prev.x] );
		}
		CWifiLocView *pView = (CWifiLocView*)GetActiveView();
		pView->Invalidate(FALSE);
#endif
	}
	else if( pDoc->m_nState == ST_GROUNDTRUTH )
	{
		CPoint index;
		if( pDoc->m_nSampleSize >= GROUNDTRUTH_SAMPLESIZE )
		{
			while(1)
			{
				CPoint cur = m_VirtualINS.GetGrid();
				if( m_VirtualINS.Move() )
				{
					pDoc->m_nState = ST_CHECK;
					KillTimer(0);
					break;
				}
				index = m_VirtualINS.GetGrid();
				if( cur != index )
					break;
				
			}
			if( pDoc->m_nState == ST_CHECK )
			{
				AfxMessageBox(_T("Measurement Complete!!") , MB_OK );
				
			}
			else
			{
				KillTimer(0);
				AfxMessageBox(_T("Go to Next Position!!") , MB_OK );
				pDoc->m_Scanner.m_RssiInfo.apnum = 0;
				SetTimer(0,TIMER_VALUE,NULL);
				
			}
			pDoc->m_nSampleSize = 0;
			CWifiLocView *pView = (CWifiLocView*)GetActiveView();
			pView->Invalidate(FALSE);
		}
		else
		{
			index = m_VirtualINS.GetGrid();
			pDoc->m_nSampleSize++;
		}
		if( pDoc->m_nState != ST_CHECK )
			pDoc->m_Scanner.GetRSSIfromAirPCap((CListCtrl*)m_wndConfigDlg.GetDlgItem(IDC_LIST_RSSI) , pDoc->m_RssiDB[index.y][index.x] );
		
	}
	/*else if( pDoc->m_nState == ST_LOCALIZATION )
	{
		memset( &pDoc->m_CurRssi , 0x00 , sizeof( RSSIINFO ) );
		pDoc->m_Scanner.GetRSSIfromAirPCap((CListCtrl*)m_wndConfigDlg.GetDlgItem(IDC_LIST_RSSI) , pDoc->m_CurRssi );
		//if( count % 2 )
		//	pDoc->m_Scanner.GetVisibleNetworkList((CListCtrl*)m_wndConfigDlg.GetDlgItem(IDC_LIST_RSSI) , &pDoc->m_CurRssi );
		//else
		//	pDoc->m_Scanner.Scan();
		//pDoc->m_Scanner.GetBssList(&pDoc->m_CurRssi);
		pDoc->m_Scanner.m_RssiInfo.apnum = 0;
		pDoc->Localization();
		CWifiLocView *pView = (CWifiLocView*)GetActiveView();
		pView->Invalidate(FALSE);
	}*/
	count++;
	CFrameWnd::OnTimer(nIDEvent);
}


int CMainFrame::IPC_SendStartMessage(int x , int y)
{
	//return 0;
	static int xx = 0;
	static int yy = 0;
    int return_value = 0;
    IPC_INSDATA *p_buffer = NULL;
 
	if( !m_wndINS )
	{
		CWnd * w = FindWindow(_T("WifiLoc") , NULL);
		if( w )
			m_wndINS = w->GetSafeHwnd();
	}
	if( !m_wndINS )
	{
		AfxMessageBox(_T("Can't find INS Window!!!"));
		return -1;
	}
    
	/*p_buffer = (IPC_INSDATA *)::HeapAlloc(::GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(IPC_INSDATA));

    if(p_buffer != NULL)
	{
		p_buffer->x = x;
		p_buffer->y = y;

		if( ( xx + yy ) % 2 )
			xx++;
		else
			yy++;
		p_buffer->x = xx;
		p_buffer->y = yy;
		p_buffer->trust = 100 - (xx+yy);
	}*/

	char *text = (char*)::HeapAlloc(::GetProcessHeap(), HEAP_ZERO_MEMORY, 100);
	x -= 32;
	y -= 30;
	y *= -1;
	sprintf( text , "%d,%d,%f" , x , y , 0.003f );
    COPYDATASTRUCT tip;
    tip.dwData = IPC_PROTOCOL_START;
    tip.cbData = 100;
    tip.lpData = text;
    if(IsWindow(m_wndINS)) return_value = ::SendMessage(m_wndINS, WM_COPYDATA,(WPARAM)m_wndINS, (LPARAM)&tip);
    //if(p_buffer != NULL) ::HeapFree(::GetProcessHeap(), 0, p_buffer);
	if(text != NULL) ::HeapFree(::GetProcessHeap(), 0, text);
    return return_value;
}

int CMainFrame::IPC_SendWifiData(int x , int y , double trust)
{
    int return_value = 0;
    IPC_WIFIDATA *p_buffer = NULL;
 
	if( !m_wndINS )
	{
		CWnd * w = FindWindow(NULL , _T("Pedestrian Navigation System") );
		if( w )
			m_wndINS = w->GetSafeHwnd();
	}
	if( !m_wndINS )
	{
		AfxMessageBox(_T("Can't find INS Window!!!"));
		return -1;
	}
    
	/*p_buffer = (IPC_WIFIDATA *)::HeapAlloc(::GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(IPC_WIFIDATA));

    if(p_buffer != NULL)
	{
		p_buffer->x = x;
		p_buffer->y = y;
		p_buffer->trust = trust;
	}*/

	x -= 32;
	y -= 30;
	y *= -1;
	char *text = (char*)::HeapAlloc(::GetProcessHeap(), HEAP_ZERO_MEMORY, 100);
	sprintf( text , "%d,%d,%f" , x , y , GetErrorFromTrust(trust) );
    COPYDATASTRUCT tip;
    tip.dwData = IPC_PROTOCOL_WIFIDATA;
    tip.cbData = 100;
    tip.lpData = text;
    if(IsWindow(m_wndINS)) return_value = ::SendMessage(m_wndINS, WM_COPYDATA,(WPARAM)m_wndINS, (LPARAM)&tip);
    //if(p_buffer != NULL) ::HeapFree(::GetProcessHeap(), 0, p_buffer);
    return return_value;
}

BOOL CMainFrame::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: Add your message handler code here and/or call default
	static int count = 0;
	CWifiLocDoc *pDoc = (CWifiLocDoc*)GetActiveDocument();
	
	switch(pCopyDataStruct->dwData){
		case IPC_PROTOCOL_START:
		{
			CString str;
			CString token;
			int x,y;
            char *p_data = (char *)pCopyDataStruct->lpData;
			int curpos = 0;
            str =  p_data;
			token = str.Tokenize(_T(","),curpos);
			x = _ttoi(token);
			token = str.Tokenize(_T(","),curpos);
			y = _ttoi(token);
			x += 32;
			y *= -1;
			y += 30;
			//x = 0;
			//y = 0;
			pDoc->m_nState = ST_TRAINING;
			pDoc->m_Scanner.m_RssiInfo.apnum = 0;
			pDoc->m_CurPos.x = x;
			pDoc->m_CurPos.y = y;
			pDoc->m_CurTrust = 1.0;
			CButton *pButton = (CButton*)m_wndConfigDlg.GetDlgItem(IDC_BUTTON_START);
			pButton->SetWindowTextW(_T("End"));
			UpdateINSInfo();
			CWifiLocView *pView = (CWifiLocView*)this->GetActiveView();
			pView->Invalidate(FALSE);
		}
		break;
		case IPC_PROTOCOL_INSDATA:
        {
			CString str;
			CString token;
			IPC_INSDATA data;
            char *p_data = (char *)pCopyDataStruct->lpData;
			int curpos = 0;
            str =  p_data;
			token = str.Tokenize(_T(","),curpos);
			data.x = _ttoi(token);
			token = str.Tokenize(_T(","),curpos);
			data.y = _ttoi(token);
			token = str.Tokenize(_T(","),curpos);
			data.trust = _wtof(token);
			
			
            //MessageBox(str);
           

			//pDoc->m_CurRssi = pDoc->m_Scanner.m_RssiBuffer2;
			memset( &pDoc->m_CurRssi , 0x00 , sizeof( RSSIINFO ) );
			pDoc->m_Scanner.GetRSSIfromAirPCap((CListCtrl*)m_wndConfigDlg.GetDlgItem(IDC_LIST_RSSI) , pDoc->m_CurRssi );
			 pDoc->RecordRSSI();
            data.y *= -1;
			data.y += 30;
			data.x += 32;
			data.trust = GetTrustFromError( data.trust );
			if( data.trust < 0 )
				data.trust = 0;
			

			//data.x = pDoc->m_CurPos.x+1;
			//data.y = pDoc->m_CurPos.y;
			if( data.x >= GRID_W )
			{
				data.x = 0;
				data.y++;
			}
			if( data.x < 0 )
				data.x = 0;
			if( data.x >= GRID_W )
				data.x = GRID_W - 1;
			if( data.y < 0 )
				data.y = 0;
			if( data.y >= GRID_H )
				data.y = GRID_H - 1;


			double score;
			CPoint loc;
			/*if( pDoc->Localization(loc , score) == 0 )
			{
				RSSIINFO revised = pDoc->GetGridRSSI( pDoc->m_RssiDB , loc.x,loc.y );//[ loc.y ][ loc.x ];
				double err1 = GetErrorFromTrust( data.trust );
				double err2 = GetErrorFromTrust( revised.trust ) + FINGERPRINT_ERROR;
				if( m_nLocCount )
					m_nLocCount--;
				if( !m_nLocCount && err1 >= 10 && err2 < err1 && score < SIMILAR_THRESHOLD )
				{
					data.x = loc.x;
					data.y = loc.y;
					data.trust = GetTrustFromError( err2 );
					IPC_SendWifiData( data.x , data.y , data.trust );
					m_nLocCount = 5;
				}
			}*/
			PATH *next = new PATH;
			next->start = pDoc->m_CurPos;
			next->end.x = data.x;
			next->end.y = data.y;
			pDoc->m_Path.AddTail(next);
			
				
			
			CWifiLocView *pView = (CWifiLocView *)GetActiveView();
			if( pView )
				pView->UpdateCursorRSSI();
			pDoc->m_Scanner.m_RssiInfo.apnum = 0;
			pDoc->m_CurPos.x = data.x;
			pDoc->m_CurPos.y = data.y;
			pDoc->m_Cursor.x = pDoc->m_CurPos.x;
			pDoc->m_Cursor.y = pDoc->m_CurPos.y;
			pDoc->m_CurTrust = data.trust;
			
			
			UpdateINSInfo();
			
			Invalidate(FALSE);
        }
        break;
    }
	return CFrameWnd::OnCopyData(pWnd, pCopyDataStruct);
}

void CMainFrame::UpdateINSInfo()
{
	WCHAR text[100];
	CWifiLocDoc *pDoc = (CWifiLocDoc*)GetActiveDocument();
	CEdit *edit = (CEdit*)m_wndConfigDlg.GetDlgItem(IDC_EDIT_INSINFO);
	if( !m_nLocCount )
		wsprintf(text,_T("POS = (%2d , %2d), TRUST=%3d" ) , pDoc->m_CurPos.x , pDoc->m_CurPos.y , (int)(pDoc->m_CurTrust*100) );
	else
		wsprintf(text,_T("POS = (%2d , %2d), TRUST=%3d(Revised!!!)" ) , pDoc->m_CurPos.x , pDoc->m_CurPos.y , (int)(pDoc->m_CurTrust*100) );
	edit->SetWindowTextW(text);
}

void CMainFrame::ShowRSSI( CListCtrl *pList , void *r )
{
	//CWifiLocDoc *pDoc = (CWifiLocDoc*)GetActiveDocument();
	pList->DeleteAllItems();
	RSSIINFO *ri = (RSSIINFO*)r;
	for( int i = 0 ; i < ri->apnum ; ++i )
	{
		int iRow = pList->GetItemCount();
		int iColumn = 0;
		LV_ITEM lvi;       
		lvi.mask=LVIF_TEXT | LVIF_PARAM;
		lvi.iItem = iRow;        //몇행
		lvi.iSubItem = iColumn;  //몇번째 컬럼
		lvi.pszText = ri->ap[i].ssid;     //내용
		lvi.lParam = (LPARAM)iRow*2+iColumn;       //셀을 식별할 고유인자. 
	 
		pList->InsertItem(&lvi);
		pList->SetItemText( iRow,iColumn,ri->ap[i].ssid);      //화면에 나타낸다
	   
		unsigned short text[100];
		_itow(ri->ap[i].rssi,(wchar_t *)text,10);
		iColumn = 1;
		pList->SetItemText( iRow,iColumn,(LPCTSTR)text);      //화면에 나타낸다
		
		wsprintf((LPWSTR)text , _T("%d") , (int)(ri->trust*100) );
		//_ftow(ri->trust,(wchar_t *)text,10);
		iColumn = 2;
		pList->SetItemText( iRow,iColumn,(LPCTSTR)text);      //화면에 나타낸다

		_itow(ri->ap[i].count,(wchar_t *)text,10);
		iColumn = 3;
		pList->SetItemText( iRow,iColumn,(LPCTSTR)text);      //화면에 나타낸다

		_itow(ri->ap[i].channel,(wchar_t *)text,10);
		iColumn = 4;
		pList->SetItemText( iRow,iColumn,(LPCTSTR)text);      //화면에 나타낸다
	}
}