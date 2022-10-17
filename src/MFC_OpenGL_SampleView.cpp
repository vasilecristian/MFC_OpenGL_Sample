
// MFC_OpenGL_SampleView.cpp : implementation of the CMFCOpenGLSampleView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MFC_OpenGL_Sample.h"
#endif

#include "MFC_OpenGL_SampleDoc.h"
#include "MFC_OpenGL_SampleView.h"


#include "Utils/GLHeaders.h"
#include "Utils/GLUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCOpenGLSampleView

IMPLEMENT_DYNCREATE(CMFCOpenGLSampleView, CView)

BEGIN_MESSAGE_MAP(CMFCOpenGLSampleView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFCOpenGLSampleView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// You will add stuff here!!!!
const char* const CMFCOpenGLSampleView::_ErrorStrings[] = {
	{"No Error"},                     // 0
	{"Unable to get a DC"},           // 1
	{"ChoosePixelFormat failed"},     // 2
	{"SelectPixelFormat failed"},     // 3
	{"wglCreateContext failed"},      // 4
	{"wglMakeCurrent failed"},        // 5
	{"wglDeleteContext failed"},      // 6
	{"SwapBuffers failed"},           // 7
};

// CMFCOpenGLSampleView construction/destruction

CMFCOpenGLSampleView::CMFCOpenGLSampleView() noexcept
	: m_hRC(0), m_pDC(0), m_timer(0), m_ErrorString(_ErrorStrings[0])
{
	// TODO: add construction code here

}

CMFCOpenGLSampleView::~CMFCOpenGLSampleView()
{
}

BOOL CMFCOpenGLSampleView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	// You will add stuff here !!!
	// An OpenGL window must be created with the following flags and must not
	// include CS_PARENTDC for the class style.
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;


	return CView::PreCreateWindow(cs);
}

// CMFCOpenGLSampleView drawing

void CMFCOpenGLSampleView::OnDraw(CDC* /*pDC*/)
{
	CMFCOpenGLSampleDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;


	gl::GLUtils::Update();
	// Find the OpenGL drawing function provided in the Lab 1 notes and call it here
	gl::GLUtils::Draw();
	
	//Swap buffers to show result
	if (m_pDC && (FALSE == ::SwapBuffers(m_pDC->GetSafeHdc())))
	{
		SetError(7);
	}
}


void CMFCOpenGLSampleView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	gl::GLUtils::ChangeSize(cx, cy);

}

// CMFCOpenGLSampleView printing


void CMFCOpenGLSampleView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFCOpenGLSampleView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMFCOpenGLSampleView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMFCOpenGLSampleView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CMFCOpenGLSampleView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFCOpenGLSampleView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFCOpenGLSampleView diagnostics

#ifdef _DEBUG
void CMFCOpenGLSampleView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCOpenGLSampleView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCOpenGLSampleDoc* CMFCOpenGLSampleView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCOpenGLSampleDoc)));
	return (CMFCOpenGLSampleDoc*)m_pDocument;
}
#endif //_DEBUG



int CMFCOpenGLSampleView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	m_pDC = GetRenderingContext(m_hRC);
	if (!m_pDC)
	{
		//Something went wrong with getting the rendering context.
		//Create a popup with the error message, then quit.
		AfxMessageBox(CString(m_ErrorString));
		return -1;
	}

	// We now have a rendering context, so we can set the initial drawing state.
	// Find the initialize OpenGL function provided in the Lab 1 notes and call it here
	gl::GLUtils::InitializeOpenGL();

	m_timer = StartTimer(20);  // a 200 mSec timer

	return 0;
}


void CMFCOpenGLSampleView::OnDestroy()
{
	CView::OnDestroy();

	StopTimer(IDT_TIMER_0);

	// TODO: Add your message handler code here
	if (FALSE == ::wglDeleteContext(m_hRC))
	{
		SetError(6);
	}

	if (m_pDC)
	{
		delete m_pDC;
	}
}



/////////////////////////////////////////////////////////////////////////////
// GL Rendering Context Creation Functions
//
// Since we are using Windows native windowing, we need to set up our own
// OpenGL rendering context. These functions do it to the main view area.
// It is possible to do it to a smaller sub view. If you are curious, you can
// find tutorials on how to do that on the net.
//

// Error reporting utility
void CMFCOpenGLSampleView::SetError(int e)
{
	// if there was no previous error,
	// then save this one
	if (_ErrorStrings[0] == m_ErrorString)
	{
		m_ErrorString = _ErrorStrings[e];
	}
}

