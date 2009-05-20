#include "StdAfx.h"
#include "WifiScanner.h"

#include "WifiLocDoc.h"

#include <time.h>
//
// Utility functions
//

// get interface state string
/*LPWSTR
GetInterfaceStateString(
    __in WLAN_INTERFACE_STATE wlanInterfaceState
)
{
    LPWSTR strRetCode;

    switch(wlanInterfaceState)
    {
        case wlan_interface_state_not_ready:
            strRetCode = L"\"not ready\"";
            break;
        case wlan_interface_state_connected:
            strRetCode = L"\"connected\"";
            break;
        case wlan_interface_state_ad_hoc_network_formed:
            strRetCode = L"\"ad hoc network formed\"";
            break;
        case wlan_interface_state_disconnecting:
            strRetCode = L"\"disconnecting\"";
            break;
        case wlan_interface_state_disconnected:
            strRetCode = L"\"disconnected\"";
            break;
        case wlan_interface_state_associating:
            strRetCode = L"\"associating\"";
            break;
        case wlan_interface_state_discovering:
            strRetCode = L"\"discovering\"";
            break;
        case wlan_interface_state_authenticating:
            strRetCode = L"\"authenticating\"";
            break;
        default:
            strRetCode = L"\"invalid interface state\"";
    }

    return strRetCode;
}

// get ACM notification string
LPWSTR
GetAcmNotificationString(
    __in DWORD acmNotif
)
{
    LPWSTR strRetCode;

    switch(acmNotif)
    {
        case wlan_notification_acm_autoconf_enabled:
            strRetCode = L"\"autoconf enabled\"";
            break;
        case wlan_notification_acm_autoconf_disabled:
            strRetCode = L"\"autoconf disabled\"";
            break;
        case wlan_notification_acm_background_scan_enabled:
            strRetCode = L"\"background scan enabled\"";
            break;
        case wlan_notification_acm_background_scan_disabled:
            strRetCode = L"\"background scan disabled\"";
            break;
        case wlan_notification_acm_power_setting_change:
            strRetCode = L"\"power setting change\"";
            break;
        case wlan_notification_acm_scan_complete:
            strRetCode = L"\"scan complete\"";
            break;
        case wlan_notification_acm_scan_fail:
            strRetCode = L"\"scan fail\"";
            break;
        case wlan_notification_acm_connection_start:
            strRetCode = L"\"connection start\"";
            break;
        case wlan_notification_acm_connection_complete:
            strRetCode = L"\"connection complete\"";
            break;
        case wlan_notification_acm_connection_attempt_fail:
            strRetCode = L"\"connection fail\"";
            break;
        case wlan_notification_acm_filter_list_change:
            strRetCode = L"\"filter list change\"";
            break;
        case wlan_notification_acm_interface_arrival:
            strRetCode = L"\"interface arrival\"";
            break;
        case wlan_notification_acm_interface_removal:
            strRetCode = L"\"interface removal\"";
            break;
        case wlan_notification_acm_profile_change:
            strRetCode = L"\"profile change\"";
            break;
        case wlan_notification_acm_profiles_exhausted:
            strRetCode = L"\"profiles exhausted\"";
            break;
        case wlan_notification_acm_network_not_available:
            strRetCode = L"\"network not available\"";
            break;
        case wlan_notification_acm_network_available:
            strRetCode = L"\"network available\"";
            break;
        case wlan_notification_acm_disconnecting:
            strRetCode = L"\"disconnecting\"";
            break;
        case wlan_notification_acm_disconnected:
            strRetCode = L"\"disconnected\"";
            break;
        case wlan_notification_acm_adhoc_network_state_change:
            strRetCode = L"\"ad hoc network state changes\"";
            break;
        default:
            strRetCode = L"\"unknown ACM notification\"";
    }

    return strRetCode;
}

// get MSMM notification string
LPWSTR
GetMsmNotificationString(
    __in DWORD msmNotif
)
{
    LPWSTR strRetCode;

    switch(msmNotif)
    {
        case wlan_notification_msm_associating:
            strRetCode = L"\"associating\"";
            break;
        case wlan_notification_msm_associated:
            strRetCode = L"\"associated\"";
            break;
        case wlan_notification_msm_authenticating:
            strRetCode = L"\"authenticating\"";
            break;
        case wlan_notification_msm_connected:
            strRetCode = L"\"connected\"";
            break;
        case wlan_notification_msm_roaming_start:
            strRetCode = L"\"roaming start\"";
            break;
        case wlan_notification_msm_roaming_end:
            strRetCode = L"\"roaming end\"";
            break;
        case wlan_notification_msm_radio_state_change:
            strRetCode = L"\"radio state change\"";
            break;
        case wlan_notification_msm_signal_quality_change:
            strRetCode = L"\"signal quality change\"";
            break;
        case wlan_notification_msm_disassociating:
            strRetCode = L"\"disassociating\"";
            break;
        case wlan_notification_msm_disconnected:
            strRetCode = L"\"disconnected\"";
            break;
        case wlan_notification_msm_peer_join:
            strRetCode = L"\"a peer joins the ad hoc network\"";
            break;
        case wlan_notification_msm_peer_leave:
            strRetCode = L"\"a peer leaves the ad hoc network\"";
            break;
        case wlan_notification_msm_adapter_removal:
            strRetCode = L"\"adapter is in a bad state\"";
            break;
        default:
            strRetCode = L"\"unknown MSM notification\"";
    }

    return strRetCode;
}

// get connection mode string
LPWSTR
GetConnectionModeString(
    __in WLAN_CONNECTION_MODE wlanConnMode
)
{
    LPWSTR strRetCode;

    switch(wlanConnMode)
    {
        case wlan_connection_mode_profile:
            strRetCode = L"\"manual connection with a profile\"";
            break;
        case wlan_connection_mode_temporary_profile:
            strRetCode = L"\"manual connection with a temporary profile\"";
            break;
        case wlan_connection_mode_discovery_secure:
            strRetCode = L"\"connection to a secure network without a profile\"";
            break;
        case wlan_connection_mode_discovery_unsecure:
            strRetCode = L"\"connection to an unsecure network without a profile\"";
            break;
        case wlan_connection_mode_auto:
            strRetCode = L"\"automatic connection with a profile\"";
            break;
        default:
            strRetCode = L"\"invalid connection mode\"";
    }

    return strRetCode;
}

// get PHY type string
LPWSTR 
GetPhyTypeString(
    __in ULONG uDot11PhyType
)
{
    LPWSTR strRetCode;

    switch(uDot11PhyType)
    {
        case dot11_phy_type_dsss:
            strRetCode = L"\"DSSS\"";
            break;
        case dot11_phy_type_erp:
            strRetCode = L"\"802.11g\"";
            break;
        case dot11_phy_type_fhss:
            strRetCode = L"\"FHSS\"";
            break;
        case dot11_phy_type_hrdsss:
            strRetCode = L"\"802.11b\"";
            break;
        case dot11_phy_type_irbaseband:
            strRetCode = L"\"IR-base band\"";
            break;
        case dot11_phy_type_ofdm:
            strRetCode = L"\"802.11a\"";
            break;
        case dot11_phy_type_any:
            strRetCode = L"\"any\"";
            break;
        default:
            strRetCode = L"\"Unknown PHY type\"";
    }

    return strRetCode;
}

// get BSS type string
LPWSTR 
GetBssTypeString(
    __in DOT11_BSS_TYPE dot11BssType
)
{
    LPWSTR strRetCode;

    switch(dot11BssType)
    {
        case dot11_BSS_type_infrastructure:
            strRetCode = L"\"Infrastructure\"";
            break;
        case dot11_BSS_type_independent:
            strRetCode = L"\"Ad hoc\"";
            break;
        case dot11_BSS_type_any:
            strRetCode = L"\"Any\"";
            break;
        default:
            strRetCode = L"\"Unknown BSS type\"";
    }

    return strRetCode;
}

// get radio state string
LPWSTR
GetRadioStateString(
    __in DOT11_RADIO_STATE radioState
)
{
    LPWSTR strRetCode;

    switch(radioState)
    {
        case dot11_radio_state_on:
            strRetCode = L"\"on\"";
            break;
        case dot11_radio_state_off:
            strRetCode = L"\"off\"";
            break;
        default:
            strRetCode = L"\"unknown state\"";
    }

    return strRetCode;
}


// get auth algorithm string
LPWSTR 
GetAuthAlgoString(
    __in DOT11_AUTH_ALGORITHM dot11AuthAlgo
)
{
    LPWSTR strRetCode = L"\"Unknown algorithm\"";

    switch(dot11AuthAlgo)
    {
        case DOT11_AUTH_ALGO_80211_OPEN:
            strRetCode = L"\"Open\"";
            break;
        case DOT11_AUTH_ALGO_80211_SHARED_KEY:
            strRetCode = L"\"Shared\"";
            break;
        case DOT11_AUTH_ALGO_WPA:
            strRetCode = L"\"WPA-Enterprise\"";
            break;
        case DOT11_AUTH_ALGO_WPA_PSK:
            strRetCode = L"\"WPA-Personal\"";
            break;
        case DOT11_AUTH_ALGO_WPA_NONE:
            strRetCode = L"\"WPA-NONE\"";
            break;
        case DOT11_AUTH_ALGO_RSNA:
            strRetCode = L"\"WPA2-Enterprise\"";
            break;
        case DOT11_AUTH_ALGO_RSNA_PSK:
            strRetCode = L"\"WPA2-Personal\"";
            break;
        default:
            if (dot11AuthAlgo & DOT11_AUTH_ALGO_IHV_START)
            {
                strRetCode = L"\"Vendor-specific algorithm\"";
            }
    }

    return strRetCode;
}

// get cipher algorithm string
LPWSTR 
GetCipherAlgoString(
    __in DOT11_CIPHER_ALGORITHM dot11CipherAlgo
)
{
    LPWSTR strRetCode = L"\"Unknown algorithm\"";

    switch(dot11CipherAlgo)
    {
        case DOT11_CIPHER_ALGO_NONE:
            strRetCode = L"\"None\"";
            break;
        case DOT11_CIPHER_ALGO_WEP40:
            strRetCode = L"\"WEP40\"";
            break;
        case DOT11_CIPHER_ALGO_TKIP:
            strRetCode = L"\"TKIP\"";
            break;
        case DOT11_CIPHER_ALGO_CCMP:
            strRetCode = L"\"AES\"";
            break;
        case DOT11_CIPHER_ALGO_WEP104:
            strRetCode = L"\"WEP104\"";
            break;
        case DOT11_CIPHER_ALGO_WPA_USE_GROUP:
            strRetCode = L"\"USE-GROUP\"";
            break;
        case DOT11_CIPHER_ALGO_WEP:
            strRetCode = L"\"WEP\"";
            break;
        default:
            if (dot11CipherAlgo & DOT11_CIPHER_ALGO_IHV_START)
            {
                strRetCode = L"\"Vendor-specific algorithm\"";
            }
    }

    return strRetCode;
}

// get SSID from the WCHAR string
DWORD
StringWToSsid(
    __in LPCWSTR strSsid, 
    __out PDOT11_SSID pSsid
)
{
    DWORD dwRetCode = ERROR_SUCCESS;
    BYTE pbSsid[DOT11_SSID_MAX_LENGTH + 1] = {0};

    if (strSsid == NULL || pSsid == NULL)
    {
        dwRetCode = ERROR_INVALID_PARAMETER;
    }
    else
    {
        pSsid->uSSIDLength = WideCharToMultiByte (CP_ACP,
                                                   0,
                                                   strSsid,
                                                   -1,
                                                   (LPSTR)pbSsid,
                                                   sizeof(pbSsid),
                                                   NULL,
                                                   NULL);

        pSsid->uSSIDLength--;
        memcpy(&pSsid->ucSSID, pbSsid, pSsid->uSSIDLength);
    }

    return dwRetCode;
}

// copy SSID to a null-terminated WCHAR string
// count is the number of WCHAR in the buffer.
LPWSTR
SsidToStringW(
    __out_ecount(count) LPWSTR   buf,
    __in ULONG   count,
    __in PDOT11_SSID pSsid
    )
{
    ULONG   bytes, i;

    bytes = min( count-1, pSsid->uSSIDLength);
    for( i=0; i<bytes; i++)
        mbtowc( &buf[i], (const char *)&pSsid->ucSSID[i], 1);
    buf[bytes] = '\0';

    return buf;
}


// the max lenght of the reason string in characters
#define WLSAMPLE_REASON_STRING_LEN 256

// print the reason string
VOID
PrintReason(
    __in WLAN_REASON_CODE reason
)
{
    WCHAR strReason[WLSAMPLE_REASON_STRING_LEN];

    if (WlanReasonCodeToString(
            reason, 
            WLSAMPLE_REASON_STRING_LEN,
            strReason, 
            NULL            // reserved
            ) == ERROR_SUCCESS)
    {
        wcout << L" The reason is \"" << strReason << L"\"." << endl;
    }
    else
    {
        wcout << L" The reason code is " << reason << L"." << endl;
    }
}

// open a WLAN client handle and check version
DWORD
OpenHandleAndCheckVersion(
    PHANDLE phClient
)
{
    DWORD dwError = ERROR_SUCCESS;
    DWORD dwServiceVersion;
    HANDLE hClient = NULL;

    __try
    {
        *phClient = NULL;
        
        // open a handle to the service
        if ((dwError = WlanOpenHandle(
                            WLAN_API_VERSION,
                            NULL,               // reserved
                            &dwServiceVersion,
                            &hClient
                            )) != ERROR_SUCCESS)
        {
            __leave;
			ERROR_INVALID_PARAMETER;
        }

        // check service version
        if (WLAN_API_VERSION_MAJOR(dwServiceVersion) < WLAN_API_VERSION_MAJOR(WLAN_API_VERSION_2_0))
        {
            // No-op, because the version check is for demonstration purpose only.
            // You can add your own logic here.
        }

        *phClient = hClient;

        // set hClient to NULL so it will not be closed
        hClient = NULL;
    }
    __finally
    {
        if (hClient != NULL)
        {
            // clean up
            WlanCloseHandle(
                hClient, 
                NULL            // reserved
                );
        }
    }

    return dwError;
}*/

