

//#include <GL/wglew.h>
//#include <GL/glew.h>

#include "GLUtils.h"
#include "ShaderUtils.h"
#include "BasicGraphics.h"

//----------------------------
#include "Utils/BasicGraphics.h"

namespace gl
{
	GLuint GLUtils::program1 = 0;
	GLint GLUtils::uColor = 0;
	GLint GLUtils::uLightPosition = 0;
	GLint GLUtils::mvIndex = 0;
	GLint GLUtils::projIndex = 0;
	gl::mat4 GLUtils::p;
	gl::mat4 GLUtils::mv;     

	GLfloat GLUtils::rotY = 0;   
	GLfloat GLUtils::rotX = 0;

	//Function: InitializeOpenGL
	//Purpose:
	//    Put OpenGL into a useful state for the intended drawing.
	//    In this one we:
	//        - choose a background color
	//        - set up depth testing (Requires GL_DEPTH in Pixel Format)
	//        - turn on the lights
	//        - set up simplified material lighting properties
	//        - set an initial camera position
	void GLUtils::InitializeOpenGL()
	{
		//Set up shader
		program1 = gl::InitShader("vshader.glsl", "fshader.glsl");

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

		//Configure urgl object in BasicGraphics library
		BasicGraphics::ConnectShader(program1, "vPosition", "vNormal", NULL);

		//glClearColor(0, 1, 0, 1);

		glEnable(GL_DEPTH_TEST);
	}

	// Function: ChangeSize
	// Purpose:
	//     Tell OpenGL how to deal with a new window size.
	// Arguments:
	//     int w, h: new width and height of the window, respectively.
	bool GLUtils::ChangeSize(int w, int h)
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
		aspect_ratio = (GLfloat)w / (GLfloat)h;

		// calculate a new projection matrix
		p = Perspective(60.0f, aspect_ratio, 0.1f, 200.0f);

		// send the projection to the shader
		glUniformMatrix4fv(projIndex, 1, GL_TRUE, p);

		return true;
	}

	// Function: Draw
	// Purpose:
	//     Control drawing of the scene. To be called whenever the window
	//     needs redrawing.
	void GLUtils::Draw()
	{
		// Clear the screen and the depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		PreRenderScene();
		RenderStockScene();
		RenderScene();
	}


	// Use this to perform view transforms or other tasks
	// that will affect both stock scene and detail scene
	void GLUtils::PreRenderScene()
	{
		// select a default viewing transformation
		// of a 20 degree rotation about the X axis
		// then a -5 unit transformation along Z
		mv = mat4();
		mv *= Translate(0.0f, 0.0f, -5.0f);
		mv *= RotateX(20.0f);

		//Allow variable controlled rotation around local x and y axes.
		mv *= RotateX(rotX);
		mv *= RotateY(rotY);
	}


	// Function: RenderStockScene
	// Purpose:
	//     Draw a stock scene that looks like a
	//     black and white checkerboard
	void GLUtils::RenderStockScene()
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
				BasicGraphics::DrawQuad(v1, v2, v3, v4);
				placementZ *= Translate(0.0f, 0.0f, delta);
			}
			placementX *= Translate(delta, 0.0f, 0.0f);

		}
	}

	// Function: RenderScene
	// Purpose:
	//     Your playground. Code additional scene details here.
	void GLUtils::RenderScene()
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
		BasicGraphics::DrawSolidSphere(0.1f, 20, 20);


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
		BasicGraphics::DrawSolidCube(1.0f);
	}
};