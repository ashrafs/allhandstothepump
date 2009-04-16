#ifndef _IPC_PROTOCOL_H____
#define _IPC_PROTOCOL_H____

#define IPC_PROTOCOL_START		0
#define IPC_PROTOCOL_INSDATA	1
#define IPC_PROTOCOL_WIFIDATA	2

typedef struct
{
	int x,y;
}IPC_START;

typedef struct
{
	int x,y;
	double trust;
}IPC_INSDATA;

typedef struct
{
	int x,y;
	double trust;
}IPC_WIFIDATA;

#endif