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
	//�� ���� ����
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


	//�� ������ �����
	m_Board = new Cell*[m_Height];
	for (size_t i=0; i<m_Height; ++i)
		m_Board[i] = new Cell[m_Width];


	//cell ��ġ ����
	for (size_t i=0; i<m_Height; ++i)
	{
		for (size_t j=0; j<m_Width; ++j)
		{
			m_Board[i][j].SetPos(CPoint(j, i));
			m_Board[i][j].SetState(CS_NORMAL);
		}
	}


	//���� ä���
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


	//nAdjBomb ���
	int countBomb;
	CPoint curr;
	for (size_t i=0; i<m_Height; ++i)
	{
		for (size_t j=0; j<m_Width; ++j)
		{
			countBomb = 0;

			//���ʺ��� �ð�������� nAdjBomb ���
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
		|| m_Board[row][col].GetIsBomb())	//���� or �ֺ��� ���� �ִ� ĭ�� ������ ���� ��������� ȣ������ �ʵ���
	{
		m_Board[row][col].LClick();
		return;
	}


	//�� ĭ�� ������ �� �� �ֺ� 8������ ����ؼ� ��������� ��������
	//����Լ�(ReLClick) �̿�
	ReLClick(row, col);
}

void Game::ReLClick(int row, int col)
{
	if (row<0 || col<0 || row>=m_Height || col>=m_Width)	//������ �ٱ� ������ ��
		return;
	if (m_Board[row][col].GetState() == CS_OPENED
		|| m_Board[row][col].GetState() == CS_EXPECTBOMB
		|| m_Board[row][col].GetState() == CS_QUESTIONMARK)	//�̹� �����ִ� ���� ��� or ���콺 ������ Ŭ���ص� ��
		return;
	

	m_Board[row][col].LClick();
	if (m_Board[row][col].GetAdjBomb() != 0)	//end case
		return;
	else
	{
		//8���� Ŭ��. �� ������ �ٱ��κ� (ex) (-1, 0) ...) �� Ŭ������ �ʵ��� ����ó��
		//���ʺ��� �ð�������� curr ����
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
	 *	1. ���ڸ� ������ ��			-> return GS_GAMEOVER
	 *	2. ��� ���ڸ� ã���� ��	-> return GS_WIN
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
	 *	������ ������ ��(GS_GAMEOVER) ���ڰ� �ƴ� ���� flag�� ���� ĭ�� ���¸� ����(CS_WRONG)
	 *	�׸��� ������ ���� ��ź�� ���� CS_UNOPENEDBOMB�� ����
	 */

	for (size_t i=0; i<m_Height; ++i)
	{
		for (size_t j=0; j<m_Width; ++j)
		{
			if (!m_Board[i][j].GetIsBomb() && m_Board[i][j].GetState() == CS_EXPECTBOMB)	//���ڰ� �ƴϸ鼭 flag�� ���� ���
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
	 *	CS_CLICKED �� Cell�� ��� CS_NORMAL�� ����
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
	 *	row, col �Ķ���Ͱ� ������ ���� �����̸� true, �ƴϸ� false ��ȯ
	 */
	
	if (row<0 || col<0 || row>=m_Height || col>=m_Width)	//������ ���� ��
	{
		return false;
	}
	else
	{
		return true;
	}
}