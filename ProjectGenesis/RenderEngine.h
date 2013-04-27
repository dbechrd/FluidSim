#ifndef RENDER_H_INCLUDED
#define RENDER_H_INCLUDED

#include <vector>
#include <map>
#include <string>

#include "sqlite3.h"
#include "SFML/Graphics.hpp"
#include "Spritesheet.h"

#define HEALTHBAR_W 30
#define HEALTHBAR_H 4

#define MAX_SPRITESHEETS 8

class Spritesheet;
class IRender;
class Camera;
extern sf::Clock Clock;

class RenderEngine{
private:
	struct TextureReference {
		sf::Texture* texture;
		int refCount;

		~TextureReference() { delete texture; }
	};

	struct SpritesheetReference {
		Spritesheet* spritesheet;
		int refCount;

		~SpritesheetReference() { delete spritesheet; }
	};

    static RenderEngine* pinstance;
	sf::RenderWindow* window;

    bool fullScreen;
    bool worldCoords;
    bool resetCoords;

	std::vector<IRender*> renderables;
	std::vector<IRender*>::iterator renderablesIter;
    std::map<std::string, TextureReference*> surfaces;
    std::map<std::string, TextureReference*>::iterator surfacesIter;

	int spritesheets;
	SpritesheetReference* spritesheetCollection[MAX_SPRITESHEETS];

    Camera* camera;

	static int LoadSpritesheetCB(void *instance, int argc, char **argv, char **azColName);

protected:
    RenderEngine();
    RenderEngine(const RenderEngine&);
    RenderEngine& operator= (const RenderEngine&);

	void AddRenderable(IRender* renderable);
	void RemoveRenderable(IRender* renderable);

public:
	friend IRender;

	static const int FPS = 60;
	static const int SCREEN_WIDTH = 1280;
	static const int SCREEN_HEIGHT = 800;
	static const bool START_FULLSCREEN = false;

	static sqlite3* db;

    static RenderEngine* Instance();
    ~RenderEngine();

    void ToggleFullScreen();

	sf::RenderWindow* Window() { return window; }
    sf::Texture* LoadSurface(std::string filename);
    void UnloadSurface(std::string filename);
    sf::Vector2u GetSurfaceRect(std::string filename);

	Spritesheet* LoadSpritesheet(int spritesheetId);
	void UnloadSpritesheet(int spritesheetId);

    void EnableWorldCoords();
    void DisableWorldCoords(bool permanant = false);

	void Draw(sf::Sprite sprite);
	void Draw(int x, int y, sf::Sprite sprite);
    void Draw(int x, int y, sf::Texture* texture, sf::IntRect* clip = NULL);
	void Draw(sf::Text text);
	void Draw(sf::FloatRect rect, sf::Color color);
    void Display();
};

#endif // RENDER_H_INCLUDED
