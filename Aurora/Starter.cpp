#include <Aurora/Utils/GameLoader.h>
#include <Aurora/Utils/GameManager.h>

#include "Tests/ExampleGame.h"
#include "Tests/Demo_ObjLoading.h"
#include "Tests/ServerTest.h"
#include "Tests/ClientTest.h"
#include "Tests/SimpleTest.h"
#include "Tests/EnetServerTest.h"
#include "Tests/EnetClientTest.h"


class ExampleGameManager : public GameManager
{
private:

	SimpleTest* exampleState;

public:

	void Configure()
	{
		//init render manager properties
		RenderManager::Instance()->setSesize(480,272);
	}

	void Init()
	{
		//init whatever you need
		exampleState = new SimpleTest();
		exampleState->Init();

		ChangeState(exampleState);
	}

	void CleanUp()
	{
		exampleState->CleanUp();
		delete exampleState;
	}
};

int main()
{
	ExampleGameManager* exampleGame = new ExampleGameManager();

	GameLoader* loader = GameLoader::getGameLoader(exampleGame);

	loader->Run();

	return 0;
}
