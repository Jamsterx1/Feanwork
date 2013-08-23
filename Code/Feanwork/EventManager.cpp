#include "EventManager.h"
#include "Parser.h"
#include "Game.h"

namespace Feanwork
{
	void EventManager::update(Game* _game)
	{
		KEYMAP_ITERATOR it;

		while(_game->getWindow()->pollEvent(mEvent))
		{
			for(int i = 0; i < sf::Keyboard::KeyCount; i++)
				mPressedOnFrame[i] = false;

			for(int i = 0; i < 2; i++)
				mMouseOnFrame[i] = false;

			switch(mEvent.type)
			{
			case sf::Event::Closed:
				_game->exit();
				break;

			case sf::Event::KeyPressed:
				for(it = mKeyMap.begin(); it != mKeyMap.end(); ++it)
				{
					sf::Keyboard::Key key = getKey(it->first);
					if(mEvent.key.code == key)
						mPressedOnFrame[key] = true;
				}

				if(mEvent.key.code == getKey("p"))
				{
					if(_game->isPaused())
					{
						_game->resume();
						_game->getWindow()->setTitle(_game->getTitle());
					}
					else
					{
						_game->pause();
						_game->getWindow()->setTitle(_game->getTitle() + " - paused");
					}
				}
				else if(mEvent.key.code == getKey("escape"))
					_game->exit();
				break;

			case sf::Event::MouseButtonPressed:
				if(mEvent.mouseButton.button == sf::Mouse::Left)
					mMouseOnFrame[0] = true;
				else if(mEvent.mouseButton.button == sf::Mouse::Right)
					mMouseOnFrame[1] = true;
				break;

			case sf::Event::MouseLeft:
				_game->pause();
				_game->getWindow()->setTitle(_game->getTitle() + " - paused");
				break;

			case sf::Event::MouseEntered:
				_game->resume();
				_game->getWindow()->setTitle(_game->getTitle());
				break;

			default:
				break;
			}
		}
	}

	bool EventManager::loadMapping(std::string _keyMap)
	{
		std::string keyword = "";
		Parser p(_keyMap);

		while((keyword = p.getNextKeyword()) != "")
			if(keyword == "Key")
			{
				if(!p.checkNumParams(2))
					std::cout << "Incorrect number of parameters passed to key when mapping" << endl;

				mKeyMap[p.getString(1)] = p.getInt(0);
			}

		return true;
	}

	sf::Keyboard::Key EventManager::getKey(std::string _key)
	{
		KEYMAP_ITERATOR it;
		for(it = mKeyMap.begin(); it != mKeyMap.end(); ++it)
			if(_key == it->first)
				return sf::Keyboard::Key(it->second);

		return sf::Keyboard::Unknown;
	}

	std::string	EventManager::getKey(int _key)
	{
		KEYMAP_ITERATOR it;
		for(it = mKeyMap.begin(); it != mKeyMap.end(); ++it)
			if(_key == it->second)
				return it->first;

		return "";
	}

	bool EventManager::keyPressed(std::string _key)
	{ 
		return sf::Keyboard::isKeyPressed(getKey(_key));
	}

	bool EventManager::keyPressedOnFrame(std::string _key)
	{
		KEYMAP_ITERATOR it;
		for(it = mKeyMap.begin(); it != mKeyMap.end(); ++it)
			if(_key == it->first)
				return mPressedOnFrame[it->second];

		return false;
	}

	bool EventManager::mousePressed(std::string _button)
	{
		if(_button == "left")
			return sf::Mouse::isButtonPressed(sf::Mouse::Left);
		else if(_button == "right")
			return sf::Mouse::isButtonPressed(sf::Mouse::Right);
		else
			return false;
	}

	bool EventManager::mousePressedOnFrame(std::string _button)
	{
		if(_button == "left")
			return mMouseOnFrame[0];
		else if(_button == "right")
			return mMouseOnFrame[1];
		else
			return false;
	}

	sf::Vector2i EventManager::getMousePos(Game* _game)
	{ 
		const sf::Window& window = *_game->getWindow();
		return sf::Mouse::getPosition(window);
	}

	EventManager::EventManager()
	{
		for(int i = 0; i < sf::Keyboard::KeyCount; i++)
		mPressedOnFrame[i] = false;

		for(int i = 0; i < 2; i++)
			mPressedOnFrame[i] = false;
	}
}
