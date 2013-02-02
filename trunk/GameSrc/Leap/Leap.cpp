#include "../Leap/Leap.hpp"

void SwipeListener::onFrame(const Leap::Controller& controller) {
	Leap::Frame frame = controller.frame();
	Leap::Hand hand = frame.hands()[0];

	Leap::FingerList fingers = hand.fingers();
	if(!fingers.empty())
	{
		Leap::Vector avgPos = Leap::Vector::zero();
		Leap::Vector avgVelocity = Leap::Vector::zero();

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