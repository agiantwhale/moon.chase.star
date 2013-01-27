#ifndef IRENDERABLE_HPP
#define IRENDERABLE_HPP

#include "../Base/Globals.hpp"
#include "../Interface/ITransform.hpp"

class IRenderable
{
public:
    IRenderable(const ITransform* transform);
    virtual ~IRenderable();

    virtual void Render() = 0;
    inline void Hide(void)
    {
        _hidden=true;
    }
    inline void Show(void)
    {
        _hidden=false;
    }
    inline bool IsHidden(void) const
    {
        return _hidden;
    }
    inline bool IsRegistered(void) const
    {
        return _registered;
    }

    void RegisterRenderable( unsigned int renderLayer );
    void UnregisterRenderable( void );

    inline const ITransform* GetTransform(void) const
    {
        return _transform;
    }
    inline unsigned int GetRenderLayer(void) const
    {
        return _renderLayer;
    }

private:
    unsigned int _renderLayer;
    bool _registered;
    bool _hidden;
    const ITransform* _transform;
};

inline const Vec2D WorldToScreen( const Vec2D& world )
{
    Vec2D screen( SCREENWIDTH/2, SCREENHEIGHT/2 );
    screen.x += world.x * RATIO;
    screen.y -= world.y * RATIO;
    return screen;
}

#endif
