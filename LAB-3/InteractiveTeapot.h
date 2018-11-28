#pragma once
#include "../LAB-1/Animation.h"


class InteractiveTeapot : public Animation {
public:
	static void Run();
protected:
	static void DrawAxes();
	static void OnResize(GLsizei horizontal, GLsizei vertical);
	static void OnMouseChange(int btn, int state, int x, int y);
	static void Motion(GLsizei x, GLsizei);
	static void Render();

	static void Init();
};