CDC* CMFCOpenGLSampleView::GetRenderingContext(HGLRC& hRC)
{
	// Can we put this in the constructor?
	CDC*  pDC = new CClientDC(this);

	if (NULL == pDC) // failure to get DC
	{
		SetError(1);
		return nullptr;
	}

	hRC = GetOldStyleRenderingContext(pDC);
	if (!hRC)
	{
		return pDC;
	}

	//Get access to modern OpenGL functionality from this old style context.
	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit())
	{
		AfxMessageBox(_T("GLEW could not be initialized!"));
		return nullptr;
	}

	//Get a new style pixel format
	if (!SetupPixelFormat(pDC))
	{
		return nullptr;
	}

	//Setup request for OpenGL 3.2 Core Profile
	int attribs[] =
	{
	   WGL_CONTEXT_MAJOR_VERSION_ARB,   3,
	   WGL_CONTEXT_MINOR_VERSION_ARB,   2,
	   WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
	   //WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
	   0, 0  //End
	};

	if (wglewIsSupported("WGL_ARB_create_context") == 1)
	{
		//If this driver supports new style rendering contexts, create one
		HGLRC oldContext = hRC;
		hRC = wglCreateContextAttribsARB(pDC->GetSafeHdc(), 0, attribs);
		if (0 == hRC)
		{
			SetError(4);
			return nullptr;
		}

		if (!wglMakeCurrent(NULL, NULL))
			wglDeleteContext(oldContext);
		if (FALSE == wglMakeCurrent(pDC->GetSafeHdc(), hRC))
		{
			SetError(5);
			return nullptr;
		}
	}
	else
	{
		//Otherwise use the old style rendering context we created earlier.
		AfxMessageBox(_T("GL 3.2 Context not possible. Using old style context. (GL 2.1 and before)"));
	}

	return pDC;
}

HGLRC CMFCOpenGLSampleView::GetOldStyleRenderingContext(CDC* pDC)
{
	HGLRC hRC = nullptr;

	//A generic pixel format descriptor. This will be replaced with a more
	//specific and modern one later, so don't worry about it too much.
	static PIXELFORMATDESCRIPTOR pfd =
	{
	   sizeof(PIXELFORMATDESCRIPTOR),
	   1,
	   PFD_DRAW_TO_WINDOW |            // support window
	   PFD_SUPPORT_OPENGL |            // support OpenGL
	   PFD_DOUBLEBUFFER,               // double buffered
	   PFD_TYPE_RGBA,                  // RGBA type
	   32,                             // 32-bit color depth
	   0, 0, 0, 0, 0, 0,               // color bits ignored
	   0,                              // no alpha buffer
	   0,                              // shift bit ignored
	   0,                              // no accumulation buffer
	   0, 0, 0, 0,                     // accum bits ignored
	   24,                        // 24-bit z-buffer
	   0,                              // no stencil buffer
	   0,                              // no auxiliary buffer
	   PFD_MAIN_PLANE,                 // main layer
	   0,                              // reserved
	   0, 0, 0                         // layer masks ignored
	};

	// Get the id number for the best match supported by the hardware device context
	// to what is described in pfd
	int pixelFormat = ChoosePixelFormat(pDC->GetSafeHdc(), &pfd);

	//If there's no match, report an error
	if (0 == pixelFormat)
	{
		SetError(2);
		return hRC;
	}

	//If there is an acceptable match, set it as the current 
	if (FALSE == SetPixelFormat(pDC->GetSafeHdc(), pixelFormat, &pfd))
	{
		SetError(3);
		return hRC;
	}

	//Create a context with this pixel format
	if (0 == (hRC = wglCreateContext(pDC->GetSafeHdc())))
	{
		SetError(4);
		return hRC;
	}

	//Make it current. Now we're ready to get extended features.
	if (FALSE == wglMakeCurrent(pDC->GetSafeHdc(), hRC))
	{
		SetError(5);
		return hRC;
	}
	return hRC;
}

BOOL CMFCOpenGLSampleView::SetupPixelFormat(CDC* pDC)
{
	//This is a modern pixel format attribute list.
	//It has an extensible structure. Just add in more argument pairs 
	//befroe the null to request more features.
	const int attribList[] =
	{
	   WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
	   WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
	   WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
	   WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
	   WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
	   WGL_COLOR_BITS_ARB,     32,
	   WGL_DEPTH_BITS_ARB,     24,
	   WGL_STENCIL_BITS_ARB,   8,
	   0, 0  //End
	};


	unsigned int numFormats;
	int pixelFormat;
	PIXELFORMATDESCRIPTOR pfd;

	//Select a pixel format number
	wglChoosePixelFormatARB(pDC->GetSafeHdc(), attribList, NULL, 1, &pixelFormat, &numFormats);

	//Optional: Get the pixel format's description. We must provide a 
	//description to SetPixelFormat(), but its contents mean little.
	//According to MSDN: 
	//  The system's metafile component uses this structure to record the logical
	//  pixel format specification. The structure has no other effect upon the
	//  behavior of the SetPixelFormat function.
	//DescribePixelFormat(m_pDC->GetSafeHdc(), pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	//Set it as the current 
	if (FALSE == SetPixelFormat(pDC->GetSafeHdc(), pixelFormat, &pfd))
	{
		SetError(3);
		return FALSE;
	}

	return TRUE;
}

void CMFCOpenGLSampleView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	this->Invalidate();

	CView::OnTimer(nIDEvent);
}

UINT CMFCOpenGLSampleView::StartTimer(UINT timerDuration)
{
	UINT    TimerVal;
	TimerVal = SetTimer(IDT_TIMER_0, timerDuration, NULL);

	if (TimerVal == 0)
	{
		AfxMessageBox(_T("Unable to obtain timer IDT_TIMER_0"));
	}
	return TimerVal;
}// end StartTimer

BOOL CMFCOpenGLSampleView::StopTimer(UINT timerVal)
{
	//
	//    Stop the timer
	//

	if (!KillTimer(timerVal))
	{
		return FALSE;
	}
	//
	//    Place clean-up code following this point.
	//
	return TRUE;
} // end StopTimer

