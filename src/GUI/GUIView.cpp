
// GUIView.cpp: Implementierung der CGUIView-Klasse
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS können in einem ATL-Projekt definiert werden, in dem Vorschau-, Miniaturansichts- 
// und Suchfilterhandler implementiert werden, und die Freigabe von Dokumentcode für das Projekt wird ermöglicht.
#ifndef SHARED_HANDLERS
#include "GUI.h"
#endif

#include "GUIDoc.h"
#include "GUIView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGUIView

IMPLEMENT_DYNCREATE(CGUIView, CView)

BEGIN_MESSAGE_MAP(CGUIView, CView)
	// Standarddruckbefehle
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CGUIView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CGUIView-Erstellung/Zerstörung

CGUIView::CGUIView() noexcept
{
	// TODO: Hier Code zur Konstruktion einfügen

}

CGUIView::~CGUIView()
{
}

BOOL CGUIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Ändern Sie hier die Fensterklasse oder die Darstellung, indem Sie
	//  CREATESTRUCT cs modifizieren.

	return CView::PreCreateWindow(cs);
}

// CGUIView-Zeichnung

void CGUIView::OnDraw(CDC* /*pDC*/)
{
	CGUIDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: Code zum Zeichnen der nativen Daten hinzufügen
}


// CGUIView-Druck


void CGUIView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CGUIView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Standardvorbereitung
	return DoPreparePrinting(pInfo);
}

void CGUIView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: Zusätzliche Initialisierung vor dem Drucken hier einfügen
}

void CGUIView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: Bereinigung nach dem Drucken einfügen
}

void CGUIView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CGUIView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CGUIView-Diagnose

#ifdef _DEBUG
void CGUIView::AssertValid() const
{
	CView::AssertValid();
}

void CGUIView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGUIDoc* CGUIView::GetDocument() const // Nichtdebugversion ist inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGUIDoc)));
	return (CGUIDoc*)m_pDocument;
}
#endif //_DEBUG


// CGUIView-Meldungshandler
