
// MineDoc.cpp : implementation of the CMineDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Mine.h"
#endif

#include "MineDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMineDoc

IMPLEMENT_DYNCREATE(CMineDoc, CDocument)

BEGIN_MESSAGE_MAP(CMineDoc, CDocument)
//	ON_COMMAND(ID_FILE_NEW, &CMineDoc::OnFileNew)
END_MESSAGE_MAP()


// CMineDoc construction/destruction

CMineDoc::CMineDoc()
{
	m_Height = 16;
	m_Width = 30;
	m_Bomb = 77;
	m_nFlag = 0;

	m_ClickedCell = nullptr;
}

CMineDoc::~CMineDoc()
{
}

BOOL CMineDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	
	m_Game.NewGame(m_Height, m_Width, m_Bomb);
	CMineView* pView = nullptr;
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL)
		pView = (CMineView*)GetNextView(pos);

	m_nFlag = 0;
	pView->Resize(MINE_Y+m_Height*16, MINE_X+m_Width*16 + 4);

	m_ClickedCell = nullptr;
	return TRUE;
}




// CMineDoc serialization

void CMineDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CMineDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CMineDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CMineDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CMineDoc diagnostics

#ifdef _DEBUG
void CMineDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMineDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMineDoc commands

void CMineDoc::CheckFlagNum()
{
	int flagNum = 0;

	for (size_t i=0; i<m_Game.GetHeight(); ++i)
	{
		for (size_t j=0; j<m_Game.GetWidth(); ++j)
		{
			if (m_Game.GetBoard()[i][j].GetState() == CS_EXPECTBOMB)
				++flagNum;
		}
	}

	m_nFlag = flagNum;
}