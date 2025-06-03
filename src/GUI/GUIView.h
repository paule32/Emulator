
// GUIView.h: Schnittstelle der CGUIView-Klasse
//

#pragma once


class CGUIView : public CView
{
protected: // Nur aus Serialisierung erstellen
	CGUIView() noexcept;
	DECLARE_DYNCREATE(CGUIView)

// Attribute
public:
	CGUIDoc* GetDocument() const;

// Vorgänge
public:

// Überschreibungen
public:
	virtual void OnDraw(CDC* pDC);  // Überschrieben, um diese Ansicht darzustellen
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementierung
public:
	virtual ~CGUIView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generierte Funktionen für die Meldungstabellen
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // Debugversion in GUIView.cpp
inline CGUIDoc* CGUIView::GetDocument() const
   { return reinterpret_cast<CGUIDoc*>(m_pDocument); }
#endif

