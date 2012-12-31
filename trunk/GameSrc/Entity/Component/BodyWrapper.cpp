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
}

BodyWrapper::BodyWrapper( const BodyWrapper& wrapper, Entity* const entity ) : IPhysics( entity )
{
    CreateBody( wrapper._bodyDef );

    for( FixtureMap::const_iterator iter = wrapper._fixtureMap.begin(); iter != wrapper._fixtureMap.end(); iter++ )
    {
        CreateFixture( iter->second.fixtureDefinition, iter->first );
    }

    this->_smoothPosition = wrapper._smoothPosition;
    this->_smoothAngle = wrapper._smoothAngle;
    this->_previousPosition = wrapper._previousPosition;
    this->_previousAngle = wrapper._previousAngle;
}

BodyWrapper::~BodyWrapper()
{
	GetEntity()->RemoveEventListenType(Event_Simulate);
	GetEntity()->RemoveEventListenType(Event_BeginContact);
	GetEntity()->RemoveEventListenType(Event_EndContact);

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
    GetEntity()->SetPosition(sf::Vector2f(_smoothPosition.x,_smoothPosition.y));
    GetEntity()->SetRotation(-_smoothAngle*RADTODEG);
}

void BodyWrapper::SmoothenTransform( float remainderRatio )
{
    const float oneMinusRatio = 1.0f - remainderRatio;

    _smoothPosition = remainderRatio * _body->GetPosition() + oneMinusRatio * _previousPosition;
    _smoothAngle = remainderRatio * _body->GetAngle() + oneMinusRatio * _previousAngle;
}

void BodyWrapper::ResetTransform( void )
{
    _smoothPosition = _previousPosition = _body->GetPosition();
    _smoothAngle = _previousAngle = _body->GetAngle();
}