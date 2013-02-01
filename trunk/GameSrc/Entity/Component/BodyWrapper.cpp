#include "../../Entity/Component/BodyWrapper.hpp"
#include "../../System/PhysicsManager.hpp"
#include "../../System/EventManager.hpp"
#include "../../Entity/Entity.hpp"
#include "../../App/Game.hpp"

BodyWrapper::BodyWrapper(Entity* const entity) : IPhysics(entity), _body(nullptr),
    _smoothPosition(0.0f,0.0f),
    _previousPosition(0.0f,0.0f),
    _smoothAngle(0.0f),
    _previousAngle(0.0f)
{
	GetEntity()->AddEventListenType(Event_Simulate);
	GetEntity()->AddEventListenType(Event_BeginContact);
	GetEntity()->AddEventListenType(Event_EndContact);
	GetEntity()->AddEventListenType(Event_PreSolve);
	GetEntity()->AddEventListenType(Event_PostSolve);
}

BodyWrapper::~BodyWrapper()
{
	GetEntity()->RemoveEventListenType(Event_Simulate);
	GetEntity()->RemoveEventListenType(Event_BeginContact);
	GetEntity()->RemoveEventListenType(Event_EndContact);
	GetEntity()->RemoveEventListenType(Event_PreSolve);
	GetEntity()->RemoveEventListenType(Event_PostSolve);

    DestroyBody();
}

void BodyWrapper::CreateBody( const b2BodyDef& bodyDef )
{
    if( _body )
    {
        DestroyBody();
    }

    _body = PhysicsManager::GetInstance()->GetWorld()->CreateBody( &bodyDef );
    _body->SetUserData( static_cast<IPhysics*>(this) );
}

void BodyWrapper::CreateFixture( const b2FixtureDef& fixtureDef, const std::string& fixtureName )
{
    if( _body )
    {
        b2Fixture* newFixture = _body->CreateFixture( &fixtureDef );

        FixtureInfo info;
        info.fixture = newFixture;
        info.fixtureDefinition = fixtureDef;

        _fixtureMap.insert( std::make_pair( fixtureName, info ) );
    }
}

b2Fixture* BodyWrapper::LookUpFixture( const std::string& fixtureName ) const
{
    FixtureMap::const_iterator iter = _fixtureMap.find( fixtureName );
    if( iter != _fixtureMap.end() )
    {
        return iter->second.fixture;
    }
    else
    {
		TRI_LOG_STR("Unable to find fixture.");
		TRI_LOG(fixtureName);
    }

    return nullptr;
}

void BodyWrapper::DestroyBody( void )
{
    if(_body)
    {
        PhysicsManager::GetInstance()->GetWorld()->DestroyBody(_body);
    }

	_body = nullptr;
}

void BodyWrapper::UpdateTransform( void )
{
	if(_body->IsActive())
	{
		GetEntity()->SetPosition(sf::Vector2f(_smoothPosition.x,_smoothPosition.y));
		GetEntity()->SetRotation(-_smoothAngle*RADTODEG);
	}
}

void BodyWrapper::SmoothenTransform( float remainderRatio )
{
    const float oneMinusRatio = 1.0f - remainderRatio;

    _smoothPosition = remainderRatio * _body->GetPosition() + oneMinusRatio * _previousPosition;
    _smoothAngle = remainderRatio * _body->GetAngle() + oneMinusRatio * _previousAngle;
}

void BodyWrapper::ResetTransform( void )
{
	if(_body)
	{
		_smoothPosition = _previousPosition = _body->GetPosition();
		_smoothAngle = _previousAngle = _body->GetAngle();
	}
}

bool BodyWrapper::IsContactRelated( const b2Contact* contact, const b2Fixture*& target ) const
{
	target = nullptr;
	
	if( contact->GetFixtureA()->GetBody()->GetUserData() == this )
	{
		target = contact->GetFixtureB();
		return true;
	}

	if( contact->GetFixtureB()->GetBody()->GetUserData() == this )
	{
		target = contact->GetFixtureA();
		return true;
	}

	return false;
}
