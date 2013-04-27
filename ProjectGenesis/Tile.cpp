#include <iostream>
#include "Tile.h"
#include "RenderEngine.h"
#include "Camera.h"

Tile::Tile(int spritesheetId, int nsprite, bool ncol) {
    renderer = RenderEngine::Instance();
	spritesheet = RenderEngine::Instance()->LoadSpritesheet(spritesheetId);
	animation = NULL;
	sprite = nsprite;
	col = ncol;

	if(sprite >= 0) {
		animation = spritesheet->Animation(nsprite);
		setTexture(*spritesheet->Texture());
		setTextureRect(*spritesheet->FrameRect(animation->frame));
	}

	//setPosition((float)x, (float)y);
}

void Tile::Update() {
    if(animation != NULL) {
		animation->Update();
		setTextureRect(*TextureRect());
    }
}

void Tile::Draw() {
    if(sprite < 0) return;

    sf::IntRect* tileSheetRect = NULL;

	if(this->spritesheet != NULL && this->animation != NULL) {
		tileSheetRect = this->spritesheet->FrameRect(this->animation->frame);
		if(tileSheetRect != NULL) {
			renderer->Draw((sf::Sprite)*this);
			//renderer->Draw((rect.left * rect.width), (rect.top * rect.height), tileSheet->GetSurface(), tileSheetRect);
		}
    }
}
