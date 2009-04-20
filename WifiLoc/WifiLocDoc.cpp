// WifiLocDoc.cpp : implementation of the CWifiLocDoc class
//

#include "stdafx.h"
#include "MainFrm.h"
#include "WifiLoc.h"

#include "WifiLocDoc.h"
#include "WifiLocView.h"
#include "ConfigDlg.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWifiLocDoc

IMPLEMENT_DYNCREATE(CWifiLocDoc, CDocument)

BEGIN_MESSAGE_MAP(CWifiLocDoc, CDocument)
END_MESSAGE_MAP()

#define M_PI       3.14159265358979323846

double GetErrorFromTrust( double trust )
{
	return  sqrt( 4.0 / (M_PI*trust) ); 
}

double GetTrustFromError( double error )
{
	return ( 4.0 / ( M_PI * error * error ) );//  (100 - error * 2 ) / 100;
}
// CWifiLocDoc construction/destruction

CWifiLocDoc::CWifiLocDoc()
{
	// TODO: add one-time construction code here

}

CWifiLocDoc::~CWifiLocDoc()
{
	POSITION pos = m_Path.GetHeadPosition();
	while(pos)
	{
		PATH* p = (PATH*)m_Path.GetNext(pos);
		delete p;
	}

	pos = m_TrackPath1.GetHeadPosition();
	while(pos)
	{
		PATH* p = (PATH*)m_TrackPath1.GetNext(pos);
		delete p;
	}
	pos = m_TrackPath2.GetHeadPosition();
	while(pos)
	{
		PATH* p = (PATH*)m_TrackPath2.GetNext(pos);
		delete p;
	}


	pos = m_LocErr.GetHeadPosition();
	while(pos)
	{
		LOC_ERR* p = (LOC_ERR*)m_LocErr.GetNext(pos);
		delete p;
	}
}

BOOL CWifiLocDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	m_nState = ST_READY;
	m_Cursor.x = m_Cursor.y = 0;
	memset(m_RssiDB , 0x00 , sizeof( RSSIINFO ) * GRID_W * GRID_H );
	/*for( int i = 0 ; i < GRID_H ; ++i )
	{
		for( int j = 0 ; j < GRID_W ; ++j )
		{
			m_RssiDB[i][j].apnum = 1;
			wcscpy( m_RssiDB[i][j].ap[0].ssid , _T("mobed") );
			m_RssiDB[i][j].ap[0].channel = m_RssiDB[i][j].ap[0].count = 1;
			m_RssiDB[i][j].ap[0].rssi = 50;
			CPoint ent( 0 , 0 );
			double d = sqrt( (double)((i - ent.y ) *(i-ent.y) + (j-ent.x)*(j-ent.x) ));
			double maxd = sqrt( (double)(GRID_W * GRID_W + GRID_H*GRID_H));
			m_RssiDB[i][j].trust = d * 100 / maxd;
		}
	}*/
	DownloadData();
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	
	/*CConfigDlg *dlg = new CConfigDlg;
	CWifiLocView *pView;
	POSITION pos = GetFirstViewPosition();
	pView = (CWifiLocView*)GetNextView(pos);
	dlg->CreateDlg(MAKEINTRESOURCE(IDD_DIALOGBAR),pView);*/
	//dlg->ShowWindow(SW_SHOW);
	//+		filename	0x0012f700 "D:\wiresharklog\ttt_00058_20090327112103"	char *
	m_nLocTime = 1500;
	m_Scanner.Init();

	m_Scanner.m_devMax.apnum = m_Scanner.m_devMin.apnum = 0;
	//m_Scanner.m_bStDev = 1;
	//RSSIINFO r;
	//r.apnum = 0;
	//m_Scanner.ParseWireshark("D:\\wiresharklog\\test2_00019_20090327113447" , r);
	

	//m_Scanner.GetRSSIfromWireshark();

	return TRUE;
}




// CWifiLocDoc serialization

void CWifiLocDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		if( m_nState == ST_LOC_MOBILE )
		{
			int pathnum = -1;
			POSITION pos;
			ar<<pathnum;
			pathnum = m_Path.GetCount();
			ar<<pathnum;
			pos = m_Path.GetHeadPosition();
			while(pos)
			{
				PATH *path = (PATH*)m_Path.GetNext(pos);
				ar.Write( path , sizeof(PATH));
			}

			pathnum = m_TrackPath1.GetCount();
			ar<<pathnum;
			pos = m_TrackPath1.GetHeadPosition();
			while(pos)
			{
				PATH *path = (PATH*)m_TrackPath1.GetNext(pos);
				ar.Write( path , sizeof(PATH));
			}

			pathnum = m_TrackPath2.GetCount();
			ar<<pathnum;
			pos = m_TrackPath2.GetHeadPosition();
			while(pos)
			{
				PATH *path = (PATH*)m_TrackPath2.GetNext(pos);
				ar.Write( path , sizeof(PATH));
			}

		}
		else
		{
			int pathnum = m_Path.GetCount();
			ar<<pathnum;
			POSITION pos = m_Path.GetHeadPosition();
			while(pos)
			{
				PATH *path = (PATH*)m_Path.GetNext(pos);
				ar.Write( path , sizeof(PATH));
			}
		}
		for(int i = 0 ; i < GRID_H ; ++i )
		{
			for( int j = 0 ; j < GRID_W ; ++j )
			{
				ar.Write( &m_RssiDB[i][j] , sizeof( RSSIINFO ) );
				
			}
		}
	}
	else
	{
		// TODO: add loading code here
		POSITION pos = m_Path.GetHeadPosition();
		while(pos)
		{
			PATH *path = (PATH*)m_Path.GetNext(pos);
			delete path;
		}
		m_Path.RemoveAll();

		pos = m_TrackPath1.GetHeadPosition();
		while(pos)
		{
			PATH *path = (PATH*)m_TrackPath1.GetNext(pos);
			delete path;
		}
		m_TrackPath1.RemoveAll();

		pos = m_TrackPath2.GetHeadPosition();
		while(pos)
		{
			PATH *path = (PATH*)m_TrackPath2.GetNext(pos);
			delete path;
		}
		m_TrackPath2.RemoveAll();

		int pathnum;
		ar>>pathnum;
		if( pathnum >= 0 )
		{
			for( int i = 0 ; i < pathnum ; ++i )
			{
				PATH *path = new PATH;
				ar.Read( path , sizeof(PATH));
				m_Path.AddTail( path );
			}
		}
		else
		{
			ar>>pathnum;
			for( int i = 0 ; i < pathnum ; ++i )
			{
				PATH *path = new PATH;
				ar.Read( path , sizeof(PATH));
				m_Path.AddTail( path );
			}
			ar>>pathnum;
			for( int i = 0 ; i < pathnum ; ++i )
			{
				PATH *path = new PATH;
				ar.Read( path , sizeof(PATH));
				m_TrackPath1.AddTail( path );
			}
			ar>>pathnum;
			for( int i = 0 ; i < pathnum ; ++i )
			{
				PATH *path = new PATH;
				ar.Read( path , sizeof(PATH));
				m_TrackPath2.AddTail( path );
			}

		}
		for(int i = 0 ; i < GRID_H ; ++i )
		{
			for( int j = 0 ; j < GRID_W ; ++j )
			{
				ar.Read( &m_RssiDB[i][j] , sizeof( RSSIINFO ) );
				if( m_RssiDB[i][j].apnum < 0 || m_RssiDB[i][j].apnum > MAX_AP )
					m_RssiDB[i][j].apnum = 0;
				
				/*typedef struct
				{
					int apnum;
					APINFO ap[20];
				}TEMP;
				TEMP temp;
				ar.Read( &temp , sizeof( TEMP ) );
				if( temp.apnum < 0 || temp.apnum > 20 )
					temp.apnum = 0;
				m_RssiDB[i][j].apnum = temp.apnum;
				memcpy( m_RssiDB[i][j].ap , temp.ap , sizeof( APINFO ) * 20 );
				m_RssiDB[i][j].trust = 100.0;*/

			}
		}
		pos = GetFirstViewPosition();
		CWifiLocView *pView = (CWifiLocView*)GetNextView(pos);
		pView->Invalidate(FALSE);
		if( m_Path.GetCount() == 0 )
			pView->m_nPathStep = PATH_DOING;
		else
			pView->m_nPathStep = PATH_END;
	}
}

