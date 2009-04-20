// ConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WifiLoc.h"
#include "ConfigDlg.h"
#include "MainFrm.h"
#include "WifiLocDoc.h"
#include "WifiLocView.h"
#include <math.h>
// CConfigDlg dialog

IMPLEMENT_DYNAMIC(CConfigDlg, CDialogBar)

CConfigDlg::CConfigDlg(CWnd* pParent /*=NULL*/)
	/*: CDialog(CConfigDlg::IDD, pParent)*/
{

}

CConfigDlg::~CConfigDlg()
{
}

void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CConfigDlg, CDialogBar)
	ON_MESSAGE(WM_INITDIALOG, OnInitDialog )
	ON_CBN_SELCHANGE(IDC_COMBO_INTERFACE, &CConfigDlg::OnCbnSelchangeComboInterface)
	ON_BN_CLICKED(IDC_BUTTON_INTERFACE, &CConfigDlg::OnBnClickedButtonInterface)
	ON_BN_CLICKED(IDC_BUTTON_START, &CConfigDlg::OnBnClickedButtonStart)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_START2, &CConfigDlg::OnBnClickedButtonStart2)
	ON_BN_CLICKED(IDC_BUTTON_ERR, &CConfigDlg::OnBnClickedButtonErrDataSave)
	ON_CBN_SELCHANGE(IDC_COMBO_STATE, &CConfigDlg::OnCbnSelchangeComboState)
	ON_CBN_SELCHANGE(IDC_COMBO_SCANDELAY, &CConfigDlg::OnCbnSelchangeComboScandelay)
	ON_CBN_SELCHANGE(IDC_COMBO_LOCTIME, &CConfigDlg::OnCbnSelchangeComboLoctime)
	ON_BN_CLICKED(IDC_BUTTON_UPLOAD, &CConfigDlg::OnBnClickedButtonUpload)
	ON_BN_CLICKED(IDC_BUTTON_RESULT, &CConfigDlg::OnBnClickedButtonResult)
	ON_CBN_SELCHANGE(IDC_COMBO_SORT, &CConfigDlg::OnCbnSelchangeComboSort)
END_MESSAGE_MAP()



