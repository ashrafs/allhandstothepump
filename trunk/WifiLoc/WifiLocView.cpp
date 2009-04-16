// WifiLocView.cpp : implementation of the CWifiLocView class
//

#include "stdafx.h"
#include "WifiLoc.h"

#include "WifiLocDoc.h"
#include "WifiLocView.h"
#include "MainFrm.h"
#include "VirtualINS.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWifiLocView

IMPLEMENT_DYNCREATE(CWifiLocView, CView)

BEGIN_MESSAGE_MAP(CWifiLocView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_MBUTTONUP()
//	ON_WM_COPYDATA()
ON_WM_COPYDATA()
ON_WM_LBUTTONDBLCLK()
ON_WM_KEYDOWN()
ON_WM_RBUTTONDOWN()
ON_WM_RBUTTONDBLCLK()
END_MESSAGE_MAP()

// CWifiLocView construction/destruction

CWifiLocView::CWifiLocView()
{
	// TODO: add construction code here
	m_bInitDC = m_bBgDC = 0;
	m_nPathStep = PATH_INIT;

}

CWifiLocView::~CWifiLocView()
{
}

BOOL CWifiLocView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CWifiLocView drawing

void CWifiLocView::OnDraw(CDC* pDC)
{
	CWifiLocDoc* pDoc = GetDocument();
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	CRect r;
	GetClientRect(r);
	if( m_bBgDC == 0 )
	{
		m_bBgDC = 1;

		m_BgDC.CreateCompatibleDC(pDC);
		CString strBMPFile = _T("engmap.bmp");
		HBITMAP hBitmap = NULL;
		hBitmap = (HBITMAP)::LoadImage(NULL,strBMPFile,IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
		m_BgBitmap.Attach(hBitmap);
		m_BgDC.SelectObject(&m_BgBitmap);
		BITMAP b;
		m_BgBitmap.GetBitmap(&b);
		m_BgBitmap.SetBitmapDimension(b.bmWidth , abs(b.bmHeight) );

	}
	if( m_bInitDC == 0 )
	{
		m_bInitDC = 1;
		
		m_memDC.CreateCompatibleDC(pDC);
		m_Bitmap.CreateBitmap( r.Width() , r.Height() , 1 , 4 , NULL );
		//m_Bitmap.CreateCompatibleBitmap(pDC , r.Width() , r.Height() );
		m_pOldBitmap = (CBitmap*)m_memDC.SelectObject( &m_Bitmap );
		BITMAP bitmap;
		HBITMAP hBitmap = (HBITMAP)m_Bitmap.GetSafeHandle();
		GetObject(hBitmap, sizeof(BITMAP), &bitmap);

		BITMAPINFOHEADER  bmih;
		bmih.biSize = sizeof(BITMAPINFOHEADER);
		bmih.biWidth = bitmap.bmWidth;
		bmih.biHeight = -bitmap.bmHeight;
		bmih.biPlanes = 1;
		bmih.biBitCount = 32;
		bmih.biCompression = BI_RGB;
		bmih.biSizeImage = 0;
		bmih.biXPelsPerMeter = 0;
		bmih.biYPelsPerMeter = 0;
		bmih.biClrUsed = 0;
		bmih.biClrImportant = 0;


		HDC hDC = m_memDC.GetSafeHdc();
		LPVOID lpBits;
		m_hBitmap = CreateDIBSection(hDC, (LPBITMAPINFO)&bmih, DIB_RGB_COLORS, &lpBits, NULL, 0);
		GetDIBits(hDC, hBitmap, 0, bitmap.bmHeight, lpBits, (LPBITMAPINFO)&bmih, DIB_RGB_COLORS);
		m_memDC.SelectObject( m_hBitmap );
		m_pBitmap = lpBits;
	}

	CComboBox *pBox = (CComboBox*)pFrame->m_wndConfigDlg.GetDlgItem(IDC_COMBO_RESULT);
	m_memDC.SelectStockObject( WHITE_BRUSH );
	m_memDC.FillRect( r , NULL );
	if( pBox->GetCurSel() != 2 )
		m_memDC.BitBlt(0,0,m_BgBitmap.GetBitmapDimension().cx , m_BgBitmap.GetBitmapDimension().cy , &m_BgDC , 0 , 0 , SRCCOPY );
	else
	{
		CBrush b;
		b.CreateSolidBrush( RGB(128,128,128));
		CBrush *ob = (CBrush*)m_memDC.SelectObject( &b );
		m_memDC.FillRect( r , NULL );
		m_memDC.SelectObject( ob );
		b.DeleteObject();
	}
	

	

	CPen gridPen;
	LOGPEN lp;
	POINT pw;
	
	lp.lopnColor = RGB(128,128,128);
	pw.x = pw.y = 1;
	lp.lopnWidth = pw;
	lp.lopnStyle = PS_DOT;
	gridPen.CreatePenIndirect(&lp);
	CPen *oldpen = (CPen*) m_memDC.SelectObject(&gridPen);
	int i;
	for( i = 0 ; i < r.Width() ; i += GRID_SIZE )
	{
		m_memDC.MoveTo( i , 0 );
		m_memDC.LineTo( i , r.Height() - 1 );
	}
	for( i = 0 ; i < r.Height() ; i += GRID_SIZE )
	{
		m_memDC.MoveTo( 0 , i );
		m_memDC.LineTo( r.Width() - 1 , i );
	}
	m_memDC.SelectObject(oldpen);
	gridPen.DeleteObject();

	
	/*int gRed[3] = { 0 , 128 , 255 };
	int gGreen[3] = { 0 , 128 , 255 };
	int gBlue[3] = { 0 , 128 ,255 };*/

	int gRed[3] = { 255 , 180 , 0 };
	int gGreen[3] = { 255 , 180 , 0 };
	int gBlue[3] = { 255 , 180 , 0 };

	int rankval = 51;
	BITMAP b;
	GetObject(m_hBitmap, sizeof(BITMAP), &b);
	DWORD temp[1000];
	//m_Bitmap.GetBitmap( &b );
	//BITMAPINFO *bh = (BITMAPINFO*) m_pBitmap;
	//m_memDC.FillRect(CRect(0,0,100,100),NULL);
	DWORD *bitmap = (DWORD*)m_pBitmap;
	memcpy( temp , bitmap , 4000 );
	
	if( pBox->GetCurSel() == 1 ||pBox->GetCurSel() == 2)	//Reliability
	{
		for( int i = 0 ; i < GRID_H ; ++i )
		{
			for( int j = 0 ; j < GRID_W ; ++j )
			{
				if( pDoc->m_RssiDB[i][j].apnum > 0 && ( pBox->GetCurSel() == 2 || pDoc->m_RssiDB[i][j].trust * 100 >= TRUST_THRESHOLD ) )
				{
					CBrush eb;
					
					if( pDoc->m_RssiDB[i][j].trust == 1.0 )
					{
						int ddd = 0;
					}
					int rank = ((int)(pDoc->m_RssiDB[i][j].trust * 100)) / rankval;
					int rindex = ((int)(pDoc->m_RssiDB[i][j].trust * 100)) % rankval;
					int red = gRed[rank] + ( gRed[rank+1] - gRed[rank] ) * rindex / rankval;
					int green = gGreen[rank] + ( gGreen[rank+1] - gGreen[rank] ) * rindex / rankval;
					int blue = gBlue[rank] + ( gBlue[rank+1] - gBlue[rank]) * rindex / rankval;
					for( int k = i*GRID_SIZE ; k <= (i+1)*GRID_SIZE ; ++k )
					{
						if( k >= b.bmHeight ) continue;
						for( int l = j*GRID_SIZE ; l <= (j+1)*GRID_SIZE ; ++l )
						{
							
							COLORREF color = (COLORREF) bitmap[ k * b.bmWidth + l ];
							//COLORREF color = m_memDC.GetPixel( l , k );
							int ab = ( ( color & 0xFF ) * 0.3 + blue *0.7);// / 2;
							int ag = ( ( (color>>8) & 0xFF )*0.3 + green *0.7);// / 2;
							int ar = ( ( (color>>16) & 0xFF )*0.3 + red *0.7);// / 2;
							bitmap[ k * b.bmWidth + l ] = (DWORD)(RGB( ab,ag,ar ));
							//m_memDC.SetPixelV(l,k,RGB(ar,ag,ab));
							
		
						}
					}
					//m_memDC.GetPixel
					/*eb.CreateSolidBrush(RGB(red,green,blue) );
					CBrush *oldb = (CBrush*)m_memDC.SelectObject(&eb);
					CRect grid(j*GRID_SIZE+1,i*GRID_SIZE+1,(j+1)*GRID_SIZE,(i+1)*GRID_SIZE);
					//m_memDC.FillRect(grid,NULL);
					m_memDC.SelectObject(oldb);
					eb.DeleteObject();*/
				}
			}
		}
	}
	if( pBox->GetCurSel() == 2 )	//Similarity
	{
		for( int i = 0 ; i < GRID_H ; ++i )
		{
			for( int j = 0 ; j < GRID_W ; ++j )
			{
				if( pDoc->m_Similarity[i][j].state == 0 )continue;

				//CPen pen;
				//pen.CreatePen( PS_SOLID , 3 , RGB(rand()%256,rand()%256,rand()%256 ) );
				
				//CPen *op = (CPen*)m_memDC.SelectObject( &pen );
				DrawArrow( &m_memDC , 2 , RGB( 255 , 255 , 255 ) , RGB(0,0,0) , j*GRID_SIZE+GRID_SIZE/2,i*GRID_SIZE+GRID_SIZE/2,pDoc->m_Similarity[i][j].x*GRID_SIZE + GRID_SIZE / 2 , pDoc->m_Similarity[i][j].y * GRID_SIZE + GRID_SIZE / 2);
				//m_memDC.MoveTo(j*GRID_SIZE + GRID_SIZE / 2, i * GRID_SIZE + GRID_SIZE / 2);
				//m_memDC.LineTo(pDoc->m_Similarity[i][j].x*GRID_SIZE + GRID_SIZE / 2 , pDoc->m_Similarity[i][j].y * GRID_SIZE + GRID_SIZE / 2);
				//m_memDC.SelectObject(op);
				//pen.DeleteObject();
			}
		}
	}

	

	lp.lopnColor = RGB( 0 , 0 , 255 );
	pw.x = pw.y = 2;
	lp.lopnWidth = pw;
	gridPen.CreatePenIndirect(&lp);
	m_memDC.SelectObject(&gridPen);
	CPtrList *p = &pDoc->m_TrackPath1;
	POSITION pos = p->GetHeadPosition();
	int head = 1;
	while( pos )
	{
		PATH *path = (PATH*)p->GetNext(pos);
		
		if( head )
		{
			m_memDC.MoveTo(path->start.x * GRID_SIZE + GRID_SIZE / 2 , path->start.y * GRID_SIZE + GRID_SIZE / 2 );
			head = 0;
		}
		else
		{
			m_memDC.LineTo(path->start.x * GRID_SIZE + GRID_SIZE / 2 , path->start.y * GRID_SIZE + GRID_SIZE / 2 );
			m_memDC.MoveTo(path->start.x * GRID_SIZE + GRID_SIZE / 2 , path->start.y * GRID_SIZE + GRID_SIZE / 2 );
		}

	}
	m_memDC.SelectObject(oldpen);
	gridPen.DeleteObject();

	gridPen.CreatePen( PS_SOLID, 2 , RGB( 0 , 255, 0 ));
	m_memDC.SelectObject(oldpen);
	p = &pDoc->m_TrackPath2;
	pos = p->GetHeadPosition();
	head = 1;
	while( pos )
	{
		PATH *path = (PATH*)p->GetNext(pos);
		
		if( head )
		{
			m_memDC.MoveTo(path->start.x * GRID_SIZE + GRID_SIZE / 2 , path->start.y * GRID_SIZE + GRID_SIZE / 2 );
			head = 0;
		}
		else
		{
			m_memDC.LineTo(path->start.x * GRID_SIZE + GRID_SIZE / 2 , path->start.y * GRID_SIZE + GRID_SIZE / 2 );
			m_memDC.MoveTo(path->start.x * GRID_SIZE + GRID_SIZE / 2 , path->start.y * GRID_SIZE + GRID_SIZE / 2 );

		}
	}
	m_memDC.SelectObject(oldpen);
	gridPen.DeleteObject();

	/*pos = p->GetHeadPosition();
	while( pos )
	{
		PATH *path = (PATH*)p->GetNext(pos);
		m_memDC.MoveTo(path->start.x * GRID_SIZE + GRID_SIZE / 2 , path->start.y * GRID_SIZE + GRID_SIZE / 2 );m_memDC.LineTo(path->end.x * GRID_SIZE + GRID_SIZE / 2  , path->end.y * GRID_SIZE + GRID_SIZE / 2);
	}*/
	
	
	p = &pDoc->m_Path;
	lp.lopnColor = RGB( 255 , 0 , 0 );
	pw.x = pw.y = 1;
	lp.lopnWidth = pw;
	lp.lopnStyle = PS_DOT;
	gridPen.CreatePenIndirect(&lp);
	m_memDC.SelectObject(&gridPen);
	pos = p->GetHeadPosition();
	while( pos )
	{
		PATH *path = (PATH*)p->GetNext(pos);
		m_memDC.MoveTo(path->start.x * GRID_SIZE + GRID_SIZE / 2 + 2 , path->start.y * GRID_SIZE + GRID_SIZE / 2 + 2 );m_memDC.LineTo(path->end.x * GRID_SIZE + GRID_SIZE / 2 + 2 , path->end.y * GRID_SIZE + GRID_SIZE / 2 + 2);
		m_memDC.MoveTo(path->start.x * GRID_SIZE + GRID_SIZE / 2 + 2 , path->start.y * GRID_SIZE + GRID_SIZE / 2 + 3 );m_memDC.LineTo(path->end.x * GRID_SIZE + GRID_SIZE / 2 + 2 , path->end.y * GRID_SIZE + GRID_SIZE / 2 + 3);
		m_memDC.MoveTo(path->start.x * GRID_SIZE + GRID_SIZE / 2 + 3 , path->start.y * GRID_SIZE + GRID_SIZE / 2 + 2 );m_memDC.LineTo(path->end.x * GRID_SIZE + GRID_SIZE / 2 + 3 , path->end.y * GRID_SIZE + GRID_SIZE / 2 + 2);
		m_memDC.MoveTo(path->start.x * GRID_SIZE + GRID_SIZE / 2 + 3 , path->start.y * GRID_SIZE + GRID_SIZE / 2 + 3 );m_memDC.LineTo(path->end.x * GRID_SIZE + GRID_SIZE / 2 + 3 , path->end.y * GRID_SIZE + GRID_SIZE / 2 + 3);
	}

	/*pos = p->GetHeadPosition();
	while( pos )
	{
		PATH *path = (PATH*)p->GetNext(pos);
		m_memDC.MoveTo(path->start.x * GRID_SIZE + GRID_SIZE / 2 + 3, path->start.y * GRID_SIZE + GRID_SIZE / 2 + 3 );m_memDC.LineTo(path->end.x * GRID_SIZE + GRID_SIZE / 2 + 3 , path->end.y * GRID_SIZE + GRID_SIZE / 2 + 3);
	}

	pos = p->GetHeadPosition();
	while( pos )
	{
		PATH *path = (PATH*)p->GetNext(pos);
		m_memDC.MoveTo(path->start.x * GRID_SIZE + GRID_SIZE / 2 + 3, path->start.y * GRID_SIZE + GRID_SIZE / 2 + 2 );m_memDC.LineTo(path->end.x * GRID_SIZE + GRID_SIZE / 2 + 3 , path->end.y * GRID_SIZE + GRID_SIZE / 2 + 2);
	}*/
	gridPen.DeleteObject();
	
	
	m_memDC.SelectObject(oldpen);

	

	if( pDoc->m_nState == ST_LOC_MOBILE )// || pDoc->m_nState == ST_GROUNDTRUTH )
	{
		//gridPen.DeleteObject();
		lp.lopnColor = RGB( 255 , 0 , 0 );
		gridPen.CreatePen(PS_SOLID, 2 , RGB(0,0,0) );//CreatePenIndirect(&lp);
		m_memDC.SelectObject(&gridPen);
		pos = p->GetHeadPosition();
		POSITION cur = pFrame->m_VirtualINS.m_CurPos;
		while( pos )
		{
			PATH *path = (PATH*)p->GetNext(pos);
			
			if( pos == cur )
			{
				CPoint pt = pFrame->m_VirtualINS.GetPoint();
				m_memDC.MoveTo(path->start.x * GRID_SIZE + GRID_SIZE / 2 + 2 , path->start.y * GRID_SIZE + GRID_SIZE / 2 + 2 );
				m_memDC.LineTo(pt);
				break;
			}
			else
			{
				m_memDC.MoveTo(path->start.x * GRID_SIZE + GRID_SIZE / 2 + 2 , path->start.y * GRID_SIZE + GRID_SIZE / 2 + 2 );
				m_memDC.LineTo(path->end.x * GRID_SIZE + GRID_SIZE / 2 + 2 , path->end.y * GRID_SIZE + GRID_SIZE / 2 + 2 );
			}
			
		}
		m_memDC.SelectObject(oldpen);
		gridPen.DeleteObject();
		//if( pDoc->m_nState == ST_GROUNDTRUTH )
	//	{
	//		CRect cur(pDoc->m_Cursor.x * GRID_SIZE , pDoc->m_Cursor.y * GRID_SIZE , pDoc->m_Cursor.x * GRID_SIZE + GRID_SIZE , pDoc->m_Cursor.y * GRID_SIZE + GRID_SIZE);
	//		m_memDC.SelectStockObject( BLACK_BRUSH );
	//		m_memDC.FillRect( cur , NULL );
	//		m_memDC.SelectObject(oldpen);
	//	}
	}
	else //if( pDoc->m_nState == ST_LOCALIZATION )
	{
		
		CRect cursor(pDoc->m_Cursor.x * GRID_SIZE , pDoc->m_Cursor.y * GRID_SIZE , pDoc->m_Cursor.x * GRID_SIZE + GRID_SIZE , pDoc->m_Cursor.y * GRID_SIZE + GRID_SIZE);
		CRect result(pDoc->m_CurPos.x * GRID_SIZE , pDoc->m_CurPos.y * GRID_SIZE, pDoc->m_CurPos.x * GRID_SIZE + GRID_SIZE , pDoc->m_CurPos.y * GRID_SIZE + GRID_SIZE);
		CBrush b;
		b.CreateSolidBrush(RGB(255,0,0));
		
		CBrush *ob = (CBrush*) m_memDC.SelectObject( &b );
		m_memDC.FillRect( result , NULL );
		m_memDC.SelectObject(oldpen);
		m_memDC.SelectObject(ob);
		b.DeleteObject();
	}
	
	CRect cur(pDoc->m_Cursor.x * GRID_SIZE , pDoc->m_Cursor.y * GRID_SIZE , pDoc->m_Cursor.x * GRID_SIZE + GRID_SIZE + 1  , pDoc->m_Cursor.y * GRID_SIZE + GRID_SIZE+1);
	m_memDC.SelectStockObject( BLACK_PEN );
	m_memDC.SelectStockObject( NULL_BRUSH );
	m_memDC.Rectangle( cur );
	//m_memDC.FillRect( cur , NULL );
	m_memDC.SelectObject(oldpen);
	pDC->BitBlt(0,0,r.Width(),r.Height() , &m_memDC, 0 , 0 , SRCCOPY);
}


// CWifiLocView printing

BOOL CWifiLocView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CWifiLocView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CWifiLocView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CWifiLocView diagnostics

#ifdef _DEBUG
void CWifiLocView::AssertValid() const
{
	CView::AssertValid();
}

void CWifiLocView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWifiLocDoc* CWifiLocView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWifiLocDoc)));
	return (CWifiLocDoc*)m_pDocument;
}
#endif //_DEBUG


