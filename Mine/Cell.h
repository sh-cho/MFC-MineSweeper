#pragma once
#pragma comment(lib, "msimg32.lib")	//Transparentblt


/*
 *	cell�� ���¸� ��Ÿ���� enum ����
 */
enum CELLSTATE
{
	CS_NORMAL,		//�⺻ cell ����
	CS_CLICKED,		//���� Ŭ���� �� ����(LButtonUp ��)
	CS_OPENED,		//LButtonUp ����(���ڰ� �ƴ� ��). nAdjBomb ���. 0�̸� ��� X
	CS_EXPECTBOMB,	//���콺 ������ �ѹ� ������ ��. 
	CS_QUESTIONMARK,	//���콺 ������ �ѹ� �� ������ ��. �ѹ� �� ���콺 ������ ������ CS_NORMAL�� ���ư�.
	CS_BOOM,			//���ڸ� ���� Ŭ������ ��. ���� ��

	CS_WRONG,		//���ڰ� �ƴ� ���� flag�� ������� ��� (���� ����ÿ��� ���)
	CS_UNOPENEDBOMB	//m_State�� CS_NORMAL�� ���ڵ��� ���� ���� ���� ����ϱ� ���� state
};


/*
 * ����ã�⿡�� �� ĭ -> Cell Ŭ����
 *
 */
class Cell
{
private:
	CPoint m_Pos;	//ĭ�� ��ǥ�� ��ġ. �� ���� ��ĭ -> (0, 0)
	bool m_IsBomb;	//������ ��� true
	int m_nAdjBomb;	//�� ĭ �ֺ��� ������ ����. (0~8)
	CELLSTATE m_State;	//ĭ�� ����. enum CELLSTATE ����
	

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

