#include "stdafx.h"
#include "Game.h"


//constructor
Game::Game()
{
	m_Board = nullptr;
	m_Height = m_Width = 0;
	m_nBomb = 0;
}


//destructor
Game::~Game()
{
}


//method
void Game::NewGame(int _height, int _width, int _bomb)
{
	//새 게임 시작
	if (m_Board)
	{
		//delete board
		for (size_t i=0; i<m_Height; ++i)
			delete[] m_Board[i];
		delete m_Board;

		m_Board = nullptr;
	}


	SetHeight(_height);
	SetWidth(_width);
	SetBomb(_bomb);


	//새 보드판 만들기
	m_Board = new Cell*[m_Height];
	for (size_t i=0; i<m_Height; ++i)
		m_Board[i] = new Cell[m_Width];


	//cell 위치 설정
	for (size_t i=0; i<m_Height; ++i)
	{
		for (size_t j=0; j<m_Width; ++j)
		{
			m_Board[i][j].SetPos(CPoint(j, i));
			m_Board[i][j].SetState(CS_NORMAL);
		}
	}


	//지뢰 채우기
	int randnum, col, row, bombCount=0;
	while (bombCount < m_nBomb)
	{
		randnum = rand() % (m_Height*m_Width);
		row = randnum / m_Width;
		col = randnum % m_Width;

		if (m_Board[row][col].GetIsBomb())
		{
			continue;
		}
		else
		{
			m_Board[row][col].SetIsBomb(true);
			++bombCount;
		}
	}


	//nAdjBomb 계산
	int countBomb;
	CPoint curr;
	for (size_t i=0; i<m_Height; ++i)
	{
		for (size_t j=0; j<m_Width; ++j)
		{
			countBomb = 0;

			//왼쪽부터 시계방향으로 nAdjBomb 계산
			curr = CPoint(-1, 0);
			if (i+curr.y>=0 && j+curr.x>=0 && i+curr.y<m_Height && j+curr.x<m_Width && m_Board[i+curr.y][j+curr.x].GetIsBomb())
				++countBomb;

			curr = CPoint(-1, -1);
			if (i+curr.y>=0 && j+curr.x>=0 && i+curr.y<m_Height && j+curr.x<m_Width && m_Board[i+curr.y][j+curr.x].GetIsBomb())
				++countBomb;

			curr = CPoint(0, -1);
			if (i+curr.y>=0 && j+curr.x>=0 && i+curr.y<m_Height && j+curr.x<m_Width && m_Board[i+curr.y][j+curr.x].GetIsBomb())
				++countBomb;

			curr = CPoint(1, -1);
			if (i+curr.y>=0 && j+curr.x>=0 && i+curr.y<m_Height && j+curr.x<m_Width && m_Board[i+curr.y][j+curr.x].GetIsBomb())
				++countBomb;

			curr = CPoint(1, 0);
			if (i+curr.y>=0 && j+curr.x>=0 && i+curr.y<m_Height && j+curr.x<m_Width && m_Board[i+curr.y][j+curr.x].GetIsBomb())
				++countBomb;

			curr = CPoint(1, 1);
			if (i+curr.y>=0 && j+curr.x>=0 && i+curr.y<m_Height && j+curr.x<m_Width && m_Board[i+curr.y][j+curr.x].GetIsBomb())
				++countBomb;

			curr = CPoint(0, 1);
			if (i+curr.y>=0 && j+curr.x>=0 && i+curr.y<m_Height && j+curr.x<m_Width && m_Board[i+curr.y][j+curr.x].GetIsBomb())
				++countBomb;

			curr = CPoint(-1, 1);
			if (i+curr.y>=0 && j+curr.x>=0 && i+curr.y<m_Height && j+curr.x<m_Width && m_Board[i+curr.y][j+curr.x].GetIsBomb())
				++countBomb;

			m_Board[i][j].SetAdjBomb(countBomb);
			TRACE(_T("countbomb:%d, i:%d, j:%d\n"), countBomb, i, j);
		}
	}
}

void Game::LClick(int row, int col)
{
	if (m_Board[row][col].GetAdjBomb() != 0
		|| m_Board[row][col].GetIsBomb())	//지뢰 or 주변에 지뢰 있는 칸을 눌렀을 때는 재귀적으로 호출하지 않도록
	{
		m_Board[row][col].LClick();
		return;
	}


	//빈 칸을 눌렀을 때 그 주변 8방향이 계속해서 재귀적으로 눌리도록
	//재귀함수(ReLClick) 이용
	ReLClick(row, col);
}