CWifiScanner::CWifiScanner(void)
{
	m_Ad = NULL;
	m_nChannel = 1;
	m_bStDev = 0;
}

CWifiScanner::~CWifiScanner(void)
{
	if(m_Ad)
	{
		m_bEnd = 1;
		while( m_bEnd != 2 );
		AirpcapClose(m_Ad);
		free(m_PacketBuffer);
		m_pThread->Delete();
	}
}

void CWifiScanner::MergeRSSIInfo( RSSIINFO *dest , RSSIINFO *src )
{
	int i ,j;
	for( i = 0 ; i < src->apnum ; ++i )
	{
		int find = 0;
		for( j = 0 ; j < dest->apnum ; ++j )
		{
			if( wcscmp( src->ap[i].ssid , dest->ap[j].ssid ) == 0 )
			{
				find = 1;
				if( dest->ap[j].channel == src->ap[i].channel || dest->ap[j].rssi - src->ap[i].rssi > -5.0 && dest->ap[j].rssi - src->ap[i].rssi < 5.0 )
				{
					dest->ap[j].rssi = dest->ap[j].rssi * dest->ap[j].count + src->ap[i].rssi * src->ap[i].count;
					dest->ap[j].count += src->ap[i].count;
					dest->ap[j].rssi /= dest->ap[j].count;
#ifdef MEASURE_VARIATION
					if( dest->ap[j].max < src->ap[i].max )
						dest->ap[j].max = src->ap[i].max;
					if( dest->ap[j].min > src->ap[i].min )
						dest->ap[j].min = src->ap[i].min;
#endif
				}
				else
				{
					if( dest->ap[j].rssi < src->ap[i].rssi )
					{
						dest->ap[j].rssi = src->ap[i].rssi;
						dest->ap[j].count = src->ap[i].count;
						dest->ap[j].channel = src->ap[i].channel;
						dest->ap[j].seq = src->ap[i].seq;
#ifdef MEASURE_VARIATION
						dest->ap[j].max = src->ap[i].max;
						dest->ap[j].min = src->ap[i].min;
#endif
					}
				}
				break;
			}
		}
		if( find == 0 )
		{
			dest->ap[dest->apnum] = src->ap[i];
			dest->apnum++;
		}
	}
}
AFX_THREADPROC th_PacketSniff(void *param )
{
	UINT BytesReceived;
	CWifiScanner *p = (CWifiScanner*)param;
	//int ch = 1;
	SYSTEMTIME t1,t2;
	int count = 1000;
	p->m_RssiBuffer.apnum = 0;
	p->m_RssiInfo.apnum = 0;
	AirpcapSetDeviceChannel  ( p->m_Ad, p->m_nChannel );
	while(TRUE)
	{
	    // capture the packets
		int diff;
		//if( count-- == 0 )
		{
			
			
			
		}
		//if( ch == 1 )
		//	GetLocalTime(&t1);

		if( p->m_bEnd == 1 )
		{
			p->m_bEnd = 2;
			ExitThread(0);
		}

		if(!AirpcapRead(p->m_Ad, 
			p->m_PacketBuffer, 
			PACKET_BUFFER_SIZE, 
			&BytesReceived))
		{
			printf("Error receiving packets: %s\n", AirpcapGetLastError(p->m_Ad));
			free(p->m_PacketBuffer);
			AirpcapClose(p->m_Ad);
			//return;
			ExitThread(0);
		}

		// parse the buffer and print the packets
		//PrintPackets(p->m_PacketBuffer, BytesReceived);
		if( BytesReceived == 0 )
			BytesReceived = 0;
		p->ParsePacket(BytesReceived , p->m_nChannel );
		// wait until some packets are available. This prevents polling and keeps the CPU low. 
		/*if( ch == 14 )
		{
			
			GetLocalTime(&t2);
			diff = t2.wMilliseconds + t2.wSecond * 1000 - t1.wSecond * 1000 - t1.wMilliseconds;
		}*/
		//if( count-- == 0 )
		{
			p->m_nChannel++;
			if( p->m_nChannel > 14 )
			{
				 p->m_nChannel = 1;
				 p->m_RssiBuffer2 = p->m_RssiBuffer;
				 p->m_RssiBuffer.apnum = 0;
				 p->MergeRSSIInfo( &p->m_RssiInfo , &p->m_RssiBuffer2 );
				 p->m_nScanCount++;
				 if( p->m_bStDev )
				 {
					 p->MinMaxRSSI();
				 }
			}
			count = 20000;
			AirpcapSetDeviceChannel  ( p->m_Ad, p->m_nChannel );
		}
		WaitForSingleObject(p->m_ReadEvent, p->m_nScanDelay);
	}
	ExitThread(0);
}
int CWifiScanner::Init()
{
	INT i, Inum;
	CHAR Ebuf[AIRPCAP_ERRBUF_SIZE];
	
	m_nScanDelay = 100;
	if(!AirpcapGetDeviceList(&m_AllDevs, Ebuf))
	{
		printf("Unable to retrieve the device list: %s\n", Ebuf);
		return -1;
	}

	//
	// Make sure that the device list is valid
	//
	if(m_AllDevs == NULL)
	{
		printf("No interfaces found! Make sure the airpcap software is installed and your adapter is properly plugged in.\n");
		return -1;
	}

	//
	// Print the list
	//
	/*for(TmpDev = AllDevs, i = 0; TmpDev; TmpDev = TmpDev->next)
	{
		printf("%d. %s", ++i, TmpDev->Name);
		if(TmpDev->Description)
		{
			printf(" (%s)\n", TmpDev->Description);
		}
		else
		{
			printf(" (No description available)\n");
		}
	}

	
	
	printf("Enter the adapter number (1-%d):",i);
	scanf("%d", &Inum);*/
	Inum = 1;
	
	// 
	// Check if the user specified a valid adapter
	//
	/*if(Inum < 1 || Inum > i)
	{
		printf("\nAdapter number out of range.\n");
		AirpcapFreeDeviceList(m_AllDevs);
		return -1;
	}*/

	//
	// Jump to the selected adapter
	//
	for(m_TmpDev = m_AllDevs, i = 0; i < Inum-1 ;m_TmpDev = m_TmpDev->next, i++);


	//
	// Open the adapter
	//
	m_Ad = AirpcapOpen(m_TmpDev->Name, Ebuf);
	if(!m_Ad)
	{
		printf("Error opening the adapter: %s\n", Ebuf);
		return -1;
	}

	//
	// We don't need the device list any more, free it
	//
	AirpcapFreeDeviceList(m_AllDevs);

	//
	// Set the link layer to 802.11 plus radio headers
	//
	if(!AirpcapSetLinkType(m_Ad, AIRPCAP_LT_802_11_PLUS_RADIO))
	{
		printf("Error setting the link layer: %s\n", AirpcapGetLastError(m_Ad));
		AirpcapClose(m_Ad);
		return -1;
	}

	AirpcapSetFcsValidation(m_Ad, AIRPCAP_VT_ACCEPT_CORRECT_FRAMES );
	AirpcapSetDeviceChannel(m_Ad,6);
	if(!AirpcapGetReadEvent(m_Ad, &m_ReadEvent))
	{
		printf("Error getting the read event: %s\n", AirpcapGetLastError(m_Ad));
		AirpcapClose(m_Ad);
		return -1;
	}

	//
	// Allocate a 256k packet buffer
	//
	m_PacketBuffer = (BYTE*)malloc(PACKET_BUFFER_SIZE);
	if(!m_PacketBuffer)
	{
		printf("No memory for the packet buffer\n");
		AirpcapClose(m_Ad);
		return -1;
	}

	m_RssiInfo.apnum = 0;
	m_pThread = AfxBeginThread((AFX_THREADPROC)th_PacketSniff,this);
	m_bEnd = 0;
	return 0;
}
// print the basic information of a visible wireless network
/*void CWifiScanner::PrintNetworkInfo( __in PWLAN_AVAILABLE_NETWORK pNetwork , CListCtrl *pList , void *rssi )
{
    WCHAR strSsid[DOT11_SSID_MAX_LENGTH+1];

	RSSIINFO *ri = (RSSIINFO*)rssi;
    if (pNetwork != NULL)
    {
        // SSID
		
        wcout << L"SSID: " << SsidToStringW(strSsid, sizeof(strSsid)/sizeof(WCHAR), &pNetwork->dot11Ssid) << endl;


		if(wcslen((const wchar_t*)strSsid) == 0 )
			return;

		int find = 0;
		for( int i = 0 ; i < ri->apnum ; ++i )
		{
			if( wcscmp( strSsid , ri->ap[i].ssid ) == 0 )
			{
				ri->ap[i].rssi = ( ri->ap[i].rssi * ri->ap[i].count + pNetwork->wlanSignalQuality ) / ( ri->ap[i].count + 1 );
				ri->ap[i].count++;
				find = 1;
				break;
			}
		}
		if( !find )
		{
			
			ri->ap[ri->apnum].count++;
			ri->ap[ri->apnum].rssi = pNetwork->wlanSignalQuality;
			wcscpy( ri->ap[ri->apnum].ssid , strSsid );
			ri->apnum++;
		}
		//return;
        // whether security is enabled
        if (pNetwork->bSecurityEnabled)
        {
            wcout << L"\tSecurity enabled." << endl;
        }
        else
        {
            wcout << L"\tSecurity not enabled." << endl;
        }

        // number of BSSIDs
        wcout << L"\tContains " << pNetwork->uNumberOfBssids << L" BSSIDs." << endl;

        // whether have a profile for this SSID
        if (pNetwork->dwFlags & WLAN_AVAILABLE_NETWORK_HAS_PROFILE)
        {
            wcout << L"\tHas a matching profile: " << pNetwork->strProfileName << L"." <<endl;
        }

        // whether it is connected
        if (pNetwork->dwFlags & WLAN_AVAILABLE_NETWORK_CONNECTED)
        {
            wcout << L"\tCurrently connected." << endl;
        }

        // whether it is connectable
        if (!pNetwork->bNetworkConnectable)
        {
            // the reason that it is not connectable
            wcout << L"\tThe network is not connectable. ";
            PrintReason(pNetwork->wlanNotConnectableReason);
        }
        else
        {
            wcout << L"\tThe network is connectable." << endl;
        }

        // BSS type
        wcout << L"\tBSS type: " << GetBssTypeString(pNetwork->dot11BssType) << endl;
        
        // Signal quality
        wcout << L"\tSignal quality: " << pNetwork->wlanSignalQuality << L"%" << endl;
		//pList->DeleteAllItems();
		int iRow = pList->GetItemCount();
		int iColumn = 0;
		LV_ITEM lvi;       
		lvi.mask=LVIF_TEXT | LVIF_PARAM;
		lvi.iItem = iRow;        //몇행
		lvi.iSubItem = iColumn;  //몇번째 컬럼
		lvi.pszText = strSsid;     //내용
		lvi.lParam = (LPARAM)iRow*2+iColumn;       //셀을 식별할 고유인자. 
	 
		pList->InsertItem(&lvi);
		pList->SetItemText( iRow,iColumn,strSsid);      //화면에 나타낸다
	   
		unsigned short sig[100];
		_itow(pNetwork->wlanSignalQuality,(wchar_t *)sig,10);
		iColumn = 1;
		lvi.mask=LVIF_TEXT | LVIF_PARAM;
		lvi.iItem = iRow;        //몇행
		lvi.iSubItem = iColumn;  //몇번째 컬럼
		lvi.pszText = (LPWSTR)sig;     //내용
		lvi.lParam = (LPARAM)iRow*2+iColumn;       //셀을 식별할 고유인자. 
	 
		//pList->InsertItem(&lvi);
		pList->SetItemText( iRow,iColumn,(LPCTSTR)sig);      //화면에 나타낸다
	


        // Default auth algorithm
        wcout << L"\tDefault authentication algorithm: " << GetAuthAlgoString(pNetwork->dot11DefaultAuthAlgorithm) << endl;

        // Default cipher algorithm
        wcout << L"\tDefault cipher algorithm: " << GetCipherAlgoString(pNetwork->dot11DefaultCipherAlgorithm) << endl;
    }
}*/
/*void CWifiScanner::GetVisibleNetworkList(CListCtrl *pList , void *rssi)
{
    DWORD dwError = ERROR_SUCCESS;
    HANDLE hClient = NULL;
    GUID guidIntf;
    PWLAN_AVAILABLE_NETWORK_LIST pVList = NULL;
    UINT i;

    __try
    {
        

        // get the interface GUID
        if (UuidFromString((RPC_WSTR)m_szInterfaceName, &guidIntf) != RPC_S_OK)
        {
            wcerr << L"Invalid GUID " << m_szInterfaceName << endl;
            dwError = ERROR_INVALID_PARAMETER;
            __leave;
        }

        // open handle
        if ((dwError = OpenHandleAndCheckVersion(
                            &hClient
                            )) != ERROR_SUCCESS)
        {
            __leave;
        }
        
        if ((dwError = WlanGetAvailableNetworkList(
                            hClient,
                            &guidIntf,
                            0,                      // only show visible networks
                            NULL,                   // reserved
                            &pVList
                            )) != ERROR_SUCCESS)
        {
            __leave;
        }

        // print all visible networks
        wcout << L"Total " << pVList->dwNumberOfItems << L" networks are visible." << endl;
		pList->DeleteAllItems();
        for (i = 0; i < pVList->dwNumberOfItems; i++)
        {
            wcout << L"Network " <<i << L":" << endl;
            PrintNetworkInfo(&pVList->Network[i],pList, rssi);
            wcout << endl;
        }

        WlanFreeMemory(pVList);
    }
    __finally
    {
        // clean up
        if (hClient != NULL)
        {
            WlanCloseHandle(
                hClient, 
                NULL            // reserved
                );
        }
    }

}*/

