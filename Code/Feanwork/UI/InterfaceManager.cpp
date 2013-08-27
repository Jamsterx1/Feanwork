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
		mDestroy = false;
		mActive  = true;
	}

	INTERFACEBLOCK::INTERFACEBLOCK(int _boundaries[4], Game* _state)
	{
		mGame = _state; 
		mInputBoundaries.left = _boundaries[0];
		mInputBoundaries.top  = _boundaries[1];
		mInputBoundaries.width  = _boundaries[2];
		mInputBoundaries.height = _boundaries[3];
		mDestroy = false;
		mActive  = true;
	}

	bool INTERFACEBLOCK::update()
	{
		if(mActive)
		{
			if(!mInterfaces.empty())
				for(unsigned i = 0; i < mInterfaces.size(); i++)
					mInterfaces[i]->update(mGame);
		}
		return true;
	}

	void INTERFACEBLOCK::render()
	{
		if(mActive)
		{
			if(!mInterfaces.empty())
				for(auto& i: mInterfaces)
					i->draw(mGame);

			if(mGame->getDebugMode())
			{
				sf::RectangleShape shape;
				shape.setPosition((float)mInputBoundaries.left + 1, (float)mInputBoundaries.top + 1);
				shape.setSize(sf::Vector2f((float)mInputBoundaries.width, (float)mInputBoundaries.height));
				shape.setOutlineThickness(2);
				shape.setOutlineColor(sf::Color::Blue);
				shape.setFillColor(sf::Color::Transparent);
				mGame->getWindow()->draw(shape);
			}
		}
	}

	void INTERFACEBLOCK::push(Interface* _interface)
	{
		mInterfaces.push_back(_interface);
	}

	void INTERFACEBLOCK::setPosition(int _x, int _y)
	{
		mInputBoundaries.left = _x;
		mInputBoundaries.top  = _y;

		for(auto& i: mInterfaces)
			i->setPosition(sf::Vector2f(_x + i->getPositon().x, _y + i->getPositon().y));
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

	Interface* INTERFACEBLOCK::getInterface(std::string _interface)
	{
		for(auto& i: mInterfaces)
			if(i->getUniqueName() == _interface)
				return i;

		return nullptr;
	}

	// INTERFACE MANAGER
	void InterfaceManager::initialize(Game* _game)
	{
		mGamePtr = _game;
		mDefaultFont.loadFromFile(_game->getResourceDir() + "Fixedsys.ttf");
		mActive = false;
	}

	bool InterfaceManager::update()
	{
		mActive = false;
		sf::Vector2i mouse = EventManager::getSingleton()->getMousePos(mGamePtr);
		if(!mBlocks.empty())
			for(auto& i: mBlocks)
			{
				if(i->insideBlock(mouse))
				{
					if(!i->isBlockActive())
						i->activate();

					mActive = true;
				}
				else
				{
					if(i->isBlockActive())
						i->deactivate();
				}
			}

		if(!mBlocks.empty())
		{
			unsigned blockSize = mBlocks.size();
			for(unsigned i = 0; i < blockSize; i++)
			{
				if(mBlocks[i]->isDead())
				{
					mBlocks.erase(mBlocks.begin() + i);
					blockSize--;
					continue;
				}
				mBlocks[i]->update();
			}
		}

		return true;
	}

	void InterfaceManager::render()
	{
		if(!mBlocks.empty())
			for(auto& i: mBlocks)
				i->render();
	}

	void InterfaceManager::clean()
	{
		for(unsigned i = 0; i < mBlocks.size(); i++)
			for(auto& p: mBlocks[i]->getInterfaces())
				delete p;

		for(unsigned b = 0; b < mBlocks.size(); b++)
			mBlocks.erase(mBlocks.begin() + b);

		mBlocks.clear();
	}

	bool InterfaceManager::loadContent(UI_BATCH _batch)
	{
		INTERFACEBLOCK* newBlock = NULL;
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
				position.x     = p.getFloat(0);
				position.y     = p.getFloat(1);
				int size	   = p.getInt(2);

				Text* newText = new Text(newBlock, position, p.getString(4));
				newText->initialize(&mDefaultFont, p.getString(3), size);
				newBlock->push(newText);
			}
		}

		mBlocks.push_back(newBlock);
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

	Interface* InterfaceManager::getInterface(int _blockPos, std::string _interface)
	{
		return mBlocks[_blockPos]->getInterface(_interface);
	}

	INTERFACEBLOCK* InterfaceManager::getInterfaceBlock(int _blockPos)
	{
		return mBlocks[_blockPos];
	}
}
