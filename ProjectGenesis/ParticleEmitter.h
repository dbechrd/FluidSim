#pragma once
#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#define MAX_PARTICLES 600
//#define MAX_SPRINGS 8
#define GRID_SIZE 20
#define VISCOSITY 0.9f

#define MIN(a,b) (a < b) ? a : b
#define MAX(a,b) (a > b) ? a : b

#include <SFML/OpenGL.hpp>
#include <vector>

extern float gravAdjust;
extern float viscosity;

class ParticleEmitter {
public:
	ParticleEmitter();
	~ParticleEmitter();
	void Update(GLfloat t, GLfloat zRot);

	const GLfloat* VertexArray(){ return particlePos; }
	const GLfloat* ColorArray(){ return particleColor; }
private:
	GLfloat particlePos[MAX_PARTICLES*3];
	GLfloat particlePosOld[MAX_PARTICLES*3];
	GLfloat particleVel[MAX_PARTICLES*3];
	GLfloat particleAcc[MAX_PARTICLES*3];
	GLfloat particleColor[MAX_PARTICLES*4];
	GLfloat particleUnit[MAX_PARTICLES];
	u_int particleLife[MAX_PARTICLES];
	std::vector<int> grid[GRID_SIZE][GRID_SIZE];
	GLfloat particleDist[MAX_PARTICLES][MAX_PARTICLES];

	void Reset(u_int i);
	void RefreshGrid();
	void ResolveCollisions();
};

#endif //PARTICLE_EMITTER_H