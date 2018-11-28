#include "Egg.h"
#include "Terrain.h"

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	//Egg::Run();
	Terrain::SetParams(TerrainParams{ 100.0f, 2.0f, ColourType::RANDOM_GRAYSCALE, 40.f });
	Terrain::Run();
}