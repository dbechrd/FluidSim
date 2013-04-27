#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include "SFML/Graphics.hpp"

class Character;

enum cameramodes {
	CAM_FREEZE = 0,
	CAM_FOLLOW_CHARACTER = 1,
	CAM_SCRIPT_DRIVEN = 2
};

class Camera {
private:
    static Camera* pinstance;
    sf::IntRect rect;
    int speed;
    int mode;
    Character* charToFollow;
protected:
    Camera();
    Camera(const Camera&);
    Camera& operator= (const Camera&);
public:
    static Camera* Instance();

	int X() { return rect.left; }
    void SetX(int newx) { rect.left = newx; }

    int Y() { return rect.top; }
    void SetY(int newy) { rect.top = newy; }

    int W() { return rect.width; }
    void SetW(int neww) { rect.width = neww; }

    int H() { return rect.height; }
    void SetH(int newh) { rect.height = newh; }

    void SetPos(int newx, int newy) { rect.left = newx; rect.top = newy; }
    void SetSize(int neww, int newh) { rect.width = neww; rect.height = newh; }

    sf::IntRect* Rect() { return &rect; }
    void SetRect(sf::IntRect* newrect = NULL) { rect.left = newrect->left; rect.top = newrect->top; rect.width = newrect->width; rect.height = newrect->height; }

    int Speed() { return speed; }
    void SetSpeed(int newspeed) { speed = newspeed; }

    void SetChar(Character* follow = NULL);

    int Mode() { return mode; }
    void SetMode(int newmode) { mode = newmode; }

    void Update();
};

#endif // CAMERA_H_INCLUDED
