#ifndef LEAP_HPP
#define LEAP_HPP

#include <Leap.h>
#include <set>

class SwipeListener : public Leap::Listener {
public:
	enum SwipeDirection
	{
		kSwipe_Left,
		kSwipe_Right,
		kSwipe_Forward,
		kSwipe_Backward,
		kSwipe_Up,
		kSwipe_Down
	};

	virtual void onFrame(const Leap::Controller&);

	const std::set<SwipeDirection>& GetSwipeStack() const { return _swipeStack; }

private:
	std::set<SwipeDirection> _swipeStack;
};

#endif