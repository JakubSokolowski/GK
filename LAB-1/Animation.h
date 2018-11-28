#pragma once
#include "Primitives.h"

class Animation {
	
public:
	static void Run();
protected:
	static void Render();
	static void OnResize(GLsizei horizontal, GLsizei vertical);
	static void Init();
	static float GetRandomInRange(float from, float to);
};

inline void Animation::Run() {
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutCreateWindow("Animation");
	glutDisplayFunc(Render);
	glutReshapeFunc(OnResize);
	Init();
	glutMainLoop();
}

inline void Animation::Render() {
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

inline void Animation::OnResize(GLsizei horizontal, GLsizei vertical) {
	GLfloat aspect_ratio;
	if (!vertical)
		vertical = 1;

	// Set window size, observer coordinate system
	glViewport(0, 0, horizontal, vertical);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	aspect_ratio = (GLfloat)horizontal / (GLfloat)vertical;
	double size = 100.0;

	if (horizontal <= vertical)
		glOrtho(-size, size, -size / aspect_ratio, size / aspect_ratio, 1.0, -1.0);
	else
		glOrtho(-size * aspect_ratio, size*aspect_ratio, -size, size, 1.0, -1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

inline void Animation::Init() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

inline float Animation::GetRandomInRange(float from, float to) {
	return 0.0f;
}
