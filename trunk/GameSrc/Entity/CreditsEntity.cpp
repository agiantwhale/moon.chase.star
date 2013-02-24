#include "CreditsEntity.hpp"
#include "PlayerEntity.hpp"
#include "../Physics/PhysicsManager.hpp"
#include "../System/ResourceCache.hpp"
#include "../System/GraphicsManager.hpp"
#include "../Event/ContactEventData.hpp"

REGISTER_ENTITY(CreditsEntity,"Credits")

const float FADE_TIME = 1.0f;

CreditsEntity::CreditsEntity()
	:	m_textPos(),
		m_text(),
		m_triggerBody(*this),
		m_triggered(false),
		m_variationTime(),
		m_totalTime(),
		m_textSize(30)
{
	addEventListenType(Event_BeginContact);
}

CreditsEntity::~CreditsEntity()
{
	removeEventListenType(Event_BeginContact);

	sb::GraphicsManager::getInstance()->addDrawable(m_text, 6);
	sb::PhysicsManager::getInstance()->removeSimulatable(&m_triggerBody);
}

void CreditsEntity::update( sf::Time deltaTime )
{
	BaseClass::update(deltaTime);

	if(m_triggered)
	{
		m_variationTime -= deltaTime;

		if( m_variationTime <= sf::Time::Zero )
		{
			m_variationTime = sf::Time::Zero;
		}

		float alphaLevel = m_variationTime.asSeconds()/m_totalTime.asSeconds();
		m_text.setColor(sf::Color(255,255,255,255 * alphaLevel));
	}
}

void CreditsEntity::initializeEntity( const TiXmlElement *propertyElement /*= NULL */ )
{
	Entity::initializeEntity(propertyElement);

	m_totalTime = sf::seconds(FADE_TIME);
	m_variationTime = sf::seconds(FADE_TIME);

	setPosition( getPosition() + 0.5f * sf::Vector2f(getScale().x,-getScale().y) * UNRATIO );

	if( propertyElement )
	{
		{
			b2BodyDef bodyDefinition;
			bodyDefinition.userData = (Entity*)this;
			bodyDefinition.position = b2Vec2(getPosition().x, getPosition().y);
			bodyDefinition.angle = 0.0f;
			bodyDefinition.fixedRotation = true;
			bodyDefinition.type = b2_staticBody;

			b2Body* triggerBody = sb::PhysicsManager::getInstance()->getPhysicsWorld()->CreateBody(&bodyDefinition);

			b2PolygonShape boxShape;
			boxShape.SetAsBox( 0.5f * getScale().x * UNRATIO, 0.5f * getScale().y * UNRATIO );

			b2FixtureDef fixtureDefinition;
			fixtureDefinition.shape = &boxShape;
			fixtureDefinition.isSensor = true;

			triggerBody->CreateFixture(&fixtureDefinition);

			m_triggerBody.setBody(triggerBody);

			sb::PhysicsManager::getInstance()->addSimulatable(&m_triggerBody);
		}

		{
			float totalDistance = 0.0f;
			const TiXmlElement* pathNode = propertyElement->FirstChildElement("node");
			if(pathNode)
			{
				float x = 0.f, y = 0.f;
				pathNode->QueryFloatAttribute("x",&x);
				pathNode->QueryFloatAttribute("y",&y);
				m_text.setPosition(x,y);
			}
		}

		{
			std::string creditsString;
			propertyElement->QueryStringAttribute("Text", &creditsString);
			propertyElement->QueryIntAttribute("Size", &m_textSize);

			thor::ResourceKey<sf::Font> key = thor::Resources::fromFile<sf::Font>("Resource/Fonts/Stroke-Bold.otf");
			std::shared_ptr<sf::Font> font = sb::ResourceCache::getInstance()->acquire<sf::Font>(key);
			m_text.setString(creditsString);
			m_text.setFont(*font);
			m_text.setColor(sf::Color::White);

			sb::GraphicsManager::getInstance()->addDrawable(m_text, 6);
		}
	}
}

bool CreditsEntity::handleEvent( const sb::EventData& theevent )
{
	switch (theevent.getEventType())
	{
	case Event_BeginContact:
		{
			const sb::ContactEventData& contactData = static_cast<const sb::ContactEventData&>(theevent);
			const b2Contact* contactInfo = contactData.getContact();

			const b2Fixture* target = nullptr;
			if(m_triggerBody.checkContact(contactInfo,target))
			{
				Entity* targetEntity = sb::getOwnerEntity(target);
				PlayerEntity* playerEntity = sb::entity_cast<PlayerEntity>(targetEntity);

				if(!m_triggered && playerEntity)
				{
					m_triggered = true;
				}
			}

			break;
		}

	default:
		break;
	}

	return false;
}