void WSTRTOSTR( char *c , unsigned short* w )
{
	int index = 0;
	while( *w )
	{
		*(c++) = *(w++);
	}
	*c = 0;
}

void STRTOWSTR( unsigned short* w, char *c )
{
	int index = 0;
	while( *c )
	{
		*(w++) = *(c++);
	}
	*w = 0;
}

/*void CWifiScanner::Scan()
{
    DWORD dwError = ERROR_SUCCESS;
    HANDLE hClient = NULL;
    GUID guidIntf;

    __try
    {
        

        // get the interface GUID
        if (UuidFromString((RPC_WSTR)m_szInterfaceName, &guidIntf) != RPC_S_OK)
        {
            wcerr << L"Invalid GUID " << m_szInterfaceName << endl;
            dwError = ERROR_INVALID_PARAMETER;
            __leave;
        }

        // open handle
        if ((dwError = OpenHandleAndCheckVersion(
                            &hClient
                            )) != ERROR_SUCCESS)
        {
            __leave;
        }

        // scan
        dwError = WlanScan(
                    hClient, 
                    &guidIntf, 
                    NULL,                   // don't perform additional probe for a specific SSID
                    NULL,                   // no IE data for the additional probe
                    NULL                    // reserved
                    );

    }
    __finally
    {
        // clean up
        if (hClient != NULL)
        {
            WlanCloseHandle(
                hClient, 
                NULL            // reserved
                );
        }
    }

    
}

void CWifiScanner::EnumInterface(CComboBox *box)
{
    DWORD dwError = ERROR_SUCCESS;
    HANDLE hClient = NULL;
    PWLAN_INTERFACE_INFO_LIST pIntfList = NULL;
    RPC_WSTR strGuid = NULL;
    UINT i = 0;

   // __try
    {
        

        // open handle
        if ((dwError = OpenHandleAndCheckVersion(
                            &hClient
                            )) != ERROR_SUCCESS)
        {
           // __leave;
        }

        // enumerate wireless interfaces
        if ((dwError = WlanEnumInterfaces(
                            hClient,
                            NULL,               // reserved
                            &pIntfList
                            )) != ERROR_SUCCESS)
        {
            //__leave;
        }

        wcout << L"There are " << pIntfList->dwNumberOfItems << L" interfaces in the system." << endl;

        // print out interface information
		box->ResetContent();
        for (i = 0; i < pIntfList->dwNumberOfItems; i++)
        {
            wcout << L"Interface " << i << L":" << endl;
            if (UuidToStringW(&pIntfList->InterfaceInfo[i].InterfaceGuid, &strGuid) == RPC_S_OK)
            {
                wcout << L"\tGUID: " << (LPWSTR)strGuid << endl;
				//WSTRTOSTR(m_szInterfaceName , (unsigned short*)strGuid );
				//m_szInterfaceName = strGuid;
				wcscpy( m_szInterfaceName , (const wchar_t*) strGuid );
				char text[100];
				WSTRTOSTR(text , (unsigned short*)m_szInterfaceName);
				CString strInt;
				strInt.Format(_T("%s") , m_szInterfaceName );
				box->InsertString(-1 , strInt );
                RpcStringFreeW(&strGuid);

            }
			//if( i == 0 )
			//{
			//	UuidToStringA(&pIntfList->InterfaceInfo[i].InterfaceGuid, (RPC_CSTR*)&m_szInterfaceName);
			//}
            wcout << L"\t" << pIntfList->InterfaceInfo[i].strInterfaceDescription << endl;
            wcout << L"\tState: " << GetInterfaceStateString(pIntfList->InterfaceInfo[i].isState) << endl;
            wcout << endl;
        }
    }
   // __finally
    {
        // clean up
        if (pIntfList != NULL)
        {
            WlanFreeMemory(pIntfList);
        }
        
        if (hClient != NULL)
        {
            WlanCloseHandle(
                hClient, 
                NULL            // reserved
                );
        }
    }

   // PrintErrorMsg(argv[0], dwError);
}

void CWifiScanner::GetBssList(void *rssi)
{
    DWORD dwError = ERROR_SUCCESS;
    HANDLE hClient = NULL;
    GUID guidIntf;
    DOT11_SSID dot11Ssid = {0};
    PDOT11_SSID pDot11Ssid = NULL;
    DOT11_BSS_TYPE dot11BssType = dot11_BSS_type_any;
    BOOL bSecurityEnabled = TRUE;
    PWLAN_BSS_LIST pWlanBssList = NULL;
    UINT i;
	RSSIINFO *ri = (RSSIINFO*)rssi;
    __try
    {
        // get the interface GUID
        if (UuidFromString((RPC_WSTR)m_szInterfaceName, &guidIntf) != RPC_S_OK)
        {
            wcerr << L"Invalid GUID " << m_szInterfaceName << endl;
            dwError = ERROR_INVALID_PARAMETER;
            __leave;
        }

       
        
        // open handle
        if ((dwError = OpenHandleAndCheckVersion(
                            &hClient
                            )) != ERROR_SUCCESS)
        {
            __leave;
        }
        
        if ((dwError = WlanGetNetworkBssList(
                            hClient,
                            &guidIntf,
                            pDot11Ssid,
                            dot11BssType,
                            bSecurityEnabled,
                            NULL,                       // reserved
                            &pWlanBssList
                            )) != ERROR_SUCCESS)
        {
            __leave;
        }

		WCHAR strSsid[DOT11_SSID_MAX_LENGTH+1];
        for (i = 0; i < pWlanBssList->dwNumberOfItems; i++)
		{
			SsidToStringW(strSsid, sizeof(strSsid)/sizeof(WCHAR), &pWlanBssList->wlanBssEntries[i].dot11Ssid);
			int rssi = pWlanBssList->wlanBssEntries[i].lRssi;
			int find = 0;
			for( int j = 0 ; j < ri->apnum ; ++j )
			{
				if( wcscmp( strSsid , ri->ap[j].ssid ) == 0 )
				{
					ri->ap[j].rssi = ( ri->ap[j].rssi * ri->ap[j].count + rssi ) / ( ri->ap[j].count + 1 );
					ri->ap[j].count++;
					find = 1;
					break;
				}
			}
			if( !find )
			{
				
				ri->ap[ri->apnum].count++;
				ri->ap[ri->apnum].rssi = rssi;
				wcscpy( ri->ap[ri->apnum].ssid , strSsid );
				ri->apnum++;
			}
        }
            
        WlanFreeMemory(pWlanBssList);
    }
    __finally
    {
        // clean up
        if (hClient != NULL)
        {
            WlanCloseHandle(
                hClient, 
                NULL            // reserved
                );
        }
    }

    
}

*/
static UCHAR *
cpack_next_boundary(UCHAR *buf, UCHAR *p, size_t alignment)
{
	size_t misalignment = (size_t)(p - buf) % alignment;

	if (misalignment == 0)
		return p;

	return p + (alignment - misalignment);
}

