#include "Ball.h"
#include <math.h>  
#include <iostream>

Ball::Ball()
{
}

Ball::Ball(std::string name, float x, float y, float r, float m, float xv, float yv)
{
	this->name = name;
	this->x = x;
	this->y = y;
	this->r = r;
	this->m = m;
	this->xv = xv;
	this->yv = yv;


	// assume mass between 1 and 100
	// bigger mass = darker color
	if (m < 1.0 || m > 10.0)
		std::cout << "Given mass is outside of 1 - 10 range." << std::endl;

	this->red = 1.0 - m / 10.0;
	this->green = 1.0 - m / 10.0;
	this->blue =1.0 - m / 10.0;

}

void Ball::update(float min_x, float max_x, float min_y, float max_y)
{
	// update ball position based on velocity 
	x += xv;
	y += yv;

	if (x - r <= min_x) {
		xv = -xv;
		x = min_x + r;
	}
	else if (x + r >= max_x) {
		xv = -xv;
		x = max_x - r;
	}
		
	if (y - r <= min_y ) {
		yv = -yv;
		y = min_y + r;
	}
	else if (y + r >= max_y) {
		yv = -yv;
		y = max_y - r;
	}
		
}

void Ball::resolve_potential_collision(Ball* b1, Ball* b2)
{

	if(Ball::is_colliding(*b1, *b2)){
		// resolve collision
		float collision_point_x = ((b1->x * b2->r) + (b2->x * b1->r)) / (b1->r + b2->r);
		float collision_point_y = ((b1->y * b2->r) + (b2->y * b1->r)) / (b1->r + b2->r);

		// elastic collision
		float b1xv = (b1->xv * (b1->m - b2->m) + (2 * b2->m * b2->xv)) / (b1->m + b2->m);
		float b1yv = (b1->yv * (b1->m - b2->m) + (2 * b2->m * b2->yv)) / (b1->m + b2->m);
		float b2xv = (b2->xv * (b2->m - b1->m) + (2 * b1->m * b1->xv)) / (b1->m + b2->m);
		float b2yv = (b2->yv * (b2->m - b1->m) + (2 * b1->m * b1->yv)) / (b1->m + b2->m);

		b1->xv = b1xv;
		b1->yv = b1yv;
		b2->xv = b2xv;
		b2->yv = b2yv;
		
	}

}

bool Ball::is_colliding(Ball b1, Ball b2)
{
	// first check AABB colision
	if ((b1.x + b1.r + b2.r > b2.x) && (b1.x < b2.x + b1.r + b2.r) &&
		(b1.y + b1.r + b2.r > b2.y) && (b1.y < b2.y + b1.r + b2.r))
	{
		// check for actual collision
		float dist = sqrt((b1.x - b2.x) * (b1.x - b2.x) + (b1.y - b2.y) * (b1.y - b2.y));
		if (dist < b1.r + b2.r)
		{
			return true;
		}
	}
	return false;
}

void Ball::draw()
{
	int num_segments = 30; // number of triangles in a fan

	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y); //  center of the fan
	for (int i = 0; i <= num_segments; i++) {
		float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);
		float x_local = r * cosf(theta);
		float y_local = r * sinf(theta); 
		glVertex2f(x_local + x, y_local + y);
	}
	glEnd();
}




