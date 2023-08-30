#ifndef FRAME_LINK_H
#define FRAME_LINK_H

class FrameLink
{
public:
	FrameLink()
	{
		this->next = 0;
		this->prev = 0;
	}

	virtual ~FrameLink()
	{

	}

	FrameLink* next;
	FrameLink* prev;
};

#endif
