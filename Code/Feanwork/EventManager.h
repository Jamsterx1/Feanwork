#ifndef EVENTMANAGER_H_
#define EVENTMANAGER_H_
#include <SFML/Graphics.hpp>

namespace Feanwork
{
	#define KEYMAP			std::map<std::string, int>
	#define KEYMAP_ITERATOR std::map<std::string, int>::iterator

	class Game;
	class EventManager
	{
	public:
		void			  update(Game* _game);
		bool			  loadMapping(std::string _keyMap);
		sf::Keyboard::Key getKey(std::string _key);
		std::string		  getKey(int _key);

		bool		 keyPressed(std::string _key);
		bool		 keyPressedOnFrame(std::string _key);
		bool		 mousePressed(std::string _button);
		bool		 mousePressedOnFrame(std::string _button);
		sf::Vector2i getMousePos(Game* _game);

		static EventManager* getSingleton()
		{
			static EventManager singleton;
			return &singleton;
		}

	protected:
		sf::Event mEvent;
		KEYMAP	  mKeyMap;
		bool	  mPressedOnFrame[sf::Keyboard::KeyCount];
		bool	  mMouseOnFrame[2];

	private:
		EventManager();
		~EventManager() {}
	};
}

#endif
