#pragma once


#include "GLHeaders.h"
#include "vec.h"
#include "mat.h"

namespace gl
{
    class BasicGraphics
    {
        static GLuint m_shaderProgram;
        static GLuint m_positionAttribLoc;
        static GLuint m_normalAttribLoc;
        static GLuint m_colourAttribLoc;
        static gl::vec3 m_colour;
        static GLuint m_primitive;

    public:

        static void ConnectShader(GLuint shaderProgram,
                                    const char* positionAttribName,
                                    const char* normalAttribName,
                                    const char* colourAttribName);

        static void SetDrawColour(gl::vec3 colour);
        static void SetDrawColour(float r, float g, float b);
        static void DrawSolidSphere(GLfloat radius, GLint slices, GLint stacks);
        static void DrawSolidCube(GLfloat size);
        static void DrawQuad(gl::vec4 v1, gl::vec4 v2, gl::vec4 v3, gl::vec4 v4);
        static void DrawWireSphere(GLfloat radius, GLint slices, GLint stacks);
        static void DrawWireCube(GLfloat size);
        static void DrawSolidTorus(GLfloat innerRadius, GLfloat outerRadius, GLint nSides, GLint nRings);
        static void DrawWireTorus(GLfloat innerRadius, GLfloat outerRadius, GLint nSides, GLint nRings);
    };
}

