#pragma once
#include "Animation.h"
#include <vector>
#include <time.h>
#include <math.h>
#include <algorithm>


struct FractalPoint {
	Colour colour;
	GLfloat x, y, z;
};

struct ParentPoints {
	FractalPoint ru, lu, ld, rd;
};
struct ChildPoints {
	FractalPoint l, u, d, r, c;
};

enum ColouringMode {
	FULL,
	GRAYSCALE
};

struct FractalParams {
	GLfloat width;
	ColourType seed_colour; // The starting value in square corners
	GLfloat noise_level;
};

using ColourMatrix = std::vector<std::vector<Colour>>;

class PlasmaFractal 
	: public Animation 
{
public:	
	static void Run();
	static void SetParams(FractalParams params);

protected:

	static void Render();
	static void Init();

	static void ComputeFractal(const ParentPoints& parent);
	static void AddPoint(const ParentPoints& parent);

	static void Draw();

	static ParentPoints GetRandomSeed();
	static ChildPoints GenerateChildBlock(const ParentPoints& parent);
	static void FindChildColour(const ParentPoints& parent, ChildPoints& child);
	static void FindChildPosition(const ParentPoints& parent, ChildPoints& child);

	static GLfloat GetDistance(const FractalPoint& frist, const FractalPoint& second);
	static GLfloat GetRandomModifier(GLfloat height);

	static std::vector<FractalPoint> points;

	static FractalParams params;
	static GLfloat width;
};