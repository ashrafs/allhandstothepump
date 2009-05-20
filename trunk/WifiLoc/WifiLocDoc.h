// WifiLocDoc.h : interface of the CWifiLocDoc class
//
#include "WifiScanner.h"
#include "ipc_protocol.h"

#pragma once

#define ST_TRAINING					0
#define ST_LOC_STATIC		1
#define ST_LOC_MOBILE		2
#define ST_CHECK					3
#define ST_GROUNDTRUTH				4
#define ST_STDEV					5
#define ST_READY		6


#define GRID_SIZE	10
#define GRID_W	72
#define GRID_H	72

#define VIRTUAL_TRAINING

#define TIMER_LOCALIZATION	3000
#define TIMER_GROUNDTRUTH	10000
#define TIMER_STDEV			50000

#define SIMILAR_THRESHOLD	6.0

#define WALK_SPEED	1.3
#define TIMER_VALUE	500

#define INTEGRATION_OVERWRITE	0
#define	INTEGRATION_MERGE		1
#define INTEGRATION_RATING		2

#define FINGERPRINT_ERROR	5

#define INTEGRATION_THRESHOLD	50.0

#define TRUST_THRESHOLD	10
#define ERROR_THRESHOLD	10

#define GROUNDTRUTH_SAMPLESIZE	10
typedef struct
{
	CPoint start;
	CPoint end;
}PATH;

typedef struct
{
	double sx,sy;
	double rx,ry;
	double err;
	RSSIINFO cur,source,result;
}LOC_ERR;

typedef struct
{
	int state;
	int x ,y;
	int similarity;
	double err;
}SIMILARITY;

double GetErrorFromTrust( double trust );


double GetTrustFromError( double error );



class CWifiLocDoc : public CDocument
{
protected: // create from serialization only
	CWifiLocDoc();
	DECLARE_DYNCREATE(CWifiLocDoc)

public:
	
	CWifiScanner m_Scanner;
	int	m_nState;
	int m_nGridSize;
	int m_nBackupCount;
	CPtrList m_Path;
	CPtrList m_TrackPath1;
	CPtrList m_TrackPath2;
	CPtrList m_TrackPath3;
	RSSIINFO m_RssiDB[GRID_H][GRID_W];
	RSSIINFO m_CompareDB[GRID_H][GRID_W];
	SIMILARITY m_Similarity[GRID_H][GRID_W];
	RSSIINFO m_CurRssi;
	CPoint	m_CurPos;
	double m_CurTrust;
	CPoint m_Cursor;
	CPoint m_Compare;
	CPtrList m_LocErr;

	RSSIINFO m_GlobalDB[GRID_H][GRID_W];

	
	int m_nSampleSize;
	int m_nLocTime;
	
// Attributes
public:

// Operations
public:
	int Localization(CPoint &result,double &score);
	int MobileLocalization1(CPoint &result,double &score);
	int MobileLocalization2(CPoint &result,double &score);
	int MobileLocalization3(CPoint &result,double &score);
	void UploadData( int type );
	void BackupData();
	void DownloadData();
	void RecordRSSI();
// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CWifiLocDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	CRect GetSearchBox(void);
public:
	void ShowSimilarity(void);
public:
	void ShowReliability(void);
public:
	void SortByRSSI(void);
public:
	void SortByName(void);
public:
	void SetSimilarity(void);
public:
	void SaveMobileLocError(void);
public:
	void DeleteMalformedAP(RSSIINFO *r);
	RSSIINFO GetGridRSSI(RSSIINFO src[GRID_H][GRID_W], int x, int y);
	void	SetGridRSSI(RSSIINFO dest[GRID_H][GRID_W], int dx, int dy, RSSIINFO src[GRID_H][GRID_W], int sx, int sy);
	void	DeleteGridRSSI(RSSIINFO dest[GRID_H][GRID_W] , int x , int y );
public:
	void ShowRSSVariation(void);
public:
	int OpenCompareFile(CString pn);
public:
	void GridTest1(void);
	void GridTest2(void);
};


 