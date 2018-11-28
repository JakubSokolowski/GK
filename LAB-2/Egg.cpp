#include "Egg.h"

#define PI 3.14159265358979323846


ModelType Egg::model = ModelType::Point;
ModelPoint matrix[N][N];

static GLfloat theta[] = { 0.0, 0.0, 0.0 }; 

void Egg::OnResize(GLsizei horizontal, GLsizei vertical) {

	GLfloat AspectRatio;
	// Deklaracja zmiennej AspectRatio  okreœlaj¹cej proporcjê
	// wymiarów okna 
	if (vertical == 0)  // Zabezpieczenie przed dzieleniem przez 0
		vertical = 1;
	glViewport(0, 0, horizontal, vertical);
	// Ustawienie wielkoœciokna okna widoku (viewport)
	// W tym przypadku od (0,0) do (horizontal, vertical)  
	glMatrixMode(GL_PROJECTION);
	// Prze³¹czenie macierzy bie¿¹cej na macierz projekcji 
	glLoadIdentity();
	// Czyszcznie macierzy bie¿¹cej            
	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
	// Wyznaczenie wspó³czynnika  proporcji okna
	// Gdy okno nie jest kwadratem wymagane jest okreœlenie tak zwanej
	// przestrzeni ograniczaj¹cej pozwalaj¹cej zachowaæ w³aœciwe
	// proporcje rysowanego obiektu.
	// Do okreslenia przestrzeni ograniczj¹cej s³u¿y funkcja
	// glOrtho(...)            
	if (horizontal <= vertical)

		glOrtho(-7.5, 7.5, -7.5 / AspectRatio, 7.5 / AspectRatio, 10.0, -10.0);
	else

		glOrtho(-7.5*AspectRatio, 7.5*AspectRatio, -7.5, 7.5, 10.0, -10.0);
	glMatrixMode(GL_MODELVIEW);
	// Prze³¹czenie macierzy bie¿¹cej na macierz widoku modelu                                   

	glLoadIdentity();
	// Czyszcenie macierzy bie¿¹cej
}

void Egg::Init() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Egg::Run() {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(300, 300);
	glutCreateWindow("Uk³ad wspó³rzêdnych 3-D");
	PrepareMatrix();
	glutDisplayFunc(Render);
	glutReshapeFunc(OnResize);
	glutIdleFunc(SpinEgg);
	glutKeyboardFunc(OnKeyPressed);
	Init();
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
}

void Egg::DrawAxes() {
	Point3D  x_min = { -5.0, 0.0, 0.0 };
	Point3D  x_max = { 5.0, 0.0, 0.0 };
	// pocz¹tek i koniec obrazu osi x

	Point3D  y_min = { 0.0, -5.0, 0.0 };
	Point3D  y_max = { 0.0,  5.0, 0.0 };
	// pocz¹tek i koniec obrazu osi y

	Point3D  z_min = { 0.0, 0.0, -5.0 };
	Point3D  z_max = { 0.0, 0.0,  5.0 };
	//  pocz¹tek i koniec obrazu osi y
	glColor3f(1.0f, 0.0f, 0.0f);  // kolor rysowania osi - czerwony
	glBegin(GL_LINES); // rysowanie osi x
	glVertex3f(x_min.x, x_min.y, x_min.z);
	glVertex3f(x_max.x, x_max.y, x_max.z);
	glEnd();

	glColor3f(0.0f, 1.0f, 0.0f);  // kolor rysowania - zielony
	glBegin(GL_LINES);  // rysowanie osi y
	glVertex3f(y_min.x, y_min.y, y_min.z);
	glVertex3f(y_max.x, y_max.y, y_max.z);
	glEnd();

	glColor3f(0.0f, 0.0f, 1.0f);  // kolor rysowania - niebieski
	glBegin(GL_LINES); // rysowanie osi z
	glVertex3f(z_min.x, z_min.y, z_min.z);
	glVertex3f(z_max.x, z_max.y, z_max.z);
	glEnd();
}

GLfloat Egg::X(GLint i, GLint j) {
	float u = i * 1.0f / (N - 1) ;
	float v = j * 1.0f / (N - 1);
	return ((-90 * pow(u, 5)) + (225 * pow(u, 4)) - (270 * pow(u, 3)) + (180 * pow(u, 2)) - 45 * u)*cos(PI * v);
}
GLfloat Egg::Y(GLint i, GLint j) {
	float u = i * 1.0f / (N - 1);
	float v = j * 1.0f / (N - 1);
	return (160 * pow(u, 4)) - (320 * pow(u, 3)) + (160 * pow(u, 2)) - 5;
}
GLfloat Egg::Z(GLint i, GLint j) {
	float u = i * 1.0f / (N - 1);
	float v = j * 1.0f / (N - 1);
	return ((-90 * pow(u, 5)) + (225 * pow(u, 4)) - (270 * pow(u, 3)) + (180 * pow(u, 2)) - 45 * u)*sin(PI * v);
}