static UCHAR *
cpack_align_and_reserve(struct cpack_state *cs, size_t wordsize)
{
	UCHAR *next;

	// Ensure alignment.
	next = cpack_next_boundary(cs->c_buf, cs->c_next, wordsize);

	// Too little space for wordsize bytes?
	if (next - cs->c_buf + wordsize > cs->c_len)
		return NULL;

	return next;
}

int
cpack_init(struct cpack_state *cs, UCHAR *buf, size_t buflen)
{
	memset(cs, 0, sizeof(*cs));

	cs->c_buf = buf;
	cs->c_len = buflen;
	cs->c_next = cs->c_buf;

	return 0;
}

int
cpack_uint64(struct cpack_state *cs, ULONGLONG *u)
{
	UCHAR *next;

	if ((next = cpack_align_and_reserve(cs, sizeof(*u))) == NULL)
		return -1;

	*u = EXTRACT_LE_64BITS(next);

	// Move pointer past the ULONGLONG.
	cs->c_next = next + sizeof(*u);
	return 0;
}

int
cpack_uint32(struct cpack_state *cs, ULONG *u)
{
	UCHAR *next;

	if ((next = cpack_align_and_reserve(cs, sizeof(*u))) == NULL)
		return -1;

	*u = EXTRACT_LE_32BITS(next);

	// Move pointer past the ULONG.
	cs->c_next = next + sizeof(*u);
	return 0;
}

