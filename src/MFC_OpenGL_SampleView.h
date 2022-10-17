
// MFC_OpenGL_SampleView.h : interface of the CMFCOpenGLSampleView class
//

#pragma once

#define	IDT_TIMER_0	WM_USER + 200
#define	IDT_TIMER_1	IDT_TIMER_0 + 1


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
	virtual HGLRC GetOldStyleRenderingContext(CDC* pDC);
	virtual BOOL SetupPixelFormat(CDC* pDC);
	UINT StartTimer(UINT timerDuration);
	BOOL StopTimer(UINT timerVal);

private:
	//OpenGL Setup
	CDC* GetRenderingContext(HGLRC& hRC);
	//Rendering Context and Device Context Pointers
	HGLRC	m_hRC;
	CDC* m_pDC;
	UINT m_timer;

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


//	afx_msg void OnTimer(UINT_PTR nIDEvent);
//	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#ifndef _DEBUG  // debug version in MFC_OpenGL_SampleView.cpp
inline CMFCOpenGLSampleDoc* CMFCOpenGLSampleView::GetDocument() const
   { return reinterpret_cast<CMFCOpenGLSampleDoc*>(m_pDocument); }
#endif

