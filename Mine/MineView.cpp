
// MineView.cpp : implementation of the CMineView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Mine.h"
#endif

#include "MineDoc.h"
#include "MineView.h"
#include "MineDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMineView

IMPLEMENT_DYNCREATE(CMineView, CView)

BEGIN_MESSAGE_MAP(CMineView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_FILE_NEW, &CMineView::OnFileNew)
END_MESSAGE_MAP()

// CMineView construction/destruction

CMineView::CMineView()
{
	// TODO: add construction code here
	
}

CMineView::~CMineView()
{
}

BOOL CMineView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMineView drawing

void CMineView::OnDraw(CDC* pDC)
{
	CMineDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;


	//double buffering
	CRect rect;
	GetClientRect(&rect);

	CDC memDC;
	CBitmap *pOldBitmap, bitmap;
	memDC.CreateCompatibleDC(pDC);
	bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.PatBlt(0, 0, rect.Width(), rect.Height(), WHITENESS);



	//print cell
	for (int i=0; i<pDoc->m_Game.GetHeight(); ++i)
	{
		for (int j=0; j<pDoc->m_Game.GetWidth(); ++j)
		{
			(pDoc->m_Game.GetBoard())[i][j].Draw(&memDC);
		}
	}


	//print other(remain bomb, seconds 등등)
	DrawCounterSpace(&memDC);	//counter space 출력
	DrawBombCounter(&memDC);	//bomb counter 출력


	

	



	//memDC -> pDC
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
	
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();
}

void CMineView::DrawCounterSpace(CDC* pDC)
{
	/*
	 *	counter 영역 배경 출력
	 */

	CMineDoc* pDoc = GetDocument();

	CRect rect;
	GetClientRect(&rect);


	CBrush brush, *pOldBrush;
	CPen pen, *pOldPen;
	
	brush.CreateSolidBrush(RGB(192, 192, 192));
	pen.CreatePen(PS_INSIDEFRAME, 2, RGB(128, 128, 128));

	pOldBrush = pDC->SelectObject(&brush);
	pOldPen = pDC->SelectObject(&pen);

	
	pDC->Rectangle(0, 0, rect.Width(), 37);
	pDC->Rectangle(COUNTER_X-1, COUNTER_Y-1, COUNTER_X+40, COUNTER_Y+24);


	//이전 dc 속성 복구
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
}

void CMineView::DrawBombCounter(CDC* pDC)
{
	//remain bomb 출력
	CMineDoc* pDoc = GetDocument();

	CDC memDC;
	CBitmap bitmap;
	memDC.CreateCompatibleDC(pDC);
	bitmap.LoadBitmapW(IDB_NUMBER);
	memDC.SelectObject(&bitmap);


	int remainBomb = pDoc->m_Game.GetBomb() - pDoc->m_nFlag;
	if (remainBomb < 0)
	{
		remainBomb = 0;
	}


	//백의 자릿수 출력
	pDC->BitBlt(COUNTER_X, COUNTER_Y, 13, 23, &memDC, (remainBomb/100), 0, SRCCOPY);
	bitmap.DeleteObject();
	


	//십의 자릿수 출력
	pDC->BitBlt(COUNTER_X+13, COUNTER_Y, 13, 23, &memDC, ((remainBomb/10) % 10)*13, 0, SRCCOPY);
	bitmap.DeleteObject();


	//일의 자릿수 출력
	pDC->BitBlt(COUNTER_X+26, COUNTER_Y, 13, 23, &memDC, (remainBomb%10)*13, 0, SRCCOPY);
	bitmap.DeleteObject();
}


// CMineView diagnostics

#ifdef _DEBUG
void CMineView::AssertValid() const
{
	CView::AssertValid();
}

void CMineView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMineDoc* CMineView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMineDoc)));
	return (CMineDoc*)m_pDocument;
}
#endif //_DEBUG


