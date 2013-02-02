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

void SwipeListener::onFrame(const Leap::Controller& controller) {
	Leap::Frame frame = controller.frame();
	Leap::Hand hand = frame.hands()[0];

	Leap::FingerList fingers = hand.fingers();
	if(!fingers.empty())
	{
		Leap::Vector avgPos = Vector::zero();
		Leap::Vector avgVelocity = Vector::zero();

		for(Leap::FingerList::const_iterator iter = fingers.begin(); iter != fingers.end(); iter++)
		{
			avgPos += (*iter).tipPosition();
			avgVelocity += (*iter).tipVelocity();
		}

		avgPos /= fingers.count();
		avgVelocity /= fingers.count();

		_swipeStack.clear();

		if (avgVelocity.y > 1500)
		{
			_swipeStack.insert(kSwipe_Up);
		}
		else if (avgVelocity.y < -1500)
		{
			_swipeStack.insert(kSwipe_Down);
		}

		if (avgVelocity.x < 1500)
		{
			_swipeStack.insert(kSwipe_Left);
		}
		else if (avgVelocity.x > -1500)
		{
			_swipeStack.insert(kSwipe_Right);
		}


		if (avgVelocity.z > 1500)
		{
			_swipeStack.insert(kSwipe_Forward);
		}
		else if (avgVelocity.z < -1500)
		{
			_swipeStack.insert(kSwipe_Backward);
		}
	}
}

#endif