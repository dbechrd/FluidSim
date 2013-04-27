#ifndef ANIMATION_H_INCLUDED
#define ANIMATION_H_INCLUDED

class Animation {
public:
	enum AnimFlag {
		FREEZE = 0,
		LOOP = 1,
		OSC = 2
	};

private:
    int startFrame;
    int currentFrame;
    int totalFrames;
    int endFrame;
    int delay;
    int delayOld;
    int flag;
    bool flagChanged;
    bool animForward;

public:
    Animation(int new_startFrame, int new_totalFrames, int new_delay, int new_flag);

    int StartFrame() { return startFrame; }
    int TotalFrames() { return totalFrames; }
    int CurrentFrame() { return currentFrame; }
    void Reset();
    int Delay() { return delay; }
    void SetDelay(int new_delay) { delay = new_delay; }
    int Flag() { return flag; }
    void SetFlag(AnimFlag new_Flag);
    void Update();
};

#endif // ANIMATION_H_INCLUDED