// CWifiLocView message handlers

int CWifiLocView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}

void CWifiLocView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CWifiLocDoc *pDoc = GetDocument();
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	if( pDoc->m_nState != ST_LOC_MOBILE )
		KillTimer(nIDEvent);
	if( pDoc->m_nState == ST_GROUNDTRUTH )
	{
		pDoc->m_Scanner.GetRSSIfromAirPCap((CListCtrl*)pFrame->m_wndConfigDlg.GetDlgItem(IDC_LIST_RSSI) , pDoc->m_RssiDB[pDoc->m_Cursor.y][pDoc->m_Cursor.x] );
		CPoint index = pDoc->m_Cursor;
		CListCtrl *pList = (CListCtrl*)pFrame->m_wndConfigDlg.GetDlgItem(IDC_LIST_FINGERPRINT);
		pList->DeleteAllItems();
		RSSIINFO *ri = &pDoc->m_RssiDB[index.y][index.x];
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
			
			_itow(ri->ap[i].count,(wchar_t *)text,10);
			iColumn = 2;
			pList->SetItemText( iRow,iColumn,(LPCTSTR)text);      //화면에 나타낸다
		}
		WCHAR text[100];
		int miss = 0;
		int count = 0;
		for( int i = 0 ; i < ri->apnum ; ++i )
		{
			miss += pDoc->m_Scanner.m_nScanCount - ri->ap[i].count;
			count += ri->ap[i].count;
		}
		ri->trust = count;
		wsprintf(text,_T("Done:%d,%d/%d(%d\%)") , count , miss , pDoc->m_Scanner.m_nScanCount * ri->apnum , miss * 100 / (pDoc->m_Scanner.m_nScanCount*ri->apnum) );

		AfxMessageBox(text);
	}
	else if( pDoc->m_nState == ST_LOC_STATIC )
	{
		memset( &pDoc->m_CurRssi , 0x00 , sizeof( RSSIINFO ) );
		pDoc->m_Scanner.GetRSSIfromAirPCap((CListCtrl*)pFrame->m_wndConfigDlg.GetDlgItem(IDC_LIST_RSSI) , pDoc->m_CurRssi );
		CPoint result;
		int score;
		pDoc->Localization(result,score);
		Invalidate(FALSE);
	}
	else if( pDoc->m_nState == ST_LOC_MOBILE )
	{
		CPoint prev = pFrame->m_VirtualINS.GetGrid();
		if( pFrame->m_VirtualINS.Move() )
		{
			//pDoc->m_nState = ST_CHECK;
			KillTimer(1);
			AfxMessageBox(_T("Localization Done!!" ) );
			pDoc->SaveMobileLocError();

		}
		CPoint cur = pFrame->m_VirtualINS.GetGrid();
		if( prev.x != cur.x || prev.y != cur.y )
		{
			memset( &pDoc->m_CurRssi , 0x00 , sizeof( RSSIINFO ) );
			pDoc->m_Scanner.GetRSSIfromAirPCap((CListCtrl*)pFrame->m_wndConfigDlg.GetDlgItem(IDC_LIST_RSSI) , pDoc->m_CurRssi );
			pDoc->m_Scanner.m_RssiInfo.apnum = 0;
			pDoc->m_Scanner.m_nScanCount = 0;
			CPoint result;
			int score;
			if( pDoc->MobileLocalization1(result,score) )
			{
				PATH *path = (PATH*)pDoc->m_TrackPath1.GetTail();
				result = path->start;
			}
			//else
			{
				PATH *np = new PATH;
				np->start.x = result.x;
				np->start.y = result.y;
				np->end.x = prev.x;
				np->end.y = prev.y;
				pDoc->m_TrackPath1.AddTail( np );
			}

			if( pDoc->MobileLocalization2(result,score) )
			{
				PATH *path = (PATH*)pDoc->m_TrackPath2.GetTail();
				result = path->start;
			}
			//else
			{
				PATH *np = new PATH;
				np->start.x = result.x;
				np->start.y = result.y;
				np->end.x = prev.x;
				np->end.y = prev.y;
				pDoc->m_TrackPath2.AddTail( np );
			}
			
		}
		Invalidate(FALSE);
	}
	CView::OnTimer(nIDEvent);
}

