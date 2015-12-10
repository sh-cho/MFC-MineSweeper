#pragma once
#include "MineDoc.h"


// CMineDlg 대화 상자입니다.
class CMineDoc;

class CMineDlg : public CDialog
{
	DECLARE_DYNAMIC(CMineDlg)

public:
	CMineDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CMineDlg(CMineDoc* pDoc);
	virtual ~CMineDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MINE_NEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CMineDoc* m_pDoc;
	int m_Height;
	int m_Width;
	int m_Bomb;
};
