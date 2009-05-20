#pragma once

#define _WIN32_DCOM

//#include <windows.h>
#include <conio.h>
#include <objbase.h>
#include <rpcsal.h>
#include <objbase.h>
#include <msxml6.h>
#include <atlbase.h>
#include <iostream>
#include <iomanip>
// headers needed to use WLAN APIs 
//#include <wlanapi.h>
#include <airpcap.h>
#include "ieee80211_radiotap.h"
using namespace std;

// get win32 error from HRESULT
#define WIN32_FROM_HRESULT(hr)           \
    (SUCCEEDED(hr) ? ERROR_SUCCESS :    \
        (HRESULT_FACILITY(hr) == FACILITY_WIN32 ? HRESULT_CODE(hr) : (hr)))


#define PACKET_BUFFER_SIZE 256000		// Size of the user-level packet buffer
#define WAIT_INTERVAL_MS 100 
#define RSSI_BUFFER_SIZE	30

//#define MEASURE_VARIATION
#define MAX_AP 40

#define _CRT_SECURE_NO_DEPRECATE



struct cpack_state 
{
	UCHAR					*c_buf;
	UCHAR					*c_next;
	ULONG					 c_len;
};

//////////////////////////////////////////////////////////////////////
// Macros and functions to extract values from the radiotap header
//////////////////////////////////////////////////////////////////////
#define	BITNO_32(x) (((x) >> 16) ? 16 + BITNO_16((x) >> 16) : BITNO_16((x)))
#define	BITNO_16(x) (((x) >> 8) ? 8 + BITNO_8((x) >> 8) : BITNO_8((x)))
#define	BITNO_8(x) (((x) >> 4) ? 4 + BITNO_4((x) >> 4) : BITNO_4((x)))
#define	BITNO_4(x) (((x) >> 2) ? 2 + BITNO_2((x) >> 2) : BITNO_2((x)))
#define	BITNO_2(x) (((x) & 2) ? 1 : 0)
#define	BIT(n)	(1 << n)
#define	IS_EXTENDED(__p)	\
	    (EXTRACT_LE_32BITS(__p) & BIT(IEEE80211_RADIOTAP_EXT)) != 0

#define EXTRACT_LE_8BITS(p) (*(p))
#define EXTRACT_LE_16BITS(p) \
	((USHORT)((USHORT)*((const UCHAR *)(p) + 1) << 8 | \
		     (USHORT)*((const UCHAR *)(p) + 0)))
#define EXTRACT_LE_32BITS(p) \
	((ULONG)((ULONG)*((const UCHAR *)(p) + 3) << 24 | \
		     (ULONG)*((const UCHAR *)(p) + 2) << 16 | \
		     (ULONG)*((const UCHAR *)(p) + 1) << 8 | \
		     (ULONG)*((const UCHAR *)(p) + 0)))
#define EXTRACT_LE_64BITS(p) \
	((ULONGLONG)((ULONGLONG)*((const UCHAR *)(p) + 7) << 56 | \
		     (ULONGLONG)*((const UCHAR *)(p) + 6) << 48 | \
		     (ULONGLONG)*((const UCHAR *)(p) + 5) << 40 | \
		     (ULONGLONG)*((const UCHAR *)(p) + 4) << 32 | \
	             (ULONGLONG)*((const UCHAR *)(p) + 3) << 24 | \
		     (ULONGLONG)*((const UCHAR *)(p) + 2) << 16 | \
		     (ULONGLONG)*((const UCHAR *)(p) + 1) << 8 | \
		     (ULONGLONG)*((const UCHAR *)(p) + 0)))

typedef struct
{
	WCHAR	ssid[16];
	double	rssi;
	short	channel;
	short	count;
	unsigned int seq;
#ifdef MEASURE_VARIATION
	double	min;
	double max;
#endif
}APINFO;



typedef struct
{
	double trust;
	int apnum;
	APINFO ap[MAX_AP];
}RSSIINFO;
double GetRssiSimilarity(RSSIINFO a , RSSIINFO b , int *count = NULL , int *maxindex = NULL , int *maxval = NULL);
int GetLandmarkDegree(RSSIINFO a , RSSIINFO b );
int GetMaxRssi( RSSIINFO a );
class CWifiScanner
{

public:
	WCHAR m_szInterfaceName[100];
	int m_bEnd;
	int m_bStDev;
	PAirpcapHandle m_Ad;
	AirpcapDeviceDescription *m_AllDevs, *m_TmpDev;
	BYTE* m_PacketBuffer;
	HANDLE m_ReadEvent;
	RSSIINFO m_RssiInfo;
	RSSIINFO m_RssiBuffer;
	RSSIINFO m_RssiBuffer2;
	RSSIINFO m_devMax,m_devMin;
	CWinThread *m_pThread;
	int m_nScanDelay;
	int m_nScanCount;
	int m_nChannel;
public:
	CWifiScanner(void);
public:
	~CWifiScanner(void);

public:
	int Init();
	//void EnumInterface( CComboBox *box);
	void MergeRSSIInfo( RSSIINFO *dest , RSSIINFO *src );
	//void PrintNetworkInfo( __in PWLAN_AVAILABLE_NETWORK pNetwork , CListCtrl *pList , void *rssi );
	//void GetVisibleNetworkList( CListCtrl *pList , void *rssi );
	//void Scan();
	//void GetBssList(void *rssi);
	void RecordRSSI();
	int GetRSSIfromAirPCap(CListCtrl *pList , RSSIINFO &r);
	//int ParseWireshark(char *filename , RSSIINFO &r);
	int ParsePacket(ULONG size , int ch);
	//APINFO PrintFrameData(BYTE *Payload, UINT PayloadLen);
	

public:
	void MinMaxRSSI(void);
};
