
// MFC_OpenGL_SampleView.h : interface of the CMFCOpenGLSampleView class
//

#pragma once

#include "Utils/Angel.h";


class CMFCOpenGLSampleView : public CView
{
protected: // create from serialization only
	CMFCOpenGLSampleView() noexcept;
	DECLARE_DYNCREATE(CMFCOpenGLSampleView)

// Attributes
public:
	CMFCOpenGLSampleDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CMFCOpenGLSampleView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

//--------------------------------------
protected:
	// You will add the following stuff!!!
	virtual BOOL GetOldStyleRenderingContext(CDC* pDC, HGLRC hRC);
	virtual BOOL SetupPixelFormat(CDC* pDC);

private:
	//OpenGL Setup
	CDC* GetRenderingContext(HGLRC hRC);
	//Rendering Context and Device Context Pointers
	HGLRC	m_hRC;
	CDC* m_pDC;

	//Error Handling
	void SetError(int e);
	static const char* const _ErrorStrings[];
	const char* m_ErrorString;
//--------------------------------------


// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);


};

#ifndef _DEBUG  // debug version in MFC_OpenGL_SampleView.cpp
inline CMFCOpenGLSampleDoc* CMFCOpenGLSampleView::GetDocument() const
   { return reinterpret_cast<CMFCOpenGLSampleDoc*>(m_pDocument); }
#endif

