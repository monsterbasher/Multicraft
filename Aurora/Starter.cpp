#include <Aurora/Utils/GameLoader.h>
#include "ExampleGame.h"

int main()
{
	ExampleGameManager* exampleGame = new ExampleGameManager();

	GameLoader* loader = GameLoader::getGameLoader(exampleGame);
	loader->Run();

	return 0;
}
