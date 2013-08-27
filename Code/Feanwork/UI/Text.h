#ifndef TEXT_H_
#define TEXT_H_

#include "Interface.h"

namespace Feanwork
{
	struct INTERFACEBLOCK;
	class Game;
	class Text : public Interface
	{
	public:
		Text(INTERFACEBLOCK* _parentBlock, sf::Vector2f _position, std::string _name);
		~Text();

		virtual void initialize(sf::Font* _font, std::string _text, int _charSize, Interface* _dependacy = NULL);
		virtual bool update(Game* _game);
		virtual bool draw(Game* _game);

		void setPosition(float _x, float _y);
		void setPosition(sf::Vector2f _position);

		std::string getString();
		void setString(std::string _text);

	protected:
		sf::Text		mText;
		Interface*		mDependancy;
		INTERFACEBLOCK* mParentBlock;
	};
}

#endif