#include "player.h"
using namespace Feanwork;

int main()
{
	Game* g = new Game("Test", "resources", 1000, 800, false);
	g->loadResources("");

	std::vector<Object*> objects;
	Player* player = new Player(0, 25, 35);
	Object* obj2 = new Object(0, 10, 10, true);
	objects.push_back(player);
	objects.push_back(obj2);

	g->setPlayer(player);
	g->initMenu(objects);
	g->changeMusic("song.ogg");
	g->initialize();
}