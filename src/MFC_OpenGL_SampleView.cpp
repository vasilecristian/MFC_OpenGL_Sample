
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

#include <GL/wglew.h>
#include <GL/glew.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//----------------------------
#include "Utils/uofrGraphics.h"

//Init
void InitializeOpenGL();

//Resize
bool ChangeSize(int w, int h);

//Draw
void Draw(void);
void PreRenderScene(void);
void RenderStockScene(void);
void RenderScene(void);
//void drawSolidSphere(GLfloat radius, GLint slices, GLint stacks);
//void drawSolidCube(GLfloat size);

//OpenGL State Management
////////
GLuint program1;  //Shader
GLint uColor;     //Shader color input
GLint uLightPosition;//Shader light position input
GLint mvIndex;    //Shader positioning input
GLint projIndex;     //Shader projection input
mat4 p, mv;       //Local projection and positioning variables
// Scene Related Functions and Variables
////////

//Model Control Variables
GLfloat rotY = 0;    //rotate model around y axis
GLfloat rotX = 0;    //rotate model around x axis
//----------------------------


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
	: m_hRC(0), m_pDC(0), m_ErrorString(_ErrorStrings[0])
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


	// Find the OpenGL drawing function provided in the Lab 1 notes and call it here
	Draw();
	
	//Swap buffers to show result
	if (m_pDC && (FALSE == ::SwapBuffers(m_pDC->GetSafeHdc())))
	{
		SetError(7);
	}
}


void CMFCOpenGLSampleView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	ChangeSize(cx, cy);

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


// CMFCOpenGLSampleView message handlers




//Function: InitializeOpenGL
//Purpose:
//    Put OpenGL into a useful state for the intended drawing.
//    In this one we:
//        - choose a background color
//        - set up depth testing (Requires GL_DEPTH in Pixel Format)
//        - turn on the lights
//        - set up simplified material lighting properties
//        - set an initial camera position
void InitializeOpenGL()
{
	//Set up shader
	program1 = InitShader("d:/work/MFC_OpenGL_Sample/x64/Debug/vshader91.glsl", "d:/work/MFC_OpenGL_Sample/x64/Debug/fshader91.glsl");

	glUseProgram(program1);

	//Get locations of transformation matrices from shader
	mvIndex = glGetUniformLocation(program1, "mv");
	projIndex = glGetUniformLocation(program1, "p");

	//Get locations of lighting uniforms from shader
	uLightPosition = glGetUniformLocation(program1, "lightPosition");
	uColor = glGetUniformLocation(program1, "uColor");

	//Set default lighting and material properties in shader.
	glUniform4f(uLightPosition, 0.0f, 0.0f, 10.0f, 0.0f);
	glUniform3f(uColor, 1.0f, 1.0f, 1.0f);

	//Configure urgl object in uofrGraphics library
	urgl.connectShader(program1, "vPosition", "vNormal", NULL);

	glClearColor(0, 1, 0, 1);

	glEnable(GL_DEPTH_TEST);
}

// Function: ChangeSize
// Purpose:
//     Tell OpenGL how to deal with a new window size.
// Arguments:
//     int w, h: new width and height of the window, respectively.
bool ChangeSize(int w, int h)
{
	GLfloat aspect_ratio; // width/height ratio

	//Make sure the window size is valid
	if (0 >= w || 0 >= h)
	{
		return false;
	}

	// tell OpenGL to render to whole window area
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	// compute the aspect ratio
	// this is used to prevent the picture from distorting when
	// the window is resized
	aspect_ratio = (GLdouble)w / (GLdouble)h;

	// calculate a new projection matrix
	p = Perspective(90.0f, aspect_ratio, 0.1f, 200.0f);

	// send the projection to the shader
	glUniformMatrix4fv(projIndex, 1, GL_TRUE, p);

	return true;
}

// Function: Draw
// Purpose:
//     Control drawing of the scene. To be called whenever the window
//     needs redrawing.
void Draw()
{
	// Clear the screen and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	PreRenderScene();
	RenderStockScene();
	RenderScene();
}


// Use this to perform view transforms or other tasks
// that will affect both stock scene and detail scene
void PreRenderScene()
{
	// select a default viewing transformation
	// of a 20 degree rotation about the X axis
	// then a -5 unit transformation along Z
	mv = mat4();
	mv *= Translate(0.0f, 0.0f, -15.0f);
	mv *= RotateX(20.0f);

	//Allow variable controlled rotation around local x and y axes.
	mv *= RotateX(rotX);
	mv *= RotateY(rotY);
}