int
cpack_uint16(struct cpack_state *cs, USHORT *u)
{
	UCHAR *next;

	if ((next = cpack_align_and_reserve(cs, sizeof(*u))) == NULL)
		return -1;

	*u = EXTRACT_LE_16BITS(next);

	// Move pointer past the USHORT.
	cs->c_next = next + sizeof(*u);
	return 0;
}

int
cpack_uint8(struct cpack_state *cs, UCHAR *u)
{
	// No space left?
	if ((size_t)(cs->c_next - cs->c_buf) >= cs->c_len)
		return -1;

	*u = *cs->c_next;

	// Move pointer past the UCHAR.
	cs->c_next++;
	return 0;
}

#define cpack_int8(__s, __p)	cpack_uint8((__s),  (UCHAR*)(__p))
#define cpack_int16(__s, __p)	cpack_uint16((__s), (USHORT*)(__p))
#define cpack_int32(__s, __p)	cpack_uint32((__s), (ULONG*)(__p))
#define cpack_int64(__s, __p)	cpack_uint64((__s), (ULONGLONG*)(__p))


//////////////////////////////////////////////////////////////////////
// Print one of the fields in the radiotap header.
// This is the function that needs to be changed to support additional 
// radiotap fields.
//////////////////////////////////////////////////////////////////////
UINT PrintRadiotapField(struct cpack_state *s, ULONG bit)
{
	union 
	{
		CHAR		i8;
		UCHAR		u8;
		SHORT		i16;
		USHORT		u16;
		ULONG		u32;
		ULONGLONG	u64;
	} u, u2;
	int rc;

	//
	// Extract the field in the u and u2 variables
	//
	switch (bit) 
	{
	case IEEE80211_RADIOTAP_FLAGS:
	case IEEE80211_RADIOTAP_RATE:
	case IEEE80211_RADIOTAP_DB_ANTSIGNAL:
	case IEEE80211_RADIOTAP_DB_ANTNOISE:
	case IEEE80211_RADIOTAP_ANTENNA:
		rc = cpack_uint8(s, &u.u8);
		break;
	case IEEE80211_RADIOTAP_DBM_ANTSIGNAL:
	case IEEE80211_RADIOTAP_DBM_ANTNOISE:
		rc = cpack_int8(s, &u.i8);
		break;
	case IEEE80211_RADIOTAP_CHANNEL:
		rc = cpack_uint16(s, &u.u16);
		if (rc != 0)
			break;
		rc = cpack_uint16(s, &u2.u16);
		break;
	case IEEE80211_RADIOTAP_FHSS:
	case IEEE80211_RADIOTAP_LOCK_QUALITY:
	case IEEE80211_RADIOTAP_TX_ATTENUATION:
		rc = cpack_uint16(s, &u.u16);
		break;
	case IEEE80211_RADIOTAP_DB_TX_ATTENUATION:
		rc = cpack_uint8(s, &u.u8);
		break;
	case IEEE80211_RADIOTAP_DBM_TX_POWER:
		rc = cpack_int8(s, &u.i8);
		break;
	case IEEE80211_RADIOTAP_TSFT:
		rc = cpack_uint64(s, &u.u64);
		break;
	case IEEE80211_RADIOTAP_FCS:
		rc = cpack_uint32(s, &u.u32);
		break;
	default:
		// this bit indicates a field whose
		// size we do not know, so we cannot
		// proceed.
		printf("Unknown field 0x%08x", bit);
		return -1;
	}

	if(rc != 0) 
	{
		//
		// Unaligned field
		//
		printf("Wrong alignment");
		return rc;
	}

	//
	// Print the value
	//
	switch (bit) 
	{
	case IEEE80211_RADIOTAP_CHANNEL:
		printf("Channel frequency: %u MHz\n", u.u16);
		
		switch(u.u16) 
		{
		case 2412:
			printf("Channel number: 1\n");
			break;
		case 2417:
			printf("Channel number: 2\n");
			break;
		case 2422:
			printf("Channel number: 3\n");
			break;
		case 2427:
			printf("Channel number: 4\n");
			break;
		case 2432:
			printf("Channel number: 5\n");
			break;
		case 2437:
			printf("Channel number: 6\n");
			break;
		case 2442:
			printf("Channel number: 7\n");
			break;
		case 2447:
			printf("Channel number: 8\n");
			break;
		case 2452:
			printf("Channel number: 9\n");
			break;
		case 2457:
			printf("Channel number: 10\n");
			break;
		case 2462:
			printf("Channel number: 11\n");
			break;
		case 2467:
			printf("Channel number: 12\n");
			break;
		case 2472:
			printf("Channel number: 13\n");
			break;
		case 2484:
			printf("Channel number: 14\n");
			break;
		};
		
		printf("Channel type:");

		if(u2.u16 & IEEE80211_CHAN_OFDM)
		{
			printf(" 802.11g", u2.u16);
		}
		else
		{
			printf(" 802.11b", u2.u16);
		}

		if(u2.u16 & IEEE80211_CHAN_2GHZ)
		{
			printf(", 2Ghz spectrum", u2.u16);
		}

		if(u2.u16 & IEEE80211_CHAN_5GHZ)
		{
			printf(", 5Ghz spectrum", u2.u16);
		}

		printf("\n");
		break;
	case IEEE80211_RADIOTAP_FHSS:
		printf("channel hoping set %d, pattern %d ", u.u16 & 0xff, (u.u16 >> 8) & 0xff);
		break;
	case IEEE80211_RADIOTAP_RATE:
		printf("Rate: %2.1f Mb/s\n", (.5 * ((u.u8) & 0x7f)));
		break;
	case IEEE80211_RADIOTAP_DBM_ANTSIGNAL:
		printf("Signal Strength: %ddB\n", u.i8);
		break;
	case IEEE80211_RADIOTAP_DBM_ANTNOISE:
		printf("Noise level: %ddB", u.i8);
		break;
	case IEEE80211_RADIOTAP_DB_ANTSIGNAL:
		printf("Signal Strength: %ddB\n", u.u8);
		break;
	case IEEE80211_RADIOTAP_DB_ANTNOISE:
		printf("Noise level: %ddB", u.u8);
		break;
	case IEEE80211_RADIOTAP_LOCK_QUALITY:
		printf("Signal Quality: %u\n", u.u16);
		break;
	case IEEE80211_RADIOTAP_TX_ATTENUATION:
		printf("Tx power: %d\n", -(int)u.u16);
		break;
	case IEEE80211_RADIOTAP_DB_TX_ATTENUATION:
		printf("Tx power: %ddB\n", -(int)u.u8);
		break;
	case IEEE80211_RADIOTAP_DBM_TX_POWER:
		printf("Tx power: %ddBm\n", u.i8);
		break;
	case IEEE80211_RADIOTAP_FLAGS:
		if(u.u8 & IEEE80211_RADIOTAP_F_CFP)
		{
			printf("cfp ");
		}
		if (u.u8 & IEEE80211_RADIOTAP_F_SHORTPRE)
		{
			printf("short preamble ");
		}
		if (u.u8 & IEEE80211_RADIOTAP_F_FRAG)
		{
			printf("fragmented ");
		}
		if (u.u8 & IEEE80211_RADIOTAP_F_FCS)
		{
			printf("Frame includes FCS\n");
		}
		else
		{
			printf("Frame doesn't include FCS\n");
		}

		break;
	case IEEE80211_RADIOTAP_ANTENNA:
		printf("antenna n. %d\n", u.u8);
		break;
	case IEEE80211_RADIOTAP_TSFT:
		printf("heardware timestamp: %I64uus\n", u.u64);
		break;
	case IEEE80211_RADIOTAP_FCS:
		printf("Frame Check Sequence: 0x%.4x\n", htonl(u.u32));
		break;
	}
	return 0;
}