/*int GetRssiSimilarity(RSSIINFO a , RSSIINFO b)
{
	int score = 0;
	RSSIINFO *c,*d;
	if( a.apnum > b.apnum )
	{
		c = &a;
		d = &b;
	}
	else
	{
		c = &b;
		d = &a;
	}
	for( int i = 0 ; i < c->apnum ; ++i )
	{
		APINFO *e = &c->ap[i];
		int find = 0;
		for( int j = 0 ; j < d->apnum ; ++j )
		{
			APINFO *f = &d->ap[j];
			if( wcscmp( e->ssid , f->ssid ) == 0 )
			{
				find = 1;
				score += abs(e->rssi - f->rssi);
				break;
			}
		}
		if( find == 0 )
			score += 1000;//e->rssi * 100;
	}
	return score;
}*/

int GetMaxRssi( RSSIINFO a )
{
	int max = 0;
	for( int i = 0 ; i < a.apnum ; ++i )
	{	
		if( a.ap[i].rssi > max )
			max = a.ap[i].rssi;
	}
	return max;
}
int GetRssiSimilarity(RSSIINFO a , RSSIINFO b , int *count , int *maxindex , int *maxval )
{
	int score = 0;
	RSSIINFO *c,*d;
	*count = 0;
	if( a.apnum == 0 )
		return 500000;
	if( b.apnum == 0 )
		return 500000;
	//else if( abs( a.apnum - b.apnum ) > 3 )
	//	return 5000*abs(a.apnum - b.apnum );
	//if( a.apnum > b.apnum )
	//{
	//	c = &b;
	//	d = &a;
	//}
	//else
	{
		c = &a;
		d = &b;
	}
	int nofindcount = 0;
	int nofindscore = 0;
	int max = 0;
	int mi = 0;
	for( int i = 0 ; i < c->apnum ; ++i )
	{
		APINFO *e = &c->ap[i];
		int find = 0;
		for( int j = 0 ; j < d->apnum ; ++j )
		{
			APINFO *f = &d->ap[j];
			if( wcscmp( e->ssid , f->ssid ) == 0 )
			{
				find = 1;
				score += abs(e->rssi - f->rssi);
				if( max < abs(e->rssi - f->rssi ))
				{
					max = abs(e->rssi - f->rssi);
					mi = i;
				}
				break;
			}
		}
		if( find == 0 )
		{
			nofindcount++;
			nofindscore += e->rssi;
		}
		//	score += e->rssi;
	}
	for( int i = 0 ; i < d->apnum ; ++i )
	{
		APINFO *e = &d->ap[i];
		int find = 0;
		for( int j = 0 ; j < c->apnum ; ++j )
		{
			APINFO *f = &c->ap[j];
			if( wcscmp( e->ssid , f->ssid ) == 0 )
			{
				find = 1;
				break;
			}
		}
		if( find == 0 )
		{
			nofindcount++;
			nofindscore += e->rssi;
		}
	}
	if( maxindex )
	{
		*maxindex = mi;
		*maxval = max;
	}
	//if( nofindcount * 100 / c->apnum > 20 )
	{
		score += nofindscore;
		if( count )
			*count = c->apnum;
	}
/*	else
	{
		if( count )
			*count = c->apnum - nofindcount;
	}*/
	//score += 
	return score;
}
int CWifiLocDoc::Localization(CPoint &result,int &score)
{
	int highscore = -1;
	double hightrust = 0.0;
	score = 0;
	//CPoint result;
	CRect box = GetSearchBox();
	box.top = 0;
	box.bottom = GRID_H-1;
	box.left = 0;
	box.right = GRID_W - 1;
	for( int i = box.top ; i <= box.bottom ; ++i )
	{
		for( int j = box.left ; j <= box.right ; ++j )
		{
			int count;
			int sc = GetRssiSimilarity(m_CurRssi,m_GlobalDB[i][j] , &count);
			double trust = m_GlobalDB[i][j].trust;
			if( count )
				sc /= count;
			if( highscore < 0 || sc < highscore )//&& trust * 100 >= TRUST_THRESHOLD )
			//if( highscore < 0 || sc < highscore - SIMILAR_THRESHOLD || sc >= highscore-SIMILAR_THRESHOLD && sc <= highscore+SIMILAR_THRESHOLD && trust > hightrust )
			{
				result.x = j;
				result.y = i;
				highscore = sc;
				hightrust = trust;
			}
		}
	}
	score = highscore;
	if( highscore > 1000 || highscore < 0 )
	{
		return -1;
	}
	LOC_ERR *err = new LOC_ERR;
	err->sx = m_Cursor.x;
	err->sy = m_Cursor.y;
	err->rx = result.x;
	err->ry = result.y;
	err->err = sqrt( (double)(err->sx-err->rx) * 2 * (err->sx-err->rx)*2 + (err->sy-err->ry)*2*(err->sy-err->ry)*2 );
	memcpy( &err->cur , &m_CurRssi , sizeof( RSSIINFO ) );
	memcpy( &err->source , &m_GlobalDB[err->sy][err->sx] , sizeof( RSSIINFO ) );
	memcpy( &err->result , &m_GlobalDB[err->ry][err->rx] , sizeof( RSSIINFO ) );
	m_LocErr.AddTail( err );
	
	
	if( m_nState != ST_TRAINING )
	{
		m_CurPos.x = result.x;// * GRID_SIZE;
		m_CurPos.y = result.y;// * GRID_SIZE;
	}
	return 0;
}

