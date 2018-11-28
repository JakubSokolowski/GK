#pragma once
#include "../LAB-1/Animation.h"
#include <time.h>


struct TerrainPoint {
	GLfloat x, y, z;
};

struct ParentTerrain {
	TerrainPoint ru, lu, ld, rd;
};
struct ChildTerrain {
	TerrainPoint l, u, d, r, c;
};


struct TerrainParams {
	GLfloat width;
	GLfloat space;
	ColourType seed_colour; // The starting value in square corners
	GLfloat noise_level;
};

class Terrain: public Animation {
public:

	static void Run();
	static void SetParams(TerrainParams params);


protected:

	// Glut callback functions

	static void Render();
	static void OnKeyPress(unsigned char key, int x, int y);
	static void Init();
	static void Spin();
	static void OnResize(GLsizei horizontal, GLsizei vertical);
	static void OnMouseChange(int btn, int state, int x, int y);
	static void Motion(GLsizei x, GLsizei y);

	static void DrawAxes();
	static void Draw();
	static void ComputeFractal(const ParentTerrain& parent);
	static void AddPoint(const ParentTerrain& parent);

	static void NormalizePoint(TerrainPoint& point);
	static void NormalizePoints();
	static float Normalize(float val);
	static float NormalizeColour(float shade);





	static ParentTerrain GetRandomSeed();
	static ChildTerrain GenerateChildBlock(const ParentTerrain& parent);
	static void FindChildColour(const ParentTerrain& parent, ChildTerrain& child);
	static void FindChildPosition(const ParentTerrain& parent, ChildTerrain& child);

	static GLfloat GetDistance(const TerrainPoint& frist, const TerrainPoint& second);
	static GLfloat GetRandomModifier(GLfloat height);

	static std::vector<TerrainPoint> points;

	static TerrainParams params;
	static GLfloat width;
};

