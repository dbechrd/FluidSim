#pragma once
#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#define MAX_PARTICLES 250000
#define MAX_SPRINGS 8
#define GRID_SIZE 8

#include <vector>
#include "Particle.h"

class ParticleEmitter {
public:
	ParticleEmitter();
	~ParticleEmitter();
	void Update(GLfloat t);

	const GLfloat* VertexArray(){ return particlePos; }
	const GLfloat* ColorArray(){ return particleColor; }
private:
	GLfloat particlePos[MAX_PARTICLES*3];
	GLfloat particleVel[MAX_PARTICLES*3];
	GLfloat particleAcc[MAX_PARTICLES*3];
	GLfloat particleColor[MAX_PARTICLES*4];
	GLfloat particleUnit[MAX_PARTICLES];
	u_int particleLife[MAX_PARTICLES];

	void Reset(u_int i);
	void ResolveCollisions();
};

#endif //PARTICLE_EMITTER_H