#include "PlasmaFractal.h"
#include <cmath>

GLfloat PlasmaFractal::width = 600.0f;
FractalParams PlasmaFractal::params = { 600.0f , ColourType::RANDOM_GRAYSCALE};
std::vector<FractalPoint> PlasmaFractal::points = std::vector<FractalPoint>(600 * 600);

void PlasmaFractal::ComputeFractal(const ParentPoints & parent) {
	if (GetDistance(parent.ru, parent.ld) > 1.0f) {
		auto child = GenerateChildBlock(parent);
		ComputeFractal({ parent.ru, child.u, child.c, child.r });
		ComputeFractal({ child.u, parent.lu, child.l, child.c });
		ComputeFractal({ child.c, child.l, parent.ld, child.d });
		ComputeFractal({ child.r, child.c, child.d, parent.rd });
	}
	else {
		AddPoint(parent);
	}
}



ParentPoints PlasmaFractal::GetRandomSeed() {
	ParentPoints seed;
	seed.ru.x = width;
	seed.ru.y = width;
	seed.ru.colour = Colour::Colour::GetColour(params.seed_colour);

	seed.lu.x = 0.0f;
	seed.lu.y = width;
	seed.lu.colour = Colour::GetColour(params.seed_colour);

	seed.ld.x = 0.0f;
	seed.ld.y = 0.0f;
	seed.ld.colour = Colour::GetColour(params.seed_colour);

	seed.rd.x = width;
	seed.rd.y = 0.0f;
	seed.rd.colour = Colour::GetColour(params.seed_colour);
	return seed;
}



ChildPoints PlasmaFractal::GenerateChildBlock(const ParentPoints & parent) {
	ChildPoints child;
	FindChildColour(parent, child);
	FindChildPosition(parent, child);
	return child;
}

void PlasmaFractal::FindChildColour(const ParentPoints & parent, ChildPoints & child) {
	auto modifier = GetRandomModifier(GetDistance(parent.ru, parent.lu));
	child.r.colour = (parent.ru.colour + parent.rd.colour) / 2.0f;
	child.l.colour = (parent.lu.colour + parent.ld.colour) / 2.0f;
	child.u.colour = (parent.lu.colour + parent.ru.colour) / 2.0f;
	child.d.colour = (parent.ld.colour + parent.rd.colour) / 2.0f;
	child.c.colour = (parent.ru.colour + parent.rd.colour
					+ parent.lu.colour + parent.ld.colour) / 4.0f + modifier;
}

void PlasmaFractal::FindChildPosition(const ParentPoints & parent, ChildPoints & child) {
	child.r.x = (parent.ru.x + parent.rd.x) / 2.0f;
	child.r.y = (parent.ru.y + parent.rd.y) / 2.0f;

	child.l.x = (parent.lu.x + parent.ld.x) / 2.0f;
	child.l.y = child.r.y;

	child.u.x = (parent.lu.x + parent.ru.x) / 2.0f;
	child.u.y = (parent.lu.y + parent.ru.y) / 2.0f;

	child.d.x = child.u.x;
	child.d.y = (parent.ld.y + parent.rd.y) / 2.0f;

	child.c.x = child.u.x;
	child.c.y = child.r.y;
}

GLfloat PlasmaFractal::GetDistance(const FractalPoint & first, const FractalPoint & second) {
	return sqrt(pow(first.x - second.x, 2) + pow(first.y - second.y, 2));
}

void PlasmaFractal::AddPoint(const ParentPoints & parent) {
	FractalPoint point;
	point.x = (parent.lu.x + parent.ru.x) / 2.0f;
	point.y = (parent.ru.y + parent.rd.y) / 2.0f;
	point.colour = (parent.ru.colour + parent.lu.colour + parent.ld.colour + parent.rd.colour) / 4.0f;
	points.push_back(point);
}

void PlasmaFractal::Draw() {;
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(2, GL_FLOAT, sizeof(FractalPoint), &points[0].x);
	glColorPointer(4, GL_FLOAT, sizeof(FractalPoint), &points[0].colour.r);
	glPointSize(3.0);
	glDrawArrays(GL_POINTS, 0, points.size());
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glFlush();
	glutSwapBuffers();
}




// Generates randm rgb modifier based on noise levels
GLfloat PlasmaFractal::GetRandomModifier(GLfloat modifier) {
	auto mul = rand() % 101 > 50 ? 1 : -1;
	return modifier / width * params.noise_level * mul;
}



void PlasmaFractal::Render() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, width);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	Draw();
}

void PlasmaFractal::Run() {
	srand(time(time_t{}));
	glutInitWindowSize(width, width);
	glutInitWindowPosition(0, 0);
	auto seed = GetRandomSeed();

	ComputeFractal(seed);

	glutCreateWindow("PlasmaFractal");
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutDisplayFunc(Render);
	glutReshapeFunc(OnResize);
	Init();
	glutMainLoop();
}

void PlasmaFractal::Init() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}




void PlasmaFractal::SetParams(FractalParams params) {
	PlasmaFractal::params = params;
	PlasmaFractal::width = params.width;
}
