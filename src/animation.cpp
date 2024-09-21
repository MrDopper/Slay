#include "animation.h"

Animation::Animation(string name)
{
    this->name = name;
}
// Return the next frame number in the list.
int Animation::getNextFrameNumber(int frameNumber)
{
    if (frameNumber + 1 < frames.size())
    {
        return frameNumber + 1;
    }
    else
    {
        return 0;
    }
}
Frame *Animation::getNextFrame(Frame *frame)
{
    return getFrame(getNextFrameNumber(frame->frameNumber));
}
// Returns the last frames number
int Animation::getEndFrameNumber()
{
    return frames.size() - 1;
}
// get frame using frameNumber
Frame *Animation::getFrame(int frameNumber)
{
    if (frames.size() == 0)
    {
        return NULL;
    }
    // point interator to first frame in the frames list
    list<Frame>::iterator i = frames.begin();

    int counter = 0;

    for (counter = 0; counter < frameNumber && counter < frames.size() - 1; counter++)
    {
        // Make interator point to the next frame in the list
        i++;
    }
    // Make frame point to the frame inside of the list our interator in position at
    //*i = gets frame at i's position in the list (de-referencing )
    //& = get whatever memory address of whatever is on the right.(referencing)
    Frame *frame = &(*i);
    return frame;
}
void Animation::loadAnimation(ifstream &file, list<DataGroupType> &groupTypes)
{
    getline(file, name);
    string buffer;
    getline(file, buffer);
    stringstream ss;
    buffer = Globals::clipOffDataHeader(buffer);
    ss << buffer;
    int numberOfFrames;
    ss >> numberOfFrames;
    for (int i = 0; i < numberOfFrames; i++)
    {
        Frame newFrame;
        newFrame.loadFrame(file, groupTypes);
        frames.push_back(newFrame);
    }
}