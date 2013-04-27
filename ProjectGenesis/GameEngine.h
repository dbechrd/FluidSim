#ifndef GAMEENGINE_H_INCLUDED
#define GAMEENGINE_H_INCLUDED

#include <string>
#include <map>
#include "Level.h"
#include "RenderEngine.h"
#include "TextEngine.h"

extern bool KeyPressed(sf::Keyboard::Key);

class GameEngine{
private:
    RenderEngine* renderer;
    TextEngine* text;

    //Termination state
    static bool quit;

    //Singleton instance pointer
    static GameEngine* pinstance;

    //Engine events
    sf::Event event;

    //Mouse coords
    int mousex;
    int mousey;

    //Mouse button states
    bool mouseLeft;
    bool mouseRight;

    //Key states
    std::map<std::string, bool> keys;
    std::map<std::string, bool>::iterator iter;

    //Current level class object
    Level* level;

    //Configuration file
    std::string configfile;

protected:
    GameEngine(const std::string &filename);
    GameEngine(const GameEngine&);
    GameEngine& operator= (const GameEngine&);

public:
	friend IRender;

    static void SetQuit();
    static bool Quit();

    static GameEngine* Instance(const std::string &filename = "");
    ~GameEngine();

    void HandleInput();
    bool KeyDown(std::string key);
    bool KeyUp(std::string key);
    int Collision();
    int Sleep(int duration);
    void Update();
	void Display();
    Level* LoadLevel(std::string mapname);
    Level* GetLevel();
};

#endif // GAMEENGINE_H_INCLUDED
