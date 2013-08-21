#ifndef INTERFACEMANAGER_H_
#define INTERFACEMANAGER_H_

#include <SFML/Graphics.hpp>
#include "Interface.h"
#include "Button.h"
#include "Switch.h"
#include "Text.h"

namespace Feanwork
{
	#define UI_BATCH std::string
	#define CALLBACK_ITERATOR std::map<std::string, UICallback>::iterator

	enum InterfaceMode
	{
		InterfaceMode_Game = 0,
		InterfaceMode_Editor,
	};

	struct INTERFACEBLOCK
	{
		INTERFACEBLOCK();
		INTERFACEBLOCK(int _boundaries[4], Game* _state);
	
		bool update();
		void render();
		void push(Interface* _interface);
		sf::Vector2f getPosition();

		void activate();
		void deactivate();
		bool insideBlock(sf::Vector2i _mouse);

		bool isBlockActive()
			{ return mActiveBlock; }

		std::vector<Interface*>& getInterfaces()
			{ return mInterfaces; }

		std::vector<Interface*> mInterfaces;
		Game*		mGame;
		bool		mActiveBlock;
		sf::IntRect mInputBoundaries;
	};

	class InterfaceManager
	{
	public:
		void initialize(Game* _game);
		bool update();
		void render();

		bool	   loadContent(UI_BATCH _batch);
		void	   addCallback(std::string _name, UICallback _callback);
		UICallback getCallback(std::string _callback);

		bool isActive() { return mActive; }

		InterfaceManager() {}
		~InterfaceManager() {}

	protected:
		std::vector<INTERFACEBLOCK>		  mBlocks;
		std::map<std::string, UICallback> mCallbacks;
		Game*							  mGamePtr;
		sf::Font						  mDefaultFont;
		bool							  mActive;
	};
}

#endif