ULONG RadiotapPrint(const u_char *p, ULONG caplen)
{
	struct cpack_state cpacker;
	struct ieee80211_radiotap_header *hdr;
	ULONG present, next_present;
	ULONG *presentp, *last_presentp;
	enum ieee80211_radiotap_type bit;
	int bit0;
	const u_char *iter;
	ULONG len;

	//
	// Sanity checks
	//
	if (caplen < sizeof(*hdr)) 
	{
		// Packet smaller than the radiotap fixed header
		return 0;
	}

	hdr = (struct ieee80211_radiotap_header *)p;

	len = EXTRACT_LE_16BITS(&hdr->it_len);

	if(caplen < len) 
	{
		// Packet smaller than the radiotap header
		return 0;
	}

	for (last_presentp = &hdr->it_present;
	     IS_EXTENDED(last_presentp) &&
	     (u_char*)(last_presentp + 1) <= p + len;
	     last_presentp++);

	// are there more bitmap extensions than bytes in header?
	if(IS_EXTENDED(last_presentp)) 
	{
		return 0;
	}

	iter = (u_char*)(last_presentp + 1);

	if (cpack_init(&cpacker, (UCHAR*)iter, len - (iter - p)) != 0) 
	{
		return 0;
	}

	//
	// Scan the fields, and print each of them
	//
	for (bit0 = 0, presentp = &hdr->it_present; presentp <= last_presentp; presentp++, bit0 += 32) 
	{
		for (present = EXTRACT_LE_32BITS(presentp); present; present = next_present) 
		{
			// clear the least significant bit that is set
			next_present = present & (present - 1);

			// extract the least significant bit that is set
			bit = (enum ieee80211_radiotap_type)(bit0 + BITNO_32(present ^ next_present));
			
			// print the field
			if(PrintRadiotapField(&cpacker, bit) != 0)
			{
				//
				// Error decoding the field, exit from the loop
				//
				return len;
			}
		}
	}

	return len;
}