void CMineView::CheckEnd()
{
	CMineDoc* pDoc = GetDocument();
	int result = pDoc->m_Game.CheckState();
	switch (result)
	{
	case GS_GAMEOVER:
		MessageBox(_T("지뢰를 밟았습니다"), _T("Game Over"), MB_ICONEXCLAMATION);
		break;

	case GS_WIN:
		MessageBox(_T("이겼습니다."), _T("Win"), MB_ICONASTERISK);
		break;

	default:
		return;
	}


	//새 게임 시작
	CMineDlg mineDlg(pDoc);

	if (mineDlg.DoModal() == IDOK)
	{
		//지뢰갯수가 너무 많을 때
		if (mineDlg.m_Bomb >= mineDlg.m_Height * mineDlg.m_Width)
		{
			MessageBox(_T("지뢰의 갯수가 너무 많습니다"), _T("지뢰찾기"), MB_ICONEXCLAMATION);
			return;
		}


		pDoc->m_Height = mineDlg.m_Height;
		pDoc->m_Width = mineDlg.m_Width;
		pDoc->m_Bomb = mineDlg.m_Bomb;
		pDoc->OnNewDocument();
		Resize(MINE_Y+pDoc->m_Height*16, MINE_X+pDoc->m_Width*16 + 4);

		Invalidate();
		return;
	}
}

void CMineView::CheckFlagNum()
{
	/*
	 *	깃발의 갯수 Update
	 */

	CMineDoc* pDoc = GetDocument();
	pDoc->CheckFlagNum();
	Invalidate(FALSE);
}

void CMineView::Resize(int _height, int _width)
{
	/*
	 *	View Resizing
	 */

	CRect windowRect, clientRect;

	GetParent()->GetWindowRect(windowRect);
	GetWindowRect(clientRect);

	windowRect.SetRect(0, 0, windowRect.Width()-clientRect.Width() + _width, windowRect.Height() - clientRect.Height() + _height);
	GetParent()->SetWindowPos(NULL, 0, 0, windowRect.Width(), windowRect.Height(), SWP_NOMOVE | SWP_NOZORDER);

	GetClientRect(clientRect);
	if (clientRect.Height() < _height)
	{
		GetParent()->SetWindowPos(NULL, 0, 0, windowRect.Width(),
			windowRect.Height()+(_height-clientRect.Height()), SWP_NOMOVE | SWP_NOZORDER);
	}
}


// CMineView message handlers


void CMineView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CMineDoc* pDoc = GetDocument();
	if (pDoc->m_Game.CheckState() != GS_ONGOING)	//게임이 끝났을 때
		return;

	//클릭된 좌표를 이용해 행, 열 계산
	int row = (point.y - MINE_Y) / 16,
		col = (point.x - MINE_X) / 16;

	if (! pDoc->m_Game.IsOnRange(row, col))	//게임판 영역 밖일 경우
		return;



	//LButtonDown -> 클릭한 곳의 Cell 상태가 Normal이면 Clicked 상태로 변경
	//MouseOver에서 클릭한 Cell이 바뀔 때 이전 Cell의 상태를 Normal로 변경
	pDoc->m_ClickedCell = &(pDoc->m_Game.GetBoard())[row][col];
	if (pDoc->m_ClickedCell->GetState() == CS_NORMAL)
	{
		pDoc->m_ClickedCell->SetState(CS_CLICKED);
		Invalidate(FALSE);
	}


	//마우스 왼쪽, 오른쪽 버튼 동시에 눌렀을 때
	if (nFlags & MK_RBUTTON)
	{
		LRButtonDown(row, col);
	}
}