void CWifiLocView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CWifiLocDoc *pDoc = GetDocument();
	PATH *cur;
	point.x /= GRID_SIZE;
	point.y /= GRID_SIZE;
	if( pDoc->m_nState == ST_CHECK || pDoc->m_nState == ST_GROUNDTRUTH || pDoc->m_nState == ST_LOC_MOBILE || pDoc->m_nState == ST_LOC_STATIC )
		return;
	if( m_nPathStep == PATH_END )
		return;
	if( pDoc->m_Path.GetCount() && ( cur = (PATH*)pDoc->m_Path.GetTail() ) )
	{
		cur->end = point;
	}
	else
		m_nPathStep = PATH_DOING;
	PATH *next = new PATH;
	next->start = next->end = point;
	pDoc->m_Path.AddTail(next);
	Invalidate(FALSE);
	CView::OnLButtonUp(nFlags, point);
}

void CWifiLocView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CWifiLocDoc *pDoc = GetDocument();
	point.x /= GRID_SIZE;
	point.y /= GRID_SIZE;
	if( pDoc->m_nState == ST_CHECK || pDoc->m_nState == ST_GROUNDTRUTH || pDoc->m_nState == ST_LOC_MOBILE || pDoc->m_nState == ST_LOC_STATIC )
		return;
	PATH *tail = NULL;
	if( pDoc->m_Path.GetCount() )
		tail = (PATH*)pDoc->m_Path.GetTail();
	if(pDoc->m_Path.GetCount() && tail )
	{
		delete tail;
		pDoc->m_Path.RemoveTail();
		//m_nPathStep = PATH_END;
	}
	
	Invalidate(FALSE);
	CView::OnRButtonUp(nFlags, point);
}

