#include "TextEntity.hpp"
#include "PlayerEntity.hpp"
#include "../Physics/PhysicsManager.hpp"
#include "../System/ResourceCache.hpp"
#include "../System/GraphicsManager.hpp"
#include "../Event/ContactEventData.hpp"

REGISTER_ENTITY(TextEntity,"Text")

const float FADE_TIME = 1.0f;

TextEntity::TextEntity()
	:	m_textPos(),
		m_text(),
		m_triggerBody(*this),
		m_triggered(false),
		m_fadeIn(false),
		m_variationTime(),
		m_totalTime(),
		m_textSize(30)
{
	addEventListenType(Event_BeginContact);
}

TextEntity::~TextEntity()
{
	removeEventListenType(Event_BeginContact);

	sb::GraphicsManager::getInstance()->removeDrawable(m_text, 6);
	sb::PhysicsManager::getInstance()->removeSimulatable(&m_triggerBody);
}

void TextEntity::update( sf::Time deltaTime )
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

		if(m_fadeIn)
		{
			alphaLevel = 1.f - alphaLevel;
		}

		m_text.setColor(sf::Color(255,255,255,255 * alphaLevel));
	}
}

void TextEntity::initializeEntity( const TiXmlElement *propertyElement /*= NULL */ )
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
			propertyElement->QueryUnsignedAttribute("Size", &m_textSize);
			std::string attributeValue = propertyElement->Attribute("FadeIn");
			m_fadeIn = (attributeValue == "True");

			thor::ResourceKey<sf::Font> key = thor::Resources::fromFile<sf::Font>("Resource/Fonts/Stroke-Bold.otf");
			std::shared_ptr<sf::Font> font = sb::ResourceCache::getInstance()->acquire<sf::Font>(key);
			m_text.setString(creditsString);
			m_text.setFont(*font);
			m_text.setCharacterSize(m_textSize);

			if(m_fadeIn)
			{
				m_text.setColor(sf::Color::Transparent);
			}
			else
			{
				m_text.setColor(sf::Color::White);
			}

			sb::GraphicsManager::getInstance()->addDrawable(m_text, 6);
		}
	}
}

bool TextEntity::handleEvent( const sb::EventData& theevent )
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

