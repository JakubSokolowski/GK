#include "InteractiveTeapot.h"

typedef float point3[3];

static GLfloat viewer[] = { 0.0, 0.0, 10.0 };
// inicjalizacja po�o�enia obserwatora

/*************************************************************************************/

// Funkcja rysuj�ca osie uk�adu wsp�?rz?dnych

static GLfloat theta_x = 0.0, theta_y = 0.0;   // k�t obrotu obiektu

static GLfloat pix2angle;     // przelicznik pikseli na stopnie

static GLint status_left = 0;       // stan klawiszy myszy
									// 0 - nie naci�ni�to �adnego klawisza
									// 1 - naci�ni�ty zosta� lewy klawisz

static int x_pos_old = 0, y_pos_old = 0;       // poprzednia pozycja kursora myszy

static int delta_x = 0, delta_y = 0;        // r�nica pomi�dzy pozycj� bie��c�
											// i poprzedni� kursora myszy
static int y_observ_old = 0, y_obs_delta = 0;
static GLint status_right = 0;

static int zFar = 30;

void InteractiveTeapot::DrawAxes() {
	double max = 5.0;
	Point3D  x_min = { -max, 0.0, 0.0 };
	Point3D  x_max = { max, 0.0, 0.0 };
	// pocz�tek i koniec obrazu osi x

	Point3D  y_min = { 0.0, -max, 0.0 };
	Point3D  y_max = { 0.0,  max, 0.0 };
	// pocz�tek i koniec obrazu osi y

	Point3D  z_min = { 0.0, 0.0, -max };
	Point3D  z_max = { 0.0, 0.0,  max };
	//  pocz�tek i koniec obrazu osi y
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

void InteractiveTeapot::OnResize(GLsizei horizontal, GLsizei vertical) {

	glMatrixMode(GL_PROJECTION);
	// Prze��czenie macierzy bie��cej na macierz projekcji

	glLoadIdentity();
	// Czyszcznie macierzy bie��cej

	//gluPerspective(70, 1.0, 0.1, 30.0 + y_obs_delta);
	// Ustawienie parametr�w dla rzutu perspektywicznego


	if (horizontal <= vertical)
		glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);

	else
		glViewport((horizontal - vertical) / 2, 0, vertical, vertical);
	// Ustawienie wielko�ci okna okna widoku (viewport) w zale�no�ci
	// relacji pomi�dzy wysoko�ci� i szeroko�ci� okna

	glMatrixMode(GL_MODELVIEW);
	// Prze��czenie macierzy bie��cej na macierz widoku modelu  

	glLoadIdentity();
	// Czyszczenie macierzy bie��cej

	pix2angle = 360.0 / (float)horizontal;  // przeliczenie pikseli na stopnie
}

void InteractiveTeapot::OnMouseChange(int btn, int state, int x, int y) {
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old = x;         // przypisanie aktualnie odczytanej pozycji kursora
		y_pos_old = y;
		// jako pozycji poprzedniej
		status_left = 1;          // wci�ni�ty zosta� lewy klawisz myszy
	}
	else
		status_left = 0;

	if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		y_observ_old = y;
		status_right = 1;
	}
	else
		status_right = 0;
}

void InteractiveTeapot::Motion(GLsizei x, GLsizei y) {
	delta_x = x - x_pos_old;     // obliczenie r�nicy po�o�enia kursora myszy
	delta_y = y - y_pos_old;
	y_obs_delta = y - y_observ_old;

	x_pos_old = x;            // podstawienie bie��cego po�o�enia jako poprzednie
	y_pos_old = y;
	y_observ_old = y;

	glutPostRedisplay();     // przerysowanie obrazu sceny
}

void InteractiveTeapot::Render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz�cym

	glLoadIdentity();
	// Czyszczenie macierzy bie??cej

	if (status_right == 1) {
		viewer[2] += y_obs_delta / 8.0f;
		zFar += y_observ_old / 8.0f;
	}

	gluPerspective(70, 1.0, 1.0, zFar);


	if (viewer[2] < 0)
		viewer[2] = 0;

	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	// Zdefiniowanie po�o�enia obserwatora
	DrawAxes();
	// Narysowanie osi przy pomocy funkcji zdefiniowanej powy�ej

	glColor3f(1.0f, 1.0f, 1.0f);
	// Ustawienie koloru rysowania na bia�y


	if (status_left == 1)                     // je�li lewy klawisz myszy wci�ni�ty
	{
		theta_x += delta_x * pix2angle;    // modyfikacja k�ta obrotu o kat proporcjonalny
		theta_y += delta_y * pix2angle;
	}                                  // do r�nicy po�o�e� kursora myszy

	glRotatef(theta_x, 0.0, 1.0, 0.0);  //obr�t obiektu o nowy k�t
	glRotatef(theta_y, 1.0, 0.0, 0.0);


	glutWireTeapot(3.0);
	// Narysowanie czajnika
	glFlush();
	// Przekazanie polece� rysuj�cych do wykonania
	glutSwapBuffers();
}

void InteractiveTeapot::Run() {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(800, 800);

	glutCreateWindow("Rzutowanie perspektywiczne");
	Init();
	glutDisplayFunc(Render);


	glutReshapeFunc(OnResize);
	glutMouseFunc(OnMouseChange);
	glutMotionFunc(Motion);


	glEnable(GL_DEPTH_TEST);

	glutMainLoop();

}

void InteractiveTeapot::Init() {
	Animation::Init();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}