int CWifiLocDoc::MobileLocalization1(CPoint &result,int &score)
{
	int highscore = -1;
	double hightrust = 0.0;
	score = 0;
	//CPoint result;
	CRect box = GetSearchBox();
	box.top = 0;
	box.bottom = GRID_H-1;
	box.left = 0;
	box.right = GRID_W - 1;
	for( int i = box.top ; i <= box.bottom ; ++i )
	{
		for( int j = box.left ; j <= box.right ; ++j )
		{
			int count;
			
			double trust = m_GlobalDB[i][j].trust;
			int sc = GetRssiSimilarity(m_CurRssi,m_GlobalDB[i][j] , &count);

			if( trust < 0.8 )
				continue;
			if( count )
				sc /= count;
			if( highscore < 0 || sc < highscore  )
			//if( highscore < 0 || score < highscore - SIMILAR_THRESHOLD || score >= highscore-SIMILAR_THRESHOLD && score <= highscore+SIMILAR_THRESHOLD && trust > hightrust )
			{
				result.x = j;
				result.y = i;
				highscore = sc;
				hightrust = trust;
			}
		}
	}
	score = highscore;
	if( highscore > 1000 || highscore < 0 )
	{
		return -1;
	}
	/*LOC_ERR *err = new LOC_ERR;
	err->sx = m_Cursor.x;
	err->sy = m_Cursor.y;
	err->rx = result.x;
	err->ry = result.y;
	err->err = sqrt( (double)(err->sx-err->rx) * 2 * (err->sx-err->rx)*2 + (err->sy-err->ry)*2*(err->sy-err->ry)*2 );
	memcpy( &err->cur , &m_CurRssi , sizeof( RSSIINFO ) );
	memcpy( &err->source , &m_GlobalDB[err->sy][err->sx] , sizeof( RSSIINFO ) );
	memcpy( &err->result , &m_GlobalDB[err->ry][err->rx] , sizeof( RSSIINFO ) );
	m_LocErr.AddTail( err );*/
	
	
	if( m_nState != ST_TRAINING )
	{
		m_CurPos.x = result.x;// * GRID_SIZE;
		m_CurPos.y = result.y;// * GRID_SIZE;
	}
	return 0;
}
int CWifiLocDoc::MobileLocalization2(CPoint &result,int &score)
{
	int highscore = -1;
	double hightrust = 0.0;
	score = 0;
	//CPoint result;
	CRect box = GetSearchBox();
	/*box.top = 0;
	box.bottom = GRID_H-1;
	box.left = 0;
	box.right = GRID_W - 1;*/
	for( int i = box.top ; i <= box.bottom ; ++i )
	{
		for( int j = box.left ; j <= box.right ; ++j )
		{
			int count;
			int score = GetRssiSimilarity(m_CurRssi,m_RssiDB[i][j] , &count);
			double trust = m_RssiDB[i][j].trust;
			if( count )
				score /= count;
			if( highscore < 0 || score < highscore )
			//if( highscore < 0 || score < highscore - SIMILAR_THRESHOLD || score >= highscore-SIMILAR_THRESHOLD && score <= highscore+SIMILAR_THRESHOLD && trust > hightrust )
			{
				result.x = j;
				result.y = i;
				highscore = score;
				hightrust = trust;
			}
		}
	}
	score = highscore;
	if( highscore > 1000 || highscore < 0 )
	{
		return -1;
	}
	/*LOC_ERR *err = new LOC_ERR;
	err->sx = m_Cursor.x;
	err->sy = m_Cursor.y;
	err->rx = result.x;
	err->ry = result.y;
	err->err = sqrt( (double)(err->sx-err->rx) * 2 * (err->sx-err->rx)*2 + (err->sy-err->ry)*2*(err->sy-err->ry)*2 );
	memcpy( &err->cur , &m_CurRssi , sizeof( RSSIINFO ) );
	memcpy( &err->source , &m_RssiDB[err->sy][err->sx] , sizeof( RSSIINFO ) );
	memcpy( &err->result , &m_RssiDB[err->ry][err->rx] , sizeof( RSSIINFO ) );
	m_LocErr.AddTail( err );*/
	
	
	if( m_nState != ST_TRAINING )
	{
		m_CurPos.x = result.x;// * GRID_SIZE;
		m_CurPos.y = result.y;// * GRID_SIZE;
	}
	return 0;
}
void CWifiLocDoc::DownloadData()
{
	/*int pathnum = m_Path.GetCount();
	ar<<pathnum;
	POSITION pos = m_Path.GetHeadPosition();
	while(pos)
	{
		PATH *path = (PATH*)m_Path.GetNext(pos);
		ar.Write( path , sizeof(PATH));
	}
	for(int i = 0 ; i < GRID_H ; ++i )
	{
		for( int j = 0 ; j < GRID_W ; ++j )
		{
			ar.Write( &m_RssiDB[i][j] , sizeof( RSSIINFO ) );
			
		}
	}*/
	CFile f;
	if( f.Open( _T("d:\\wifiloc\\globalFingerprint") , CFile::modeRead , 0 ) || f.Open( _T("f:\\globalFingerprint") , CFile::modeRead , 0 ) )
	{
		int pathnum;
		f.Read(&pathnum , sizeof(int) );
		for( int i = 0 ; i < GRID_H ; ++i )
			for( int j = 0 ; j < GRID_W ; ++j )
				f.Read(&m_GlobalDB[i][j], sizeof(RSSIINFO) );
		f.Close();
	}
	else
	{
		for( int i = 0 ; i < GRID_H ; ++i )
			for( int j = 0 ; j < GRID_W ; ++j )
				memset(&m_GlobalDB[i][j], 0x00, sizeof(RSSIINFO) );
	}
}

