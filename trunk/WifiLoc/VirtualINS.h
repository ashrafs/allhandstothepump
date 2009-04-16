#pragma once

class CVirtualINS
{
public:
	CVirtualINS(void);
public:
	~CVirtualINS(void);
public:
	double m_X;
	double m_Y;
	double m_Speed;
	double m_SpdX;
	double m_SpdY;
	int	m_bEnd;
	CPtrList *m_Path;
	POSITION m_CurPos;
	CPoint	m_Start , m_End;
	void Init(CPtrList *path , double spd );
	int Move();
	CPoint GetPoint();
	CPoint GetGrid();
	CPoint SetGrid( int x , int y);
	
};
