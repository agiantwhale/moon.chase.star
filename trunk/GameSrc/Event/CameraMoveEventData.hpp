#ifndef CAMERAMOVEEVENTDATA_HPP
#define CAMERAMOVEEVENTDATA_HPP

#include "../Event/EventData.hpp"
#include "../Base/Vec2D.hpp"

class CameraMoveEventData : public EventData
{
public:
	CameraMoveEventData(const Vec2D& deltaAmount, bool isFinal) {}

private:
	Vec2D _deltaAmount;
};

#endif