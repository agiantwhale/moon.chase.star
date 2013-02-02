#include "../Entity/BGMEntity.hpp"
#include "../System/ResourceCache.hpp"

REGISTER_ENTITY(BGMEntity,"BGM")

BGMEntity::BGMEntity()
	:	Entity(),
		_backgroundMusic(nullptr)
{

}

BGMEntity::~BGMEntity()
{
	if(_backgroundMusic)
	{
		delete _backgroundMusic;
	}
}

void BGMEntity::Initialize( const TiXmlElement *propertyElement /*= NULL */ )
{
	BaseClass::Initialize(propertyElement);


	if(propertyElement)
	{
		{
			std::string bgmFile;
			propertyElement->QueryStringAttribute("BackgroundMusic",&bgmFile);

			_backgroundMusic = new sf::Music();
			_backgroundMusic->openFromFile(bgmFile);
			_backgroundMusic->setLoop(true);
			_backgroundMusic->play();
		}
	}
}