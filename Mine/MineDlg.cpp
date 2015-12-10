// MineDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Mine.h"
#include "MineDlg.h"
#include "afxdialogex.h"


// CMineDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMineDlg, CDialog)

CMineDlg::CMineDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMineDlg::IDD, pParent)
	, m_Height(0)
	, m_Width(0)
	, m_Bomb(0)
{
	m_pDoc = nullptr;
}

CMineDlg::CMineDlg(CMineDoc* pDoc)
	: CDialog(CMineDlg::IDD, nullptr)
	, m_pDoc(pDoc)
	, m_Height(m_Height = m_pDoc->m_Game.GetHeight())
	, m_Width(m_Width = m_pDoc->m_Game.GetWidth())
	, m_Bomb(m_pDoc->m_Game.GetBomb())
{
}

CMineDlg::~CMineDlg()
{
}

void CMineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_HEIGHT, m_Height);
	DDV_MinMaxInt(pDX, m_Height, 9, 24);
	DDX_Text(pDX, IDC_WIDTH, m_Width);
	DDV_MinMaxInt(pDX, m_Width, 9, 30);
	DDX_Text(pDX, IDC_BOMB, m_Bomb);
}


BEGIN_MESSAGE_MAP(CMineDlg, CDialog)
END_MESSAGE_MAP()


// CMineDlg 메시지 처리기입니다.
