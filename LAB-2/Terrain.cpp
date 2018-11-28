#include "Terrain.h"


GLfloat Terrain::width = 600.0f;
TerrainParams Terrain::params = { 600.0f , 4.0f, ColourType::RANDOM_GRAYSCALE };
std::vector<TerrainPoint> Terrain::points = std::vector<TerrainPoint>();

static GLfloat angle[] = { 0.0, 0.0, 0.0 };
static GLfloat viewer[] = { 0.0, 0.0, 0.0 };
static GLfloat theta = 0.0;    // kat obrotu obiektu
static GLfloat theta2 = 0.0;   // kat obrotu obiektu
static GLfloat pix2angle;      // przelicznik pikseli na stopnie
static GLfloat pix2angle_x;    // przelicznik pikseli na stopnie
static GLfloat pix2angle_y;    // przelicznik pikseli na stopnie

static GLint status = 0;      // stan klawiszy myszy
							  // 0 - nie naci?ni?to ?adnego klawisza
							  // 1 - naci?ni?ty zosta? lewy klawisz
							  // 2 - naciœniêty prawy klawisz

// last coursor position

static int x_pos_old = 0;      
static int y_pos_old = 0;
static int z_pos_old = 0;

// difference between mouse positions

static int delta_x = 0;      
static int delta_y = 0;
static int delta_z = 0;


double R = 13.0f;
float fi = 0.0;


void Terrain::Run() {
	srand(time(0));
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(300, 300);
	glutCreateWindow("Układ współrzędnych 3-D");
	ComputeFractal(GetRandomSeed());
	NormalizePoints();
	glutDisplayFunc(Render);
	glutReshapeFunc(OnResize);
	glutMouseFunc(OnMouseChange);
	glutKeyboardFunc(OnKeyPress);
	glutIdleFunc(Spin);
	glutMotionFunc(Motion);
	Init();
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
}

void Terrain::SetParams(TerrainParams params) {
	Terrain::params = params;
	Terrain::width = params.width;
}

void Terrain::DrawAxes() {
	double max = 5.0;
	Point3D  x_min = { -max, 0.0, 0.0 };
	Point3D  x_max = { max, 0.0, 0.0 };
	// początek i koniec obrazu osi x

	Point3D  y_min = { 0.0, -max, 0.0 };
	Point3D  y_max = { 0.0,  max, 0.0 };
	// początek i koniec obrazu osi y

	Point3D  z_min = { 0.0, 0.0, -max };
	Point3D  z_max = { 0.0, 0.0,  max };
	//  początek i koniec obrazu osi y
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

void Terrain::Spin() {
	angle[0] -= 0.1;
	if (angle[0] > 360.0) angle[0] -= 360.0;

	angle[1] -= 0.2;
	if (angle[1] > 360.0) angle[1] -= 360.0;

	angle[2] -= 0.0;
	if (angle[2] > 360.0) angle[2] -= 360.0;

	glutPostRedisplay();
}

void Terrain::OnResize(GLsizei horizontal, GLsizei vertical) {
	//pix2angle = 360.0 / (float)horizontal; // przeliczenie pikseli na stopnie
	pix2angle_x = 360.0*0.01 / (float)horizontal; // przeliczenie pikseli na stopnie
	pix2angle_y = 360.0*0.01 / (float)vertical;
	GLfloat AspectRatio;
	// Deklaracja zmiennej AspectRatio  okreslajacej proporcje
	// wymiarów okna 

	if (vertical == 0)  // Zabezpieczenie przed dzieleniem przez 0

		vertical = 1;

	glViewport(0, 0, horizontal, vertical);
	// Ustawienie wielkosciokna okna widoku (viewport)
	// W tym przypadku od (0,0) do (horizontal, vertical)  

	glMatrixMode(GL_PROJECTION);
	// Przelaczenie macierzy biezacej na macierz projekcji 

	glLoadIdentity();
	// Czyszcznie macierzy biezacej            

	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
	// Wyznaczenie wspólczynnika  proporcji okna
	// Gdy okno nie jest kwadratem wymagane jest okreslenie tak zwanej
	// przestrzeni ograniczajacej pozwalajacej zachowac wlasciwe
	// proporcje rysowanego obiektu.
	// Do okreslenia przestrzeni ograniczjacej sluzy funkcja
	// glOrtho(...)            

	//if (horizontal <= vertical)

	//	glOrtho(-7.5, 7.5, -7.5 / AspectRatio, 7.5 / AspectRatio, 10.0, -10.0);

	//else

	//	glOrtho(-7.5*AspectRatio, 7.5*AspectRatio, -7.5, 7.5, 10.0, -10.0);
	gluPerspective(70, 1.0, 1.0, 30.0);
	// Ustawienie parametrów dla rzutu perspektywicznego
	glMatrixMode(GL_MODELVIEW);
	// Przelaczenie macierzy biezacej na macierz widoku modelu                                    

	glLoadIdentity();
	// Czyszcenie macierzy biezacej
}

void Terrain::OnMouseChange(int btn, int state, int x, int y) {
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old = x;   
		y_pos_old = y;
		status = 1;         //
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		z_pos_old = y;
		status = 2;
	}

	else

		status = 0;     
}

