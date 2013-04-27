#include "Character.h"
#include "Animation.h"
#include "RenderEngine.h"
#include "TextEngine.h"
#include "GameEngine.h"
#include "Object.h"
#include "Level.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

std::vector<Character*> Character::characters;

Character::Character(int uid, int new_x, int new_y) {
	healthTotal = 100;
	health = 100;
    UID = uid;

    objColMain = NULL;
    for(int i = 0; i < 4; ++i) {
        objCol[i] = NULL;
    }
    charCol = NULL;

    renderer = RenderEngine::Instance();

    ////////////////////////////////////////////////////////////////////////////////////
    //Load character data

	//TODO: Factor out this call to SQLiteExec(char* zSQL, Callback, this)
	char* zErrMsg = NULL;
	char *zSQL = sqlite3_mprintf("SELECT * FROM Character WHERE Id = %d", UID);
	if(sqlite3_exec(RenderEngine::db, zSQL, LoadCharacterCB, this, &zErrMsg) != SQLITE_OK) {
		printf("SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	sqlite3_free(zSQL);
    ////////////////////////////////////////////////////////////////////////////////////

	facing = DIR_DOWN;
    walking = false;
	attacking = false;
	special = SPECIAL_NONE;

	setPosition((float)new_x, (float)new_y);

    tileWidth = GameEngine::Instance()->GetLevel()->TileWidth();
    tileHeight = GameEngine::Instance()->GetLevel()->TileHeight();

    colliding = false;

    characters.push_back(this);
    std::cout << "Character Created\n";
}

Character::~Character() {

}

int Character::LoadCharacterCB(void *instance, int argc, char **argv, char **azColName) {
	Character* me = ((Character*)instance);

	int i;
	for(i=0; i<argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
		std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << "\n";
	}
	printf("\n");
	std::cout << "\n";
	
	int spritesheetId = atoi(argv[1]);
	int name = atoi(argv[2]);
	int NPC = atoi(argv[3]);
	int walkSpeed = atoi(argv[4]);

	me->spritesheet = me->renderer->LoadSpritesheet(spritesheetId);
	me->animation = me->spritesheet->Animation(1);
    me->setTexture(*me->spritesheet->Texture());
	me->setTextureRect(*me->TextureRect());
	me->name = name;
	me->NPC = (NPC != 0);
	me->walkSpeed = walkSpeed;

	return SQLITE_OK;
}

/*
	WITHOUT ATTACKING:
	face up
	face down
	face left
	face right
	walk up
	walk down
	walk left
	walk right
	slide up
	slide down
	slide left
	slide right
	roll up
	roll down
	roll left
	roll right
	//All the above in same order with attacking
*/

void Character::Walk(Direction dir, Special spec) {
	special = spec;
	int newSprite = dir + (special * 4) + ((int)attacking * 12);

	//Start new animation
	if(animation != spritesheet->Animation(newSprite)) {
		animation->Reset();
		animation = spritesheet->Animation(newSprite);
		setTextureRect(*TextureRect());
	}

	facing = dir;
    walking = true;
}

void Character::Stop() {
	animation->Reset();

    walking = false;
	attacking = false;
	special = SPECIAL_NONE;

	animation = spritesheet->Animation(facing);
	setTextureRect(*TextureRect());
}

void Character::Action() {
    //Inspect object at current location
}

bool Character::Collision() {
    bool collided = false;

	//Reset stored collision info
    objColMain = NULL;
    charCol = NULL;

	sf::IntRect colRect = ColRect();

	//Store position
	int x = colRect.left;
	int y = colRect.top;

	//Add velocity to position
	if(facing == DIR_UP || facing == DIR_DOWN)
		y += (facing == DIR_DOWN) ? walkSpeed : -walkSpeed;
	else
		x += (facing == DIR_RIGHT) ? walkSpeed : -walkSpeed;

	//Check edge-of-screen collision
	//TODO: disallow walking off screen by placing walls in levels
	 sf::IntRect* levelRect = GameEngine::Instance()->GetLevel()->LevelRect();
	 if(y < 0 || y + colRect.height > levelRect->height
	 || x < 0 || x + colRect.width > levelRect->width) {
		 return true;
	}

	//Iterate characters
    if(!characters.empty()) {
        for(int j = 0; j < (int)characters.size(); j++) {
            //Ignore null and self
            if(characters[j] == NULL || characters[j]->GetUID() == UID) continue;
			
			//Collision check
			sf::IntRect other = characters[j]->ColRect(); //TODO: Stop creating new rects n times per frame!!  Cache this shit.
			if(y < other.top + other.height && y + colRect.height > other.top
			&& x < other.left + other.width && x + colRect.width > other.left) {
				//Store collision info
				collided = true;
				charCol = characters[j];
				break; //Only collide with one character at a time
			}
        }
    }

	//|-----|-----|
	//| 0 ##### 1 | If character size equals tile size the most
	//|---#-|-#---| tiles a character can collide with at one
	//| 2 ##### 3 | time is four.
	//|-----|-----|
	
	//Overlap
	bool xOverlap = (x % tileWidth + colRect.width > tileWidth);
	bool yOverlap = (y % tileHeight + colRect.height > tileHeight);

	//Convert to index coords
	x /= tileWidth;
	y /= tileHeight;

	int tileX = x * tileWidth;
	int tileY = y * tileHeight;

	//Check rects
	sf::IntRect checkRects[4];
	checkRects[0] = sf::IntRect(tileX, tileY, tileWidth, tileHeight);
	checkRects[1] = sf::IntRect(tileX + tileWidth, tileY, tileWidth, tileHeight);
	checkRects[2] = sf::IntRect(tileX, tileY + tileHeight, tileWidth, tileHeight);
	checkRects[3] = sf::IntRect(tileX + tileWidth, tileY + tileHeight, tileWidth, tileHeight);

	//Store tile collision
	bool tileCol[4];
	tileCol[0] = GameEngine::Instance()->GetLevel()->IsCollision(x, y);
	tileCol[1] = xOverlap && GameEngine::Instance()->GetLevel()->IsCollision(x + 1, y);
	tileCol[2] = yOverlap && GameEngine::Instance()->GetLevel()->IsCollision(x, y + 1);
	tileCol[3] = xOverlap && yOverlap && GameEngine::Instance()->GetLevel()->IsCollision(x + 1, y + 1);

	//Store objects collided with
	objCol[0] = GameEngine::Instance()->GetLevel()->GetObject(ColRect());
	objCol[1] = NULL;
	objCol[2] = NULL;
	objCol[3] = NULL;
	//objCol[1] = xOverlap ? GameEngine::Instance()->GetLevel()->GetObject(x + 1, y) : NULL;
	//objCol[2] = yOverlap ? GameEngine::Instance()->GetLevel()->GetObject(x, y + 1) : NULL;
	//objCol[3] = (xOverlap && yOverlap) ? GameEngine::Instance()->GetLevel()->GetObject(x + 1, y + 1) : NULL;

	//Major collision with primary object or secondary object?
	int mainIndex;
	if(facing == DIR_UP || facing == DIR_DOWN) {
		mainIndex = (colRect.left % tileWidth <= (tileWidth / 2)) ? 0 : 1;
		if(facing == DIR_DOWN) mainIndex += 2;
	} else {
		mainIndex = (colRect.top % tileHeight <= (tileHeight / 2)) ? 0 : 2;
		if(facing == DIR_RIGHT) mainIndex += 1;
	}

	//objColMain = objCol[mainIndex];
	objColMain = objCol[0];

	for(int i = 0; i < 4; i++) {
		//No tile / object collision
		if(tileCol[i]) {
			collided = true;
			break;
		}

		if(objCol[i] != NULL && objCol[i]->GetCollide()) {
			sf::IntRect oRect = objCol[i]->ColRect();
			sf::IntRect cRect = ColRect();
			int dx = (oRect.left + (oRect.width / 2)) - (cRect.left + (cRect.width / 2));
			int dy = (oRect.top + (oRect.height / 2)) - (cRect.top + (cRect.height / 2));

			int nx = (oRect.width / 2 + cRect.width / 2);
			int ny = (oRect.height / 2 + cRect.height / 2);

			dx -= (dx < 0) ? -nx : nx;
			dy -= (dy < 0) ? -ny : ny;

			if(abs(dx) < abs(dy)) {
				collided = ((dx < 0 && facing == DIR_RIGHT) || (dx > 0 && facing == DIR_LEFT));
			} else {
				collided = ((dy < 0 && facing == DIR_DOWN) || (dy > 0 && facing == DIR_UP));
			}
			break;
		}
	}
	return collided;
}

void Character::Update() {
    colliding = Collision();

    //Update Player Position
    if(walking) {
        if(colliding) {
            Stop();
        } else {
			switch(facing) {
                case DIR_UP:
                    move(0, (float)-walkSpeed);
                    break;
                case DIR_DOWN:
                    move(0, (float)walkSpeed);
                    break;
                case DIR_LEFT:
                    move((float)-walkSpeed, 0);
                    break;
                case DIR_RIGHT:
                    move((float)walkSpeed, 0);
                    break;
            }
        }
    }
    animation->Update();
	setTextureRect(*TextureRect());
}

void Character::Render() {
	//Draw me
	renderer->Draw((sf::Sprite)*this);

	//Health bar
	//TODO: Save healthbar Rect and just adjust width (inherit IFight?)
	sf::Vector2f pos = getPosition();
	renderer->Draw(sf::FloatRect(pos.x + (Rect().width / 2) - (HEALTHBAR_W / 2), pos.y - HEALTHBAR_H, (HEALTHBAR_W * ((float)health / (float)healthTotal)), HEALTHBAR_H), sf::Color::Red);

	//World Coords textbox
	int mapX = (int)(pos.x + Rect().width/2) / tileWidth;
    int mapY = (int)(pos.y + Rect().height/2) / tileHeight;
    std::stringstream ssout;
    ssout << mapX << "," << mapY << std::endl;
	ssout << health << "/" << healthTotal;
    TextEngine::Instance()->DebugString(ssout.str(), (int)pos.x - 20, (int)pos.y - 36, false);

	//DEBUG: Write collision info to top left corner
    if(objColMain != NULL) {
		ssout.str(std::string());
        ssout << "ObjId: " << objColMain->GetId();
        TextEngine::Instance()->DebugString(ssout.str(), 0, 0);
		ssout.str(std::string());
		ssout << "ObjName: " << objColMain->GetName();
        TextEngine::Instance()->DebugString(ssout.str(), 0, 20);
    }
}