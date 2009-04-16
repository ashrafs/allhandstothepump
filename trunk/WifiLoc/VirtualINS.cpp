#include "StdAfx.h"
#include "VirtualINS.h"
#include "WifiLocDoc.h"
#include <math.h>
CVirtualINS::CVirtualINS(void)
{
}

CVirtualINS::~CVirtualINS(void)
{
}

void CVirtualINS::Init(CPtrList *path , double spd )
{
	m_Path = path;
	m_Speed = spd;
	m_CurPos = path->GetHeadPosition();
	PATH* p =(PATH*) path->GetNext( m_CurPos );
	CPoint pt = SetGrid( p->start.x , p->start.y );
	m_X = pt.x;
	m_Y = pt.y;
	m_Start = pt;
	m_End = SetGrid( p->end.x , p->end.y );
	double w = m_End.x - m_Start.x;
	double h = m_End.y - m_Start.y;
	double l = sqrt((double)(w*w +h*h));
	m_SpdX = spd * w / l;
	m_SpdY = spd * h / l;
	m_bEnd = 0;
}

int CVirtualINS::Move()
{
	if( m_bEnd )
		return 0;
	double prevx = m_X;
	double prevy = m_Y;
	m_X += m_SpdX;
	m_Y += m_SpdY;
	if( m_SpdX && ( prevx - m_End.x ) * ( m_X - m_End.x ) <= 0 || m_SpdY && ( prevy - m_End.y ) * ( m_Y - m_End.y ) <= 0)
	{
		if( !m_CurPos )
		{
			m_bEnd = 1;
			return 1;
		}
		PATH* p =(PATH*) m_Path->GetNext( m_CurPos );

		CPoint pt = SetGrid( p->start.x , p->start.y );
		m_X = pt.x;
		m_Y = pt.y;
		m_Start = pt;
		m_End = SetGrid( p->end.x , p->end.y );
		double w = m_End.x - m_Start.x;
		double h = m_End.y - m_Start.y;
		double l = sqrt((double)(w*w +h*h));
		m_SpdX = m_Speed * w / l;
		m_SpdY = m_Speed * h / l;
	}
	return 0;

}

CPoint CVirtualINS::SetGrid( int x , int y)
{
	CPoint p;
	p.x = x * GRID_SIZE + GRID_SIZE / 2;
	p.y = y * GRID_SIZE + GRID_SIZE / 2;
	return p;
}


CPoint CVirtualINS::GetPoint()
{
	CPoint p;
	p.x = (int)m_X;
	p.y = (int)m_Y;
	return p;
}

CPoint CVirtualINS::GetGrid()
{
	CPoint p;
	p.x = (int)m_X / GRID_SIZE;
	p.y = (int)m_Y / GRID_SIZE;
	return p;
}