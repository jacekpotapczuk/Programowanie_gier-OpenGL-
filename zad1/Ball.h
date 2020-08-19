#pragma once
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <string>

class Ball
{
public:
	Ball();
	Ball(std::string name, float x, float y, float r, float m, float xv, float yv);
	std::string name;
	float x, y, r, m, xv, yv;
	float red, green, blue;
	void update(float min_x, float max_x, float min_y, float max_y);
	void draw();
	static void resolve_potential_collision(Ball* b1, Ball* b2);
	static bool is_colliding(Ball b1, Ball b2);
	
};

