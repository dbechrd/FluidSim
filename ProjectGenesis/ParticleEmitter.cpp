#include "ParticleEmitter.h"

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <iostream>

#define PI			  3.14159265// PI Constant
#define MAX_VEL		  0.05f		// Max velocity
#define HALF_MAX_VEL  0.025f	// Max velocity / 2
#define MAX_LIFE	200.0f		// Lifetime of particles
#define G			 -0.00981f	// Gravitational constant
#define RADIUS_INF	  0.025f	// Radius of influence
#define KSTIFF		  0.0004f		// Stiffness parameter
#define KSTIFF_NEAR	  0.0004f		// Near stiffness parameter
#define REST_DENSITY 800.0f		// Density of fluid at rest
#define DELTA_CLAMP	  99.0f		// Maximum delta x / delta y per frame 
#define EPSILON		  0.000001f	// Minimum velocity
#define VISCOSITY	  0.95f		//Artificial viscosity

#define BOX_X	0.99f	// Container bounds
#define BOX_Y	0.99f
#define BOX_Z	0.99f

#define ALPHA	1.0f;	// Particle transparency

float gravAdjust = 1.0f;

const float max_color_rand = 1.0f/RAND_MAX;
const float max_velocity_rand = MAX_VEL/RAND_MAX;
const float max_lifetime_rand = MAX_LIFE/RAND_MAX;

int reset = 0;

ParticleEmitter::ParticleEmitter() {
	for (int i = 0; i < MAX_PARTICLES; ++i) {
		Reset(i);
	}
}

ParticleEmitter::~ParticleEmitter() {
	//int i, j;
	//for(j = 0; j < GRID_SIZE; ++j) {
	//	for(i = 0; i < GRID_SIZE; ++i) {
	//		delete grid[i][j];
	//	}
	//}
}

void ParticleEmitter::Reset(u_int i) {
	int i1 = i*3;	// x | r
	int i2 = i1+1;	// y | g
	int i3 = i1+2;	// z | b
	particleAcc[i1] = 0.004f;
	particleAcc[i2] = 0.0f;
	particleAcc[i3] = 0.0f;
	particleVel[i1] = ((GLfloat)rand()*max_velocity_rand) - HALF_MAX_VEL;
	particleVel[i2] = ((GLfloat)rand()*max_velocity_rand) - HALF_MAX_VEL;
	particleVel[i3] = 0.0f;//((GLfloat)rand()*max_velocity_rand) - HALF_MAX_VEL;
	particlePos[i1] = -0.9f + particleVel[i1];
	particlePos[i2] = 0.9f + particleVel[i2];
	particlePos[i3] = 0.0f;//particleVel[i3]*50;
	particleVel[i1] = 0.0f;
	particleVel[i2] = 0.0f;
	particleVel[i3] = 0.0f;
	particleColor[i1+i] = 0.0f;//(rand()*max_color_rand);
	particleColor[i2+i] = 0.0f;//(rand()*max_color_rand);
	particleColor[i3+i] = 1.0f;//(rand()*max_color_rand);
	particleColor[i3+i+1] = ALPHA;//(rand()*max_color_rand);
	//Cube
	particleLife[i] = (u_int)(rand()*max_lifetime_rand + MAX_LIFE);
	//Sphere
	//particleLife[i] = 2 / sqrt((particleVel[i1]*particleVel[i1]) + (particleVel[i2]*particleVel[i2]) + (particleVel[i3]*particleVel[i3]));
}

