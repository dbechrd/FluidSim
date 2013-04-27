#include "Camera.h"
#include "Character.h"
#include <iostream>

using namespace std;

Camera* Camera::pinstance = NULL;
Camera* Camera::Instance() {
    if (pinstance == NULL) {
        pinstance = new Camera();
    }
    return pinstance;
}

Camera::Camera() {
    charToFollow = NULL;
    speed = 4;
    mode = CAM_FREEZE;
}

void Camera::SetChar(Character* follow) {
    charToFollow = follow;
    speed = charToFollow->Speed();
}

void Camera::Update() {
    switch(mode) {
        case CAM_FREEZE:
            break;
        case CAM_FOLLOW_CHARACTER:
            if(charToFollow != NULL) {
                if(rect.top - speed >= (charToFollow->Rect().top + charToFollow->Rect().height/2) - rect.height/2) {
                    rect.top -= speed;
                }else if(rect.top + speed <= (charToFollow->Rect().top + charToFollow->Rect().height/2) - rect.height/2) {
                    rect.top += speed;
                }
                if(rect.left - speed >= (charToFollow->Rect().left + charToFollow->Rect().width/2) - rect.width/2) {
                    rect.left -= speed;
                }else if(rect.left + speed <= (charToFollow->Rect().left + charToFollow->Rect().width/2) - rect.width/2) {
                    rect.left += speed;
                }
            }
            break;
        case CAM_SCRIPT_DRIVEN:
            break;
    }
}
