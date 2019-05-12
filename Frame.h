//
// Created by israel on 5/2/19.
//

#ifndef UNTITLED_FRAME_H
#define UNTITLED_FRAME_H


class Frame {
public:
    Frame();

    //returns time that this frame was last used
    int lastUsed() const;

    int getPid() const;

    int getPageNumber() const;

    void setTime(int time);

    void setFrame(int pid, int pageNum, int time);

    bool isEmpty();

    //removes everything from memory
    void empty();

private:
    int _lastUsed;
    int _pid;
    int _pageNumber;
};


#endif //UNTITLED_FRAME_H
