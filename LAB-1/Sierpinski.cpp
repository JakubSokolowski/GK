#include "Sierpinski.h"
#include <thread>

GLfloat Sierpinski::deformation_level = 0.0;
GLfloat Sierpinski::width = 100;
GLint Sierpinski::recursion_level = 4;
ColourType Sierpinski::colour = ColourType::WHITE;

void Sierpinski::DrawGasket(GLfloat x, GLfloat y, GLfloat width, GLint level) {

	// If the final level is not yet reached
	// Divide the current square into nine parts.
	// For each part, except middle call recursive
	// drawing function
	if (level > 0){
		width = width / 3;
		// Draw each square in level, clockwise
		DrawGasket(x, y, width, level - 1);
		DrawGasket(x, y - width, width, level - 1);
		DrawGasket(x, y - 2 * width, width, level - 1);
		DrawGasket(x - width, y - 2 * width, width, level - 1);
		DrawGasket(x - 2 * width, y - 2 * width, width, level - 1);
		DrawGasket(x - 2 * width, y - width, width, level - 1);
		DrawGasket(x - 2 * width, y, width, level - 1);
		DrawGasket(x - width, y, width, level - 1);	
	} else {
		DrawSquare(x, y, width);
	}
}

void Sierpinski::DrawSquare(GLfloat x, GLfloat y, GLfloat width) {
	GLfloat rnd_offset = (rand() % 100)*deformation_level / (50 * width);
	glBegin(GL_POLYGON);
	Colour::SetColour(colour);
	glVertex2f(x + rnd_offset, y + rnd_offset);
	Colour::SetColour(colour);
	glVertex2f(x + rnd_offset, y - width + rnd_offset);
	Colour::SetColour(colour);
	glVertex2f(x - width + rnd_offset, y - width + rnd_offset);
	Colour::SetColour(colour);
	glVertex2f(x - width + rnd_offset, y + rnd_offset);
	glEnd();
}

void Sierpinski::Render() {
	glClear(GL_COLOR_BUFFER_BIT);
	Colour::SetColour(ColourType::BLACK);
	DrawGasket(width/2, width/2, width, recursion_level);
	glFlush();
}

void Sierpinski::Run() {
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutCreateWindow("Sierpinski");
	glutDisplayFunc(Sierpinski::Render);
	glutReshapeFunc(Sierpinski::OnResize);
	glutMainLoop();
}

void Sierpinski::SetParams(GLfloat deformation, GLfloat width, GLint levels, ColourType colour) {
	Sierpinski::deformation_level = deformation;
	Sierpinski::width = width;
	Sierpinski::recursion_level = levels;
	Sierpinski::colour = colour;
}
