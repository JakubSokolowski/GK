#include "InteractiveEgg.h"
#define PI 3.14159265358979323846


ModelType model = ModelType::Point;
ModelPoint matrix[N][N];

static GLfloat angle[] = { 0.0, 0.0, 0.0 };
static GLfloat viewer[] = { 0.0, 0.0, 0.0 };
static GLfloat theta = 0.0;    // kat obrotu obiektu
static GLfloat theta2 = 0.0;   // kat obrotu obiektu
static GLfloat theta_arr[] = { 0.0, 0.0, 0.0 };

static GLfloat pix2angle;      // przelicznik pikseli na stopnie
static GLfloat pix2angle_x;    // przelicznik pikseli na stopnie
static GLfloat pix2angle_y;    // przelicznik pikseli na stopnie

static GLint status = 0;      // stan klawiszy myszy
							  

static int x_pos_old = 0;
static int y_pos_old = 0;
static int z_pos_old = 0;

// difference between mouse positions

static int delta_x = 0;
static int delta_y = 0;
static int delta_z = 0;


double R = 13.0f;
float fi = 0.0;

void InteractiveEgg::Run() {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(300, 300);
	glutCreateWindow("Egg");
	PrepareMatrix();
	glutDisplayFunc(Render);
	glutReshapeFunc(OnResize);
	glutMouseFunc(OnMouseChange);
	glutMotionFunc(Motion);
	glutKeyboardFunc(OnKeyPressed);
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
}

void InteractiveEgg::Render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	viewer[0] = R * cos(theta) * cos(fi);
	viewer[1] = R * sin(fi);
	viewer[2] = R * sin(theta) * cos(fi);

	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, cos(fi), 0.0);

	glRotatef(theta, 0.0, 1.0, 0.0);  //obrót obiektu o nowy k?t
	glRotatef(theta2, 1.0, 0.0, 0.0);

	glRotatef(angle[0], 1.0, 0.0, 0.0);

	glRotatef(angle[1], 0.0, 1.0, 0.0);

	glRotatef(angle[2], 0.0, 0.0, 1.0);

	glRotated(45.0, 1.0, 0.0, 0.0);

	DrawAxes();
	GenerateEggModel();

	glFlush();
	glutSwapBuffers();
}

void InteractiveEgg::OnResize(GLsizei horizontal, GLsizei vertical) {
	//pix2angle = 360.0 / (float)horizontal; // przeliczenie pikseli na stopnie
	pix2angle_x = 360.0*0.03 / (float)horizontal; // przeliczenie pikseli na stopnie
	pix2angle_y = 360.0*0.03 / (float)vertical;
	GLfloat AspectRatio;
	glMatrixMode(GL_PROJECTION);
	// Prze³¹czenie macierzy bie¿¹cej na macierz projekcji

	glLoadIdentity();
	// Czyszcznie macierzy bie¿¹cej 

	gluPerspective(70, 1.0, 1.0, 100.0);
	// Ustawienie parametrów dla rzutu perspektywicznego


	if (horizontal <= vertical)
		glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);

	else
		glViewport((horizontal - vertical) / 2, 0, vertical, vertical);
	// Ustawienie wielkoœci okna okna widoku (viewport) w zale¿noœci
	// relacji pomiêdzy wysokoœci¹ i szerokoœci¹ okna

	glMatrixMode(GL_MODELVIEW);
	// Prze³¹czenie macierzy bie¿¹cej na macierz widoku modelu  

	glLoadIdentity();
}

void InteractiveEgg::OnMouseChange(int btn, int state, int x, int y) {
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old = x;
		y_pos_old = y;
		status = 1;
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		z_pos_old = y;
		status = 2;
	}
	else

		status = 0;
}

void InteractiveEgg::OnKeyPressed(unsigned char key, GLint x, GLint y) {
	if (key == '1') model = ModelType::Point;
	if (key == '2') model = ModelType::Mesh;
	if (key == '3') model = ModelType::Fill;

	Render();
}

