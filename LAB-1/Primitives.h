#pragma once
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <random>
using Point2D = float[2];

struct Point3D {
	GLfloat x, y, z;
};

enum ColourType {
	WHITE,
	BLACK,
	RED,
	GREEN,
	RANDOM,
	RANDOM_GRAYSCALE,
};


struct Colour {
	GLfloat r = 0.0;
	GLfloat g = 0.0;
	GLfloat b = 0.0;

	Colour operator + (const GLfloat& other) const {
		return { r + other, g + other, b + other };
	}
	Colour operator / (GLfloat divider) const {
		return { r / divider, g / divider, b / divider };
	}
	Colour operator * (const Colour& other) const {
		return { r *other.r, g * other.g, b *other.b };
	}
	Colour operator * (const GLfloat& mul) const {
		return { r *mul, g*mul, b*mul };
	}
	bool operator == (const Colour& other) const {
		return (r == other.r && g == other.g && b == other.b);
	}
	friend Colour operator + (const Colour& lhs, const Colour& rhs) {
		return { lhs.r + rhs.r, lhs.g + rhs.g, lhs.b + rhs.b };
	}

	static Colour GetColour(ColourType type) {
		Colour colour{ 1.0f, 1.0f, 1.0f };
		switch (type) {
		case ColourType::WHITE:
			break;
		case ColourType::BLACK:
			colour = { 0.0f, 0.0f, 0.0f };
			break;
		case ColourType::RED:
			colour = { 1.0f, 0.0f, 0.0f };
			break;
		case ColourType::GREEN:
			colour = { 0.0f, 1.0f, 0.0f };
			break;
		case ColourType::RANDOM:
			colour = {
				static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
				static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
				static_cast <float> (rand()) / static_cast <float> (RAND_MAX)
			};
			break;
		case ColourType::RANDOM_GRAYSCALE:
			auto shade = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			colour = {
				shade,
				shade,
				shade
			};
		}
		return colour;

	}
	static void SetColour(Colour colour) {
		glColor3f(colour.r, colour.g, colour.b);
	}
	static void SetColour(ColourType type) {
		auto colour = GetColour(type);
		glColor3f(colour.r, colour.g, colour.b);
	}
	static void SetColour(float shade) {
		glColor3f(shade, shade, shade);
	}
};