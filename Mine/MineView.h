
// MineView.h : interface of the CMineView class
//

#pragma once
#include "MineDoc.h"

class CMineDoc;

class CMineView : public CView
{
protected: // create from serialization only
	CMineView();
	DECLARE_DYNCREATE(CMineView)

// Attributes
public:
	CMineDoc* GetDocument() const;
	void CheckEnd();
	void CheckFlagNum();
	void LRButtonDown(int row, int col);
	void LRButtonUp(int row, int col);
	void Resize(int _height, int _width);
	void DrawCounterSpace(CDC* pDC);
	void DrawBombCounter(CDC* pDC);


// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CMineView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnFileNew();
};

#ifndef _DEBUG  // debug version in MineView.cpp
inline CMineDoc* CMineView::GetDocument() const
   { return reinterpret_cast<CMineDoc*>(m_pDocument); }
#endif

