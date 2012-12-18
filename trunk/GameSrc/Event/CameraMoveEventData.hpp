#ifndef CAMERAMOVEEVENTDATA_HPP
#define CAMERAMOVEEVENTDATA_HPP

#include "../Event/EventData.hpp"
#include "../Base/Vec2D.hpp"

class CameraMoveEventData : public EventData
{
public:
	CameraMoveEventData(float lerpTime, const Vec2D& finalDest, bool isFinal) : EventData( Event_CameraMove ), _lerpTime( lerpTime ), _finalDest( finalDest ), _isFinal( isFinal ) {}

	//Gets the camera movement velocity (m/s). Frametime and move scale should be multiplied to this and added to the camera location.
	inline float GetLerpTime(void) const { return _lerpTime; }
	//final desitnation for the camera movement. Set this value to true is isFinal == true.
	inline const Vec2D& GetDestination(void) const { return _finalDest; }
	//See is the movement is final.
	inline bool IsFinal(void) const { return _isFinal; }

private:
	const float _lerpTime;
	const Vec2D _finalDest;
	const bool  _isFinal;
};

#endif