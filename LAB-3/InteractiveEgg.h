#pragma once
#include "../LAB-1/Animation.h"

const int N = 20;

enum ModelType {
	Point,
	Mesh,
	Fill
};



struct ModelPoint {
	Point3D coords;
	Colour colour;
};


class InteractiveEgg {
public:
	static void Run();
	static void Render();

private:
	static void Motion(GLsizei x, GLsizei y);
	static void OnResize(GLsizei horizontal, GLsizei vertical);
	static void OnMouseChange(int btn, int state, int x, int y);
	static void OnKeyPressed(unsigned char key, GLint x, GLint y);
	static void DrawAxes();

	static GLfloat X(GLint i, GLint j);
	static GLfloat Y(GLint i, GLint j);
	static GLfloat Z(GLint i, GLint j);
	static void PrepareMatrix();
	static void GenerateEggModel();
	static void GeneratePointModel();

	static void GenerateMeshModel();
	static void DrawVerticalLine(int i, int j);
	static void DrawHorizontalLine(int i, int j);
	static void DrawDiagonalLine(int i, int j);

	static void GenerateFullModel();
	static void DrawTriangles(int i, int j);
};

