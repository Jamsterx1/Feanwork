#include "Game.h"

namespace Feanwork
{
	Game::Game(string _title, string _resourceDir, int _width, int _height, WINDOWSTYLE _screen) :
		mCamera(sf::FloatRect(0, 0, (float)_width, (float)_height))
	{
		srand((unsigned)time(NULL));

		mWidth		 = _width;
		mHeight		 = _height;
		mTitle		 = _title;
		mResourceDir = _resourceDir;
		mRunning	 = true;
		mDebugMode   = false;
		mDeltaTime   = 0.0f;
		mClock.restart();

		unsigned style;
		if(_screen == FULLSCREEN)
		{
			style   = sf::Style::Fullscreen;
			mWidth  = sf::VideoMode::getDesktopMode().width;
			mHeight = sf::VideoMode::getDesktopMode().height;
		}
		else
			style = sf::Style::Titlebar | sf::Style::Close;

		mMusic.setVolume(70.f);
		mGameState		  = MENU;
		mCollision		  = new Collision;
		mInterfaceManager = new InterfaceManager;
		mSoundManager	  = new SoundManager;
		mPlayer			  = NULL;
		mWindow.create(sf::VideoMode(mWidth, mHeight), mTitle, style);
		mWindow.setFramerateLimit(60);

		mInterfaceManager->initialize(this);
		EventManager::getSingleton()->loadMapping(mResourceDir + "custom.keymap");
	}

	Game::~Game(void)
	{
		delete mCollision;
		delete mInterfaceManager;
		delete mSoundManager;

		if(!mCollisionCheck.empty())
			mCollisionCheck.clear();
	}

	void Game::expandResources(string _resources)
	{
		ResourceManager::getSingleton()->expandBatch(mResourceDir, _resources, "sprites.batch");
	}

	void Game::loadResources(string _dir)
	{
		ResourceManager::getSingleton()->loadBatch(mResourceDir + _dir, "sprites.batch");
	}

	void Game::initMenu(OBJECTS _objects)
	{
		for(auto& i: _objects)
			mStates[MENU].push_back(i);
	}

	void Game::initOptions(OBJECTS _objects)
	{
		for(auto& i: _objects)
			mStates[MENU].push_back(i);
	}

	void Game::initGame(OBJECTS _objects)
	{
		for(auto& i: _objects)
			mStates[GAME].push_back(i);
	}

	void Game::pushObject(Object* _object)		  { mStates[mGameState].push_back(_object); }
	void Game::addCollisionCheck(Object* _object) { mCollisionCheck.push_back(_object);		}
	void Game::addEmitter(Emitter* _emitter)	  { mEmitters.push_back(_emitter);			}

	void Game::changeMusic(std::string _file, bool play)
	{
		if(mMusic.Playing)
			mMusic.stop();

		if(!mMusic.openFromFile(mResourceDir + _file))
			std::cout << "could not locate " << _file << "\n";

		mMusic.setLoop(true);
		if(play)
			mMusic.play();
	}

	void Game::addSound(std::string _name, std::string _sound)
	{
		if(mSoundManager)
			mSoundManager->addSound(_name, _sound);
	}

	void Game::playSound(std::string _name)
	{
		if(mSoundManager)
			mSoundManager->playSound(_name);
	}

	void Game::loadUIContent(string _batch)							  
	{ 
		mInterfaceManager->loadContent(mResourceDir + _batch); 
	}
	
	void Game::addUICallback(std::string _name, UICallback _callback) 
	{ 
		mInterfaceManager->addCallback(_name, _callback);	   
	}
	
	void Game::moveCamera(float _x, float _y) { mCamera.move(sf::Vector2f(_x, _y));	}
	sf::Vector2f Game::getCameraPosition()	  { return mCamera.getCenter();			}				

	void Game::initialize()
	{
		while(mRunning)
		{
			mDeltaTime = (mLastTime - mClock.getElapsedTime()).asSeconds();
			update();
			render();

			mLastTime  =  mClock.getElapsedTime();
			mClock.restart();
		}
		clean();
	}

	void Game::update()
	{
		EventManager::getSingleton()->update(this);
		mSoundManager->update();
		unsigned size		 = mStates[mGameState].size();
		unsigned emitterSize = mEmitters.size();

		if(!mPaused)
		{
			for(unsigned i = 0; i < size; i++)
			{
				if(mStates[mGameState][i]->isDestroyed())
				{
					mStates[mGameState].erase(mStates[mGameState].begin() + i);
					size--;
					continue;
				}

				Object* current = mStates[mGameState][i];
				current->update(this);

				if(mCollision)
				{
					for(auto& i: mCollisionCheck)
						if(current->canCollide() && current != i)
						{
							bool skip = false;
							if(!i->getIgnores().empty())
							{
								for(auto& i: i->getIgnores())
									if(i == current)
										skip = true;
							}

							if(!skip)
								mCollision->checkCollides(i, current, this);
						}
				}
			}

			if(!mEmitters.empty())
			{
				for(unsigned i = 0; i < emitterSize; i++)
				{
					if(!mEmitters[i]->isActive())
					{
						mEmitters[i]->clean();
						mEmitters.erase(mEmitters.begin() + i);

						emitterSize--;
						continue;
					}
					mEmitters[i]->update(this);
				}
			}
		}
		mInterfaceManager->update();
	}

	void Game::render()
	{
		mWindow.clear(sf::Color(107, 107, 107, 255));
		mWindow.setView(mCamera);
		unsigned size = mStates[mGameState].size();
		if(!mStates[mGameState].empty())
			for(unsigned i = 0; i < size; i++)
				mStates[mGameState][i]->render(this);

		if(!mEmitters.empty())
			for(auto& i: mEmitters)
				i->render(this);

		mWindow.setView(mWindow.getDefaultView());
		mInterfaceManager->render();
		mWindow.display();
	}

	void Game::clean()
	{
		if(!mStates[MENU].empty())
			mStates[MENU].clear();

		if(!mStates[GAME].empty())
			mStates[GAME].clear();
	}

	sf::Texture* Game::getTexture(int _resourceID)
	{
		return ResourceManager::getSingleton()->getResource(_resourceID);
	}

	void Game::resume() { mPaused = false; }
	void Game::pause()  { mPaused = true;  }

	sf::Vector2i Game::getMousePosition()	  { return EventManager::getSingleton()->getMousePos(this);		  }
	bool Game::keyPressed(string _key)		  { return EventManager::getSingleton()->keyPressed(_key);		  }
	bool Game::keyPressedOnFrame(string _key) { return EventManager::getSingleton()->keyPressedOnFrame(_key); }
	bool Game::mousePressed(string _mouse)    { return EventManager::getSingleton()->mousePressed(_mouse);	  }
}