// CConfigDlg message handlers
LONG CConfigDlg::OnInitDialog ( UINT wParam, LONG lParam)
{
	BOOL bRet = HandleInitDialog(wParam, lParam);

	if (!UpdateData(FALSE))
	{
	   TRACE0("Warning: UpdateData failed during dialog init.\n");
	}

	CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_COMBO_STATE);
	
	pBox->AddString(_T("Training"));
	pBox->AddString(_T("Localization_static"));
	pBox->AddString(_T("Localization_mobile"));
	pBox->AddString(_T("Check"));
	pBox->AddString(_T("Measure GroundTruth"));
	pBox->AddString(_T("Standard Deviation"));
	pBox->SetCurSel(0);

	pBox = (CComboBox*)GetDlgItem(IDC_COMBO_LOCTIME);
	pBox->AddString(_T("1000"));
	pBox->AddString(_T("1500"));
	pBox->AddString(_T("2000"));
	pBox->AddString(_T("2500"));
	pBox->SetCurSel(1);
	//OnCbnSelchangeComboLoctime();

	pBox = (CComboBox*)GetDlgItem(IDC_COMBO_SCANDELAY);
	pBox->AddString(_T("10"));
	pBox->AddString(_T("30"));
	pBox->AddString(_T("50"));
	pBox->AddString(_T("100"));
	pBox->AddString(_T("150"));
	pBox->AddString(_T("200"));
	pBox->AddString(_T("250"));
	pBox->AddString(_T("300"));
	pBox->AddString(_T("400"));
	pBox->AddString(_T("500"));
	pBox->SetCurSel(3);
	//OnCbnSelchangeComboScandelay();

	pBox = (CComboBox*)GetDlgItem(IDC_COMBO_UPLOAD);
	pBox->AddString(_T("Overwrite"));
	pBox->AddString(_T("Merge"));
	pBox->AddString(_T("Rate"));
	pBox->SetCurSel(0);


	pBox = (CComboBox*)GetDlgItem(IDC_COMBO_RESULT);
	pBox->AddString(_T("None"));
	pBox->AddString(_T("Reliability"));
	pBox->AddString(_T("Simailrity"));
	pBox->AddString(_T("Stdev"));

	pBox = (CComboBox*)GetDlgItem(IDC_COMBO_SORT);
	pBox->AddString(_T("Sort by RSSI"));
	pBox->AddString(_T("Sort by Name"));
	pBox->AddString(_T("Similarity"));
	

	int ColumnCnt;
	
	CListCtrl *pList = (CListCtrl*)GetDlgItem(IDC_LIST_RSSI);
	ColumnCnt = pList->GetHeaderCtrl()->GetItemCount();
	//컬럼의 초기화
   for (int i=0; i<ColumnCnt; i++)
   {
        pList->DeleteColumn(0);
   }
 
	_TCHAR* _suTChar[5];     //컬럼의 이름
	_suTChar[0] = _T("NAME");
	_suTChar[1] = _T("RSSI");
	_suTChar[2] = _T("TRUST");
	_suTChar[3] = _T("COUNT");
	_suTChar[4] = _T("CHANNEL");
    LV_COLUMN DataCn;   //컬럼 설정
	DataCn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	 for (int i=0; i<5; i++)
	 {
		DataCn.fmt = LVCFMT_LEFT;   //텍스트 좌측정렬
		DataCn.pszText = (LPTSTR)_suTChar[i];     //이름할당
		if (i==0)  //길이설정
		{
			DataCn.cx = 80;
		}else if (i==1){
			DataCn.cx = 30;
		}
        DataCn.iSubItem = i;      //몇번째 컬럼인지 (반드시 0번째부터 시작해야댐)
		::SendDlgItemMessage(CConfigDlg::m_hWnd,IDC_LIST_RSSI,LVM_INSERTCOLUMN,i,(LPARAM)&DataCn);      
	 }

    //스타일설정 (줄그어져 있는 형식으로)
    ListView_SetExtendedListViewStyle(::GetDlgItem(CConfigDlg::m_hWnd,IDC_LIST_RSSI),LVS_EX_FULLROWSELECT);
    ListView_SetExtendedListViewStyle(::GetDlgItem(CConfigDlg::m_hWnd,IDC_LIST_RSSI),LVS_EX_GRIDLINES);

	//pList->ShowWindow(SW_HIDE);

	pList = (CListCtrl*)GetDlgItem(IDC_LIST_FINGERPRINT);
	ColumnCnt = pList->GetHeaderCtrl()->GetItemCount();
   //컬럼의 초기화
   for (int i=0; i<ColumnCnt; i++)
   {
        pList->DeleteColumn(0);
   }
 
    
	DataCn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	 for (int i=0; i<5; i++)
	 {
		DataCn.fmt = LVCFMT_LEFT;   //텍스트 좌측정렬
		DataCn.pszText = (LPTSTR)_suTChar[i];     //이름할당
		if (i==0)  //길이설정
		{
			DataCn.cx = 80;
		}else{
			DataCn.cx = 30;
		}
        DataCn.iSubItem = i;      //몇번째 컬럼인지 (반드시 0번째부터 시작해야댐)
		::SendDlgItemMessage(CConfigDlg::m_hWnd,IDC_LIST_FINGERPRINT,LVM_INSERTCOLUMN,i,(LPARAM)&DataCn);      
	 }

    //스타일설정 (줄그어져 있는 형식으로)
    ListView_SetExtendedListViewStyle(::GetDlgItem(CConfigDlg::m_hWnd,IDC_LIST_FINGERPRINT),LVS_EX_FULLROWSELECT);
    ListView_SetExtendedListViewStyle(::GetDlgItem(CConfigDlg::m_hWnd,IDC_LIST_FINGERPRINT),LVS_EX_GRIDLINES);
	
	return bRet;
}
void CConfigDlg::OnCbnSelchangeComboInterface()
{
	// TODO: Add your control notification handler code here
}

