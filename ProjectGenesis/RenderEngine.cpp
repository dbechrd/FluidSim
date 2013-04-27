#include <iostream>
#include <string>
#include <vector>

#include "sqlite3.h"
#include "SFML/Graphics.hpp"
#include "RenderEngine.h"
#include "IRender.h"
#include "Camera.h"

using namespace std;

sf::Clock Clock;
sqlite3* RenderEngine::db = NULL;
RenderEngine* RenderEngine::pinstance = NULL;
RenderEngine* RenderEngine::Instance() {
    if (pinstance == NULL) {
        pinstance = new RenderEngine;
    }
    return pinstance;
}

RenderEngine::RenderEngine() {
	fullScreen = START_FULLSCREEN;
    worldCoords = true;
    resetCoords = false;

	window = new sf::RenderWindow(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Initial Prototype 071512.1", (fullScreen) ? sf::Style::Fullscreen : sf::Style::Default);
	window->setFramerateLimit(FPS);
    if(window == NULL) {
        cout << "RenderWindow creation Error\n";
    }

	spritesheets = 0;
	for(int i = 0; i < MAX_SPRITESHEETS; ++i) {
		spritesheetCollection[i] = NULL;
	}

    camera = Camera::Instance();
}

RenderEngine::~RenderEngine() {
	int i;
	cout << "Terminating Render Engine\n";

	std::vector<IRender*>::iterator renderablesEnd = renderables.end();
	for(renderablesIter = renderables.begin(); renderablesIter != renderablesEnd; renderablesIter++) {
        renderables.erase(renderablesIter);
		cout << "Renderable deleted\n";
    }
	renderables.clear();

	std::map<std::string, TextureReference*>::iterator surfacesEnd = surfaces.end();
    for(surfacesIter = surfaces.begin(); surfacesIter != surfacesEnd; surfacesIter++) {
		delete surfacesIter->second;
		cout << "Surface deleted\n";
    }
    surfaces.clear();

    for(i = 0; i < MAX_SPRITESHEETS; ++i) {
		delete spritesheetCollection[i];
    }

	delete window;
}

void RenderEngine::AddRenderable(IRender* renderable) {
	renderables.push_back(renderable);
}

void RenderEngine::RemoveRenderable(IRender* renderable) {
	std::vector<IRender*>::iterator end = renderables.end();
	for(renderablesIter = renderables.begin(); renderablesIter != end; renderablesIter++) {
        if(*renderablesIter == renderable) renderables.erase(renderablesIter);
		return;
    }
}

void RenderEngine::ToggleFullScreen() {
	fullScreen = !fullScreen;
	window->create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Initial Prototype 071512.1", (fullScreen) ? sf::Style::Fullscreen : sf::Style::Default);
}

sf::Texture* RenderEngine::LoadSurface(std::string filename) {
	std::map<std::string, TextureReference*>::iterator end = surfaces.end();
    for(surfacesIter = surfaces.begin(); surfacesIter != end; surfacesIter++) {
        if(filename == surfacesIter->first) {
            std::cout << "Existing Surface - Not loaded again (Yay Success!)\n";
			surfacesIter->second->refCount++;
			return surfacesIter->second->texture;
        }
    }

    //Load texture
    sf::Texture* texture = new sf::Texture();
	if(!texture->loadFromFile(filename.c_str()))
		return NULL;

	TextureReference* texRef = new TextureReference();
	texRef->refCount = 1;
	texRef->texture = texture;
    surfaces[filename] = texRef;

    return texture;
}

void RenderEngine::UnloadSurface(std::string filename) {
    surfacesIter = surfaces.find(filename);
	if(surfacesIter == surfaces.end()) return; //Oops, not found

	if(--surfacesIter->second->refCount == 0) {
		delete surfacesIter->second;
		surfaces.erase(surfacesIter);
	}
}

sf::Vector2u RenderEngine::GetSurfaceRect(std::string filename) {
	surfacesIter = surfaces.find(filename);
	if(surfacesIter != surfaces.end()) {
		return surfacesIter->second->texture->getSize();
	}
	return sf::Vector2u(0, 0); //Oops, not found
}

int RenderEngine::LoadSpritesheetCB(void *instance, int argc, char **argv, char **azColName) {
	RenderEngine* me = ((RenderEngine*)instance);
	
	int i;
	for(i=0; i<argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
		std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << "\n";
	}
	
	int id = atoi(argv[0]);
	std::string filename = argv[1];
	int frames = atoi(argv[2]);
	int frameW = atoi(argv[3]);
	int frameH = atoi(argv[4]);

	sf::Texture* texture = me->LoadSurface(filename);

	SpritesheetReference* spriteRef = new SpritesheetReference();
	spriteRef->refCount = 1;
	spriteRef->spritesheet = new Spritesheet(id, texture, frames, frameW, frameH);
	me->spritesheetCollection[id] = spriteRef;
	
	return SQLITE_OK;
}

Spritesheet* RenderEngine::LoadSpritesheet(int spritesheetId) {
	//Check if already loaded
	if(spritesheetCollection[spritesheetId] != NULL)
		return spritesheetCollection[spritesheetId]->spritesheet;

	char* zErrMsg = NULL;

	char *zSQL = sqlite3_mprintf("SELECT * FROM Spritesheet WHERE Id = %d", spritesheetId);
	if(sqlite3_exec(RenderEngine::db, zSQL, LoadSpritesheetCB, this, &zErrMsg) != SQLITE_OK) {
		printf("SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	sqlite3_free(zSQL);

	return spritesheetCollection[spritesheetId]->spritesheet;
}

void RenderEngine::UnloadSpritesheet(int spritesheetId) {
	if(spritesheetCollection[spritesheetId] == NULL) return;

	if(--spritesheetCollection[spritesheetId]->refCount == 0) {
		delete spritesheetCollection[spritesheetId];
		spritesheetCollection[spritesheetId] = NULL;
	}
}

void RenderEngine::EnableWorldCoords() {
    worldCoords = true;
}

void RenderEngine::DisableWorldCoords(bool permanant) {
    worldCoords = false;
    if(!permanant) {
        resetCoords = true;
    }
}

void RenderEngine::Draw(sf::Sprite sprite) {

	//World coords as opposed to screen coords (which disregard camera position)
    if(worldCoords) {
        sprite.move((float)-camera->Rect()->left, (float)-camera->Rect()->top);
    } else if(resetCoords) {
        worldCoords = true;
        resetCoords = false;
    }

	window->draw(sprite);
}

void RenderEngine::Draw(int x, int y, sf::Sprite sprite) {
	sprite.setPosition((float)x, (float)y);
	Draw(sprite);
}

void RenderEngine::Draw(int x, int y, sf::Texture* texture, sf::IntRect* clip) {
	sf::Sprite sprite;

    //World coords as opposed to screen coords (which disregard camera position)
    if(worldCoords) {
        x -= camera->Rect()->left;
        y -= camera->Rect()->top;
    }else if(resetCoords) {
        worldCoords = true;
        resetCoords = false;
    }

	sprite.setPosition((float)x, (float)y);
	sprite.setTexture(*texture);
	if(clip != NULL) sprite.setTextureRect(*clip);
	window->draw(sprite);
}

void RenderEngine::Draw(sf::Text text) {
	if(worldCoords) {
		sf::Vector2f pos = text.getPosition();
		pos.x -= camera->Rect()->left;
        pos.y -= camera->Rect()->top;
		text.setPosition(pos);
    }else if(resetCoords) {
        worldCoords = true;
        resetCoords = false;
    }

	window->draw(text);
}

void RenderEngine::Draw(sf::FloatRect rect, sf::Color color) {
	if(worldCoords) {
		rect.left -= camera->Rect()->left;
		rect.top -= camera->Rect()->top;
    }else if(resetCoords) {
        worldCoords = true;
        resetCoords = false;
    }

	sf::RectangleShape healthBar;
	healthBar.setPosition((float)rect.left, (float)rect.top);
	healthBar.setSize(sf::Vector2f((float)rect.width, (float)rect.height));
	healthBar.setFillColor(sf::Color::Red);
	window->draw(healthBar);
}

void RenderEngine::Display() {
	//HACK: TEST: Draw characters (for now)
	for(int i = renderables.size() - 1; i >= 0; --i) {
		Draw(renderables[i]->rect.left, renderables[i]->rect.top, renderables[i]->surface, renderables[i]->clipRect);
	}

    window->display();
	window->clear();
}
