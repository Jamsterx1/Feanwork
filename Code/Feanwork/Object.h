#ifndef OBJECT_H_
#define OBJECT_H_
#include <SFML/Graphics.hpp>

namespace Feanwork
{
	class Game;
	class Object
	{
	public:
		Object(int _resourceID, float _xPos, float _yPos, bool _canCollide);
		~Object(void) {}

		virtual bool update(Game* _game) { return true; }
		virtual bool render(Game* _game);
		virtual void collisionCallback(sf::Vector2f _depth, sf::Vector2f _normal, Game* _game);
		void		 setPosition(float _x, float _y);
		void		 addPosition(float _x, float _y);

		float		getX()		 { return mX;			}
		float		getY()		 { return mY;			}
		bool		canCollide() { return mCanCollide;  }
		sf::IntRect getAABB()	 { return mAABB;		}

		std::vector<Object*>& getIgnores() { return collisionIgnore;			 }
		void ignore(Object* _object)	   { collisionIgnore.push_back(_object); }

		void		setUniqueType(std::string _type) { mUniqueType = _type; }
		std::string getUniqueType()					 { return mUniqueType;  }

		bool isDestroyed() { return mDestroy; }
		void destroy()	   { mDestroy = true; }

	protected:
		float	    mX;
		float	    mY;
		bool	    mCanCollide;
		bool		mDestroy;

		std::string mUniqueType;
		sf::Sprite  mSprite;
		sf::IntRect mAABB;
		std::vector<Object*> collisionIgnore;
	};
}

#endif