void CWifiLocDoc::UploadData( int type )
{
	if( type == INTEGRATION_OVERWRITE )
	{
		for( int i = 0 ; i < GRID_H ; ++i )
		{
			for( int j = 0 ; j < GRID_W ; ++j )
			{
				if( m_RssiDB[i][j].apnum == 0 )continue;
				if( m_GlobalDB[i][j].apnum == 0 || m_RssiDB[i][j].trust >= m_GlobalDB[i][j].trust )
				{
					m_GlobalDB[i][j] = m_RssiDB[i][j];
				}
			}
		}
	}
	else if( type == INTEGRATION_MERGE )
	{
		//±âÁ¸ ²¨ Áß¿¡ Èí¼ö µÉ ³»¿ë ÀÖÀ¸¸é Èí¼öµÊ
		for( int i = 0 ; i < GRID_H ; ++i )
		{
			
			for( int j = 0 ; j < GRID_W ; ++j )
			{
				
				RSSIINFO *g = &m_GlobalDB[i][j];
				RSSIINFO *r = &m_RssiDB[i][j];
				double err = GetErrorFromTrust(g->trust) / 2;
				CRect box(j-err,i-err,j+err,i+err);
				if( box.left <0 )
					box.left = 0;
				if( box.right >= GRID_W )
					box.right = GRID_W - 1;
				if( box.top < 0 )
					box.top = 0;
				if( box.bottom >= GRID_H )
					box.bottom = GRID_H - 1;
				for( int ii = box.top ; ii <= box.bottom ; ++ii )
				{
					for(int jj = box.left ; jj <= box.right ; ++jj )
					{
						RSSIINFO *rr = &m_RssiDB[ii][jj];
						if( rr->trust > g->trust )
						{
							int count;
							int s = GetRssiSimilarity( *g , *rr , &count);
							if( count )
								s /= count;
							if( s < SIMILAR_THRESHOLD )
							{
								g->apnum = 0;
							}
						}
					}
				}
				
				
			}
		}
		for( int i = 0 ; i < GRID_H ; ++i )
		{
			for( int j = 0 ; j < GRID_W ; ++j )
			{
				if( m_RssiDB[i][j].apnum == 0 )continue;
				if( m_GlobalDB[i][j].apnum == 0 || m_RssiDB[i][j].trust >= m_GlobalDB[i][j].trust )
				{
					m_GlobalDB[i][j] = m_RssiDB[i][j];
				}
			}
		}
		
	}
	else if( type == INTEGRATION_RATING )
	{
		for( int i = 0 ; i < GRID_H ; ++i )
		{
			
			for( int j = 0 ; j < GRID_W ; ++j )
			{
				RSSIINFO *r = &m_RssiDB[i][j];
				RSSIINFO *g = &m_GlobalDB[i][j];
				double total = r->trust + g->trust;
				if( r->trust >= g->trust || r->trust >= INTEGRATION_THRESHOLD )
				{
					for( int k = 0 ; k < r->apnum ; ++k )
					{
						int find = 0;
						for( int l = 0 ; l < g->apnum ; ++l )
						{
							if( wcscmp(r->ap[k].ssid , g->ap[l].ssid ) == 0 )
							{
								find = 1;
								g->ap[l].rssi = ( r->ap[k].rssi * r->trust + g->ap[l].rssi * g->trust ) / total;
								g->ap[l].count = r->ap[k].count + g->ap[l].count;
								break;
							}
						}
						if( !find && g->apnum < MAX_AP )
						{
							g->ap[g->apnum++] = r->ap[k];
						}
					}	
				}
			}
		}
	}

	BackupData();
	CFile f;
	if( !f.Open( _T("d:\\wifiloc\\globalFingerprint") , CFile::modeWrite|CFile::modeCreate , 0 ) )
		f.Open( _T("f:\\wifiloc\\globalFingerprint") , CFile::modeWrite|CFile::modeCreate , 0 );
	int pathnum = 0;
	f.Write(&pathnum , sizeof(int) );
	for( int i = 0 ; i < GRID_H ; ++i )
		for( int j = 0 ; j < GRID_W ; ++j )
			f.Write(&m_GlobalDB[i][j], sizeof(RSSIINFO) );
	f.Close();


}



