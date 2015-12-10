#include "stdafx.h"
#include "Cell.h"
#include "resource.h"


//constructor
Cell::Cell()
{
	m_Pos = CPoint(0, 0);
	m_IsBomb = false;
	m_nAdjBomb = 0;
	m_State = CS_NORMAL;
}

//destructor
Cell::~Cell()
{
}


//method
void Cell::Draw(CDC *pDC)
{
	//draw
	//TransparentBlt -> msimg32.lib �ʿ�
	//��� ��� ���� ���� �� ���
	CDC memDC;
	memDC.CreateCompatibleDC(&memDC);

	CBitmap *pOldBitmap, bmp;
	BITMAP bmpInfo;

	size_t id_state = 0;
	switch (m_State)
	{
	case CS_NORMAL: case CS_EXPECTBOMB: case CS_QUESTIONMARK:
		id_state = IDB_CELL;
		break;

	case CS_BOOM:
		id_state = IDB_EXPLODE;
		break;

	default:
		id_state = IDB_CELL_CLICKED;
		break;
	}

	bmp.LoadBitmapW(id_state);
	pOldBitmap = memDC.SelectObject(&bmp);
	bmp.GetBitmap(&bmpInfo);
	

	//��� ��� (IDC_CELL or IDC_CELL_CLICKED)
	pDC->TransparentBlt(MINE_X+m_Pos.x*16, MINE_Y+m_Pos.y*16, bmpInfo.bmWidth, bmpInfo.bmHeight, &memDC,
		0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, RGB(255, 0, 255));

	
	bmp.DeleteObject();
	//memDC.DeleteDC();


	id_state = 0;
	switch (m_State)
	{
	case CS_OPENED:
		id_state = IDB_ADJBOMB;
		break;

	case CS_EXPECTBOMB:
		id_state = IDB_CELL_FLAG;
		break;

	case CS_QUESTIONMARK:
		id_state = IDB_CELL_QMARK;
		break;

	case CS_BOOM:	case CS_WRONG:	case CS_UNOPENEDBOMB:
		id_state = IDB_BOMB;
		break;

	default:
		return;
	}


	if (m_State==CS_OPENED && m_nAdjBomb==0)	//�߰��� ����� �� ���� ��
		return;


	
	//memDC.CreateCompatibleDC(pDC);
	bmp.LoadBitmapW(id_state);
	pOldBitmap = memDC.SelectObject(&bmp);
	bmp.GetBitmap(&bmpInfo);

	if (id_state == IDB_ADJBOMB)	//�ֺ� ���� ���� ���
	{
		pDC->TransparentBlt(MINE_X+m_Pos.x*16, MINE_Y+m_Pos.y*16, 16, 16, &memDC, (m_nAdjBomb-1)*16, 0, 16, 16, RGB(255, 0, 255));
	}
	else
	{
		pDC->TransparentBlt(MINE_X+m_Pos.x*16, MINE_Y+m_Pos.y*16, bmpInfo.bmWidth, bmpInfo.bmHeight, &memDC,
			0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, RGB(255, 0, 255));
	}


	
	bmp.DeleteObject();
	bmp.LoadBitmapW(IDB_WRONG);
	pOldBitmap = memDC.SelectObject(&bmp);
	bmp.GetBitmap(&bmpInfo);

	if (m_State == CS_WRONG)	//CS_WRONG -> X ǥ�� ���
	{
		pDC->TransparentBlt(MINE_X+m_Pos.x*16, MINE_Y+m_Pos.y*16, bmpInfo.bmWidth, bmpInfo.bmHeight, &memDC,
			0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, RGB(255, 0, 255));
	}


	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bmp.DeleteObject();
}

void Cell::LClick()
{
	//���� ĭ�� LButton���� Ŭ������ ��, ��� ǥ���ص� ĭ�� ������ ��, ����ǥ ǥ�� ĭ�� ������ ��
	if (m_State == CS_OPENED || m_State == CS_EXPECTBOMB || m_State == CS_QUESTIONMARK)
		return;


	if (m_IsBomb)	//���ڸ� ������ ��
	{
		m_State = CS_BOOM;
	}
	else	//�Ϲ� ĭ�� ������ ��
	{
		m_State = CS_OPENED;
	}
}

void Cell::RClick()
{
	//ĭ�� Rbutton���� Ŭ������ �� ���� ��ȯ
	switch (m_State)
	{
	case CS_NORMAL:
		m_State = CS_EXPECTBOMB;
		break;

	case CS_CLICKED:
		break;

	case CS_OPENED:
		break;

	case CS_EXPECTBOMB:
		m_State = CS_QUESTIONMARK;
		break;

	case CS_QUESTIONMARK:
		m_State = CS_NORMAL;
		break;

	case CS_BOOM:
		break;

	default:
		break;
	}
}