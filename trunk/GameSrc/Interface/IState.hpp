#ifndef ISTATE_HPP
#define ISTATE_HPP

#include "../State/StatesDef.h"

class IState
{
public:
    IState();
    virtual ~IState();

    virtual void Enter( void );
    //Return true when stop updating and continue.
    virtual bool Update( float deltaTime ) = 0;
    virtual void Render( void ) = 0;
    virtual void Exit( void );

protected:
    const float GetEnterTime(void) const
    {
        return _enterTime;
    }
    const float GetExitTime(void) const
    {
        return _exitTime;
    }

private:
    float _enterTime;
    float _exitTime;
};

#endif