void InteractiveEgg::Motion(GLsizei x, GLsizei y) {
	delta_x = x - x_pos_old;//Obliczenie ró¿nocy po³o¿enia kursora myszy
	delta_y = y - y_pos_old;//Obliczenie ró¿nocy po³o¿enia kursora myszy

	if (status == 1)		//Jeœli lewy klawisz wciœniêty
	{
		theta += delta_x * pix2angle_x;		//modyfikacja k¹ta obrotu 
		fi += delta_y * pix2angle_y;		//do róznicy po³ozenia kursora myszy
		if (theta >= 360.0)					//Jeœli k¹t >= 360 stopni
			theta = 0.0;					// to k¹t = 0
		if (fi >= 360.0)
			fi = 0.0;
	}
	else if (status == 2) {	//Jeœli lewy klawisz wciœniêty
		R += 0.05* delta_y;	//Przyblizanie siê obserwatora do obiektu
		if (R <= 5.0)		//ustalone maksymalne przyblizenia i oddalenia
			R = 5.0;		//aby nie wejœæ w œrodek jajka
		if (R >= 25.0)
			R = 25.0;
	}
	x_pos_old = x;          //Ustawienie aktualnego po³o¿enia jako poprzednie
	y_pos_old = y;
	z_pos_old = y;
	glutPostRedisplay();    // przerysowanie obrazu sceny
}


void InteractiveEgg::DrawAxes() {
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

GLfloat InteractiveEgg::X(GLint i, GLint j) {
	float u = i * 1.0f / (N - 1);
	float v = j * 1.0f / (N - 1);
	return ((-90 * pow(u, 5)) + (225 * pow(u, 4)) - (270 * pow(u, 3)) + (180 * pow(u, 2)) - 45 * u)*cos(PI * v);
}
GLfloat InteractiveEgg::Y(GLint i, GLint j) {
	float u = i * 1.0f / (N - 1);
	float v = j * 1.0f / (N - 1);
	return (160 * pow(u, 4)) - (320 * pow(u, 3)) + (160 * pow(u, 2)) - 5;
}
GLfloat InteractiveEgg::Z(GLint i, GLint j) {
	float u = i * 1.0f / (N - 1);
	float v = j * 1.0f / (N - 1);
	return ((-90 * pow(u, 5)) + (225 * pow(u, 4)) - (270 * pow(u, 3)) + (180 * pow(u, 2)) - 45 * u)*sin(PI * v);
}

void InteractiveEgg::PrepareMatrix() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			matrix[i][j].coords.x = X(i, j);
			matrix[i][j].coords.y = Y(i, j);
			matrix[i][j].coords.z = Z(i, j);
			matrix[i][j].colour = Colour::Colour::GetColour(ColourType::RANDOM);
		}
	}
}


void InteractiveEgg::GenerateEggModel() {
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

void InteractiveEgg::GeneratePointModel() {
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

void InteractiveEgg::GenerateMeshModel() {
	glColor3f(0.0f, 1.0f, 0.0f);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			DrawVerticalLine(i, j);
			DrawDiagonalLine(i, j);
			DrawHorizontalLine(i, j);
		}
	}
}

void InteractiveEgg::DrawVerticalLine(int i, int j) {
	glBegin(GL_LINES);
	auto from = matrix[i][j].coords;
	auto to = matrix[(i + 1) % N][j].coords;
	glVertex3f(from.x, from.y, from.z);
	glVertex3f(to.x, to.y, to.z);
	glEnd();
}

void InteractiveEgg::DrawHorizontalLine(int i, int j) {
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

void InteractiveEgg::DrawDiagonalLine(int i, int j) {
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



void InteractiveEgg::GenerateFullModel() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if ((j + 1) != N) {
				DrawTriangles(i, j);
			}
		}
	}
}

void InteractiveEgg::DrawTriangles(int i, int j) {

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