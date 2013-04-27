#include "Animation.h"
#include <iostream>
#include "RenderEngine.h"

Animation::Animation(int new_startFrame, int new_totalFrames, int new_delay, int new_flag) {
    startFrame = new_startFrame;
    currentFrame = new_startFrame;
    totalFrames = new_totalFrames;
    endFrame = startFrame + (totalFrames - 1);
    delay = new_delay;
    delayOld = Clock.getElapsedTime().asMilliseconds();
    flag = new_flag;
    flagChanged = false;
    animForward = true;
}

void Animation::Reset() {
    //flag = FREEZE;
    currentFrame = startFrame;
}

void Animation::SetFlag(AnimFlag new_Flag) {
    flag = new_Flag;
    flagChanged = true;
}

void Animation::Update() {
    int delayNew = Clock.getElapsedTime().asMilliseconds();

    if(flagChanged) {
        delayOld = delayNew;
        flagChanged = false;
    }

	//No animation
	if(flag == FREEZE) return;

	//Update if frame delay has elapsed
    if(delayNew - delayOld >= delay) {
        delayOld = delayNew;

        switch(flag) {
            //Loop Normally
            case LOOP:
                if(currentFrame < endFrame) {
                    currentFrame += 1;
                } else if(currentFrame == endFrame) {
                    currentFrame = startFrame;
                }
                break;
            //Oscillate
            case OSC:
                if(currentFrame > startFrame && currentFrame < endFrame) {
                    if(animForward == true) {
                        currentFrame += 1;
                    } else {
                        currentFrame -= 1;
                    }
                } else if(currentFrame == startFrame) {
                    currentFrame += 1;
                    animForward = true;
                } else if(currentFrame == endFrame) {
                    currentFrame -= 1;
                    animForward = false;
                }
                break;
        }
    }
}
