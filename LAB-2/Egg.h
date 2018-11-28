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

class Egg :
	public Animation {

public:
	static void Run();
	
protected:
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

	static void SpinEgg();
	static void Render();
	static void OnKeyPressed(unsigned char key, GLint x, GLint y);
	static void OnResize(GLsizei horizontal, GLsizei vertical);
	static void Init();

	static ModelType model;
};

