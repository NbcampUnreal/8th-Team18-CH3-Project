//UWP_DamageWidget.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWP_DamageWidget.generated.h"

class UTextBlock;

UCLASS()
class CH3_PROJECT_API UUWP_DamageWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* DamageText;

public:
	UFUNCTION(BlueprintCallable, Category = "Damage UI")
	void ViewDamageText(float DamageAmount);
	
};
