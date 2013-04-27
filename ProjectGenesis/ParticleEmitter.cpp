#include "ParticleEmitter.h"
#include "Particle.h"

#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX_VEL		  0.01f		// Max velocity
#define HALF_MAX_VEL  0.005f	// Max velocity / 2
#define MAX_LIFE	200.0f		// Lifetime of particles
#define G			 -0.000981f	// Gravitational constant

#define BOX_X	0.99f	// Container bounds
#define BOX_Y	0.99f
#define BOX_Z	0.99f

#define ALPHA	0.8f;	// Particle transparency

const float max_color_rand = 1.0f/RAND_MAX;
const float max_velocity_rand = MAX_VEL/RAND_MAX;
const float max_lifetime_rand = MAX_LIFE/RAND_MAX;

ParticleEmitter::ParticleEmitter() {
	for (int i = 0; i < MAX_PARTICLES; ++i) {
		Reset(i);
	}
}

ParticleEmitter::~ParticleEmitter() {

}

void ParticleEmitter::Reset(u_int i) {
	int i1 = i*3;	// x | r
	int i2 = i1+1;	// y | g
	int i3 = i1+2;	// z | b
	particleAcc[i1] = 0.0f;
	particleAcc[i2] = 0.0f;
	particleAcc[i3] = 0.0f;
	particleVel[i1] = ((GLfloat)rand()*max_velocity_rand) - HALF_MAX_VEL;
	particleVel[i2] = ((GLfloat)rand()*max_velocity_rand) - HALF_MAX_VEL;
	particleVel[i3] = ((GLfloat)rand()*max_velocity_rand) - HALF_MAX_VEL;
	particlePos[i1] = particleVel[i1]*20;
	particlePos[i2] = particleVel[i2]*20;
	particlePos[i3] = particleVel[i3]*20;
	particleColor[i1+i] = 1.0f;//(rand()*max_color_rand);
	particleColor[i2+i] = 1.0f;//(rand()*max_color_rand);
	particleColor[i3+i] = 1.0f;//(rand()*max_color_rand);
	particleColor[i3+i+1] = ALPHA;//(rand()*max_color_rand);
	//Cube
	particleLife[i] = (u_int)(rand()*max_lifetime_rand + MAX_LIFE);
	//Sphere
	//particleLife[i] = 2 / sqrt((particleVel[i1]*particleVel[i1]) + (particleVel[i2]*particleVel[i2]) + (particleVel[i3]*particleVel[i3]));
}

void ParticleEmitter::ResolveCollisions() {
	int x, y, z;
	for (int i = 0; i < MAX_PARTICLES; ++i) {
		x = i*3;
		y = x+1;
		z = x+2;

		if(particlePos[x] < -BOX_X) {
			particlePos[x] = -BOX_X;
			particleVel[x] *= -1;
		} else if (particlePos[x] > BOX_X) {
			particlePos[x] = BOX_X;
			particleVel[x] *= -1;
		}

		if(particlePos[y] < -BOX_Y) {
			particlePos[y] = -BOX_Y;
		} else if (particlePos[y] > BOX_Y) {
			particlePos[y] = BOX_Y;
		}

		if(particlePos[z] < -BOX_Z) {
			particlePos[z] = -BOX_Z;
			particleVel[z] *= -1;
		} else if (particlePos[z] > BOX_Z) {
			particlePos[z] = BOX_Z;
			particleVel[z] *= -1;
		}
	}
}

void ParticleEmitter::Update(GLfloat dt) {
	int i;
	int ix, iy, iz;
	
	//Gravity
	for (i = 0; i < MAX_PARTICLES; ++i) {
		particleVel[i*3+1] += G;
	}

	//Update position
	for (i = 0; i < MAX_PARTICLES; ++i) {
		if(particleLife[i] <= 0) Reset(i);
		particleLife[i]--;

		ix = i*3;
		iy = ix+1;
		iz = ix+2;

		//Artificial acceleration (zero)
		particleVel[ix] += particleAcc[ix] * dt;
		particleVel[iy] += (particleAcc[iy] + G) * dt;
		particleVel[iz] += particleAcc[iz] * dt;

		//particleVel[ix] *= 0.995f;
		//particleVel[iz] *= 0.995f;

		//Velocity
		particlePos[ix] += particleVel[ix] * dt;
		particlePos[iy] += particleVel[iy] * dt;
		particlePos[iz] += particleVel[iz] * dt;

		//Color
		particleColor[i*4] = ((float)particleLife[i]/MAX_LIFE) * 0.25f;
		particleColor[i*4+1] = ((float)particleLife[i]/MAX_LIFE) * 0.25f;
	}

	ResolveCollisions();
}