int CWifiScanner::ParsePacket(ULONG BufferSize , int ch)
{
	static unsigned int seq = 0;
	BYTE *PacketBuffer = m_PacketBuffer;
	BYTE *Buf;
	UINT Off = 0;
	u_int TLen, TLen1;
	PAirpcapBpfHeader Hdr;
	char *pChar;
	ULONG RadioHdrLen;

	Buf = PacketBuffer;
	Off=0;
	double rssi;
	char ssidlen;
	char ssid[20];
	WCHAR wid[20];
	int apnum = m_RssiBuffer.apnum;
	while(Off < BufferSize)
	{
		//BYTE ttt[2000];
		
		Hdr = (PAirpcapBpfHeader)(Buf + Off);
		BYTE *buffer = Buf + Off;
		TLen1 = Hdr->Originallen;
		TLen = Hdr->Caplen;
		
		Off += Hdr->Hdrlen;
				
		pChar =(char*)(Buf + Off);
		//if( TLen > 1000 )
		//	TLen = TLen;
		Off = AIRPCAP_WORDALIGN(Off + TLen);
		buffer = (BYTE*)pChar;
		//memcpy( ttt, Buf + Off , TLen );
		RadioHdrLen = RadiotapPrint((const u_char*)pChar, TLen);
		
		BYTE *frame = (BYTE*)(pChar + RadioHdrLen);
		//BYTE test[50];
		//memcpy( test , frame , 50 );
		if( frame[0] != 0x80)continue;
		if( *(frame + 36) != 0x00 ) continue;
		memcpy( &ssidlen , frame+37 , 1 );
		if( ssidlen <= 0 || ssidlen > 19 )
		{
			continue;
		}
		memcpy( ssid , frame + 38 , ssidlen );
		ssid[ssidlen]=0;
		/*int err = 0;
		for( int k = 0 ; k < ssidlen ; ++k )
		{
			if( ssid[k] < 0 )
			{
				err = 1;
				break;
			}
		}
		if( err )
			continue;*/
		
		rssi = *(buffer + 14 ) - *(buffer + 15);
		STRTOWSTR( (unsigned short*) wid , ssid );
		int find = 0;
		for( int i = 0 ; i < apnum ; ++i )
		{
			if( wcscmp( wid , m_RssiBuffer.ap[i].ssid ) == 0 )
			{
				find = 1;
				if( m_RssiBuffer.ap[i].channel == ch || m_RssiBuffer.ap[i].rssi - rssi > -5.0 &&m_RssiBuffer.ap[i].rssi - rssi < 5.0)
				{
					m_RssiBuffer.ap[i].rssi = m_RssiBuffer.ap[i].rssi * m_RssiBuffer.ap[i].count + rssi;
					m_RssiBuffer.ap[i].count++;
					m_RssiBuffer.ap[i].rssi /= m_RssiBuffer.ap[i].count;
#ifdef MEASURE_VARIATION
					if( rssi < m_RssiBuffer.ap[i].min )
						m_RssiBuffer.ap[i].min = rssi;
					if( rssi > m_RssiBuffer.ap[i].max )
						m_RssiBuffer.ap[i].max = rssi;
#endif
				}
				else
				{
					if( m_RssiBuffer.ap[i].rssi < rssi )
					{
						m_RssiBuffer.ap[i].rssi = rssi;
						m_RssiBuffer.ap[i].count = 1;
						m_RssiBuffer.ap[i].channel = ch;
						m_RssiBuffer.ap[i].seq = seq++;
#ifdef MEASURE_VARIATION
						m_RssiBuffer.ap[i].min = m_RssiBuffer.ap[i].max = rssi;
#endif
					}
				}
				break;
			}
		}
		if( !find )
		{
#ifdef MEASURE_VARIATION
			m_RssiBuffer.ap[apnum].rssi = m_RssiBuffer.ap[apnum].min = m_RssiBuffer.ap[apnum].max = rssi;
#else
			m_RssiBuffer.ap[apnum].rssi = rssi;
#endif
			m_RssiBuffer.ap[apnum].count = 1;
			m_RssiBuffer.ap[apnum].channel = ch;
			m_RssiBuffer.ap[apnum].seq = seq++;
			wcscpy( m_RssiBuffer.ap[apnum].ssid , wid );
			apnum++;
		}
		
	}
	m_RssiBuffer.apnum = apnum;
	return 0;
}

///////////////////////////////////////////////////////////////////////
// This function prints the content frame
///////////////////////////////////////////////////////////////////////
/*APINFO CWifiScanner::PrintFrameData(BYTE *Payload, UINT PayloadLen)
{
	APINFO a;
	a.count=0;
	ULONG i, j, ulLines, ulen;
	BYTE *pLine, *Base;

	ulLines = (PayloadLen + 15) / 16;
	Base = Payload;


	for(i = 0; i < ulLines; i++)
	{
		
		pLine = Payload;
		
		
		
		ulen = PayloadLen;
		ulen = ( ulen > 16 ) ? 16 : ulen;
		PayloadLen -= ulen;
		//Palyload += ulen;
		//for(j=0; j<ulen; j++ )
		//	printf( "%02x ", *(BYTE *)Payload++ );
		
		//if(ulen < 16 )
		//	printf( "%*s", (16-ulen)*3, " " );
		
		Payload = pLine;
		
		for(j = 0; j < ulen; j++, Payload++ )
		{
			printf("%c", isprint((unsigned char)*Payload) ? *Payload : '.' );
		}
		
		printf("\n");
	} 
	return a;
}*/


