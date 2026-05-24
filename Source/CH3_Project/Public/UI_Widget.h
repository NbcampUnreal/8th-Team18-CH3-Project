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
		UPROPERTY(meta = (BindWidget))
		UProgressBar* SlideCooldown_bar;

		float CurrentCooldownTime = 0.0f;


public:
	UFUNCTION()
	void UpdateHP(float CurrentPlayerHP, float PlayerMaxHP, float DamageAmount);
	UFUNCTION()
	void ShowKillImage(APlayerController* InstigatorController);
	UFUNCTION()
	void HideKillImage();


	
	void HideHitMarker();
	void UpdateAmmo(int32 CurrentAmmo, int32 MaxAmmo);
	void ShowHitMarker();


	virtual void NativeConstruct()override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