// Function: RenderStockScene
// Purpose:
//     Draw a stock scene that looks like a
//     black and white checkerboard
void RenderStockScene()
{
	const GLfloat delta = 0.5f;

	// define four vertices that make up a square.
	vec4 v1(0.0f, 0.0f, 0.0f, 1.0f);
	vec4 v2(0.0f, 0.0f, delta, 1.0f);
	vec4 v3(delta, 0.0f, delta, 1.0f);
	vec4 v4(delta, 0.0f, 0.0f, 1.0f);


	int color = 0;

	// define the two colors
	vec3 color1(0.9f, 0.9f, 0.9f);
	vec3 color2(0.05f, 0.05f, 0.05f);

	mat4 placementX = mv;
	mat4 placementZ;
	placementX *= Translate(-10.0f * delta, 0.0f, -10.0f * delta);
	for (int x = -10; x <= 10; x++)
	{
		placementZ = placementX;
		for (int z = -10; z <= 10; z++)
		{
			glUniform3fv(uColor, 1, (color++) % 2 ? color1 : color2);
			glUniformMatrix4fv(mvIndex, 1, GL_TRUE, placementZ);
			urgl.drawQuad(v1, v2, v3, v4);
			placementZ *= Translate(0.0f, 0.0f, delta);
		}
		placementX *= Translate(delta, 0.0f, 0.0f);

	}
}

// Function: RenderScene
// Purpose:
//     Your playground. Code additional scene details here.
void RenderScene()
{
	// draw a red sphere inside a light blue cube

	// Set the drawing color to red
	// Arguments are Red, Green, Blue
	glUniform3f(uColor, 1.0f, 0.0f, 0.0f);

	// Move the "drawing space" up by the sphere's radius
	// so the sphere is on top of the checkerboard
	// mv is a transformation matrix. It accumulates transformations through
	// right side matrix multiplication.
	mv *= Translate(0.0f, 0.5f, 0.0f);

	// Rotate drawing space by 90 degrees around X so the sphere's poles
	// are vertical
	mv *= RotateX(90.0f);

	//Send the transformation matrix to the shader
	glUniformMatrix4fv(mvIndex, 1, GL_TRUE, mv);

	// Draw a sphere.
	// Arguments are Radius, Slices, Stacks
	// Sphere is centered around current origin.
	urgl.drawSolidSphere(0.1f, 20, 20);


	// when we rotated the sphere earlier, we rotated drawing space
	// and created a new "frame"
	// to move the cube up or down we now have to refer to the z-axis
	mv *= Translate(0.0f, 0.0f, 2.5f);

	//Send the transformation matrix to the shader
	glUniformMatrix4fv(mvIndex, 1, GL_TRUE, mv);

	// set the drawing color to light blue
	glUniform3f(uColor, 0.5f, 0.5f, 1.0f);

	// Draw the cube.
	// Argument refers to length of side of cube.
	// Cube is centered around current origin.
	urgl.drawSolidCube(2.0f);
}

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
	InitializeOpenGL();


	return 0;
}


void CMFCOpenGLSampleView::OnDestroy()
{
	CView::OnDestroy();

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

CDC* CMFCOpenGLSampleView::GetRenderingContext(HGLRC hRC)
{
	// Can we put this in the constructor?
	CDC*  pDC = new CClientDC(this);

	if (NULL == pDC) // failure to get DC
	{
		SetError(1);
		return nullptr;
	}


	if (!GetOldStyleRenderingContext(pDC, hRC))
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
		if (0 == (hRC = hRC = wglCreateContextAttribsARB(pDC->GetSafeHdc(), 0, attribs)))
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

BOOL CMFCOpenGLSampleView::GetOldStyleRenderingContext(CDC* pDC, HGLRC hRC)
{
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
		return FALSE;
	}

	//If there is an acceptable match, set it as the current 
	if (FALSE == SetPixelFormat(pDC->GetSafeHdc(), pixelFormat, &pfd))
	{
		SetError(3);
		return FALSE;
	}

	//Create a context with this pixel format
	if (0 == (hRC = wglCreateContext(pDC->GetSafeHdc())))
	{
		SetError(4);
		return FALSE;
	}

	//Make it current. Now we're ready to get extended features.
	if (FALSE == wglMakeCurrent(pDC->GetSafeHdc(), hRC))
	{
		SetError(5);
		return FALSE;
	}
	return TRUE;
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
