//HUD_Mission.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUD_Mission.generated.h"

class UTextBlock;

UCLASS()
class CH3_PROJECT_API UHUD_Mission : public UUserWidget
{
	GENERATED_BODY()
public:
	class AMainCharacter* MyCharacter;

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Mission_Text;

public:
	UFUNCTION()
	void GetMission(FString MissionName, int32 CurrentScore, int32 MaxScore);
	virtual void NativeConstruct()override;

	
};
