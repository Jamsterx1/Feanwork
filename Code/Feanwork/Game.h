#ifndef GAME_H_
#define GAME_H_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <map>

#include "ResourceManager.h"
#include "EventManager.h"
#include "Object.h"
#include "Collision.h"
#include "SoundQueue.h"
#include "UI/InterfaceManager.h"

using namespace std;

namespace Feanwork
{
	enum    GAMESTATE { MENU, GAME };
	#define OBJECTS   std::vector<Object*>

	class Game
	{
	public:
		Game(string _title, string _resourceDir, int _width, int _height, bool _fullscreen);
		~Game(void);

		void expandResources(string _resources);
		void loadResources(string _dir);
		void initMenu(OBJECTS _objects);
		void initGame(OBJECTS _objects);
		void pushObject(Object* _object);

		void setPlayer(Object* _object) { mPlayerPtr = _object; }
		void changeMusic(std::string _file);
		void pushSound(std::string _soundFile);

		void loadUIContent(string _batch);
		void addUICallback(std::string _name, UICallback _callback);

		void initialize();
		void update();
		void render();
		void clean();

		int				  getWidth()		{ return mWidth;	  }
		int				  getHeight()		{ return mHeight;	  }
		string			  getTitle()		{ return mTitle;	  }
		bool			  isFullscreen()	{ return mFullscreen; }
		sf::RenderWindow* getWindow()		{ return &mWindow;	  }
		sf::Texture*	  getTexture(int _resourceID);

		void setTitle(string _title)	{ mTitle = _title;	   }
		void setState(GAMESTATE _state) { mGameState = _state; }
		void exit()						{ mRunning = false;	   }

		void resume()					{ mPaused = false;	   }
		void pause()					{ mPaused = true;	   }
		bool isPaused()					{ return mPaused;	   }

		sf::Vector2i getMousePosition();
		bool		 keyPressed(string _key);
		bool		 keyPressedOnFrame(string _key);

	protected:
		int		mWidth;
		int		mHeight;
		string	mTitle;
		string  mResourceDir;
		bool	mFullscreen;
		bool	mRunning;
		bool	mPaused;

		GAMESTATE		  mGameState;
		Collision*		  mCollision;
		InterfaceManager* mInterfaceManager;
		SoundQueue*		  mSoundQueue;
		sf::RenderWindow  mWindow;
		sf::Music		  mMusic;

		std::map<GAMESTATE, OBJECTS> mStates;
		Object*						 mPlayerPtr;
	};
}

#endif
