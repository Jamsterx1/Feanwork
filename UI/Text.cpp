#include "Text.h"
#include "../Game.h"
#include "InterfaceManager.h"

namespace Feanwork
{
	Text::Text(INTERFACEBLOCK* _parentBlock, sf::Vector2f _position, std::string _name) :
		Interface(_position, sf::Vector2f(0.0f, 0.0f), _name)
	{
		mDependancy  = NULL;
		mParentBlock = _parentBlock;

		setPosition(mParentBlock->getPosition() + _position);
	}

	Text::~Text()
	{
	}

	void Text::initialize(sf::Font* _font, std::string _text, int _charSize, Interface* _dependacy)
	{
		mDependancy = _dependacy;
		mText.setPosition(mPosition.x, mPosition.y);
		mText.setString(_text);
		mText.setCharacterSize(_charSize);
		mText.setFont(*_font);
	}

	bool Text::update(Game* _game)
	{
		return true;
	}

	bool Text::draw(Game* _game)
	{
		if(mDependancy && mDependancy->getActive())
			_game->getWindow()->draw(mText);
		else if(!mDependancy)
			_game->getWindow()->draw(mText);

		return true;
	}

	std::string Text::getString()
	{
		return mText.getString();
	}

	void Text::setString(std::string _text)
	{
		mText.setString(_text);
	}
}