void Egg::PrepareMatrix() {		
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			matrix[i][j].coords.x = X(i, j);
			matrix[i][j].coords.y = Y(i, j);
			matrix[i][j].coords.z = Z(i, j);
			matrix[i][j].colour = Colour::Colour::GetColour(ColourType::RANDOM);
		}
	}
}


void Egg::GenerateEggModel() {	
	switch (model) {
	case ModelType::Point:
		GeneratePointModel();
		break;
	case ModelType::Mesh:
		GenerateMeshModel();
		break;
	case ModelType::Fill:
		GenerateFullModel();
		break;
	default:
		break;
	}
}

void Egg::GeneratePointModel() {
	glBegin(GL_POINTS);
	auto colour = Colour::GetColour(ColourType::RANDOM);
	glColor3f(colour.r, colour.g, colour.b);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			auto coords = matrix[i][j].coords;
			glVertex3f(coords.x, coords.y, coords.z);
		}
	}
	glEnd();
}

void Egg::GenerateMeshModel() {
	glColor3f(0.0f, 1.0f, 0.0f);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			DrawVerticalLine(i, j);
			DrawDiagonalLine(i, j);
			DrawHorizontalLine(i, j);
		}
	}
}

void Egg::DrawVerticalLine(int i, int j) {
	glBegin(GL_LINES);
	auto from = matrix[i][j].coords;
	auto to = matrix[(i + 1) % N][j].coords;
	glVertex3f(from.x, from.y, from.z);	
	glVertex3f(to.x, to.y, to.z);
	glEnd();
}

void Egg::DrawHorizontalLine(int i, int j) {
	Point3D from, to;
	glBegin(GL_LINES);
	if ((j + 1) == N) {
		if (i > 0) {
			from = matrix[N - i][0].coords;
			to = matrix[N - i][0].coords;
			glVertex3f(from.x, from.y, from.z);
			glVertex3f(to.x, to.y, to.z);
		}
	}
	else {
		from = matrix[i][j + 1].coords;
		to = matrix[i][j].coords;
		glVertex3f(from.x, from.y, from.z);
		glVertex3f(to.x, to.y, to.z);
	}
	glEnd();
}

void Egg::DrawDiagonalLine(int i, int j) {
	Point3D from, to;
	glBegin(GL_LINES);
	if (((j + 1) != N) && ((i + 1) != N)) {
		from = matrix[i + 1][j].coords;
		to = matrix[i][j + 1].coords;
		glVertex3f(from.x, from.y, from.z);
		glVertex3f(to.x, to.y, to.z);
	}
	else {
		if (i > 0) {
			from = matrix[i][j].coords;
			to = matrix[N - i - 1][0].coords;
			glVertex3f(from.x, from.y, from.z);
			glVertex3f(to.x, to.y, to.z);
		}
	}
	glEnd();
}



void Egg::GenerateFullModel() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {			
			if ((j + 1) != N) {
				DrawTriangles(i, j);
			}
		}
	}
}

void Egg::DrawTriangles(int i, int j) {
	
	ModelPoint point1, point2, point3, point4;

	point1 = matrix[i][j];
	point2 = matrix[(i + 1) % N][j];
	point3 = matrix[i][j + 1];
	point4 = matrix[(i + 1) % N][j + 1];

	glBegin(GL_TRIANGLES);
	Colour::SetColour(point1.colour);
	glVertex3f(point1.coords.x, point1.coords.y, point1.coords.z);
	Colour::SetColour(point2.colour);
	glVertex3f(point2.coords.x, point2.coords.y, point2.coords.z);
	Colour::SetColour(point3.colour);
	glVertex3f(point3.coords.x, point3.coords.y, point3.coords.z);
	glEnd();

	glBegin(GL_TRIANGLES);
	Colour::SetColour(point2.colour);
	glVertex3f(point2.coords.x, point2.coords.y, point2.coords.z);
	Colour::SetColour(point4.colour);
	glVertex3f(point4.coords.x, point4.coords.y, point4.coords.z);
	Colour::SetColour(point3.colour);
	glVertex3f(point3.coords.x, point3.coords.y, point3.coords.z);
	glEnd();
}
void Egg::SpinEgg() {
	theta[0] -= 0.5;
	if (theta[0] > 360.0) theta[0] -= 360.0;

	theta[1] -= 0.5;
	if (theta[1] > 360.0) theta[1] -= 360.0;

	theta[2] -= 0.5;
	if (theta[2] > 360.0) theta[2] -= 360.0;

	glutPostRedisplay();
}

void Egg::Render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	DrawAxes();
	glRotatef(theta[0], 1.0, 0.0, 0.0);
	glRotatef(theta[1], 0.0, 1.0, 0.0);
	glRotatef(theta[2], 0.0, 0.0, 1.0);
	GenerateEggModel();
	glFlush();
	glutSwapBuffers();
}

void Egg::OnKeyPressed(unsigned char key, GLint x, GLint y) {
	if (key == '1') model = ModelType::Point;
	if (key == '2') model = ModelType::Mesh;
	if (key == '3') model = ModelType::Fill;

	Render(); // przerysowanie obrazu sceny
}