void ParticleEmitter::RefreshGrid() {
	int x, y; //Grid coords
	for(y = 0; y < GRID_SIZE; ++y) {
		for(x = 0; x < GRID_SIZE; ++x) {
			grid[x][y].clear();
		}
	}
	
	/*for(int i = 0; i < MAX_PARTICLES; ++i) {
		x = (int)((particlePos[i*3] + 1.0f) / 2.0f * GRID_SIZE);
		y = (int)((particlePos[i*3+1] + 1.0f) / 2.0f * GRID_SIZE);
		grid[x][y].push_back(i);
	}*/

	int nx, ny; //Neighbor coords
	for(int i = 0; i < MAX_PARTICLES; ++i) {
		x = (int)((particlePos[i*3] + 1.0f) / 2.0f * GRID_SIZE);
		y = (int)((particlePos[i*3+1] + 1.0f) / 2.0f * GRID_SIZE);

		//Add to cell and its closest neighbors
		for(ny = MAX(y-1,0); ny <= y+1 && ny < GRID_SIZE; ++ny) {
			for(nx = MAX(x-1,0); nx <= x+1 && nx < GRID_SIZE; ++nx) {
				grid[nx][ny].push_back(i);
			}
		}
	}
}

void ParticleEmitter::ResolveCollisions() {
	int x, y, z;
	for (int i = 0; i < MAX_PARTICLES; ++i) {
		x = i*3;
		y = x+1;
		z = x+2;

		if(particlePos[x] < -BOX_X) {
			particlePos[x] = -BOX_X;
			//particleVel[x] *= -1.0f; //Absorb some horizontal energy
			particleAcc[x] = 0.0f;
		} else if (particlePos[x] > BOX_X) {
			particlePos[x] = BOX_X;
			//particleVel[x] *= -1.0f; //Absorb some horizontal energy
			particleAcc[x] = 0.0f;
		}
		if(particleVel[x] < EPSILON) particleVel[x] = 0.0f;

		if(particlePos[y] < -BOX_Y) {
			particlePos[y] = -BOX_Y;
			//particleVel[y] *= -1.0f;
		} else if (particlePos[y] > BOX_Y) {
			particlePos[y] = BOX_Y;
			//particleVel[y] *= -1.0f;
		}
		if(particleVel[y] < EPSILON) particleVel[y] = 0.0f;

		/*if(particlePos[z] < -BOX_Z) {
			particlePos[z] = -BOX_Z;
			particleVel[z] *= -1;
		} else if (particlePos[z] > BOX_Z) {
			particlePos[z] = BOX_Z;
			particleVel[z] *= -1;
		}*/
	}
}

