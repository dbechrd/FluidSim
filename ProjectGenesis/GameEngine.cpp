#include <iostream>
#include <fstream>

#include "GameEngine.h"
#include "RenderEngine.h"
#include "TextEngine.h"

using namespace std;

//SFML global keypressed wrapper
bool KeyPressed(sf::Keyboard::Key key) {
	return sf::Keyboard::isKeyPressed(key);
}

GameEngine* GameEngine::pinstance = NULL;
bool GameEngine::quit = false;
GameEngine* GameEngine::Instance(const std::string &filename) {
    if (pinstance == NULL) {
        pinstance = new GameEngine(filename.c_str());
    }
    return pinstance;
}

GameEngine::GameEngine(const std::string &filename) {
    renderer = NULL;
    text = NULL;
    level = NULL;
    renderer = RenderEngine::Instance();
    text = TextEngine::Instance();

    //Delete old log file
    //remove("output.log");

    //Initialize engine variables
    mousex = 0;
    mousey = 0;
    mouseLeft = false;
    mouseRight = false;
    level = NULL;

    //Configuration file
    std::ifstream configStream(filename.c_str());

    if(configStream == NULL) {
        cout << "Config Stream is NULL\n";
    }

	//TODO: Configuration file for what map to load

    //Read in map filename
    std::string mapname = "";
    configStream >> mapname;

    configStream.close();
}

GameEngine::~GameEngine() {
	cout << "Terminating Game Engine\n";
    cout.flush();

    if(level != NULL) {
        delete level;
        level = NULL;
        cout << "Level Deleted\n";
        cout.flush();
    }
}

void GameEngine::HandleInput() {
	while(RenderEngine::Instance()->Window()->pollEvent(event)) {
        if(event.type == sf::Event::Closed) {
            SetQuit();
        }
		if (event.type == sf::Event::KeyPressed) {
			switch(event.key.code) {
				case sf::Keyboard::F4:
                    renderer->ToggleFullScreen();
                    break;
				default:
					break;
            }
        }
		if (event.type == sf::Event::KeyReleased) {
			switch(event.key.code) {
				case sf::Keyboard::Escape:
                    this->SetQuit();
                    break;
				default:
					break;
            }
        }
		if(event.type == sf::Event::MouseButtonReleased) {
			switch(event.mouseButton.button) {
				case sf::Mouse::Left:
                    mouseLeft = false;
                    break;
				case sf::Mouse::Right:
                    mouseRight = false;
                    break;
				default:
					break;
            }
        }
		if(event.type == sf::Event::MouseButtonPressed) {
            mousex = event.mouseButton.x;
            mousey = event.mouseButton.y;
            switch(event.mouseButton.button) {
				case sf::Mouse::Left:
                    mouseRight = false;
                    mouseLeft = true;
                    //Do Something On Mouse Button Left
                    break;
				case sf::Mouse::Right:
                    mouseLeft = false;
                    mouseRight = true;
                    //Do Something On Mouse Button Right
                    break;
				case sf::Mouse::Middle:
                    //Do Something On Mouse Button Middle
                    break;
                default:
                    break;
            }
        }
		if(event.type == sf::Event::MouseWheelMoved) {
			int mouseWheelDelta = event.mouseWheel.delta;
			//if(mouseWheelDelta > 0)
			//	//Mouse wheel up
			//else
			//	//Mouse wheel down
        }
		if(event.type == sf::Event::MouseMoved) {
			mousex = event.mouseMove.x;
			mousey = event.mouseMove.y;
            //Do Something On Mouse Move
        }
    }
}

bool GameEngine::KeyDown(std::string key) {
    iter = keys.find(key);
    if(iter != keys.end()) {
        return iter->second;
    }else{
        return false;
    }
}

bool GameEngine::KeyUp(std::string key) {
    iter = keys.find(key);
    if(iter != keys.end()) {
        return !iter->second;
    }else{
        return true;
    }
}

int GameEngine::Collision() {
	return 0;
}

//int GameEngine::Sleep(int duration)
//{
//    if(duration == -1) {
//        int newTime = Clock.getElapsedTime().asMilliseconds();
//        int deltaTime = newTime - oldTime;
//        if(deltaTime < 1000 / fps)
//        {
//            SDL_Delay((1000 / fps) - deltaTime);
//            oldTime = newTime;
//        }
//    }else{
//        SDL_Delay(duration);
//    }
//    return 0;
//}

void GameEngine::Update() {
    level->Update();
    text->Update();
}

void GameEngine::Display() {
	renderer->Display();
}

Level* GameEngine::LoadLevel(std::string mapname) {
    cout << mapname << endl;
    level = new Level(mapname);
    return level;
}

Level* GameEngine::GetLevel() {
    return level;
}

void GameEngine::SetQuit() {
    quit = true;
}

bool GameEngine::Quit() {
    return quit;
}