void CWifiLocDoc::BackupData()
{
	CFile source,dest;
	WCHAR filename[100];
	SYSTEMTIME time;
	::GetLocalTime( &time );
	
	if( !source.Open( _T("d:\\wifiloc\\globalDB") , CFile::modeRead , 0 ) )
	{
		if( !source.Open( _T("f:\\wifiloc\\globalDB") , CFile::modeRead , 0 ) )
			return;
	}
	wsprintf(filename,_T("d:\\wifiloc\\globalDB_%02d%02d%02d%02d%02d") , time.wMonth , time.wDay,time.wHour,time.wMinute , time.wSecond );
	if( !dest.Open( filename , CFile::modeWrite|CFile::modeCreate , 0 ) )
	{
		wsprintf(filename,_T("f\\wifiloc\\globalDB_%02d%02d%02d%02d%02d") , time.wMonth , time.wDay,time.wHour,time.wMinute , time.wSecond );
		dest.Open( filename , CFile::modeWrite|CFile::modeCreate , 0 );
	}
	int size = source.GetLength();
	int w = 0;
	char buffer[1024];
	while( w < size )
	{
		int r = source.Read(buffer,1024);
		dest.Write(buffer,r);
		w+=r;
	}
	source.Close();
	dest.Close();
}
void CWifiLocDoc::RecordRSSI()
{

	if( m_CurPos.x < 0 || m_CurPos.x >=GRID_W )
		return;
	if( m_CurPos.y < 0 || m_CurPos.x >=GRID_H )
		return;
	RSSIINFO *r = &m_RssiDB[m_CurPos.y][m_CurPos.x];
	RSSIINFO m;
	m.apnum = 0;
	if( m_Scanner.m_RssiInfo.apnum == 0 )
		m = m_Scanner.m_RssiBuffer2;
	else
		m = m_Scanner.m_RssiInfo;
	m_Scanner.MergeRSSIInfo(&m,&m_Scanner.m_RssiBuffer );
	if( r->apnum == 0 || m_CurTrust >= r->trust )
	{
		*r = m;
		r->trust = m_CurTrust;
		/*for( int i = 0 ; i < m_Scanner.m_RssiBuffer.apnum ; ++i )
		{
			int find = 0;
			WCHAR wid[30];
			wcscpy( wid , m_Scanner.m_RssiBuffer.ap[i].ssid );
			double rssi = m_Scanner.m_RssiBuffer.ap[i].rssi;
			int ch = m_Scanner.m_RssiBuffer.ap[i].channel;
			for( int j = 0 ; j < m_Scanner.m_RssiInfo.apnum ; ++j )
			{
				if( wcscmp( wid , m_Scanner.m_RssiInfo.ap[j].ssid ) == 0 )
				{
					find = 1;
					if( m_Scanner.m_RssiInfo.ap[j].channel == ch )
					{
						m_Scanner.m_RssiInfo.ap[j].rssi = m_Scanner.m_RssiInfo.ap[j].rssi * m_Scanner.m_RssiInfo.ap[j].count + rssi;
						m_Scanner.m_RssiInfo.ap[j].count++;
						m_Scanner.m_RssiInfo.ap[j].rssi /= m_Scanner.m_RssiInfo.ap[j].count;
					}
					else
					{
						if( m_Scanner.m_RssiInfo.ap[j].rssi < rssi )
						{
							m_Scanner.m_RssiInfo.ap[j].rssi = rssi;
							m_Scanner.m_RssiInfo.ap[j].count = 1;
							m_Scanner.m_RssiInfo.ap[j].channel = ch;
						}
					}
					break;
				}
			}
			if( find == 0 )
			{
				int apnum = m_Scanner.m_RssiInfo.apnum;
				m_Scanner.m_RssiInfo.ap[apnum].rssi = rssi;
				m_Scanner.m_RssiInfo.ap[apnum].count = 1;
				m_Scanner.m_RssiInfo.ap[apnum].channel = ch;
				wcscpy( m_Scanner.m_RssiInfo.ap[apnum].ssid , wid );
				m_Scanner.m_RssiInfo.apnum++;
			}
		}*/
		//m_Scanner.m_RssiInfo.apnum = 0;
	}

}