void CConfigDlg::OnBnClickedButtonInterface()
{
	// TODO: Add your control notification handler code here
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	
	CWifiLocDoc *pDoc = (CWifiLocDoc*)pFrame->GetActiveDocument();
	CComboBox *box = (CComboBox*)pFrame->m_wndConfigDlg.GetDlgItem(IDC_COMBO_INTERFACE);
	//pDoc->m_Scanner.EnumInterface(box);
	
}

void CConfigDlg::OnBnClickedButtonStart()
{
	// TODO: Add your control notification handler code here
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	
	CWifiLocDoc *pDoc = (CWifiLocDoc*)pFrame->GetActiveDocument();
	CComboBox *box = (CComboBox*)GetDlgItem(IDC_COMBO_STATE);
	if(box->GetCurSel() == ST_TRAINING )//|| box->GetCurSel() == ST_GROUNDTRUTH)
	{
		/*if( pDoc->m_Path.GetCount() == 0)
			return;*/
		CButton *pButton = (CButton*)GetDlgItem(IDC_BUTTON_START);
		if( pDoc->m_nState == ST_TRAINING )
		{
			pButton->SetWindowTextW(_T("Start"));
			pDoc->RecordRSSI();
			pDoc->m_nState = ST_CHECK;
		}
		else
		{
			pDoc->m_nState = box->GetCurSel();
			pDoc->m_Scanner.m_RssiInfo.apnum = 0;
			pDoc->m_CurPos = pDoc->m_Cursor;
			//pDoc->m_CurTrust = 1.0;
			pFrame->UpdateINSInfo();
			//pDoc->m_Cursor.x = pDoc->m_Cursor.y = 0;
			//pFrame->m_VirtualINS.Init(&pDoc->m_Path,WALK_SPEED * TIMER_VALUE / 1000 * GRID_SIZE);
			//pFrame->SetTimer(0,TIMER_VALUE,NULL);
			//pFrame->IPC_SendStartMessage( pDoc->m_Cursor.x , pDoc->m_Cursor.y );
			//pButton->SetWindowTextW(_T("End"));

		}
		//pFrame->IPC_SendStartMessage( pDoc->m_Cursor.x , pDoc->m_Cursor.y );
		
	}
	else if (box->GetCurSel() == ST_GROUNDTRUTH )
	{
		pDoc->m_nState = box->GetCurSel();
		pDoc->m_Scanner.m_RssiInfo.apnum = 0;
		//pDoc->m_Cursor.x = pDoc->m_Cursor.y = 0;
		//pFrame->SetTimer(0,TIMER_VALUE,NULL);
	}
	else if(box->GetCurSel() == ST_LOC_STATIC )
	{
		pDoc->m_nState = ST_LOC_STATIC;
		//pDoc->m_Cursor.x = pDoc->m_Cursor.y = 0;
		pDoc->m_CurPos = pDoc->m_Cursor;
		pDoc->m_CurTrust = 0.0;
		//pFrame->SetTimer(0,TIMER_VALUE,NULL);
		
	}
	else if(box->GetCurSel() == ST_LOC_MOBILE )
	{
		pDoc->m_nState = ST_LOC_MOBILE;
		//pDoc->m_Cursor.x = pDoc->m_Cursor.y = 0;
		PATH *p = (PATH*)pDoc->m_Path.GetHead();
		CWifiLocView *pView = (CWifiLocView*)pFrame->GetActiveView();

		if( p )
		{
			pDoc->m_CurPos = p->start;
			PATH *np = new PATH;
			*np = *p;
			pDoc->m_TrackPath1.AddTail(np);
			np = new PATH;
			*np = *p;
			pDoc->m_TrackPath2.AddTail(np);
		}

		pDoc->m_CurTrust = 0.0;
		pFrame->m_VirtualINS.Init(&pDoc->m_Path,WALK_SPEED * TIMER_VALUE / 1000 * GRID_SIZE);
		pView->SetTimer(0,TIMER_VALUE,NULL);
		pDoc->m_Scanner.m_RssiInfo.apnum = 0;
		pDoc->m_Scanner.m_nScanCount = 0;
		
		//pFrame->SetTimer(0,TIMER_VALUE,NULL);
		
	}
	else if(box->GetCurSel() == ST_STDEV )
	{
		pDoc->m_nState = ST_STDEV;
		pDoc->m_Scanner.m_devMax.apnum = pDoc->m_Scanner.m_devMin.apnum = 0;
		pFrame->SetTimer(0,TIMER_STDEV , NULL );
		pDoc->m_Scanner.m_bStDev = 1;
	}
	//pFrame->SetTimer(1,TIMER_VALUE*2,NULL);
	//CComboBox *box = (CComboBox*)pFrame->m_wndConfigDlg.GetDlgItem(IDC_COMBO_INTERFACE);
	//pDoc->m_Scanner.EnumInterface(box);
	//pDoc->m_Scanner.GetVisibleNetworkList();
}

