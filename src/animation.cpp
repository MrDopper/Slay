#include "animation.h"
#include <iostream>
#include <stdexcept>

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
    if (frame == nullptr)
    {
        return nullptr;
    }
    return getFrame(getNextFrameNumber(frame->frameNumber));
}

// Returns the last frame's number
int Animation::getEndFrameNumber()
{
    return frames.size() > 0 ? frames.size() - 1 : 0;
}

// Get frame using frameNumber
Frame *Animation::getFrame(int frameNumber)
{
    if (frames.data())
    {
        if (frames.size() == 0 || frameNumber < 0 || frameNumber >= frames.size())
        {
            return nullptr;
        }

        // Point iterator to the first frame in the frames list
        auto i = frames.begin();

        // Advance iterator to the desired frame number
        std::advance(i, frameNumber);

        // Return a pointer to the frame
        return &(*i);
    }
    return nullptr;
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
