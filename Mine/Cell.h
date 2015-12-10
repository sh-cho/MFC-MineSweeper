#pragma once
#pragma comment(lib, "msimg32.lib")	//Transparentblt


/*
 *	cell의 상태를 나타내는 enum 변수
 */
enum CELLSTATE
{
	CS_NORMAL,		//기본 cell 상태
	CS_CLICKED,		//왼쪽 클릭이 된 상태(LButtonUp 전)
	CS_OPENED,		//LButtonUp 이후(지뢰가 아닐 때). nAdjBomb 출력. 0이면 출력 X
	CS_EXPECTBOMB,	//마우스 오른쪽 한번 눌렀을 때. 
	CS_QUESTIONMARK,	//마우스 오른쪽 한번 더 눌렀을 때. 한번 더 마우스 오른쪽 누르면 CS_NORMAL로 돌아감.
	CS_BOOM,			//지뢰를 왼쪽 클릭했을 때. 게임 끝

	CS_WRONG,		//지뢰가 아닌 곳에 flag를 만들었을 경우 (게임 종료시에만 출력)
	CS_UNOPENEDBOMB	//m_State가 CS_NORMAL인 지뢰들을 게임 종료 이후 출력하기 위한 state
};


/*
 * 지뢰찾기에서 한 칸 -> Cell 클래스
 *
 */
class Cell
{
private:
	CPoint m_Pos;	//칸의 좌표상 위치. 맨 왼쪽 위칸 -> (0, 0)
	bool m_IsBomb;	//지뢰일 경우 true
	int m_nAdjBomb;	//내 칸 주변의 지뢰의 갯수. (0~8)
	CELLSTATE m_State;	//칸의 상태. enum CELLSTATE 참고
	

public:
	Cell();
	~Cell();


public:
	CPoint GetPos() const { return m_Pos; }
	bool GetIsBomb() const { return m_IsBomb; }
	int GetAdjBomb() const { return m_nAdjBomb; }
	CELLSTATE GetState() const { return m_State; }
	void SetPos(CPoint val) { m_Pos = val; }
	void SetIsBomb(bool val) { m_IsBomb = val; }
	void SetAdjBomb(int val) { m_nAdjBomb = val; }
	void SetState(CELLSTATE val) { m_State = val; }

	void Draw(CDC *pDC);
	void LClick();
	void RClick();
};

