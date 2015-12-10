#pragma once
#pragma warning(disable:4018)
#include "Cell.h"


/*
 *	game state ����.
 *	Ŭ���� �Ͼ �� ���� game�� CheckState�� ȣ���ؼ� state�� Ȯ��.
 */
enum GAMESTATE
{
	GS_ONGOING,		//������. GAMEOVER, WIN�� �ƴ϶�� ��� ONGOING
	GS_GAMEOVER,	//���� ��. ���ڸ� ������ ��
	GS_WIN			//��� ���ڸ� ã���� ��
};


/*
 *	Game Ŭ����
 *	����ã�� ���� Ŭ����
 *	Document�� ����
 *	NewDocument�� �� NewGame
 */
class Game
{
private:
	Cell** m_Board;			//������. ��ť��Ʈ�� ���� ���鶧���� delete�ϰ� �����Ҵ��� ���.
	int m_Height, m_Width;	//�������� ����, ����ũ��
	int m_nBomb;			//������ ����
	

public:
	Game();
	~Game();


public:
	Cell** GetBoard() const { return m_Board; }
	int GetHeight() const { return m_Height; }
	int GetWidth() const { return m_Width; }
	int GetBomb() const { return m_nBomb; }
	void SetBoard(Cell** val) { m_Board = val; }
	void SetHeight(int val) { m_Height = val; }
	void SetWidth(int val) { m_Width = val; }
	void SetBomb(int val) { m_nBomb = val; }

	void NewGame(int _height=10, int _width=10, int _bomb=10);
	void LClick(int row, int col);
	void ReLClick(int row, int col);
	GAMESTATE CheckState();
	void CheckCellBeforeEnd();
	void ClearClickedCell();
	bool IsOnRange(int row, int col);
};

