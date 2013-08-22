#include "player.h"
using namespace Feanwork;

int main()
{
	Game* g = new Game("Demo", "resources/", 1240, 800, DEFAULT);
	g->expandResources("test/");
	g->expandResources("ui/");
	g->expandResources("particles/");
	g->loadResources("");
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

	g->initMenu(objects);
	g->changeMusic("song.ogg");
	g->initialize();
}