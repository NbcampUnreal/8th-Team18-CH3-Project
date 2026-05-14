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
	FTimerHandle HitMarkerTimer;
	FTimerHandle KillImageTimer;

		UPROPERTY(meta = (BindWidget))
		UProgressBar* HP_Bar;
		UPROPERTY(meta = (BindWidget))
		UTextBlock* Ammo_Text;
		UPROPERTY(meta = (BindWidget))
		UImage* Crosshair;
		UPROPERTY(meta = (BindWidget))
		UImage* HitMarker;
		UPROPERTY(meta = (BindWidget))
		UImage* Kill_Image1;


public:
	UFUNCTION()
	void UpdateHP(float UpdatePlayerHP, float PlayerMaxHP, float CurrentPlayerHP);
	UFUNCTION()
	void ShowKillImage(APlayerController* InstigatorController);
	void HideKillImage();

	void UpdateAmmo(int32 CurrentAmmo, int32 MaxAmmo);
	void ShowHitMarker();
	void HideHitMarker();

	virtual void NativeConstruct()override;
};
