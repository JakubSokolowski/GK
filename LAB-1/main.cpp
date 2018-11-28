#include "Animation.h"
#include "Sierpinski.h"
#include "PlasmaFractal.h"


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	//Sierpinski::SetParams(2.0f, 300.0f, 4, ColourType::RANDOM);
	//Sierpinski::Run();
	PlasmaFractal::SetParams(FractalParams{300.0f, ColourType::RANDOM, 0.5f});
	PlasmaFractal::Run();
}