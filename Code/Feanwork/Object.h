#ifndef OBJECT_H_
#define OBJECT_H_
#include <SFML/Graphics.hpp>

namespace Feanwork
{
	struct Frame;
	class Game;
	class Object
	{
	public:
		Object(int _resourceID, float _xPos, float _yPos, bool _canCollide, bool _active = true);
		~Object(void) {}

		virtual bool update(Game* _game);
		virtual bool render(Game* _game);
		virtual void collisionCallback(sf::Vector2f _depth, sf::Vector2f _normal, Object* _collision, Game* _game);

		void setRect(int _x, int _y, int _width, int _height);
		void setRect(Frame _frame);
		void switchID(int _resourceID);

		void		 setPosition(float _x, float _y);
		void		 addPosition(float _x, float _y);
		sf::Vector2f getPosition() { return sf::Vector2f(mX, mY); }

		float		getX()		 { return mX;			}
		float		getY()		 { return mY;			}
		bool		canCollide() { return mCanCollide;  }
		sf::IntRect getAABB()	 { return mAABB;		}
		sf::Sprite  getSprite()  { return mSprite;		}

		std::vector<Object*>& getIgnores() { return collisionIgnore;			 }
		void ignore(Object* _object)	   { collisionIgnore.push_back(_object); }

		void		setUniqueType(std::string _type) { mUniqueType = _type; }
		std::string getUniqueType()					 { return mUniqueType;  }

		bool isDestroyed()			 { return mDestroy;   }
		void destroy()				 { mDestroy = true;   }
		void setActive(bool _active) { mActive = _active; }

		/* Keep off the collide call! */
		void collide()	   { mColliding = true; }
		bool isColliding() { return mColliding; }

	protected:
		float	    mX;
		float	    mY;
		bool	    mCanCollide;
		bool		mDestroy;
		bool		mActive;
		std::string mUniqueType;
		sf::Sprite  mSprite;

		bool				 mColliding;
		sf::IntRect			 mAABB;
		sf::RectangleShape	 mDebug;
		std::vector<Object*> collisionIgnore;
	};
}

#endif
