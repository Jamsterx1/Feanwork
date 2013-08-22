#include "Game.h"

namespace Feanwork
{
	Game::Game(string _title, string _resourceDir, int _width, int _height, WINDOWSTYLE _screen)
	{
		srand((unsigned)time(NULL));

		mWidth		 = _width;
		mHeight		 = _height;
		mTitle		 = _title;
		mResourceDir = _resourceDir;
		mRunning	 = true;

		unsigned style;
		if(_screen == FULLSCREEN)
		{
			style   = sf::Style::Fullscreen;
			mWidth  = sf::VideoMode::getDesktopMode().width;
			mHeight = sf::VideoMode::getDesktopMode().height;
		}
		else
			style = sf::Style::Titlebar | sf::Style::Close;

		mGameState		  = MENU;
		mCollision		  = new Collision;
		mInterfaceManager = new InterfaceManager;
		mSoundQueue		  = new SoundQueue;
		mWindow.create(sf::VideoMode(mWidth, mHeight), mTitle, style);
		mWindow.setFramerateLimit(60);

		mInterfaceManager->initialize(this);
		EventManager::getSingleton()->loadMapping(mResourceDir + "custom.keymap");
	}

	Game::~Game(void)
	{
		delete mCollision;
		delete mInterfaceManager;
		delete mSoundQueue;

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

	void Game::initGame(OBJECTS _objects)
	{
		for(auto& i: _objects)
			mStates[GAME].push_back(i);
	}

	void Game::pushObject(Object* _object)		  { mStates[mGameState].push_back(_object); }
	void Game::addCollisionCheck(Object* _object) { mCollisionCheck.push_back(_object);		}
	void Game::addEmitter(Emitter* _emitter)	  { mEmitters.push_back(_emitter);			}

	void Game::changeMusic(std::string _file)
	{
		if(mMusic.Playing)
			mMusic.stop();

		if(!mMusic.openFromFile(mResourceDir + _file))
			std::cout << "could not locate " << _file << "\n";

		mMusic.setLoop(true);
		mMusic.play();
	}

	void Game::pushSound(std::string _soundFile)
	{
		if(mSoundQueue)
			mSoundQueue->pushSound(mResourceDir + _soundFile);
	}

	void Game::loadUIContent(string _batch)
	{
		mInterfaceManager->loadContent(mResourceDir + _batch);
	}

	void Game::addUICallback(std::string _name, UICallback _callback)
	{
		mInterfaceManager->addCallback(_name, _callback);
	}

	void Game::initialize()
	{
		while(mRunning)
		{
			update();
			render();
		}

		clean();
	}

	void Game::update()
	{
		EventManager::getSingleton()->update(this);
		mSoundQueue->update();
		unsigned size = mStates[mGameState].size();

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
				for(unsigned i = 0; i < mEmitters.size(); i++)
				{
					if(!mEmitters[i]->isActive())
					{
						mEmitters[i]->clean();
						delete mEmitters[i];

						mEmitters.erase(mEmitters.begin() + i);
						size--;
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
		mWindow.clear(sf::Color::Black);
		unsigned size = mStates[mGameState].size();
		if(!mStates[mGameState].empty())
			for(unsigned i = 0; i < size; i++)
				mStates[mGameState][i]->render(this);

		if(!mEmitters.empty())
			for(auto& i: mEmitters)
				i->render(this);

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

	void Game::resume()
	{
		mPaused = false;
		if(mMusic.getStatus() == sf::Music::Paused)
			mMusic.play();
	}

	void Game::pause()
	{
		mPaused = true;
		if(mMusic.getStatus() == sf::Music::Playing)
			mMusic.pause();
	}

	sf::Vector2i Game::getMousePosition()	  { return EventManager::getSingleton()->getMousePos(this);		  }
	bool Game::keyPressed(string _key)		  { return EventManager::getSingleton()->keyPressed(_key);		  }
	bool Game::keyPressedOnFrame(string _key) { return EventManager::getSingleton()->keyPressedOnFrame(_key); }
}
