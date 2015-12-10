
// MineDoc.h : interface of the CMineDoc class
//


#pragma once
#include "Game.h"
#include "MineDlg.h"
#include "MineView.h"

class CMineView;

class CMineDoc : public CDocument
{
protected: // create from serialization only
	CMineDoc();
	DECLARE_DYNCREATE(CMineDoc)

// Attributes
public:
	Game m_Game;
	int m_Height, m_Width, m_Bomb;
	int m_nFlag;	//±ê¹ß °¹¼ö
	Cell *m_ClickedCell;

	void CheckFlagNum();


// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CMineDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
//	afx_msg void OnFileNew();
};
