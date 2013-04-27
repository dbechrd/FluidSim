#include "Spritesheet.h"
#include "RenderEngine.h"
#include "IAnimate.h"
#include <assert.h>
#include <iostream>

Spritesheet::Spritesheet(int nid, sf::Texture* ntexture, int nframes, int nframeWidth, int nframeHeight)
{
	assert(nframes <= 128);

	for(int i = 0; i < 128; ++i) {
		frameCollection[i] = NULL;
		animationCollection[i] = NULL;
	}

	id = nid;
	texture = ntexture;
	frameSize.x = nframeWidth;
	frameSize.y = nframeHeight;
	frames = nframes;
	animations = 0;

	sf::Vector2u size = texture->getSize();

	int i = 0;
	sf::IntRect* frameRect;
	for(int j = 0; j < ((int)size.y / frameSize.y); j++) {
        for(int k = 0; k < ((int)size.x / frameSize.x); k++) {
            frameRect = new sf::IntRect();
            frameRect->left = k * frameSize.x;
            frameRect->top = j * frameSize.y;
            frameRect->width = frameSize.x;
            frameRect->height = frameSize.y;
            frameCollection[i] = frameRect;
			if(++i == nframes) break;
        }
    }

	char* zErrMsg = NULL;
	char *zSQL = sqlite3_mprintf("SELECT * FROM Sprite WHERE SpritesheetId = %d", id);
	if(sqlite3_exec(RenderEngine::db, zSQL, LoadAnimationsCB, this, &zErrMsg) != SQLITE_OK) {
		printf("SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	sqlite3_free(zSQL);
}

Spritesheet::~Spritesheet()
{
	for(int i = 0; i < MAX_FRAMES; ++i) {
		delete frameCollection[i];
	}
	for(int i = 0; i < MAX_ANIMATIONS; ++i) {
		delete animationCollection[i];
	}
	//RenderEngine::Instance()->UnloadSpritesheet(id);
}

int Spritesheet::LoadAnimationsCB(void *instance, int argc, char **argv, char **azColName) {
	Spritesheet* me = ((Spritesheet*)instance);

	int i;
	for(i=0; i<argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
		std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << "\n";
	}
	printf("\n");
	std::cout << "\n";
	
	int id = atoi(argv[1]);
	int startFrame = atoi(argv[2]);
	int frames = atoi(argv[3]);
	int delay = atoi(argv[4]);
	AnimFlag flag = (AnimFlag)atoi(argv[5]);

	assert(id < MAX_ANIMATIONS);

	me->animationCollection[id] = new AnimationStruct(startFrame, frames, delay, flag);
	me->animations++;

	return SQLITE_OK;
}

AnimationStruct* Spritesheet::Animation(int id) {
	assert(id < animations);
	return (id >= 0) ? animationCollection[id] : NULL;
}