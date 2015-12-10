#pragma once
#pragma warning(disable:4018)
#include "Cell.h"


/*
 *	game state 변수.
 *	클릭이 일어날 때 마다 game의 CheckState를 호출해서 state를 확인.
 */
enum GAMESTATE
{
	GS_ONGOING,		//게임중. GAMEOVER, WIN이 아니라면 모두 ONGOING
	GS_GAMEOVER,	//게임 끝. 지뢰를 눌렀을 때
	GS_WIN			//모든 지뢰를 찾았을 때
};


/*
 *	Game 클래스
 *	지뢰찾기 게임 클래스
 *	Document에 선언
 *	NewDocument할 때 NewGame
 */
class Game
{
private:
	Cell** m_Board;			//게임판. 도큐먼트를 새로 만들때마다 delete하고 동적할당해 사용.
	int m_Height, m_Width;	//게임판의 세로, 가로크기
	int m_nBomb;			//지뢰의 갯수
	

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