void CWifiLocView::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CWifiLocDoc *pDoc = GetDocument();
	point.x /= GRID_SIZE;
	point.y /= GRID_SIZE;
	if( pDoc->m_nState == ST_CHECK || pDoc->m_nState == ST_GROUNDTRUTH || pDoc->m_nState == ST_LOC_MOBILE || pDoc->m_nState == ST_LOC_STATIC )
		return;
	
	PATH *tail = NULL;
	if( pDoc->m_Path.GetCount() )
		tail = (PATH*)pDoc->m_Path.GetTail();
	if(pDoc->m_Path.GetCount() && tail )
	{
		delete tail;
		pDoc->m_Path.RemoveTail();
		m_nPathStep = PATH_END;
	}
	
	Invalidate(FALSE);
	CView::OnMButtonUp(nFlags, point);
}


void CWifiLocView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CWifiLocDoc *pDoc = GetDocument();
	point.x /= GRID_SIZE;
	point.y /= GRID_SIZE;
	if( pDoc->m_nState == ST_CHECK || pDoc->m_nState == ST_GROUNDTRUTH || pDoc->m_nState == ST_LOC_MOBILE || pDoc->m_nState == ST_LOC_STATIC )
		return;
	
	
	if(m_nPathStep == PATH_DOING && pDoc->m_Path.GetCount() )
	{
		PATH *tail = (PATH*)pDoc->m_Path.GetTail();
		tail->end = point;
	}
	Invalidate(FALSE);
	CView::OnMouseMove(nFlags, point);
}