void Game::ReLClick(int row, int col)
{
	if (row<0 || col<0 || row>=m_Height || col>=m_Width)	//게임판 바깥 영역일 때
		return;
	if (m_Board[row][col].GetState() == CS_OPENED
		|| m_Board[row][col].GetState() == CS_EXPECTBOMB
		|| m_Board[row][col].GetState() == CS_QUESTIONMARK)	//이미 열려있는 곳일 경우 or 마우스 오른쪽 클릭해둔 곳
		return;
	

	m_Board[row][col].LClick();
	if (m_Board[row][col].GetAdjBomb() != 0)	//end case
		return;
	else
	{
		//8방향 클릭. 단 게임판 바깥부분 (ex) (-1, 0) ...) 은 클릭되지 않도록 예외처리
		//왼쪽부터 시계방향으로 curr 변경
		CPoint curr(col, row);

		curr.Offset(-1, 0);
		ReLClick(curr.y, curr.x);

		curr.Offset(0, -1);
		ReLClick(curr.y, curr.x);

		curr.Offset(1, 0);
		ReLClick(curr.y, curr.x);

		curr.Offset(1, 0);
		ReLClick(curr.y, curr.x);

		curr.Offset(0, 1);
		ReLClick(curr.y, curr.x);

		curr.Offset(0, 1);
		ReLClick(curr.y, curr.x);

		curr.Offset(-1, 0);
		ReLClick(curr.y, curr.x);

		curr.Offset(-1, 0);
		ReLClick(curr.y, curr.x);
	}
}

GAMESTATE Game::CheckState()
{
	/*
	 *	1. 지뢰를 눌렀을 때			-> return GS_GAMEOVER
	 *	2. 모든 지뢰를 찾았을 때	-> return GS_WIN
	 *	3. otherwise				-> return GS_ONGOING
	 */

	
	//1. GS_GAMEOVER
	for (size_t i=0; i<m_Height; ++i)
	{
		for (size_t j=0; j<m_Width; ++j)
		{
			if (m_Board[i][j].GetState() == CS_BOOM)
			{
				CheckCellBeforeEnd();
				return GS_GAMEOVER;
			}
		}
	}


	//2. GS_WIN
	bool isWin = true;
	for (size_t i=0; i<m_Height; ++i)
	{
		for (size_t j=0; j<m_Width; ++j)
		{
			if (m_Board[i][j].GetIsBomb() && m_Board[i][j].GetState() != CS_EXPECTBOMB)
			{
				isWin = false;
				goto ONGOING;
			}
		}
	}
	if (isWin)
		return GS_WIN;


	//3. CS_ONGOING
ONGOING:
	return GS_ONGOING;
}

void Game::CheckCellBeforeEnd()
{
	/*
	 *	게임이 끝났을 때(GS_GAMEOVER) 지뢰가 아닌 곳에 flag를 만든 칸의 상태를 변경(CS_WRONG)
	 *	그리고 열리지 않은 폭탄들 상태 CS_UNOPENEDBOMB로 변경
	 */

	for (size_t i=0; i<m_Height; ++i)
	{
		for (size_t j=0; j<m_Width; ++j)
		{
			if (!m_Board[i][j].GetIsBomb() && m_Board[i][j].GetState() == CS_EXPECTBOMB)	//지뢰가 아니면서 flag가 있을 경우
			{
				m_Board[i][j].SetState(CS_WRONG);
			}
			else if (m_Board[i][j].GetIsBomb() && (m_Board[i][j].GetState() == CS_NORMAL || m_Board[i][j].GetState() == CS_QUESTIONMARK))
			{
				m_Board[i][j].SetState(CS_UNOPENEDBOMB);
			}
		}
	}
}

void Game::ClearClickedCell()
{
	/*
	 *	CS_CLICKED 인 Cell을 모두 CS_NORMAL로 변경
	 */

	for (size_t i=0; i<m_Height; ++i)
	{
		for (size_t j=0; j<m_Width; ++j)
		{
			if (m_Board[i][j].GetState() == CS_CLICKED)
			{
				m_Board[i][j].SetState(CS_NORMAL);
			}
		}
	}
}

bool Game::IsOnRange(int row, int col)
{
	/*
	 *	row, col 파라미터가 게임판 안의 영역이면 true, 아니면 false 반환
	 */
	
	if (row<0 || col<0 || row>=m_Height || col>=m_Width)	//게임판 영역 밖
	{
		return false;
	}
	else
	{
		return true;
	}
}