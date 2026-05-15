//MainMenuWidget.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"


UCLASS()
class CH3_PROJECT_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* StartButton;

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnStartButtonClicked();
	
};
