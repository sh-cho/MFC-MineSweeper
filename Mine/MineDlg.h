#pragma once
#include "MineDoc.h"


// CMineDlg ��ȭ �����Դϴ�.
class CMineDoc;

class CMineDlg : public CDialog
{
	DECLARE_DYNAMIC(CMineDlg)

public:
	CMineDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	CMineDlg(CMineDoc* pDoc);
	virtual ~CMineDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MINE_NEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CMineDoc* m_pDoc;
	int m_Height;
	int m_Width;
	int m_Bomb;
};
