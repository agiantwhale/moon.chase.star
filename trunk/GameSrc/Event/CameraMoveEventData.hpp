#ifndef CAMERAMOVEEVENTDATA_HPP
#define CAMERAMOVEEVENTDATA_HPP

#include "../Event/EventData.hpp"
#include "../Base/Vec2D.hpp"

class CameraMoveEventData : public EventData
{
public:
    CameraMoveEventData(const Vec2D& deltaMove, const Vec2D& finalDest, bool isFinal) : EventData( Event_CameraMove ), _deltaMove( deltaMove ), _finalDest( finalDest ), _isFinal( isFinal ) {}

    //Gets the camera movement amount.
    inline const Vec2D& GetDeltaMovement(void) const
    {
        return _deltaMove;
    }
    //final desitnation for the camera movement. Set this value to true is isFinal == true.
    inline const Vec2D& GetDestination(void) const
    {
        return _finalDest;
    }
    //See is the movement is final.
    inline bool IsFinal(void) const
    {
        return _isFinal;
    }

private:
    const Vec2D _deltaMove;
    const Vec2D _finalDest;
    const bool  _isFinal;
};

#endif