void CWifiLocView::OnDestroy()
{
	CView::OnDestroy();
	
	// TODO: Add your message handler code here
	m_memDC.DeleteDC();
	m_Bitmap.DeleteObject();
	m_BgDC.DeleteDC();
	m_BgBitmap.DeleteObject();
}

void CWifiLocView::UpdateCursorRSSI()
{
	CWifiLocDoc *pDoc = GetDocument();
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	CListCtrl *pList = (CListCtrl*)pFrame->m_wndConfigDlg.GetDlgItem(IDC_LIST_FINGERPRINT);
	pList->DeleteAllItems();
	RSSIINFO *ri = &pDoc->m_RssiDB[pDoc->m_Cursor.y][pDoc->m_Cursor.x];
	//RSSIINFO *ri = &pDoc->m_GlobalDB[pDoc->m_Cursor.y][pDoc->m_Cursor.x];
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
void CWifiLocView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CWifiLocDoc *pDoc = GetDocument();
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	//if( pDoc->m_nState == ST_CHECK )
	{
		CPoint index;
		index.x = point.x / GRID_SIZE;
		index.y = point.y / GRID_SIZE;
		pDoc->m_Cursor = index;

		UpdateCursorRSSI();
		
		Invalidate(FALSE);
	}
	CView::OnLButtonDown(nFlags, point);
}

