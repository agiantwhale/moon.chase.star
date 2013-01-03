#ifndef BODYWRAPPER_HPP
#define BODYWRAPPER_HPP

#include <string>
#include <unordered_map>

#include "../../Base/Callback.hpp"
#include "../../Interface/IPhysics.hpp"

struct FixtureInfo
{
    b2Fixture* fixture;
    b2FixtureDef fixtureDefinition;
};

class BodyWrapper : public IPhysics
{
public:
    BodyWrapper(Entity* const entity);
    BodyWrapper( const BodyWrapper& wrapper, Entity* const entity );
    virtual ~BodyWrapper();

    virtual void UpdateTransform( void );
    virtual void SmoothenTransform(float remainderRatio);
    virtual void ResetTransform( void );

    void CreateBody( const b2BodyDef& bodyDef );
    void CreateFixture( const b2FixtureDef& fixtureDef, const std::string& fixtureName );
    void DestroyBody( void );

    inline b2Body* GetBody(void) const
    {
        return _body;
    }
    b2Fixture* LookUpFixture( const std::string& fixtureName ) const;
	bool IsContactRelated(const b2Contact* contact, const b2Fixture*& target) const;

private:
    typedef std::unordered_map<std::string, FixtureInfo> FixtureMap;

    b2BodyDef _bodyDef;
    b2Body* _body;

    FixtureMap _fixtureMap;

    b2Vec2 _smoothPosition;
    b2Vec2 _previousPosition;
    float _smoothAngle;
    float _previousAngle;
};

#endif
