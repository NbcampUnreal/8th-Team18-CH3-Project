//UI_Widget.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Widget.generated.h"


class UProgressBar;
class UTextBlock;
class UImage;

UCLASS()
class CH3_PROJECT_API UUI_Widget : public UUserWidget
{
	GENERATED_BODY()
public:
	class AMainCharacter* MyCharacter;

protected:
		UPROPERTY(meta = (BindWidget))
		UProgressBar* HP_Bar;
		UPROPERTY(meta = (BindWidget))
		UTextBlock* Ammo_Text;
		UPROPERTY(meta = (BindWidget))
		UImage* Crosshair;
		UPROPERTY(meta = (BindWidget))
		UTextBlock* Score_Text;

public:
	UFUNCTION()
	void UpdateHP(float UpdatePlayerHP, float PlayerMaxHP, float CurrentPlayerHP);
	virtual void NativeConstruct()override;
	void UpdateAmmo(int32 CurrentAmmo, int32 MaxAmmo);
};
