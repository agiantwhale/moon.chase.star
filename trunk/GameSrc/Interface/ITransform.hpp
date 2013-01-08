#ifndef ITRANSFORM_HPP
#define ITRANSFORM_HPP

#include "../Base/Vec2D.hpp"

class ITransform
{
public:
    ITransform() : _position(0.0f,0.0f),
        _size(1.0f,1.0f),
        _rotation(0.0f)
    {
    }

    inline const Vec2D& GetPosition( void ) const
    {
        return _position;
    }
    inline const Vec2D& GetSize( void ) const
    {
        return _size;
    }
    inline float GetRotation( void ) const
    {
        return _rotation;
    }

    inline void  SetPosition(const Vec2D& position)
    {
        _position=position;
    }
    inline void  SetSize(const Vec2D& size)
    {
        _size=size;
    }
    inline void  SetRotation(float rotation)
    {
        _rotation=rotation;
    }

private:
    Vec2D _position;
    Vec2D _size;
    float _rotation;
};

#endif
