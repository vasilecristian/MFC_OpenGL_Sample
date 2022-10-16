#pragma once

//  Define M_PI in the case it's not defined in the math header file
#ifndef M_PI
#  define M_PI  3.14159265358979323846
#endif


#ifdef __APPLE__  // include Mac OS X verions of headers
#include <GLEW/GLEW.h>
#ifdef GLUT_3_2_CORE_PROFILE
#include <OpenGL/gl3.h>
#endif
#include <GLUT/GLUT.h>
#else // non-Mac OS X operating systems
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#endif  // __APPLE__

#include <GL/wglew.h>
#include <GL/glew.h>
