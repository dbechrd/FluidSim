#include "Object.h"
#include "ObjectEngine.h"
#include <iostream>

Object::Object(ObjectData* nobjData, sf::Vector2f position) {
    objData = nobjData;
	spritesheet = NULL;
	animation = NULL;

	if(objData->spriteId >= 0) {
		this->spritesheet = RenderEngine::Instance()->LoadSpritesheet(objData->spritesheetId);
		this->animation = this->spritesheet->Animation(objData->spriteId);
		this->setTexture(*this->spritesheet->Texture());
		this->setTextureRect(*this->TextureRect());
	}

	this->setPosition(position);

    std::cout << "Object Created: " << objData->name << std::endl;
}

std::string Object::GetName() {
    return objData->name;
}

void Object::SetName(std::string newname) {
    objData->name = newname;
}

int Object::GetId() {
    return objData->id;
}

void Object::SetId(int objId) {
    objData->id = objId;
}

bool Object::GetCollide() {
    return objData->collide;
}

void Object::SetCollide(bool col) {
    objData->collide = col;
}

int Object::GetFlags() {
    return objData->flags;
}

void Object::SetFlags(int newflags) {
    objData->flags = newflags;
}

void Object::Update() {
	if(this->animation != NULL) {
        this->animation->Update();
    }
}

void Object::Draw() {
	if(this->spritesheet != NULL) {
		if(this->animation != NULL) {
			this->spritesheet->FrameRect(this->animation->frame);
		}

        RenderEngine::Instance()->Draw((sf::Sprite)*this);
        //Cleanup: RenderEngine::Instance()->Draw((rect.left * rect.width), (rect.top * rect.height), tileSheetSurface, tileSheetRect);
    }
}
