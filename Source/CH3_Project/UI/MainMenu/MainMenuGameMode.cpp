//MainMenuGameMode.cpp

#include "UI/MainMenu/MainMenuGameMode.h"
#include "MainMenuController.h"


AMainMenuGameMode::AMainMenuGameMode()
{
	PlayerControllerClass = AMainMenuController::StaticClass();
	DefaultPawnClass = nullptr;
}
