#ifndef IRENDER_H_INCLUDED
#define IRENDER_H_INCLUDED

#include "GameEngine.h"
#include "RenderEngine.h"

class IRender {
	friend RenderEngine;
protected:
	//Set by subclass (Character/Tile/Object)
	sf::Texture* surface;
	sf::IntRect rect;

	//Set by subclass (IAnimate)
	sf::IntRect* clipRect;

public:
	IRender() {
		GameEngine::Instance()->renderer->AddRenderable(this);
	}
	~IRender() {
		GameEngine::Instance()->renderer->RemoveRenderable(this);
	}
};

#endif // IRENDER_H_INCLUDED