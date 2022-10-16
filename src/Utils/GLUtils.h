#pragma once

#include "GLHeaders.h"
#include "mat.h"

namespace gl
{
	class GLUtils
	{
		//OpenGL State Management
		////////
		static GLuint program1;  //Shader
		static GLint uColor;     //Shader color input
		static GLint uLightPosition;//Shader light position input
		static GLint mvIndex;    //Shader positioning input
		static GLint projIndex;     //Shader projection input
		static gl::mat4 p, mv;       //Local projection and positioning variables
		// Scene Related Functions and Variables
		////////

		//Model Control Variables
		static GLfloat rotY;    //rotate model around y axis
		static GLfloat rotX;    //rotate model around x axis
		//----------------------------

	public:
		//Init
		static void InitializeOpenGL();

		//Resize
		static bool ChangeSize(int w, int h);

		//Draw
		static void Draw(void);
		static void PreRenderScene(void);
		static void RenderStockScene(void);
		static void RenderScene(void);
		//void drawSolidSphere(GLfloat radius, GLint slices, GLint stacks);
		//void drawSolidCube(GLfloat size);
	};
}