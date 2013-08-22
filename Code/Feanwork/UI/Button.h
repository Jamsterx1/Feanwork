#ifndef BUTTON_H_
#define BUTTON_H_

#include "Interface.h"

namespace Feanwork
{
	typedef void (*UICallback)(Game* _game);
	enum CLICKSTATE
	{
		CLICKSTATE_Released = 0,
		CLICKSTATE_Hover,
		CLICKSTATE_Pressed,
	};

	struct INTERFACEBLOCK;
	struct Button_Data
	{
	public:
		Button_Data()
		{
			textures[0] = new sf::Texture();
			textures[1] = new sf::Texture();
			textures[2] = new sf::Texture();
		}

		sf::Texture* textures[3];
		sf::Sprite sprites[3];
	};

	class Game;
	class Button : public Interface
	{
	public:
		Button(INTERFACEBLOCK* _parentBlock, sf::Vector2f _position, std::string _name);
		~Button();

		void initialize(int _resourceIDS[3],  UICallback _callback);
		virtual bool update(Game* _game);
		virtual bool draw(Game* _game);

		bool pressed();
		void setClickState(CLICKSTATE _state);
		void callEvent(Game* _game);

	protected:
		UICallback		mCallback;
		CLICKSTATE		mClickState;
		Button_Data		mData;
		bool			mClicked;
		INTERFACEBLOCK* mParentBlock;
	};
}

#endif