void Terrain::Motion(GLsizei x, GLsizei y) {
	delta_x = x - x_pos_old;//Obliczenie różnocy położenia kursora myszy
	delta_y = y - y_pos_old;//Obliczenie różnocy położenia kursora myszy

	if (status == 1)		//Jeśli lewy klawisz wciśnięty
	{
		theta += delta_x * pix2angle_x;		//modyfikacja kąta obrotu 
		fi += delta_y * pix2angle_y;		//do róznicy połozenia kursora myszy
		if (theta >= 360.0)					//Jeśli kąt >= 360 stopni
			theta = 0.0;					// to kąt = 0
		if (fi >= 360.0)
			fi = 0.0;
	}
	else if (status == 2) {	//Jeśli lewy klawisz wciśnięty
		R += 0.03* delta_y;	//Przyblizanie się obserwatora do obiektu
		if (R <= 1.0)		//ustalone maksymalne przyblizenia i oddalenia
			R = 1.0;		//aby nie wejść w środek jajka
		if (R >= 40.0)
			R = 40.0;
	}
	x_pos_old = x;          //Ustawienie aktualnego położenia jako poprzednie
	y_pos_old = y;
	z_pos_old = y;
	glutPostRedisplay();    // przerysowanie obrazu sceny
}


void Terrain::Render() {
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

	Draw();

	glFlush();
	glutSwapBuffers();
}

void Terrain::OnKeyPress(unsigned char key, int x, int y) {
	if (key == 'r') {
		ComputeFractal(GetRandomSeed());
		NormalizePoints();
		Render();
	}

}

void Terrain::Init() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Terrain::NormalizePoint(TerrainPoint & point) {
	point.x = Normalize(point.x);
	point.y = Normalize(point.y);
	point.z = Normalize(point.z);
}

void Terrain::NormalizePoints() {
	for (auto& point : points)
		NormalizePoint(point);
}

float Terrain::Normalize(float val) {
	float max = width;
	float min = 0.0f;
	float new_max = 6.0;
	float new_min = -6.0;

	return(new_max - new_min) / (max - min) * (val - max) + new_max;
}

float Terrain::NormalizeColour(float shade) {
	float max = width;
	float min = 0.0f;
	float new_max = 1.0;
	float new_min = 0.0;

	return(new_max - new_min) / (max - min) * (shade - max) + new_max;
}

void Terrain::ComputeFractal(const ParentTerrain & parent) {
	if (GetDistance(parent.ru, parent.ld) > params.space) {
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

void Terrain::AddPoint(const ParentTerrain & parent) {
	TerrainPoint point;
	point.x = (parent.lu.x + parent.ru.x) / 2.0f;
	point.y = (parent.ru.y + parent.rd.y) / 2.0f;
	point.z = (parent.ru.z + parent.lu.z + parent.ld.z + parent.rd.z) / 4.0f;
	points.push_back(point);
}

void Terrain::Draw() {	
	Colour::SetColour({ 0.317f, 0.694f, 0.901f });
	glBegin(GL_POINTS);
	for (auto point : points) {
		glVertex3f(point.x, point.y, point.z);
	}	
	glEnd();
}

ParentTerrain Terrain::GetRandomSeed() {
	ParentTerrain seed;
	seed.ru.x = width;
	seed.ru.y = width;
	seed.ru.z = GetRandomInRange(0.0f, width);

	seed.lu.x = 0.0f;
	seed.lu.y = width;
	seed.lu.z = GetRandomInRange(0.0f, width);

	seed.ld.x = 0.0f;
	seed.ld.y = 0.0f;
	seed.ld.z = GetRandomInRange(0.0f, width);

	seed.rd.x = width;
	seed.rd.y = 0.0f;
	seed.rd.z = GetRandomInRange(0.0f, width);
	return seed;
}

ChildTerrain Terrain::GenerateChildBlock(const ParentTerrain & parent) {
	ChildTerrain child;
	FindChildColour(parent, child);
	FindChildPosition(parent, child);
	return child;
}

void Terrain::FindChildColour(const ParentTerrain & parent, ChildTerrain & child) {
	auto modifier = GetRandomModifier(GetDistance(parent.ru, parent.lu));
	child.r.z = (parent.ru.z + parent.rd.z) / 2.0f;
	child.l.z = (parent.lu.z + parent.ld.z) / 2.0f;
	child.u.z = (parent.lu.z + parent.ru.z) / 2.0f;
	child.d.z = (parent.ld.z + parent.rd.z) / 2.0f;
	child.c.z = (parent.ru.z + parent.rd.z
		+ parent.lu.z + parent.ld.z) / 4.0f + modifier;
}

void Terrain::FindChildPosition(const ParentTerrain & parent, ChildTerrain & child) {
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

GLfloat Terrain::GetDistance(const TerrainPoint & first, const TerrainPoint & second) {
	return sqrt(pow(first.x - second.x, 2) + pow(first.y - second.y, 2));
}

GLfloat Terrain::GetRandomModifier(GLfloat height) {
	auto mul = rand() % 101 > 50 ? 1 : -1;
	return height / width * params.noise_level * mul;
}
