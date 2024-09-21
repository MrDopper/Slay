#ifndef ANIMATION_H
#define ANIMATION_H

#include "frame.h"
using namespace std;

class Animation
{
public:
    // Name of animation
    string name;
    // list out of frames
    list<Frame> frames;

    Animation(string name = " ");
    // Return the next frame number in the list.
    int getNextFrameNumber(int frameNumber);
    Frame *getNextFrame(Frame *frame);
    // Returns the last frames number
    int getEndFrameNumber();
    // get frame using frameNumber
    Frame *getFrame(int frameNumber);
    void loadAnimation(ifstream &file, list<DataGroupType> &groupTypes);
};
#endif