void CMineView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CMineDoc* pDoc = GetDocument();
	if (pDoc->m_Game.CheckState() != GS_ONGOING)	//게임이 끝났을 때
		return;



	//클릭된 좌표를 이용해 행, 열 계산
	int row = (point.y - MINE_Y) / 16,
		col = (point.x - MINE_X) / 16;

	if (! pDoc->m_Game.IsOnRange(row, col))	//게임판 영역 밖일 경우
		return;

	pDoc->m_Game.LClick(row, col);
	Invalidate(FALSE);

	

	//LRButtonUp
	if (nFlags & MK_RBUTTON)
	{
		LRButtonUp(row, col);
	}


	CheckEnd();
}


void CMineView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CMineDoc* pDoc = GetDocument();
	if (pDoc->m_Game.CheckState() != GS_ONGOING)	//게임이 끝났을 때
		return;

	//클릭된 좌표를 이용해 행, 열 계산
	int row = (point.y - MINE_Y) / 16,
		col = (point.x - MINE_X) / 16;

	if (! pDoc->m_Game.IsOnRange(row, col))	//게임판 영역 밖일 경우
		return;

	(pDoc->m_Game.GetBoard())[row][col].RClick();
	Invalidate(FALSE);


	//마우스 왼쪽, 오른쪽 버튼 동시에 눌렀을 때
	if (nFlags & MK_LBUTTON)
	{
		LRButtonDown(row, col);
	}


	CheckFlagNum();
	CheckEnd();
}


void CMineView::OnRButtonUp(UINT nFlags, CPoint point)
{
	CMineDoc* pDoc = GetDocument();
	if (pDoc->m_Game.CheckState() != GS_ONGOING)	//게임이 끝났을 때
		return;


	//클릭된 좌표를 이용해 행, 열 계산
	int row = (point.y - MINE_Y) / 16,
		col = (point.x - MINE_X) / 16;

	if (! pDoc->m_Game.IsOnRange(row, col))	//게임판 영역 밖일 경우
		return;

	


	//LRButtonUp
	if (nFlags & MK_LBUTTON)
	{
		LRButtonUp(row, col);
	}


	CheckEnd();
}


void CMineView::OnMouseMove(UINT nFlags, CPoint point)
{
	CMineDoc* pDoc = GetDocument();
	if (pDoc->m_Game.CheckState() != GS_ONGOING)	//게임이 끝났을 때
		return;

	//클릭된 좌표를 이용해 행, 열 계산
	int row = (point.y - MINE_Y) / 16,
		col = (point.x - MINE_X) / 16;

	if (! pDoc->m_Game.IsOnRange(row, col))	//게임판 영역 밖일 경우
		return;



	//LButtonDown
	if (nFlags == MK_LBUTTON)
	{
		if (pDoc->m_ClickedCell != &(pDoc->m_Game.GetBoard())[row][col])	//마우스 누른채로 다른 칸으로 마우스를 옮겼을 경우
		{
			pDoc->m_Game.ClearClickedCell();


			pDoc->m_ClickedCell = &(pDoc->m_Game.GetBoard())[row][col];
			if (pDoc->m_ClickedCell->GetState() == CS_NORMAL)
			{
				pDoc->m_ClickedCell->SetState(CS_CLICKED);
			}
			
			
			Invalidate(FALSE);
		}

		return;
	}


	//LButtonDown + RButtonDown
	if (nFlags == MK_LBUTTON+MK_RBUTTON)
	{
		if (pDoc->m_ClickedCell != &(pDoc->m_Game.GetBoard())[row][col])	//마우스 누른채로 다른 칸으로 마우스를 옮겼을 경우
		{
			pDoc->m_ClickedCell = &(pDoc->m_Game.GetBoard())[row][col];

			pDoc->m_Game.ClearClickedCell();
			LRButtonDown(row, col);


			Invalidate(FALSE);
		}
	}
}


