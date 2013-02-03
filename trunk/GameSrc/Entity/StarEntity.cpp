#include <Thor/Graphics.hpp>
#include <Thor/Animation.hpp>
#include <Thor/Math.hpp>
#include "../System/ResourceCache.hpp"
#include "../Entity/StarEntity.hpp"
#include "../Task/Task.hpp"

REGISTER_ENTITY(StarEntity,"Star")

const float STAR_RADIUS = 0.5f;
const float EMISSION_RATE = 200.0f;
const float TRAVEL_SPEED = 7.0f;
const float TRAVEL_LIFETIME = 3.0f;
const float ARRIVAL_LIFETIME = 0.1f;

StarEntity::StarEntity()
	:	BaseClass(),
		_starSprite(this),
		_starParticle(this),
		_starBody(this),
		_starState(kStar_Traveling),
		_currentTime(0.f),
		_totalTravelTime(0.f),
		_previousPosition(0.f,0.f),
		_particleVelocity(0.f,0.f),
		_xSpline(),
		_ySpline(),
		_emitter(nullptr)
{

}

StarEntity::~StarEntity()
{

}

void StarEntity::Update( float deltaTime )
{
	BaseClass::Update(deltaTime);

	switch (_starState)
	{
	case kStar_Traveling:
		{
			_currentTime += deltaTime;

			if(_currentTime>=_totalTravelTime)
			{
				_starState = kStar_Arrived;
				_currentTime = 1.0f;
				break;
			}
			else
			{
				SetPosition(Vec2D(_xSpline(_currentTime),_ySpline(_currentTime)));

				Vec2D position = WorldToScreen(GetPosition());
				Vec2D velocity = (GetPosition() - _previousPosition)/deltaTime;
				velocity *= RATIO;
				velocity.x *= -1;

				//_emitter->setParticleRotationSpeed(20.f);
				_emitter->setParticleVelocity(thor::Distributions::deflect(velocity, 20.f) );
				_emitter->setParticlePosition(position);
				//_emitter->setParticlePosition(thor::Distributions::circle(position,STAR_RADIUS*RATIO * 0.2));

				_starParticle.Update(deltaTime);

				_previousPosition = GetPosition();

				_particleVelocity = velocity;
			}

			break;
		}

	case kStar_Arrived:
		{
			if(_currentTime >= 0.f)
			{
				_currentTime -= deltaTime;
			}
			else if( _currentTime < 0.f )
			{
				_currentTime = 0.0f;
			}

			float lerp = 1.0f - _currentTime;

			float time = TRAVEL_LIFETIME + lerp * (ARRIVAL_LIFETIME-TRAVEL_LIFETIME);
			_emitter->setParticleLifetime(sf::seconds(time));

			_starParticle.Update(deltaTime);
			
			break;
		}
	}
}

void StarEntity::Initialize( const TiXmlElement *propertyElement /* = nullptr */ )
{
	BaseClass::Initialize(propertyElement);

	if( propertyElement )
	{
		{
			b2BodyDef bodyDefinition;
			bodyDefinition.userData = (IPhysics*)this;
			bodyDefinition.position = b2Vec2(GetPosition().x, GetPosition().y);
			bodyDefinition.angle = 0.0f;
			bodyDefinition.fixedRotation = true;
			bodyDefinition.type = b2_dynamicBody;

			_starBody.CreateBody( bodyDefinition );

			b2PolygonShape boxShape;
			boxShape.SetAsBox( 0.5f * UNRATIO, 0.5f * UNRATIO );

			b2FixtureDef fixtureDefinition;
			fixtureDefinition.shape = &boxShape;
			fixtureDefinition.isSensor = true;

			_starBody.CreateFixture( fixtureDefinition, "Star" );

			_starBody.ResetTransform();
		}

		{
			thor::ResourceKey<sf::Texture> key = thor::Resources::fromFile<sf::Texture>("Resource/Particles/whiteSpark.png");
			std::shared_ptr<sf::Texture> texture = ResourceCache::GetInstance()->acquire<sf::Texture>(key);
			thor::ParticleSystem* particleSystem = new thor::ParticleSystem(texture);

			// Create emitter
			_emitter = thor::UniversalEmitter::create();
			_emitter->setEmissionRate(EMISSION_RATE);
			_emitter->setParticleLifetime(sf::seconds(TRAVEL_LIFETIME));
			_emitter->setParticlePosition(WorldToScreen(GetPosition()));

			particleSystem->addEmitter(_emitter);

			// Build color gradient (green -> teal -> blue)
			/*
			thor::ColorGradient gradient = thor::createGradient
				(sf::Color(255, 255, 255))		(1)
				(sf::Color(0, 0, 0));

			// Create color and fade in/out animations
			thor::ColorAnimation colorizer(gradient);
			*/
			thor::FadeAnimation fader(0.f, 1.f);

			// Add particle affectors
			//particleSystem->addAffector( thor::AnimationAffector::create(colorizer) );
			particleSystem->addAffector( thor::AnimationAffector::create(fader) );
			particleSystem->addAffector( thor::TorqueAffector::create(100.f) );
			//particleSystem->addAffector( thor::ForceAffector::create(sf::Vector2f(0.f, 9.8f * RATIO * 0.01f))  );

			_starParticle.SetParticleSystem(particleSystem);
			_starParticle.RegisterRenderable(5);
		}

		{
			const TiXmlElement* pathNode = propertyElement->FirstChildElement("node");
			float previousX = GetPosition().x, previousY = GetPosition().y;
			while(pathNode)
			{
				float x = 0.f, y = 0.f;
				pathNode->QueryFloatAttribute("x",&x);
				pathNode->QueryFloatAttribute("y",&y);

				x = (x - SCREENWIDTH/2) * UNRATIO;
				y = (y - SCREENHEIGHT/2) * UNRATIO * -1;

				{
					_totalTravelTime += Magnitude(Vec2D(x-previousX,y-previousY))/TRAVEL_SPEED;
					_xSpline.addPoint(_totalTravelTime,x);
					_ySpline.addPoint(_totalTravelTime,y);
				}

				previousX = x;
				previousY = y;

				pathNode = pathNode->NextSiblingElement();
			}
		}

		{
			_previousPosition = GetPosition();
		}
	}
}

bool StarEntity::HandleEvent(const EventData& theevent)
{
	switch (theevent.GetEventType())
	{
	case Event_Simulate:
		{
			Vec2D position = GetPosition();
			_starBody.GetBody()->SetTransform(position,0);
			_starBody.GetBody()->SetAwake(true);
			break;
		}

	default:
		{
			break;
		}
	}

	return false;
}
