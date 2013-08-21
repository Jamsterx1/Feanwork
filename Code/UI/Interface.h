#ifndef INTERFACE_H_
#define INTERFACE_H_

#include <SFML/Graphics.hpp>

namespace Feanwork
{
	class Game;
	class Interface
	{
	public:
		Interface(sf::Vector2f _position, sf::Vector2f _bounds, std::string _name);
		~Interface();

		virtual bool update(Game*_game);
		virtual bool draw(Game* _game) { return true; }

		void setPosition(float _x, float _y);
		void setPosition(sf::Vector2f _position);

		sf::Vector2f getPositon()
			{ return mPosition; }

		void setBounds(float _x, float _y);
		void setBounds(sf::Vector2f _bounds);

		sf::Vector2f getBounds()
			{ return mBounds; }

		bool getActive()
			{ return mActive; }

		void setActive(bool _set)
			{ mActive = _set; }

		std::string getUniqueName()
			{ return mUniqueName; }

	protected:
		sf::Vector2f mPosition;
		sf::Vector2f mBounds;
		bool		 mInside;
		bool		 mActive;
		std::string  mUniqueName;
	};
}

#endif