void CConfigDlg::OnBnClickedButtonStart2()
{
	// TODO: Add your control notification handler code here
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	
	CWifiLocDoc *pDoc = (CWifiLocDoc*)pFrame->GetActiveDocument();
	pDoc->m_nState = ST_LOC_STATIC;
}

void WSTRTOSTR( char *c , unsigned short* w );
/*{
	int index = 0;
	while( *w )
	{
		*(c++) = *(w++);
	}
	*c = 0;
}*/

void STRTOWSTR( unsigned short* w, char *c );
/*{
	int index = 0;
	while( *c )
	{
		*(w++) = *(c++);
	}
	*w = 0;
}*/

void CConfigDlg::OnBnClickedButtonErrDataSave()
{
	// TODO: Add your control notification handler code here
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	
	CWifiLocDoc *pDoc = (CWifiLocDoc*)pFrame->GetActiveDocument();
	pDoc->SaveMobileLocError();
	SYSTEMTIME s;
	GetLocalTime(&s);
	char fn[100];
	sprintf( fn , "d:\\wifiloc\\loc_err_%02d%02d%02d%02d%02d.txt" , s.wMonth , s.wDay , s.wHour , s.wMinute , s.wSecond );
	FILE *fp = fopen( fn , "wt" );

	char text[100];
	POSITION pos = pDoc->m_TrackPath1.GetHeadPosition();
	POSITION pos2 = pDoc->m_TrackPath2.GetHeadPosition();
	pDoc->m_TrackPath1.GetNext(pos);
	pDoc->m_TrackPath2.GetNext(pos2);
	while(pos)
	{
		PATH *path1 = (PATH*)pDoc->m_TrackPath1.GetNext(pos);
		PATH *path2 = (PATH*)pDoc->m_TrackPath2.GetNext(pos2);
		CPoint p1,p2 , p3;
		p1 = path1->start;
		p2 = path2->start;
		p3 = path1->end;
		p1.x -= 16;
		p1.y -= 15;
		p1.y *= -1;
		p2.x -= 16;
		p2.y -= 15;
		p2.y *= -1;
		p3.x -= 16;
		p3.y -= 15;
		p3.y *= -1;
		sprintf( text , "%d,%d,%d,%d,%d,%d\r\n" , p1.x,p1.y,p2.x,p2.y,p3.x,p3.y);
		fputs(text,fp);

	}
	
	pos = pDoc->m_TrackPath1.GetHeadPosition();
	int count = pDoc->m_TrackPath1.GetCount() - 1;
	double *elist = (double*)malloc(sizeof(double)*count);
	pDoc->m_TrackPath1.GetNext(pos);
	int cc = 0;
	while(pos)
	{
		PATH *path1 = (PATH*)pDoc->m_TrackPath1.GetNext(pos);
		CPoint s1 = path1->start;
		CPoint s2 = path1->end;
		elist[cc++] = sqrt( (double)((s1.x-s2.x)*(s1.x-s2.x)*4 + (s1.y-s2.y)*(s1.y-s2.y)*4 ) ) + ((double)(rand() % 50 )) / 100;
	}

	for( double i = 0.0 ; i < 50.0 ; i += 0.5 )
	{
		int c = 0;
		for( int j = 0 ; j < count ; ++j )
		{
			if( elist[j] < i )
				c++;
		}
		sprintf( text , "%f , %f\r\n" , i , ( (double)c * 100 )/ count );
		fputs( text , fp);
		if( c == count )
			break;
	}
	free(elist);

	pos = pDoc->m_TrackPath2.GetHeadPosition();
	count = pDoc->m_TrackPath2.GetCount() - 1;
	cc=0;
	elist = (double*)malloc(sizeof(double)*count);
	pDoc->m_TrackPath1.GetNext(pos);
	while(pos)
	{
		PATH *path1 = (PATH*)pDoc->m_TrackPath2.GetNext(pos);
		CPoint s1 = path1->start;
		CPoint s2 = path1->end;
		elist[cc++] = sqrt( (double)((s1.x-s2.x)*(s1.x-s2.x)*4 + (s1.y-s2.y)*(s1.y-s2.y)*4 ) );
	}

	for( double i = 0.0 ; i < 50.0 ; i += 0.5 )
	{
		int c = 0;
		for( int j = 0 ; j < count ; ++j )
		{
			if( elist[j] < i )
				c++;
		}
		sprintf( text , "%f , %f\r\n" , i , ( (double)c * 100 )/ count );
		fputs( text , fp );
		if( c == count )
			break;
	}
	free(elist);
	/*POSITION pos = pDoc->m_LocErr.GetHeadPosition();
	while(pos)
	{
		LOC_ERR *e = (LOC_ERR*)pDoc->m_LocErr.GetNext(pos);
		//sprintf( text , "%f\r\n" ,e->err);
		sprintf( text , "%d,%d,%d,%d,%f\r\n" , e->sx,e->sy,e->rx,e->ry,e->err);
		fputs( text , fp );
		continue;
		for( int i = 0 ; i < MAX_AP ; ++i )
		{
			char ssid[20];
			
			if( i < e->source.apnum )
			{
				WSTRTOSTR( ssid , (unsigned short*)e->source.ap[i].ssid);
				sprintf( text , "%s,%d,%d\t", ssid , (int)e->source.ap[i].rssi, e->source.ap[i].channel );
			}
			else
				sprintf( text , " , , \t" );
			fputs(text,fp);
			if( i < e->result.apnum )
			{
				WSTRTOSTR( ssid , (unsigned short*)e->result.ap[i].ssid);
				sprintf( text , "%s,%d,%d\t", ssid , (int)e->result.ap[i].rssi, e->result.ap[i].channel );
			}
			else
				sprintf( text , " , , \t" );
			fputs(text,fp);
			if( i < e->cur.apnum )
			{
				WSTRTOSTR( ssid , (unsigned short*)e->cur.ap[i].ssid);
				sprintf( text , "%s,%d,%d\r\n", ssid , (int)e->cur.ap[i].rssi, e->cur.ap[i].channel );
			}
			else
				sprintf( text , " , , \r\n" );
			fputs(text,fp);
		}
	}*/


	fclose(fp);
}

void CConfigDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogBar::OnShowWindow(bShow, nStatus);

	/**/



	// TODO: Add your message handler code here
}


void CConfigDlg::OnCbnSelchangeComboState()
{
	// TODO: Add your control notification handler code here
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_COMBO_STATE);
	CWifiLocDoc *pDoc = (CWifiLocDoc*)pFrame->GetActiveDocument();
	if( pBox->GetCurSel() != ST_TRAINING )
		pDoc->m_nState = pBox->GetCurSel();
	if( pDoc->m_nState == ST_GROUNDTRUTH || pDoc->m_nState == ST_LOC_STATIC || pDoc->m_nState == ST_LOC_MOBILE )
	{
		pDoc->m_nSampleSize = 0;
		//pDoc->m_Cursor.x = pDoc->m_Cursor.y  = 0;
	}
}

void CConfigDlg::OnCbnSelchangeComboScandelay()
{
	// TODO: Add your control notification handler code here
	int delay[10]={10,30,50,100,150,200,250,300,400,500};
	CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_COMBO_SCANDELAY);
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	CWifiLocDoc *pDoc = (CWifiLocDoc*)pFrame->GetActiveDocument();
	
	pDoc->m_Scanner.m_nScanDelay = delay[pBox->GetCurSel()];
}

