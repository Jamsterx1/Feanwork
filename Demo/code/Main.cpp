#include "player.h"
#include <SFML/Audio.hpp>
using namespace Feanwork;

void switchMusic(Game* _game)
{
	if(_game->getMusic()->getStatus() == sf::Music::Playing)
		_game->getMusic()->pause();
	else if(_game->getMusic()->getStatus() == sf::Music::Paused || _game->getMusic()->getStatus() == sf::Music::Stopped)
		_game->getMusic()->play();
}

void switchDebug(Game* _game)
{
	if(_game->getDebugMode())
		_game->setDebugMode(false);
	else if(!_game->getDebugMode())
		_game->setDebugMode(true);
}

int main()
{
	Game* g = new Game("Demo", "resources/", 1240, 800, DEFAULT);
	g->expandResources("test/");
	g->expandResources("ui/");
	g->expandResources("particles/");
	g->loadResources("");

	g->addUICallback("switchMusic", switchMusic);
	g->addUICallback("switchDebug", switchDebug);
	g->loadUIContent("test.block");

	std::vector<Object*> objects;
	Player* player = new Player(3, (float)g->getWidth() / 2, (float)g->getHeight() / 2);
	Object* obj2 = new Object(4, 10, 10, true);
	Object* obj3 = new Object(4, 200, 500, true);
	objects.push_back(player);
	objects.push_back(obj2);
	objects.push_back(obj3);

	g->addCollisionCheck(player);
	g->addCollisionCheck(obj2);
	g->addCollisionCheck(obj3);
	g->setPlayer(player);

	g->changeMusic("song.ogg");
	g->addSound("explosion", "resources/bulletExplosion.wav");
	g->addSound("shoot", "resources/shoot.wav");

	g->initMenu(objects);
	g->initialize();
}