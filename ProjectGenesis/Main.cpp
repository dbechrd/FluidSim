//*****************************
// Project
// Dan Bechard
// Copyright 2012
//*****************************

#include <iostream>
#include <fstream>

#include <vld.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <GL/glut.h>

#include "ParticleEmitter.h"

#define FPS 60
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 800
#define CAMERA_DISTANCE 2.5f
#define FULLSCREEN false

using namespace std;

bool mouseLeftDown = false;
bool mouseRightDown = false;
float mouseX, mouseY = 0;
float cameraAngleX = 0.0f;
float cameraAngleY = 0.0f;
float cameraAngleZ = 0.0f;
float cameraDistance = CAMERA_DISTANCE;
int drawMode = 0; // 0:fill, 1: wireframe, 2:points
sf::Event event;
bool quit = false;

//						 x    y    z      x    y    z      x    y    z
GLfloat vertices[] = { -1.1, 1.1, 0.1,  -1.1,-1.1, 0.1,  -1.0, 1.1, 0.1, 
                       -1.0, 1.1, 0.1,  -1.1,-1.1, 0.1,  -1.0,-1.1, 0.1, 

					   -1.1, 1.1,-0.1,  -1.1,-1.1,-0.1,  -1.1, 1.1, 0.1, 
                       -1.1, 1.1, 0.1,  -1.1,-1.1,-0.1,  -1.1,-1.1, 0.1, 

                       -1.0, 1.1,-0.1,  -1.0,-1.1,-0.1,  -1.1, 1.1,-0.1, 
                       -1.1, 1.1,-0.1,  -1.0,-1.1,-0.1,  -1.1,-1.1,-0.1, 

                       -1.0, 1.1, 0.1,  -1.0,-1.1, 0.1,  -1.0, 1.1,-0.1, 
                       -1.0, 1.1,-0.1,  -1.0,-1.1, 0.1,  -1.0,-1.1,-0.1 };

// normal array
GLfloat normals[]  = {  0, 0, 1,   0, 0, 1,   0, 0, 1, 
                        0, 0, 1,   0, 0, 1,   0, 0, 1, 

                       -1, 0, 0,  -1, 0, 0,  -1, 0, 0, 
                       -1, 0, 0,  -1, 0, 0,  -1, 0, 0, 

						0, 0,-1,   0, 0,-1,   0, 0,-1, 
                        0, 0,-1,   0, 0,-1,   0, 0,-1,

                        1, 0, 0,   1, 0, 0,   1, 0, 0, 
                        1, 0, 0,   1, 0, 0,   1, 0, 0 };

// color array
GLfloat colors[]   = { 1, 1, 1,   1, 1, 0,   1, 0, 0, 
                       1, 0, 0,   1, 0, 1,   1, 1, 1, 

                       1, 1, 1,   1, 0, 1,   0, 0, 1, 
                       0, 0, 1,   0, 1, 1,   1, 1, 1, 

                       1, 1, 0,   0, 1, 0,  .1,.1,.1, 
                      .1,.1,.1,   1, 0, 0,   1, 1, 0, 

                       0, 0, 1,  .1,.1,.1,   0, 1, 0, 
                       0, 1, 0,   0, 1, 1,   0, 0, 1 };
///////////////////////////////////////////////////////////////////////////////

sf::RenderWindow* window;
ParticleEmitter emitter;

bool initSFML() {
	sf::ContextSettings context;
	context.antialiasingLevel = 8;
	context.depthBits = 24;
	window = new sf::RenderWindow(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Initial Prototype 071512.1", (FULLSCREEN) ? sf::Style::Fullscreen : sf::Style::Default, context);
	if(!window->isOpen()) {
        cout << "RenderWindow creation Error\n";
		return false;
    }
	window->setFramerateLimit(FPS);

	//sf::Font font;
	//font.loadFromFile("fonts/times.ttf");

	//sf::Text text("Font test", font, 16);
	//text.setPosition(0, 0);

	//Load music
	//sf::Music music;
	//if (!music.openFromFile("music/kocpr.ogg"))
	//	return false;

	//Play music
	//music.setLoop(true);
	//music.play();

	return true;
}

void initGL() {
	//glShadeModel(GL_SMOOTH);					// shading mathod: GL_SMOOTH or GL_FLAT
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 4);	// 4-byte pixel alignment

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.75f, 0.75f, 0.75f, 0);					// background color
    //glClearStencil(0);						// clear stencil buffer
    //glClearDepth(1.0f);						// near (0) or far (1), default far assumes back-to-front rendering
	glDepthFunc(GL_LESS);						// depth compare function
    //glDepthFunc(GL_LEQUAL);
	//glCullFace(GL_BACK);						// back-face culling
	//glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    //glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    //glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	//glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	//glHint(GL_FOG_HINT, GL_NICEST);

	
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_LIGHTING);
    //glEnable(GL_TEXTURE_2D);
    //glEnable(GL_CULL_FACE);
    glEnable(GL_COLOR_MATERIAL);

	glPointSize(15.0f);
}

void initLights() {
	// set up light colors (ambient, diffuse, specular)
    GLfloat lightKa[] = {.2f, .2f, .2f, 1.0f};  // ambient light
    GLfloat lightKd[] = {.7f, .7f, .7f, 1.0f};  // diffuse light
    GLfloat lightKs[] = {1, 1, 1, 1};           // specular light
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightKa);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightKd);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightKs);

    // position the light
    float lightPos[4] = {0, 0, 20, 1};			// positional light
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glEnable(GL_LIGHT0);
}

