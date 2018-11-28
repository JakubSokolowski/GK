#pragma once
#include "Animation.h"
class Sierpinski :
	public Animation {
public:	
	static void Run();
	static void SetParams(GLfloat deformation, GLfloat width, GLint levels, ColourType colour);
	
private:
	static void Render();
	static void DrawGasket(GLfloat x, GLfloat y, GLfloat szerokosc, GLint poziom);
	static void DrawSquare(GLfloat x, GLfloat y, GLfloat width);


	static GLfloat deformation_level;
	static GLfloat width;
	static GLint recursion_level;
	static ColourType colour;
};

