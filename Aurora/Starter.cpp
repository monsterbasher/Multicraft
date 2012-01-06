#include <Aurora/Utils/GameLoader.h>
#include "Tests/ExampleGame.h"
#include "Tests/Demo_ObjLoading.h"

int main()
{
	Demo_ObjLoadingGameManager* exampleGame = new Demo_ObjLoadingGameManager();

	GameLoader* loader = GameLoader::getGameLoader(exampleGame);
	loader->Run();

	return 0;
}
