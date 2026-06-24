
// MyFractalView.h : interface de la classe CMyFractalView
//

#pragma once


class CMyFractalView : public CView
{
protected: // création à partir de la sérialisation uniquement
	CMyFractalView() noexcept;
	DECLARE_DYNCREATE(CMyFractalView)

// Attributs
public:
	CMyFractalDoc* GetDocument() const;

// Opérations
public:

// Substitutions
public:
	virtual void OnDraw(CDC* pDC);  // substitué pour dessiner cette vue
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implémentation
public:
	virtual ~CMyFractalView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Fonctions générées de la table des messages
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // version Debug dans MyFractalView.cpp
inline CMyFractalDoc* CMyFractalView::GetDocument() const
   { return reinterpret_cast<CMyFractalDoc*>(m_pDocument); }
#endif