void setPerspective() {
	// Full window viewport
    glViewport(0, 0, (GLsizei)SCREEN_WIDTH, (GLsizei)SCREEN_HEIGHT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (float)(SCREEN_WIDTH)/SCREEN_HEIGHT, 0.01f, 1000.0f); // FOV, AspectRatio, NearClip, FarClip

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void draw() {
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);

	glNormalPointer(GL_FLOAT, 0, normals);
    glColorPointer(3, GL_FLOAT, 0, colors);
    glVertexPointer(3, GL_FLOAT, 0, vertices);

	glPushMatrix();
	glColor3f(0.9f, 0.9f, 0.9f);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	glDrawArrays(GL_TRIANGLES, 0, 36);
    glPopMatrix();

	glDisableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
    //glNormalPointer(GL_FLOAT, 0, part_normals);
    glColorPointer(4, GL_FLOAT, 0, emitter.ColorArray());
    glVertexPointer(3, GL_FLOAT, 0, emitter.VertexArray());

    glPushMatrix();
	glDrawArrays(GL_POINTS, 0, MAX_PARTICLES);
    glPopMatrix();

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}

void HandleEvents() {
	//////////////////// Event Handling ///////////////////////////////////
	while (window->pollEvent(event)) {
		// Close window : exit
		if (event.type == sf::Event::Closed) {
            quit = true;
		} else if (event.type == sf::Event::Resized) {
			//TODO: Maintain aspect ratio
			glViewport(0, 0, (GLsizei)event.size.width, (GLsizei)event.size.height);
		} else if (event.type == sf::Event::KeyPressed) {
			// Escape key : exit
			if (event.key.code == sf::Keyboard::Escape) {
				quit = true;
			} else if (event.key.code == sf::Keyboard::D) {
				drawMode = ++drawMode % 3;
				if(drawMode == 0)        // fill mode
				{
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glEnable(GL_DEPTH_TEST);
					glEnable(GL_CULL_FACE);
				}
				else if(drawMode == 1)  // wireframe mode
				{
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					glDisable(GL_DEPTH_TEST);
					glDisable(GL_CULL_FACE);
				}
				else                    // point mode
				{
					glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
					glDisable(GL_DEPTH_TEST);
					glDisable(GL_CULL_FACE);
				}
			} else if (event.key.code == sf::Keyboard::G) {
				//Reverse gravity
				gravAdjust *= -1.0f;
			} else if (event.key.code == sf::Keyboard::R) {
				//Reset settings
				gravAdjust = 1.0f;
				cameraAngleX = 0.0f;
				cameraAngleY = 0.0f;
				cameraAngleZ = 0.0f;
				cameraDistance = CAMERA_DISTANCE;
			}
		} else if (event.type == sf::Event::MouseButtonPressed) {
			mouseX = sf::Mouse::getPosition().x;//event.mouseButton.x;
			mouseY = sf::Mouse::getPosition().y;//event.mouseButton.y;

			if(event.mouseButton.button == sf::Mouse::Left) {
				mouseLeftDown = true;
			} else if(event.mouseButton.button == sf::Mouse::Right) {
				mouseRightDown = true;
			}
		} else if (event.type == sf::Event::MouseButtonReleased) {
			mouseX = sf::Mouse::getPosition().x;//event.mouseButton.x;
			mouseY = sf::Mouse::getPosition().y;//event.mouseButton.y;

			if(event.mouseButton.button == sf::Mouse::Left) {
				mouseLeftDown = false;
			} else if(event.mouseButton.button == sf::Mouse::Right) {
				mouseRightDown = false;
			}
		} else if (event.type == sf::Event::MouseMoved) {
			int x = sf::Mouse::getPosition().x;//event.mouseButton.x;
			int y = sf::Mouse::getPosition().y;//event.mouseButton.y;

			if(mouseLeftDown) {
				cameraAngleY += (x - mouseX);
				cameraAngleX += (y - mouseY);
				mouseX = x;
				mouseY = y;
			} if(mouseRightDown) {
				cameraDistance -= (y - mouseY) * 0.2f;
				mouseY = y;
			}
		} else if (event.type == sf::Event::MouseWheelMoved) {
			float delta = (float)event.mouseWheel.delta;
			cameraAngleZ += delta * 4.0f;
			if(cameraAngleZ > 360.0f && delta > 0.0f)
				cameraAngleZ -= 360.0f;
			else if(cameraAngleZ < 0.0f && delta < 0.0f)
				cameraAngleZ += 360.0f;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv) {
	//Random seed
	srand((u_int)time(NULL));

	remove("output.log");

	//Set up logging
	ofstream logFile;
	logFile.open("output.log", std::ios_base::app);
	cout.rdbuf(logFile.rdbuf()); //Redirect cout to log file

	if(!initSFML()) return EXIT_FAILURE;
	initGL();
	initLights();
	setPerspective();

	while(!quit) {
		HandleEvents();
		emitter.Update(1.0f, cameraAngleZ+90.0f);

		//////////////////// OpenGL Rendering /////////////////////////////////
		// Clear buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glPushMatrix();
			// Camera
			glTranslatef(0, 0, -cameraDistance);
			glRotatef(cameraAngleX, 1, 0, 0);   // pitch
			glRotatef(cameraAngleY, 0, 1, 0);   // yaw
			glRotatef(cameraAngleZ, 0, 0, 1);   // roll

			// Scene
			draw();
		glPopMatrix();

		// Swap buffers
		window->display();
    }

	cout << "Terminating Application\n";
	window->close();
	delete window;
	logFile.close();
	return EXIT_SUCCESS;
}
