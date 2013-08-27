#ifndef SWITCH_H_
#define SWITCH_H_

#include "Interface.h"

namespace Feanwork
{
	typedef void (*UICallback)(Game* _game);
	enum SWITCHSTATE
	{
		SWITCHSTATE_INACTIVE = 0,
		SWITCHSTATE_ACTIVE,
	};

	struct INTERFACEBLOCK;
	struct Switch_Data
	{
	public:
		Switch_Data()
		{
			textures[0] = new sf::Texture();
			textures[1] = new sf::Texture();
		}

		sf::Texture* textures[2];
		sf::Sprite sprites[2];
	};

	class Game;
	class Switch : public Interface
	{
	public:
		Switch(INTERFACEBLOCK* _parentBlock, sf::Vector2f _position, std::string _name);
		~Switch();

		void initialize(int _resourceIDs[2], UICallback _callback);
		virtual bool update(Game*_game);
		virtual bool draw(Game*_game);

		void setPosition(float _x, float _y);
		void setPosition(sf::Vector2f _position);

		bool pressed();
		void setSwitchState(SWITCHSTATE _state);
		void callEvent(Game* _game);
		bool isActive();

	protected:
		UICallback		mCallback;
		SWITCHSTATE		mSwitchState;
		Switch_Data		mData;
		bool			mClicked;
		INTERFACEBLOCK* mParentBlock;
	
		bool			mRecent;
		int				mCooldown;
	};
}

#endif