void CWifiLocView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	if( m_bInitDC )
	{
		CRect r;
		GetClientRect(r);
		m_memDC.SelectObject( m_pOldBitmap );
		m_Bitmap.DeleteObject();
		m_memDC.DeleteDC();
		m_bInitDC = 0;
		//m_Bitmap.CreateCompatibleBitmap(pDC , r.Width() , r.Height() );
		//m_memDC.SelectObject( &m_Bitmap );
	}
	// TODO: Add your message handler code here
}


//BOOL CWifiLocView::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
//{
//	// TODO: Add your message handler code here and/or call default
//
//	return CView::OnCopyData(pWnd, pCopyDataStruct);
//}

BOOL CWifiLocView::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: Add your message handler code here and/or call default
	switch(pCopyDataStruct->dwData){
		case 0://MM_TEST_DATA:
        {
            CString str;
            char *p_data = (char *)pCopyDataStruct->lpData;
            str =  p_data;
            //AfxMessageBox(str);
        }
        break;
    }
	return CView::OnCopyData(pWnd, pCopyDataStruct);
}



void CWifiLocView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	CWifiLocDoc *pDoc = (CWifiLocDoc*)GetDocument();
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	//if( pDoc->m_nState == ST_GROUNDTRUTH || pDoc->m_nState == ST_LOCALIZATION )
	{
		
		switch( nChar )
		{
		case VK_UP:
			pDoc->m_Cursor.y--;
			if( pDoc->m_Cursor.y < 0 )
				pDoc->m_Cursor.y = 0;
			Invalidate(FALSE);
			break;
		case VK_DOWN:
			pDoc->m_Cursor.y++;
			if( pDoc->m_Cursor.y >= GRID_H )
				pDoc->m_Cursor.y = GRID_H - 1;
			Invalidate(FALSE);
			break;
		case VK_LEFT:
			pDoc->m_Cursor.x--;
			if( pDoc->m_Cursor.x < 0 )
				pDoc->m_Cursor.x = 0;
			Invalidate(FALSE);
			break;
		case VK_RIGHT:
			pDoc->m_Cursor.x++;
			if( pDoc->m_Cursor.x >= GRID_W )
				pDoc->m_Cursor.x = GRID_W - 1;
			Invalidate(FALSE);
			break;
		case VK_ESCAPE:
			pDoc->m_RssiDB[pDoc->m_Cursor.y][pDoc->m_Cursor.x].apnum = 0;
			break;
		case VK_RETURN:
			if( pDoc->m_nState == ST_GROUNDTRUTH )
				SetTimer( 1 , TIMER_GROUNDTRUTH , NULL );
			else if( pDoc->m_nState == ST_LOC_STATIC )
				SetTimer( 1 , pDoc->m_nLocTime , NULL );
			pDoc->m_Scanner.m_RssiInfo.apnum = 0;
			pDoc->m_Scanner.m_nScanCount = 0;
			//pDoc->m_Scanner.m_nChannel = 14;
			break;
		}
		UpdateCursorRSSI();
		/*if( pDoc->m_nState == ST_GROUNDTRUTH )
		{
			CPoint index = pDoc->m_Cursor;
			CListCtrl *pList = (CListCtrl*)pFrame->m_wndConfigDlg.GetDlgItem(IDC_LIST_FINGERPRINT);
			pList->DeleteAllItems();
			RSSIINFO *ri = &pDoc->m_RssiDB[index.y][index.x];
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
				
				_itow(ri->ap[i].count,(wchar_t *)text,10);
				iColumn = 2;
				pList->SetItemText( iRow,iColumn,(LPCTSTR)text);      //화면에 나타낸다
			}
		}*/
		
	}
	
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CWifiLocView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	/*CWifiLocDoc *pDoc = GetDocument();
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	pDoc->m_Compare.x = point.x / GRID_SIZE;
	pDoc->m_Compare.y = point.y / GRID_SIZE;

	RSSIINFO *r1 = &pDoc->m_RssiDB[ pDoc->m_Compare.y ][ pDoc->m_Compare.x ];
	RSSIINFO *r2 = &pDoc->m_GlobalDB[ pDoc->m_Cursor.y ][ pDoc->m_Cursor.x ];

	CompareRSSI(r1,r2);
	CListCtrl *pList = (CListCtrl*)pFrame->m_wndConfigDlg.GetDlgItem(IDC_LIST_FINGERPRINT);
	pFrame->ShowRSSI( pList , r1 );
	pList = (CListCtrl*)pFrame->m_wndConfigDlg.GetDlgItem(IDC_LIST_RSSI);
	pFrame->ShowRSSI( pList , r2 );*/
	CView::OnRButtonDown(nFlags, point);
}

