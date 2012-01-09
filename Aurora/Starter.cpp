#include <Aurora/Utils/GameLoader.h>
#include "Tests/ExampleGame.h"
#include "Tests/Demo_ObjLoading.h"
#include "Tests/ServerTest.h"
#include "Tests/ClientTest.h"

int main()
{
	ServerTestGameManager* exampleGame = new ServerTestGameManager();
	//ClientTestGameManager* exampleGame = new ClientTestGameManager();
	//ExampleGameManager * exampleGame = new ExampleGameManager();

	GameLoader* loader = GameLoader::getGameLoader(exampleGame);
	loader->Run();

	return 0;
}
