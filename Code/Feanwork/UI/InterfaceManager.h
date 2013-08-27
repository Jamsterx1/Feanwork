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
		void setPosition(int _x, int _y);

		void activate();
		void deactivate();
		bool insideBlock(sf::Vector2i _mouse);

		bool isActive()				 { return mActive;	  }
		void setActive(bool _active) { mActive = _active; }

		bool isDead()
			{ return mDestroy; }

		void destroy()
			{ mDestroy = true; }

		bool isBlockActive()
			{ return mActiveBlock; }

		std::vector<Interface*>& getInterfaces()
			{ return mInterfaces; }

		Interface* getInterface(std::string _interface);

		std::vector<Interface*> mInterfaces;
		Game*		mGame;
		bool		mActiveBlock;
		bool		mDestroy;
		bool		mActive;
		sf::IntRect mInputBoundaries;
	};

	class InterfaceManager
	{
	public:
		void initialize(Game* _game);
		bool update();
		void render();
		void clean();

		bool	   loadContent(UI_BATCH _batch);
		void	   addCallback(std::string _name, UICallback _callback);
		UICallback getCallback(std::string _callback);

		Interface*	    getInterface(int _blockPos, std::string _interface);
		INTERFACEBLOCK* getInterfaceBlock(int _blockPos);

		bool isActive() { return mActive; }

		InterfaceManager() {}
		~InterfaceManager() {}

	protected:
		std::vector<INTERFACEBLOCK*>	  mBlocks;
		std::map<std::string, UICallback> mCallbacks;
		Game*							  mGamePtr;
		sf::Font						  mDefaultFont;
		bool							  mActive;
	};
}

#endif