void CWifiLocView::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CWifiLocDoc *pDoc = GetDocument();
	point.x /= GRID_SIZE;
	point.y /= GRID_SIZE;
	if( pDoc->m_nState == ST_CHECK )
		return;
	
	PATH *tail = (PATH*)pDoc->m_Path.GetTail();
	if(pDoc->m_Path.GetCount() && tail )
	{
		delete tail;
		pDoc->m_Path.RemoveTail();
		m_nPathStep = PATH_END;
	}

	/*CWifiLocDoc *pDoc = GetDocument();
	pDoc->m_Cursor.x = point.x / GRID_SIZE;
	pDoc->m_Cursor.y = point.y / GRID_SIZE;

	RSSIINFO *r1 = &pDoc->m_RssiDB[ pDoc->m_Cursor.y ][ pDoc->m_Cursor.x ];
	RSSIINFO *r2 = &pDoc->m_GlobalDB[ pDoc->m_Cursor.y ][ pDoc->m_Cursor.x ];
	CompareRSSI(r1,r2);
	Invalidate(FALSE);*/


	CView::OnRButtonDblClk(nFlags, point);
}

void CWifiLocView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	/*CWifiLocDoc *pDoc = GetDocument();
	pDoc->m_Cursor.x = point.x / GRID_SIZE;
	pDoc->m_Cursor.y = point.y / GRID_SIZE;

	RSSIINFO *r1 = &pDoc->m_CurRssi;
	RSSIINFO *r2 = &pDoc->m_GlobalDB[ pDoc->m_Cursor.y ][ pDoc->m_Cursor.x ];
	CompareRSSI(r1,r2);
	Invalidate(FALSE);*/
	CView::OnLButtonDblClk(nFlags, point);
}