void CMineView::OnFileNew()
{
	CMineDoc* pDoc = GetDocument();
	CMineDlg mineDlg(pDoc);

	if (mineDlg.DoModal() == IDOK)
	{
		//지뢰갯수가 너무 많을 때
		if (mineDlg.m_Bomb >= mineDlg.m_Height * mineDlg.m_Width)
		{
			MessageBox(_T("지뢰의 갯수가 너무 많습니다"), _T("지뢰찾기"), MB_ICONEXCLAMATION);
			return;
		}


		pDoc->m_Height = mineDlg.m_Height;
		pDoc->m_Width = mineDlg.m_Width;
		pDoc->m_Bomb = mineDlg.m_Bomb;
		pDoc->OnNewDocument();
		Resize(MINE_Y+pDoc->m_Height*16, MINE_X+pDoc->m_Width*16 + 4);

		Invalidate();
		return;
	}
}

void CMineView::LRButtonDown(int row, int col)
{
	//주위 모든 칸의 상태를 CLICKED로 변경
	CMineDoc* pDoc = GetDocument();
	CPoint mousePoint(col, row), curr;
	
	for (int i=-1; i<2; ++i)
	{
		for (int j=-1; j<2; ++j)
		{
			curr = CPoint(mousePoint.x + i, mousePoint.y + j);

			if (! pDoc->m_Game.IsOnRange(curr.y, curr.x))	//게임판 영역 밖일 경우
				continue;

			if (pDoc->m_Game.GetBoard()[curr.y][curr.x].GetState() != CS_NORMAL)	//클릭되지 않은 칸이 아닐 경우
				continue;

			pDoc->m_Game.GetBoard()[curr.y][curr.x].SetState(CS_CLICKED);
		}
	}

	Invalidate(FALSE);
}

void CMineView::LRButtonUp(int row, int col)
{
	/*
	 *	주변에 있는 Flag의 갯수 == 현재 칸의 nAdjBomb일 경우에만
	 *	CS_NORMAL 상태의 주변 칸들이 클릭되도록 설정
	 */
	CMineDoc* pDoc = GetDocument();

	int flagCount = 0;
	CPoint mousePoint(col, row), curr;
	if (pDoc->m_Game.GetBoard()[row][col].GetState() != CS_OPENED)	//현재 마우스가 있는 칸이 숫자가 있는 칸이 아닐 경우
	{
		pDoc->m_Game.ClearClickedCell();
		return;
	}

	//count flag
	for (int i=-1; i<2; ++i)
	{
		for (int j=-1; j<2; ++j)
		{
			curr = CPoint(mousePoint.x + i, mousePoint.y + j);

			if (! pDoc->m_Game.IsOnRange(curr.y, curr.x))	//게임판 영역 밖일 경우
				continue;

			if (pDoc->m_Game.GetBoard()[curr.y][curr.x].GetState() == CS_EXPECTBOMB)
				++flagCount;
		}
	}



	//flagCount != nAdjBomb
	if (pDoc->m_Game.GetBoard()[row][col].GetAdjBomb() != flagCount)
	{
		pDoc->m_Game.ClearClickedCell();
		Invalidate(FALSE);
		return;
	}


	//3x3 칸 클릭
	for (int i=-1; i<2; ++i)
	{
		for (int j=-1; j<2; ++j)
		{
			curr = CPoint(mousePoint.x + i, mousePoint.y + j);

			if (! pDoc->m_Game.IsOnRange(curr.y, curr.x))	//게임판 영역 밖일 경우
				continue;

			if (pDoc->m_Game.GetBoard()[curr.y][curr.x].GetState() == CS_CLICKED)
				pDoc->m_Game.LClick(curr.y, curr.x);
		}
	}

	pDoc->m_Game.ClearClickedCell();
	Invalidate(FALSE);
}



//CMineView* CMineView::GetView()
//{
//	CFrameWnd* pFrame = (CFrameWnd*)(AfxGetApp()->m_pMainWnd);
//	CView* pView = pFrame->GetActiveView();
//	if (!pView)
//		return nullptr;
//
//	if (! pView->IsKindOf(RUNTIME_CLASS(CMineView)))
//		return nullptr;
//
//	return (CMineView*)pView;
//}