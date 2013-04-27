#pragma once
#ifndef PARTICLE_H
#define PARTICLE_H

#include <SFML/OpenGL.hpp>

struct Particle {
	GLfloat x, y, z;
	GLfloat vx, vy, vz;
	GLfloat ax, ay, az;
	GLbyte r, g, b, a;
	u_int lifetime;
};

#endif //PARTICLE_H