void CWifiLocView::CompareRSSI(RSSIINFO *r1 , RSSIINFO *r2)
{
	CWifiLocDoc *pDoc = GetDocument();
	

	int count;
	int max = 0,maxindex = 0;
	int score = GetRssiSimilarity( *r1 , *r2 , &count ,&maxindex,&max);
	int score2=0;
	if( count )
		score2 = score/count;
	WCHAR text[100];
	wsprintf( text , _T("%d/%d=%d , max=%d(%s)") , score,count,score2 , max , r1->ap[maxindex].ssid );
	AfxMessageBox( text );

	/*double change = 0;
	int count = 0;
	for( int i = 0 ; i < r1->apnum ; ++i )
	{
		int find = 0;
		for( int j = 0 ; j < r2->apnum ; ++j )
		{
			if( wcscmp( r1->ap[i].ssid , r2->ap[j].ssid ) == 0 )
			{
				if( r1->ap[i].channel != r2->ap[j].channel )
				{
					double val = r1->ap[i].rssi - r2->ap[j].rssi;
					if( val > 0 )
						change += val;
					else
						change -= val;
					count++;
				}
				find = 1;
				r2->ap[j].count *= -1;
				break;
			}
		}
		if( !find )
		{
			double val = r1->ap[i].rssi;
			if( val > 0 )
				change += val;
			else
				change -= val;
			count++;
		}
	}
	for( int j = 0 ; j < r2->apnum ; ++j )
	{
		if( r2->ap[j].count < 0 )
		{
			r2->ap[j].count *= -1;
		}
		else
		{
			double val = r2->ap[j].rssi;
			if( val > 0 )
				change += val;
			else
				change -= val;
			count++;
		}
	}
	if( count )
		change /= count;
	WCHAR text[100];
	wsprintf( text , _T("%d") , (int)(change * 100) );
	AfxMessageBox( text );*/
	


}

#include <math.h>
void CWifiLocView::DrawArrow(CDC* pDC, int nWidth, COLORREF nColorIn, COLORREF nColorOut, int X1, int Y1, int X2, int Y2)
{
    double slopy, cosy, siny;
    double Par;
    CPoint m_One, m_Two;
    CPoint pt[3];
 
    Par = 13.0; //화살표 길이
 
    m_One.x = X1;
    m_One.y = Y1;
    m_Two.x = X2;
    m_Two.y = Y2;
 
    slopy = atan((double)(m_One.y - m_Two.y)/(m_One.x - m_Two.x));
	if( X1 < X2 )
	{
		cosy = -cos(slopy);
		siny = -sin(slopy);
	}
	else
	{
		cosy = cos(slopy);
		siny = sin(slopy);
	}
 
    pDC->SelectObject(CreatePen(PS_DOT , nWidth - 1, nColorIn));
    pDC->MoveTo(m_One.x, m_One.y);
    pDC->LineTo(m_Two.x, m_Two.y);
    pDC->MoveTo(m_One.x, m_One.y);
    DeleteObject(pDC->SelectObject(GetStockObject(BLACK_PEN)));
 
    pDC->SelectObject(GetStockObject(NULL_BRUSH));
 
    pDC->SelectObject(CreatePen(PS_SOLID, nWidth + 3, nColorOut));
    pDC->MoveTo(m_One.x, m_One.y);
    pDC->LineTo(m_Two.x, m_Two.y);
    DeleteObject(pDC->SelectObject(GetStockObject(BLACK_PEN)));
 
    pDC->SelectObject(CreatePen(PS_SOLID, nWidth, nColorIn));
    pDC->MoveTo(m_One.x, m_One.y);
    pDC->LineTo(m_Two.x, m_Two.y);
    DeleteObject(pDC->SelectObject(GetStockObject(BLACK_PEN)));
 
    pDC->MoveTo(m_One.x, m_One.y);
 
    pt[0].x = m_Two.x;
    pt[0].y = m_Two.y;
 
    pt[1].x = m_Two.x + int(Par * cosy - (Par / 2.0 * siny) + 0.5);
    pt[1].y = m_Two.y + int(Par * siny + (Par / 2.0 * cosy) + 0.5);
 
    pt[2].x = m_Two.x + int(Par * cosy + (Par / 2.0 * siny) + 0.5);
    pt[2].y = m_Two.y - int(Par / 2.0 * cosy - Par * siny + 0.5);
 
    pDC->SelectObject(GetStockObject(BLACK_BRUSH)); //BLACK_BRUSH
    pDC->SelectObject(CreatePen(PS_DOT, nWidth - 1, nColorIn));
    pDC->Polygon(pt, 3);
    DeleteObject(pDC->SelectObject(GetStockObject(BLACK_PEN)));
}