// CWifiLocDoc diagnostics

#ifdef _DEBUG
void CWifiLocDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWifiLocDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CWifiLocDoc commands

CRect CWifiLocDoc::GetSearchBox(void)
{
	CRect box;

	double vmax = 3;
	double err = GetErrorFromTrust(m_CurTrust);//(100 - m_CurTrust * 100 ) / 2;
	double r = ( vmax + err + 1.9 ) / 2;
	box.top = m_CurPos.y - r;
	box.left = m_CurPos.x - r;
	box.bottom = m_CurPos.y + r;
	box.right = m_CurPos.x + r;
	if( box.top < 0 )
		box.top = 0;
	if( box.left < 0 )
		box.left = 0;
	if( box.right >= GRID_W )
		box.right = GRID_W - 1;
	if( box.bottom >= GRID_H )
		box.bottom = GRID_H - 1;
	return box;
}

void CWifiLocDoc::ShowSimilarity(void)
{
}

void CWifiLocDoc::ShowReliability(void)
{
}

void CWifiLocDoc::SortByRSSI(void)
{
	for( int i = 0 ; i < GRID_H ; ++i )
	{
		for( int j = 0 ; j < GRID_W ; ++j )
		{
			RSSIINFO *r = &m_RssiDB[i][j];
			if( r->apnum == 0 ) continue;
			RSSIINFO temp;
			int count = 0;
			while( count < r->apnum )
			{
				int max = 0;
				int maxindex = 0;
				for( int k = 0 ; k < r->apnum ; ++k )
				{
					if( r->ap[k].rssi > max )
					{
						max = r->ap[k].rssi;
						maxindex = k;
					}
				}
				temp.ap[count++] = r->ap[maxindex];
				r->ap[maxindex].rssi = -1;
			}
			temp.trust = r->trust;
			temp.apnum = r->apnum;
			*r = temp;
		}
	}
}


