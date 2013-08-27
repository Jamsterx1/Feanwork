#ifndef GAME_H_
#define GAME_H_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <map>
#include <chrono>

#include "ResourceManager.h"
#include "EventManager.h"
#include "Object.h"
#include "Collision.h"
#include "SoundManager.h"
#include "Emitter.h"
#include "UI/InterfaceManager.h"

using namespace std;
using namespace std::chrono;

namespace Feanwork
{
	enum    GAMESTATE   { MENU = 0, GAME, INSTRUCTIONS };
	enum	WINDOWSTYLE { FULLSCREEN = 0, DEFAULT	   };
	#define OBJECTS		vector<Object*>

	class Game
	{
	public:
		Game(string _title, string _resourceDir, int _width, int _height, WINDOWSTYLE _screen);
		~Game(void);

		void expandResources(string _resources);
		void loadResources(string _dir);
		void initMenu(OBJECTS _objects);
		void initInstructions(OBJECTS _objects);
		void initGame(OBJECTS _objects);
		void pushObject(Object* _object);

		void    setPlayer(Object* _player) { mPlayer = _player;		  }
		Object* getPlayer()				   { return mPlayer;		  }
		void	setCursor(Object* _cursor) { mCustomCursor = _cursor; }

		void addCollisionCheck(Object* _object);
		void addEmitter(Emitter* _emitter);

		void	   changeMusic(std::string _file, bool play = false);
		void	   addSound(std::string _name, std::string _sound);
		void	   playSound(std::string _name);
		sf::Music* getMusic()   { return &mMusic; }
		void	   pauseMusic() { mMusic.pause(); }
		void	   setVolume(float _volume) { mMusic.setVolume(_volume); }

		void loadUIContent(string _batch);
		void addUICallback(std::string _name, UICallback _callback);

		sf::View	 getCamera() { return mCamera; }
		void		 moveCamera(float _x, float _y);
		sf::Vector2f getCameraPosition();

		void initialize();
		void update();
		void render();
		void clean();

		int				  getWidth()  { return mWidth;	   }
		int				  getHeight() { return mHeight;	   }
		string			  getTitle()  { return mTitle;	   }
		sf::RenderWindow* getWindow() { return &mWindow;   }
		float			  getDelta()  { return mDeltaTime; }
		sf::Texture*	  getTexture(int _resourceID);
		bool			  isInterfaceActive() { return mInterfaceManager->isActive(); }
		InterfaceManager* getInterface()	  { return mInterfaceManager;			  }
		void			  flushInterface()	  { mInterfaceManager->clean();			  }

		void   setTitle(string _title)	  { mTitle = _title;	  }
		string getResourceDir()			  { return mResourceDir;  }
		void   exit()					  { mRunning = false;	  }
		void   setState(GAMESTATE _state);

		void resume();
		void pause();
		bool isPaused()					{ return mPaused;	   }

		void setDebugMode(bool _mode)	{ mDebugMode = _mode; }
		bool getDebugMode()				{ return mDebugMode;  }

		sf::Vector2i getMousePosition();
		bool		 keyPressed(string _key);
		bool		 keyPressedOnFrame(string _key);
		bool		 mousePressed(string _mouse);

		std::vector<Object*>& getObjects()
			{ return mStates[mGameState]; }

	protected:
		int		mWidth;
		int		mHeight;
		string	mTitle;
		string  mResourceDir;
		bool	mRunning;
		bool	mPaused;
		bool	mDebugMode;
		bool	mSwapState;

		GAMESTATE		  mGameState;
		GAMESTATE		  mNextState;
		Collision*		  mCollision;
		InterfaceManager* mInterfaceManager;
		SoundManager*	  mSoundManager;
		sf::RenderWindow  mWindow;
		sf::Music		  mMusic;
		sf::View		  mCamera;
		Object*			  mPlayer;
		Object*			  mCustomCursor;

		map<GAMESTATE, OBJECTS> mStates;
		vector<Object*>			mCollisionCheck;
		vector<Emitter*>		mEmitters;

		sf::Clock	mClock;
		sf::Time	mLastTime;
		float		mDeltaTime;
	};
}

#endif