/*int CWifiScanner::ParsePacket(BYTE packetsize)
{

	
	BYTE *buffer =  m_PacketBuffer + 20;
	
	char test[ 1000 ];
	if( packetsize < 20 )
		return 0;
	memcpy( test , buffer , packetsize - 20 );

	//헤더 24바이트
	if( buffer[24] != 0x80 )//|| buffer[25] != 0x00 )
		return 0;

	
	
	//여기서부터 읽기 시작
	
	int	size;
	double rssi;
	char ssidlen;
	char ssid[20];
	WCHAR wid[20];
	int apnum = m_RssiInfo.apnum;
	int totalsize = 0;
	
	char *header = (char*)m_PacketBuffer + 24;
	
	//size = *(int*)(header + 8 );
	//if( size < 60 )
	//	return 0;
	memcpy( &ssidlen , buffer+61 , 1 );
	if( ssidlen <= 0 || ssidlen > 25 )
	{
		return 0;
	}
	memcpy( ssid , buffer + 62 , ssidlen );
	ssid[ssidlen]=0;
	
	rssi = *(buffer + 14 ) - *(buffer + 15);
	STRTOWSTR( (unsigned short*) wid , ssid );
	int find = 0;
	for( int i = 0 ; i < apnum ; ++i )
	{
		if( wcscmp( wid , m_RssiInfo.ap[i].ssid ) == 0 )
		{
			find = 1;
			m_RssiInfo.ap[i].rssi = m_RssiInfo.ap[i].rssi * m_RssiInfo.ap[i].count + rssi;
			m_RssiInfo.ap[i].count++;
			m_RssiInfo.ap[i].rssi /= m_RssiInfo.ap[i].count;
			break;
		}
	}
	if( !find )
	{
		m_RssiInfo.ap[apnum].rssi = rssi;
		m_RssiInfo.ap[apnum].count = 1;
		wcscpy( m_RssiInfo.ap[apnum].ssid , wid );
		apnum++;
	}
	
	m_RssiInfo.apnum = apnum;
	
	return 0;

}*/

/*int CWifiScanner::ParseWireshark(char *filename, RSSIINFO &r)
{

	FILE *f = fopen( filename , "rb" );
	if( !f )
		return -1;
	char buffer[1024];
	
	//헤더 24바이트
	fread( buffer , 24 , 1 , f );

	//여기서부터 읽기 시작
	char header[16];
	int	size;
	double rssi;
	char ssidlen;
	char ssid[20];
	WCHAR wid[20];
	int apnum = r.apnum;
	int totalsize = 0;
	while( !feof( f ) )
	{
		int rs = fread( header , 16 , 1 , f );
		if( !rs )
			break;
		totalsize += rs;
		size = *(int*)(header + 8 );
		totalsize += fread( buffer , size , 1 , f );
		if( size < 60 )
			continue;
		memcpy( &ssidlen , buffer+61 , 1 );
		if( ssidlen < 0 || ssidlen > 25 )
		{
			continue;
		}
		memcpy( ssid , buffer + 62 , ssidlen );
		ssid[ssidlen]=0;
		
		rssi = *(buffer + 14 ) - *(buffer + 15);
		STRTOWSTR( (unsigned short*) wid , ssid );
		int find = 0;
		for( int i = 0 ; i < apnum ; ++i )
		{
			if( wcscmp( wid , r.ap[i].ssid ) == 0 )
			{
				find = 1;
				r.ap[i].rssi = r.ap[i].rssi * r.ap[i].count + rssi;
				r.ap[i].count++;
				r.ap[i].rssi /= r.ap[i].count;
				break;
			}
		}
		if( !find )
		{
			r.ap[apnum].rssi = rssi;
			r.ap[apnum].count = 1;
			wcscpy( r.ap[apnum].ssid , wid );
			apnum++;
		}
	}
	r.apnum = apnum;
	fclose(f);
	if( totalsize <= 0 )
		return -1;
	return 0;

}*/

int CWifiScanner::GetRSSIfromAirPCap(CListCtrl *pList , RSSIINFO &r )
{
	
	int i,j;
	RSSIINFO m;
	//if( m_RssiInfo.apnum == 0 )
	//	m = m_RssiBuffer2;
	//else
	m = m_RssiInfo;
	MergeRSSIInfo( &m , &m_RssiBuffer );
	for( i = 0 ; i < m.apnum ; ++i )
	{
		int find = 0;
		for( j = 0 ; j < r.apnum ; ++j )
		{
			if( wcscmp( r.ap[ j ].ssid , m.ap[i].ssid ) == 0 )
			{
				find = 1;
				r.ap[j].rssi = r.ap[j].rssi * r.ap[j].count + m.ap[i].rssi * m.ap[i].count;
				r.ap[j].count += m.ap[i].count;
				r.ap[j].rssi /= r.ap[j].count;
				break;
			}
		}
		if( !find )
		{
			r.ap[j]=m.ap[i];
			r.apnum++;
		}

	}
	m_RssiInfo.apnum = 0;
	m_RssiBuffer.apnum = 0;
	pList->DeleteAllItems();
	for( i = 0; i < r.apnum ; ++i )
	{
		int iRow = pList->GetItemCount();
		int iColumn = 0;
		LV_ITEM lvi;       
		lvi.mask=LVIF_TEXT | LVIF_PARAM;
		lvi.iItem = iRow;        //몇행
		lvi.iSubItem = iColumn;  //몇번째 컬럼
		lvi.pszText = r.ap[i].ssid;     //내용
		lvi.lParam = (LPARAM)iRow*2+iColumn;       //셀을 식별할 고유인자. 
	 
		pList->InsertItem(&lvi);
		pList->SetItemText( iRow,iColumn,r.ap[i].ssid);      //화면에 나타낸다
	   
		unsigned short sig[100];
		
		int rssi = r.ap[i].rssi;
		_itow(r.ap[i].rssi,(wchar_t *)sig,10);
		iColumn = 1;
		lvi.mask=LVIF_TEXT | LVIF_PARAM;
		lvi.iItem = iRow;        //몇행
		lvi.iSubItem = iColumn;  //몇번째 컬럼
		lvi.pszText = (LPWSTR)sig;     //내용
		lvi.lParam = (LPARAM)iRow*2+iColumn;       //셀을 식별할 고유인자. 
	 
		//pList->InsertItem(&lvi);
		pList->SetItemText( iRow,iColumn,(LPCTSTR)sig);      //화면에 나타낸다
	}
	if( r.apnum == 0 )
		OutputDebugString( _T("no ap!!\n") );
	else
		OutputDebugString( _T("get ap!!\n") );
	return 0;	
}
void CWifiScanner::MinMaxRSSI(void)
{
	for( int i = 0 ; i < m_RssiBuffer2.apnum ; ++i )
	{
		int find = 0;
		for( int j = 0 ; j < m_devMin.apnum ; ++j )
		{
			if( wcscmp( m_RssiBuffer2.ap[i].ssid , m_devMin.ap[j].ssid ) == 0 )
			{
				find = 1;
				if( m_RssiBuffer2.ap[i].rssi < m_devMin.ap[j].rssi )
				{
					m_devMin.ap[j] = m_RssiBuffer2.ap[i];
				}
			}
		}
		if(!find )
		{
			m_devMin.ap[m_devMin.apnum++] = m_RssiBuffer2.ap[i];
		}
		find = 0;
		for( int j = 0 ; j < m_devMax.apnum ; ++j )
		{
			if( wcscmp( m_RssiBuffer2.ap[i].ssid , m_devMax.ap[j].ssid ) == 0 )
			{
				find = 1;
				if( m_RssiBuffer2.ap[i].rssi > m_devMax.ap[j].rssi )
				{
					m_devMax.ap[j] = m_RssiBuffer2.ap[i];
				}
			}
		}
		if(!find )
		{
			m_devMax.ap[m_devMax.apnum++] = m_RssiBuffer2.ap[i];
		}
	}
}
