//MainMenuController.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainMenuController.generated.h"


UCLASS()
class CH3_PROJECT_API AMainMenuController : public APlayerController
{
	GENERATED_BODY()

public:
	AMainMenuController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UUserWidget> MainMenuWidgetClass;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	class UUserWidget* MainMenuWidgetInstance;
	void SetupMainMenuCamera();

	
};
