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

    virtual void Simulate( void );
	virtual void UpdateTransform( void );
	virtual void SmoothenTransform(float remainderRatio);
	virtual void ResetTransform( void );
	virtual void BeginContact(b2Contact* contact, const b2Fixture* contactFixture );
	virtual void EndContact(b2Contact* contact, const b2Fixture* contactFixture );

	void CreateBody( const b2BodyDef& bodyDef );
	void CreateFixture( const b2FixtureDef& fixtureDef, const std::string& fixtureName );
	void DestroyBody( void );

	inline b2Body* GetBody(void) const { return _body; }
	b2Fixture* LookUpFixture( const std::string& fixtureName ) const;

	inline void SetSimulateCallback( Callback<void (void)> callback ) { _simulateCallbackSet = true; _simulateCallback = callback; }
	inline void SetBeginConactCallback( Callback<void (b2Contact*,const b2Fixture*)> callback ) { _beginContactCallbackSet = true; _beginContactCallback = callback; }
	inline void SetEndConactCallback( Callback<void (b2Contact*,const b2Fixture*)> callback ) { _endContactCallbackSet = true; _endContactCallback = callback; }

private:
	typedef std::unordered_map<std::string, FixtureInfo> FixtureMap;

	b2BodyDef _bodyDef;
    b2Body* _body;

	FixtureMap _fixtureMap;

    b2Vec2 _smoothPosition;
	b2Vec2 _previousPosition;
	float _smoothAngle;
	float _previousAngle;

	bool _simulateCallbackSet;
	Callback<void (void)> _simulateCallback;

	bool _beginContactCallbackSet;
	Callback<void (b2Contact*,const b2Fixture*)> _beginContactCallback;

	bool _endContactCallbackSet;
	Callback<void (b2Contact*,const b2Fixture*)> _endContactCallback;
};

#endif
