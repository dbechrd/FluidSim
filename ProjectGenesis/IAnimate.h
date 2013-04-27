#ifndef IANIMATE_H_INCLUDED
#define IANIMATE_H_INCLUDED

//#include "GameEngine.h"
//#include "IRender.h"
#include "RenderEngine.h"

enum AnimFlag {
	FREEZE = 0,
	LOOP = 1,
	OSCILLATE = 2
};

//TODO: Rename to Animation after Animation.cpp class dependendancy removed
class AnimationStruct {
public:
    int startFrame;
	int endFrame;
	int frames;
    int frame;
    int delay;
    int lastUpdate;
    AnimFlag flag;
    bool flagChanged;
    bool oscForward;

	AnimationStruct(int nstartFrame, int nframes, int ndelay, AnimFlag nflag)
		: startFrame(nstartFrame),
		endFrame(nstartFrame + (nframes - 1)),
		frames(nframes),
		frame(nstartFrame),
		delay(ndelay),
		flag(nflag),
		flagChanged(false),
		oscForward(true)
	{
		lastUpdate = Clock.getElapsedTime().asMilliseconds();
	}

	void AnimationStruct::Reset() {
		frame = startFrame;
	}

	void AnimationStruct::SetFlag(AnimFlag nFlag) {
		flag = nFlag;
		flagChanged = true;
	}

	void AnimationStruct::Update() {
		//No animation
		if(flag == FREEZE) return;

		int tick = Clock.getElapsedTime().asMilliseconds();

		//Update if frame delay elapses or animation flag changes
		if(flagChanged || tick - lastUpdate >= delay) {
			flagChanged = false;
			lastUpdate = tick;

			switch(flag) {
				case LOOP:
					if(frame < startFrame + (frames - 1)) {
						frame++;
					} else if(frame == endFrame) {
						frame = startFrame;
					}
					break;
				case OSCILLATE:
					if(frame == startFrame || frame == endFrame) {
						oscForward = (frame == startFrame);
					}
					frame += (oscForward) ? 1 : -1;
					break;
			}
		}
	}
};

#include "Spritesheet.h"

class IAnimate {
protected:
	Spritesheet* spritesheet; //Point to spritesheet (possibly shared)
    AnimationStruct* animation; //Point to current animation (of many)

public:
	IAnimate() {
		//GameEngine::Instance()->animator->AddAnimation(this);
	}
	sf::IntRect* TextureRect(){ return spritesheet->FrameRect(animation->frame); }
};

#endif // IANIMATE_H_INCLUDED