void ParticleEmitter::Update(GLfloat dt, GLfloat zRot) {
	int i, j;
	int ix, iy;//, iz;
	int a, b;

	//Rebuild neighbor search grid
	RefreshGrid();

	int gx, gy, x, y; //Grid coords
	GLfloat p, pNear; //Density
	GLfloat P, PNear; //Presure
	GLfloat q;
	GLfloat bdx, bdy;
	GLfloat dx, dy, D;

	/*if(reset < MAX_PARTICLES - 10) {
		for(i = 0; i < 10; ++i) {
			Reset(reset++);
		}
	}*/

	GLfloat Gx = (GLfloat)-cos(zRot*PI/180.0f) * G;
	GLfloat Gy = (GLfloat)sin(zRot*PI/180.0f) * G;

	//Update position
	for (i = 0; i < MAX_PARTICLES; ++i) {
		//if(particleLife[i] <= 0) Reset(i);
		//particleLife[i]--;

		ix = i*3;
		iy = ix+1;
		//iz = ix+2;

		//Apply acceleration
		particleVel[ix] += (particleAcc[ix] + (Gx*gravAdjust)) * dt;
		particleVel[iy] += (particleAcc[iy] + (Gy*gravAdjust)) * dt;
		//particleVel[iz] += particleAcc[iz] * dt;

		particlePosOld[ix] = particlePos[ix];
		particlePosOld[iy] = particlePos[iy];

		//Apply velocity
		particlePos[ix] += particleVel[ix] * dt;
		particlePos[iy] += particleVel[iy] * dt;
		//particlePos[iz] += particleVel[iz] * dt;

		//particleVel[iy] += G;
		//particlePos[iy] += particleVel[iy];

		//Color
		//particleColor[i*4] = ((float)particleLife[i]/MAX_LIFE) * 0.25f;
		//particleColor[i*4+1] = ((float)particleLife[i]/MAX_LIFE) * 0.25f;
	}

	//Each grid cell
	for(gy = 0; gy < GRID_SIZE; ++gy) {
		for(gx = 0; gx < GRID_SIZE; ++gx) {

			//Each particle in cell
			for(i = grid[gx][gy].size() - 1; i >= 0; --i) {

				//Density calculation
				a = grid[gx][gy][i]; //Particle A
				p = pNear = 0; //Reset density

				//Each other particle in cell and neighboring cells
				for(y = MAX(gy-1,0); y <= gy+1 && y < GRID_SIZE; ++y) {
					for(x = MAX(gx-1,0); x <= gx+1 && x < GRID_SIZE; ++x) {
						for(j = grid[x][y].size() - 1; j >= 0; --j) {
							if(i == j) continue; //Don't compare particles to themselves

							b = grid[x][y][j]; //Particle B
							bdx = particlePos[a*3] - particlePos[b*3];
							bdy = particlePos[a*3+1] - particlePos[b*3+1];
							particleDist[a][b] = sqrt(bdx*bdx + bdy*bdy); //Store for reuse in pressure calulation
							q = particleDist[a][b] / RADIUS_INF;

							//Check if particle within range of influence
							if(q < 1) {
								p += (1-q)*(1-q);
								pNear += (1-q)*(1-q)*(1-q);
							}
						}
					}
				}

				//Pressure calculation
				P = KSTIFF * (p - REST_DENSITY);
				PNear = KSTIFF_NEAR * pNear;
				dx = dy = 0;

				//Each other particle in cell and neighboring cells
				for(y = MAX(gy-1,0); y <= gy+1 && y < GRID_SIZE; ++y) {
					for(x = MAX(gx-1,0); x <= gx+1 && x < GRID_SIZE; ++x) {
						for(j = grid[x][y].size() - 1; j >= 0; --j) {
							if(i == j) continue; //Don't compare particles to themselves

							b = grid[x][y][j]; //Particle B
							bdx = particlePos[a*3] - particlePos[b*3];
							bdy = particlePos[a*3+1] - particlePos[b*3+1];
							q = particleDist[a][b] / RADIUS_INF;

							//Check if particle within range of influence
							if(q < 1) {
								//p += (1-q)*(1-q);
								D = dt*dt * ((P*(1-q)) + (PNear*(1-q)*(1-q)));
								particlePos[b*3] += MIN((D*bdx) / 2.0f, DELTA_CLAMP);
								particlePos[b*3+1] += MIN((D*bdy) / 2.0f, DELTA_CLAMP);
								dx -= (D*bdx) / 2.0f;
								dy -= (D*bdy) / 2.0f;
							}
						}
					}
				}

				particlePos[a*3] += MIN(dx, DELTA_CLAMP);
				particlePos[a*3+1] += MIN(dy, DELTA_CLAMP);
			}
		}
	}

	ResolveCollisions();

	//Update velocity
	for (i = 0; i < MAX_PARTICLES; ++i) {
		ix = i*3;
		iy = ix+1;
		
		dx = (particlePos[ix] - particlePosOld[ix]);
		dy = (particlePos[iy] - particlePosOld[iy]);

		if(dx > DELTA_CLAMP) dx = DELTA_CLAMP;
		if(dy > DELTA_CLAMP) dy = DELTA_CLAMP;

		particleVel[ix] = dx / dt;
		particleVel[iy] = dy / dt;

		particleVel[ix] *= VISCOSITY;
		particleVel[iy] *= VISCOSITY;

		//particleColor[i*4] = (particlePos[iy] + 1.0f) / 0.4f;
		//particleColor[i*4+1] = (particlePos[iy] + 1.0f) / 0.3f;
	}
}
