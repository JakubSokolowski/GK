#include "InteractiveTeapot.h"
#include "InteractiveEgg.h"

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	InteractiveTeapot::Run();
	//InteractiveEgg::Run();
}