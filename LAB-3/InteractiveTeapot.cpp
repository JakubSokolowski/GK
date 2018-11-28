#include "InteractiveTeapot.h"

typedef float point3[3];

static GLfloat viewer[] = { 0.0, 0.0, 10.0 };
// inicjalizacja po³o¿enia obserwatora

/*************************************************************************************/

// Funkcja rysuj¹ca osie uk³adu wspó?rz?dnych

static GLfloat theta_x = 0.0, theta_y = 0.0;   // k¹t obrotu obiektu

static GLfloat pix2angle;     // przelicznik pikseli na stopnie

static GLint status_left = 0;       // stan klawiszy myszy
									// 0 - nie naciœniêto ¿adnego klawisza
									// 1 - naciœniêty zostaæ lewy klawisz

static int x_pos_old = 0, y_pos_old = 0;       // poprzednia pozycja kursora myszy

static int delta_x = 0, delta_y = 0;        // ró¿nica pomiêdzy pozycj¹ bie¿¹c¹
											// i poprzedni¹ kursora myszy
static int y_observ_old = 0, y_obs_delta = 0;
static GLint status_right = 0;

static int zFar = 30;

void InteractiveTeapot::DrawAxes() {
	double max = 5.0;
	Point3D  x_min = { -max, 0.0, 0.0 };
	Point3D  x_max = { max, 0.0, 0.0 };
	// pocz¹tek i koniec obrazu osi x

	Point3D  y_min = { 0.0, -max, 0.0 };
	Point3D  y_max = { 0.0,  max, 0.0 };
	// pocz¹tek i koniec obrazu osi y

	Point3D  z_min = { 0.0, 0.0, -max };
	Point3D  z_max = { 0.0, 0.0,  max };
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

void InteractiveTeapot::OnResize(GLsizei horizontal, GLsizei vertical) {

	glMatrixMode(GL_PROJECTION);
	// Prze³¹czenie macierzy bie¿¹cej na macierz projekcji

	glLoadIdentity();
	// Czyszcznie macierzy bie¿¹cej

	//gluPerspective(70, 1.0, 0.1, 30.0 + y_obs_delta);
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
	// Czyszczenie macierzy bie¿¹cej

	pix2angle = 360.0 / (float)horizontal;  // przeliczenie pikseli na stopnie
}

void InteractiveTeapot::OnMouseChange(int btn, int state, int x, int y) {
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old = x;         // przypisanie aktualnie odczytanej pozycji kursora
		y_pos_old = y;
		// jako pozycji poprzedniej
		status_left = 1;          // wciêniêty zosta³ lewy klawisz myszy
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
	delta_x = x - x_pos_old;     // obliczenie ró¿nicy po³o¿enia kursora myszy
	delta_y = y - y_pos_old;
	y_obs_delta = y - y_observ_old;

	x_pos_old = x;            // podstawienie bie¿¹cego po³o¿enia jako poprzednie
	y_pos_old = y;
	y_observ_old = y;

	glutPostRedisplay();     // przerysowanie obrazu sceny
}

void InteractiveTeapot::Render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz¹cym

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

	// Zdefiniowanie po³o¿enia obserwatora
	DrawAxes();
	// Narysowanie osi przy pomocy funkcji zdefiniowanej powy¿ej

	glColor3f(1.0f, 1.0f, 1.0f);
	// Ustawienie koloru rysowania na bia³y


	if (status_left == 1)                     // jeœli lewy klawisz myszy wciêniêty
	{
		theta_x += delta_x * pix2angle;    // modyfikacja k¹ta obrotu o kat proporcjonalny
		theta_y += delta_y * pix2angle;
	}                                  // do ró¿nicy po³o¿eñ kursora myszy

	glRotatef(theta_x, 0.0, 1.0, 0.0);  //obrót obiektu o nowy k¹t
	glRotatef(theta_y, 1.0, 0.0, 0.0);


	glutWireTeapot(3.0);
	// Narysowanie czajnika
	glFlush();
	// Przekazanie poleceñ rysuj¹cych do wykonania
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
