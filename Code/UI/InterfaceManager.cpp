#include "InterfaceManager.h"
#include "../Parser.h"
#include "../Game.h"

namespace Feanwork
{
	// INTERFACEBLOCK
	INTERFACEBLOCK::INTERFACEBLOCK()
	{ 
		mGame = NULL; 
		mActiveBlock = false;
		mInputBoundaries.left = 0;
		mInputBoundaries.top  = 0;
		mInputBoundaries.width  = 200;
		mInputBoundaries.height = 200;
	}

	INTERFACEBLOCK::INTERFACEBLOCK(int _boundaries[4], Game* _state)
	{
		mGame = _state; 
		mInputBoundaries.left = _boundaries[0];
		mInputBoundaries.top  = _boundaries[1];
		mInputBoundaries.width  = _boundaries[2];
		mInputBoundaries.height = _boundaries[3];
	}

	bool INTERFACEBLOCK::update()
	{
		if(!mInterfaces.empty())
			for(auto& i: mInterfaces)
				i->update(mGame);

		return true;
	}

	void INTERFACEBLOCK::render()
	{
		if(!mInterfaces.empty())
			for(auto& i: mInterfaces)
				i->draw(mGame);

		sf::RectangleShape shape;
		shape.setPosition((float)mInputBoundaries.left + 1, (float)mInputBoundaries.top + 1);
		shape.setSize(sf::Vector2f((float)mInputBoundaries.width, (float)mInputBoundaries.height));
		shape.setOutlineThickness(2);
		shape.setOutlineColor(sf::Color::Blue);
		shape.setFillColor(sf::Color::Transparent);
		mGame->getWindow()->draw(shape);
	}

	void INTERFACEBLOCK::push(Interface* _interface)
	{
		mInterfaces.push_back(_interface);
	}

	sf::Vector2f INTERFACEBLOCK::getPosition()
	{
		sf::Vector2f position;
		position.x = (float)mInputBoundaries.left;
		position.y = (float)mInputBoundaries.top;
		return position;
	}

	void INTERFACEBLOCK::activate()
	{
		for(auto& i: mInterfaces)
			i->setActive(true);

		mActiveBlock = true;
	}

	void INTERFACEBLOCK::deactivate()
	{
		for(auto& i: mInterfaces)
			i->setActive(false);

		mActiveBlock = false;
	}

	bool INTERFACEBLOCK::insideBlock(sf::Vector2i _mouse)
	{
		if(_mouse.x > mInputBoundaries.left && _mouse.x < mInputBoundaries.left + mInputBoundaries.width &&
			_mouse.y > mInputBoundaries.top && _mouse.y < mInputBoundaries.top + mInputBoundaries.height)
		{
			return true;
		}

		return false;
	}

	// INTERFACE MANAGER
	void InterfaceManager::initialize(Game* _game)
	{
		mGamePtr = _game;
		mDefaultFont.loadFromFile("arial.ttf");
	}

	bool InterfaceManager::update()
	{
		sf::Vector2i mouse = EventManager::getSingleton()->getMousePos(mGamePtr);
		if(!mBlocks.empty())
			for(auto& i: mBlocks)
			{
				if(i.insideBlock(mouse))
				{
					if(!i.isBlockActive())
						i.activate();
				}
				else
				{
					if(i.isBlockActive())
						i.deactivate();
				}
			}

		if(!mBlocks.empty())
			for(auto& i: mBlocks)
				i.update();

		return true;
	}

	void InterfaceManager::render()
	{
		if(!mBlocks.empty())
			for(auto& i: mBlocks)
				i.render();
	}

	bool InterfaceManager::loadContent(UI_BATCH _batch)
	{
		INTERFACEBLOCK* newBlock;
		Parser p(_batch);
	
		std::string keyword = "";
		while((keyword = p.getNextKeyword()) != "")
		{
			if(keyword == "Boundaries")
			{
				if(!p.checkNumParams(4))
					cout << "Incorrect number of parameters passed to boundaries" << endl;

				int boundaries[4] = { p.getInt(0), p.getInt(1), p.getInt(2), p.getInt(3) };
				newBlock = new INTERFACEBLOCK(boundaries, mGamePtr);
			}
			else if(keyword == "Button")
			{
				if(!p.checkNumParams(7))
					cout << "Incorrect number of parameters passed to button" << endl;

				sf::Vector2f position;
				position.x = p.getFloat(0);
				position.y = p.getFloat(1);

				int IDs[3] = { p.getInt(2), p.getInt(3), p.getInt(4) };
				Button* newButton = new Button(newBlock, position, p.getString(6));
				newButton->initialize(IDs, getCallback(p.getString(5)));
				newBlock->push(newButton);
			}
			else if(keyword == "Switch")
			{
				if(!p.checkNumParams(6))
					cout << "Incorrect number of parameters passed to switch" << endl;

				sf::Vector2f position;
				position.x = p.getFloat(0);
				position.y = p.getFloat(1);

				int IDs[2] = { p.getInt(2), p.getInt(3) };
				Switch* newSwitch = new Switch(newBlock, position, p.getString(5));
				newSwitch->initialize(IDs, getCallback(p.getString(4)));
				newBlock->push(newSwitch);
			}
			else if(keyword == "Text")
			{
				if(!p.checkNumParams(5))
					cout << "Incorrect number of parameters passed to text" << endl;

				sf::Vector2f position;
				position.x = p.getFloat(0);
				position.y = p.getFloat(1);
				int size   = p.getInt(2);

				Text* newText = new Text(newBlock, position, p.getString(4));
				newText->initialize(&mDefaultFont, p.getString(3), size);
				newBlock->push(newText);
			}
		}
		mBlocks.push_back(*newBlock);

		return true;
	}

	void InterfaceManager::addCallback(std::string _name, UICallback _callback)
	{
		mCallbacks[_name] = _callback;
	}

	UICallback InterfaceManager::getCallback(std::string _callback)
	{
		CALLBACK_ITERATOR it;
		for(it = mCallbacks.begin(); it != mCallbacks.end(); ++it)
			if(_callback == it->first)
				return it->second;

		return nullptr;
	}
}
