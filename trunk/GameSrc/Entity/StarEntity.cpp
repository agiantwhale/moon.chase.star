#include <Thor/Graphics.hpp>
#include <Thor/Animation.hpp>
#include <Thor/Vectors.hpp>
#include <Thor/Math.hpp>
#include "StarEntity.hpp"
#include "../Helper/Conversion.hpp"
#include "../Physics/PhysicsManager.hpp"
#include "../System/ResourceCache.hpp"
#include "../System/SceneManager.hpp"
#include "../System/GraphicsManager.hpp"
#include "../Task/Task.hpp"
#include "../Event/EventData.hpp"

REGISTER_ENTITY(StarEntity,"Star")

const float STAR_RADIUS = 0.5f;
const float EMISSION_RATE = 200.0f;
const float TRAVEL_SPEED = 8.0f;
const float TRAVEL_LIFETIME = 3.0f;
const float ARRIVAL_LIFETIME = 0.1f;

StarEntity::StarEntity()
	:	BaseClass(),
		m_starSprite(),
		m_starParticle(nullptr),
		m_starTranslator(*this),
		m_starBody(*this),
		m_arrived(false),
		m_totalTime(0.f),
		m_arrivalTime(),
		m_previousPosition(),
		m_particleVelocity(),
		m_xSpline(),
		m_ySpline(),
		m_emitter(nullptr)
{
	addEventListenType(Event_Simulate);
}

StarEntity::~StarEntity()
{
	removeEventListenType(Event_Simulate);

	sb::GraphicsManager::getInstance()->removeDrawable(m_starSprite,3);
	sb::GraphicsManager::getInstance()->removeDrawable(*m_starParticle,3);
	sb::PhysicsManager::getInstance()->removeSimulatable(&m_starBody);
	delete m_starParticle;
}

void StarEntity::update( sf::Time deltaTime )
{
	BaseClass::update(deltaTime);

	if(m_arrivalTime >= sf::Time::Zero)
	{
		m_arrivalTime -= deltaTime;

		if(!m_arrived)
		{
			m_arrived = true;
			sb::EventData* eventData = new sb::EventData(Event_StarArrived);
			eventData->triggerEvent();
		}

		setPosition(sf::Vector2f(m_xSpline(m_totalTime),m_ySpline(m_totalTime)));
		m_emitter->setEmissionRate(0.f);
	}
	else
	{
		float passedTime = m_totalTime - m_arrivalTime.asSeconds();
		setPosition(sf::Vector2f(m_xSpline(passedTime),m_ySpline(passedTime)));

		sf::Vector2f position = sb::Translate::Position(getPosition());
		sf::Vector2f velocity = sb::Translate::Velocity( (getPosition() - m_previousPosition) / deltaTime.asSeconds());

		m_emitter->setParticleVelocity(thor::Distributions::deflect(velocity, 20.0f) );
		m_emitter->setParticlePosition(position);

		m_previousPosition = getPosition();
		m_particleVelocity = velocity;
	}

	m_starParticle->update(deltaTime);
	m_starTranslator.translate(m_starSprite);
}

void StarEntity::initializeEntity( const TiXmlElement *propertyElement /* = nullptr */ )
{
	BaseClass::initializeEntity(propertyElement);

	if( propertyElement )
	{
		{
			b2BodyDef bodyDefinition;
			bodyDefinition.userData = (Entity*)this;
			bodyDefinition.position = ToVector(getPosition());
			bodyDefinition.angle = 0.0f;
			bodyDefinition.fixedRotation = true;
			bodyDefinition.type = b2_dynamicBody;

			b2Body* starBody = sb::PhysicsManager::getInstance()->getPhysicsWorld()->CreateBody(&bodyDefinition);

			b2PolygonShape boxShape;
			boxShape.SetAsBox( 0.5f * UNRATIO, 0.5f * UNRATIO );

			b2FixtureDef fixtureDefinition;
			fixtureDefinition.shape = &boxShape;
			fixtureDefinition.isSensor = true;

			starBody->CreateFixture(&fixtureDefinition);

			m_starBody.setBody(starBody);

			sb::PhysicsManager::getInstance()->addSimulatable(&m_starBody);
		}

		{
			thor::ResourceKey<sf::Texture> key = thor::Resources::fromFile<sf::Texture>("Resource/Ogmo/Entities/Star.png");
			std::shared_ptr<sf::Texture> texture = sb::ResourceCache::getInstance()->acquire<sf::Texture>(key);
			m_starSprite.setTexture(*texture);
			m_starSprite.setOrigin(32,32);

			sb::GraphicsManager::getInstance()->addDrawable(m_starSprite,3);
		}

		{
			thor::ResourceKey<sf::Texture> key = thor::Resources::fromFile<sf::Texture>("Resource/Particles/whiteSpark.png");
			std::shared_ptr<sf::Texture> texture = sb::ResourceCache::getInstance()->acquire<sf::Texture>(key);
			m_starParticle = new thor::ParticleSystem(texture);

			// Create emitter
			m_emitter = thor::UniversalEmitter::create();
			m_emitter->setEmissionRate(EMISSION_RATE);
			m_emitter->setParticleLifetime(sf::seconds(TRAVEL_LIFETIME));
			m_emitter->setParticlePosition(sb::Translate::Position(getPosition()));

			m_starParticle->addEmitter(m_emitter);

			thor::FadeAnimation fader(0.f, 1.f);

			m_starParticle->addAffector( thor::AnimationAffector::create(fader) );
			m_starParticle->addAffector( thor::TorqueAffector::create(100.f) );

			sb::GraphicsManager::getInstance()->addDrawable(*m_starParticle,3);
		}

		{
			sf::Vector2f previousPos = getPosition();
			for(const TiXmlElement* pathNode = propertyElement->FirstChildElement("node"); pathNode != nullptr; pathNode = pathNode->NextSiblingElement())
			{
				float x = 0.f, y = 0.f;
				pathNode->QueryFloatAttribute("x",&x);
				pathNode->QueryFloatAttribute("y",&y);

				x = (x - SCREENWIDTH/2) * UNRATIO;
				y = (y - SCREENHEIGHT/2) * UNRATIO * -1;

				sf::Vector2f currentPos(x,y);

				{
					m_totalTime += thor::length<float>(currentPos - previousPos)/TRAVEL_SPEED;
					m_xSpline.addPoint(m_totalTime,x);
					m_ySpline.addPoint(m_totalTime,y);
				}

				previousPos = currentPos;
			}
			m_arrivalTime = (sf::seconds(m_totalTime));
		}

		{
			m_previousPosition = getPosition();
		}
	}

	sb::SceneManager::getInstance()->setStarEntity(this);
}

bool StarEntity::handleEvent(const sb::EventData& theevent)
{
	switch (theevent.getEventType())
	{
	case Event_Simulate:
		{
			sf::Vector2f position = getPosition();
			m_starBody.getBody()->SetTransform(ToVector(position),0);
			m_starBody.getBody()->SetAwake(true);
			break;
		}

	default:
		{
			break;
		}
	}

	return false;
}