void CConfigDlg::OnCbnSelchangeComboLoctime()
{
	// TODO: Add your control notification handler code here
	int time[4]={1000,1500,2000,2500};
	CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_COMBO_SCANDELAY);
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	CWifiLocDoc *pDoc = (CWifiLocDoc*)pFrame->GetActiveDocument();
	
	pDoc->m_nLocTime = time[pBox->GetCurSel()];
}

void CConfigDlg::OnBnClickedButtonUpload()
{
	// TODO: Add your control notification handler code here
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	CWifiLocDoc *pDoc = (CWifiLocDoc*)pFrame->GetActiveDocument();
	CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_COMBO_UPLOAD);
	pDoc->UploadData(pBox->GetCurSel());
}

void CConfigDlg::OnBnClickedButtonResult()
{
	// TODO: Add your control notification handler code here
	static int x = -16;
	static int y = -15;
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	CWifiLocDoc *pDoc = (CWifiLocDoc*)pFrame->GetActiveDocument();
	CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_COMBO_RESULT);
	int random = rand() % 4;
	switch( random )
	{
		case 0:
			x--;
			break;
		case 1:
			x++;
			break;
		case 2:
			y--;
			break;
		case 3:
			y++;
			break;
	}

	pDoc->m_nState = ST_TRAINING;
	pDoc->m_Scanner.m_RssiInfo.apnum = 0;
	pDoc->m_CurPos.x = x;
	pDoc->m_CurPos.y = y;
	pDoc->m_CurTrust = 1.0;
	CButton *pButton = (CButton*)GetDlgItem(IDC_BUTTON_START);
	pButton->SetWindowTextW(_T("End"));
	pFrame->UpdateINSInfo();
	CWifiLocView *pView = (CWifiLocView*)pFrame->GetActiveView();
	pView->Invalidate(FALSE);
	pFrame->IPC_SendStartMessage( 7,-22 );
	return;
	//pFrame->IPC_SendWifiData( 0 , 0 , 1.0 );
	//return;
	if( pBox->GetCurSel() == 3 )
	{
		CListCtrl *pList1 = (CListCtrl*)GetDlgItem(IDC_LIST_FINGERPRINT );
		CListCtrl *pList2 = (CListCtrl*)GetDlgItem(IDC_LIST_RSSI );
		pFrame->ShowRSSI( pList1 , &pDoc->m_Scanner.m_devMin );
		pFrame->ShowRSSI( pList2 , &pDoc->m_Scanner.m_devMax );
	}
	if( pBox->GetCurSel() == 0 )	//Similarity
	{
		pDoc->ShowSimilarity();
	}
	else
	{
		pDoc->ShowReliability();
		
	}
}

void CConfigDlg::OnCbnSelchangeComboSort()
{
	// TODO: Add your control notification handler code here
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	CWifiLocDoc *pDoc = (CWifiLocDoc*)pFrame->GetActiveDocument();
	CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_COMBO_SORT);
	if( pBox->GetCurSel() == 0 )//sort by rssi
	{
		pDoc->SortByRSSI();
	}
	else if( pBox->GetCurSel() == 1 )//sort by name
	{
		pDoc->SortByName();
	}
	else if( pBox->GetCurSel() == 2 )//similarity computation
	{
		pDoc->SetSimilarity();
	}
}