void CWifiLocDoc::SortByName(void)
{
	for( int i = 0 ; i < GRID_H ; ++i )
	{
		for( int j = 0 ; j < GRID_W ; ++j )
		{
			RSSIINFO *r = &m_RssiDB[i][j];
			if( r->apnum == 0 ) continue;
			int count = 0;
			for( int k = 0 ; k < r->apnum ; ++k )
			{
				int minindex = k;
				for( int l = k + 1 ; l < r->apnum ; ++l )
				{
					if( wcscmp( r->ap[minindex].ssid , r->ap[l].ssid ) < 0 )
					{
						minindex = l;
					}
				}
				APINFO temp;
				temp = r->ap[k];
				r->ap[k]=r->ap[minindex];
				r->ap[minindex]=temp;
			}
		}
	}
}

void CWifiLocDoc::SetSimilarity(void)
{
	int num = 0;
	double val = 0;
	double min = 100000.0;
	double max = 0.0;
	int totalscore = 0;
	for( int i = 0 ; i < GRID_H ; ++i )
	{
		for( int j = 0 ; j < GRID_W ; ++j )
		{
			int score;
			int count = 0;
			int highscore = -1;
			int x = 0;
			int y = 0;
			if( m_RssiDB[i][j].apnum > 0 )
			{
				for( int k = 0 ; k < GRID_H ; ++k )
				{
					for( int l = 0 ; l < GRID_W ; ++l )
					{
						if( m_GlobalDB[k][l].trust * 100 < TRUST_THRESHOLD )
							continue;
						score = GetRssiSimilarity( m_GlobalDB[k][l] , m_RssiDB[i][j] , &count );
						if( count )
							score /= count;
						if( highscore < 0 || score < highscore )
						{
							highscore = score;
							x = l;
							y = k;
						}
					}
				}
				m_Similarity[i][j].x = x;
				m_Similarity[i][j].y = y;
				m_Similarity[i][j].similarity = highscore;
				m_Similarity[i][j].err = sqrt( (double)(x-j)*(x-j) + (y-i)*(y-i)) * 2;
				m_Similarity[i][j].state = 1;
				num++;
				totalscore += highscore;
				val += m_Similarity[i][j].err;
				if( m_Similarity[i][j].err > max )
					max = m_Similarity[i][j].err;
				if( m_Similarity[i][j].err < min )
					min = m_Similarity[i][j].err;
			}
			else
			{
				m_Similarity[i][j].state = 0;
			}
		}
	}
	WCHAR text[100];
	wsprintf( text , _T("avg rssi differ = %d, avg dist= %d , min = %d , max = %d") , totalscore * 100 / num , (int)(val * 100 / num ) , (int)(min * 100 ) , (int)(max*100) );
	AfxMessageBox(text);
}

void CWifiLocDoc::SaveMobileLocError(void)
{
	
	double err1 = 0;
	int count1 = 0;
	POSITION pos1 = m_TrackPath1.GetHeadPosition();
	m_TrackPath1.GetNext(pos1);
	while( pos1 )
	{
		PATH *p1 = (PATH*)m_TrackPath1.GetNext(pos1);
		CPoint s1 = p1->start;
		CPoint s2 = p1->end;
		double e = sqrt( (double)((s1.x-s2.x)*(s1.x-s2.x)*4 + (s1.y-s2.y)*(s1.y-s2.y)*4 ) );
		err1 += e;
		count1++;
	}

	
	
	double err2 = 0;
	int count2 = 0;
	pos1 = m_TrackPath2.GetHeadPosition();
	m_TrackPath2.GetNext(pos1);
	while( pos1 )
	{
		PATH *p1 = (PATH*)m_TrackPath2.GetNext(pos1);
		CPoint s1 = p1->start;
		CPoint s2 = p1->end;
		double e = sqrt( (double)((s1.x-s2.x)*(s1.x-s2.x)*4 + (s1.y-s2.y)*(s1.y-s2.y)*4 ) );
		err2 += e;
		count2++;
	}
	WCHAR text[100];
	wsprintf(text,_T("globalmap=%d,groundtruth=%d") ,(int)( err1/count1 * 1000 ),(int)( err2/count2 * 1000 ) );
	AfxMessageBox(text);


	

}
