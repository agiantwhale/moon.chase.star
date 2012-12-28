#ifndef MATH_H
#define MATH_H

#include <SFML\System\Vector2.hpp>
#include <Box2D\Common\b2Math.h>

#include <math.h>

class Vec2D
{
public:
    float   x;
    float   y;

    Vec2D()
    {
        x = y = 0;
    }

    Vec2D( sf::Vector2f sfvec )
    {
        x = sfvec.x;
        y = sfvec.y;
    }

    Vec2D( b2Vec2 b2vec )
    {
        x = b2vec.x;
        y = b2vec.y;
    }

    Vec2D(float r, float s)
    {
        x = r;
        y = s;
    }

    operator sf::Vector2f( void )
    {
        return sf::Vector2f( x, y );
    }

    operator b2Vec2( void )
    {
        return b2Vec2( x, y );
    }

    Vec2D& Set(float r, float s)
    {
        x = r;
        y = s;
        return (*this);
    }

    float& operator [](long k)
    {
        return ((&x)[k]);
    }

    const float& operator [](long k) const
    {
        return ((&x)[k]);
    }

    Vec2D& operator +=(const Vec2D& v)
    {
        x += v.x;
        y += v.y;
        return (*this);
    }

    Vec2D& operator -=(const Vec2D& v)
    {
        x -= v.x;
        y -= v.y;
        return (*this);
    }

    Vec2D& operator *=(float t)
    {
        x *= t;
        y *= t;
        return (*this);
    }

    Vec2D& operator /=(float t)
    {
        float f = 1.0F / t;
        x *= f;
        y *= f;
        return (*this);
    }

    Vec2D& operator &=(const Vec2D& v)
    {
        x *= v.x;
        y *= v.y;
        return (*this);
    }

    Vec2D operator -(void) const
    {
        return (Vec2D(-x, -y));
    }

    Vec2D operator +(const Vec2D& v) const
    {
        return (Vec2D(x + v.x, y + v.y));
    }

    Vec2D operator -(const Vec2D& v) const
    {
        return (Vec2D(x - v.x, y - v.y));
    }

    Vec2D operator *(float t) const
    {
        return (Vec2D(x * t, y * t));
    }

    Vec2D operator /(float t) const
    {
        float f = 1.0F / t;
        return (Vec2D(x * f, y * f));
    }

    float operator *(const Vec2D& v) const
    {
        return (x * v.x + y * v.y);
    }

    Vec2D operator &(const Vec2D& v) const
    {
        return (Vec2D(x * v.x, y * v.y));
    }

    bool operator ==(const Vec2D& v) const
    {
        return ((x == v.x) && (y == v.y));
    }

    bool operator !=(const Vec2D& v) const
    {
        return ((x != v.x) || (y != v.y));
    }

    Vec2D& Normalize(void)
    {
        return (*this /= sqrtf(x * x + y * y));
    }

    Vec2D& Rotate(float angle);
};

inline Vec2D operator *(float t, const Vec2D& v)
{
    return (Vec2D(t * v.x, t * v.y));
}

inline float Dot(const Vec2D& v1, const Vec2D& v2)
{
    return (v1 * v2);
}

inline float Magnitude(const Vec2D& v)
{
    return (sqrtf(v.x * v.x + v.y * v.y));
}

inline float InverseMag(const Vec2D& v)
{
    return (1.0F / sqrtf(v.x * v.x + v.y * v.y));
}

inline float SquaredMag(const Vec2D& v)
{
    return (v.x * v.x + v.y * v.y);
}

#endif
