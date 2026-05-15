//UI_Widget.cpp


#include "UI_Widget.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"


void UUI_Widget::NativeConstruct()
{
	Super::NativeConstruct();
	MyCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	if (MyCharacter)
	{
		MyCharacter->HealthDamaged.AddUniqueDynamic(this, &UUI_Widget::UpdateHP);
		UpdateHP(MyCharacter->CurrentPlayerHP, MyCharacter->PlayerMaxHP, 0.0f);
	}
	if (HitMarker)
	{
		HitMarker->SetVisibility(ESlateVisibility::Hidden);
	}
	
	if (Kill_Image1)
	{
		MyCharacter->EnemyKillUpdate.AddUniqueDynamic(this, &UUI_Widget::ShowKillImage);
		Kill_Image1->SetVisibility(ESlateVisibility::Hidden);
		
	}

}

void UUI_Widget::UpdateHP(float CurrentPlayerHP, float PlayerMaxHP, float DamageAmount)
{
	if (HP_Bar)
	{
		if (PlayerMaxHP > 0.0f)
		{
			float HPPercent = CurrentPlayerHP / PlayerMaxHP;
			HP_Bar->SetPercent(HPPercent);
		}
		
	}
}

void UUI_Widget::ShowKillImage(APlayerController* InstigatorController)
{
	if (Kill_Image1) 
	{
		Kill_Image1->SetVisibility(ESlateVisibility::HitTestInvisible);
		MyCharacter->EnemyKillAnimation();
		GetWorld()->GetTimerManager().ClearTimer(KillImageTimer);
		GetWorld()->GetTimerManager().SetTimer(
			KillImageTimer,
			this,
			&UUI_Widget::HideKillImage,
			1.3f,
			false
		);
	}
	
}

void UUI_Widget::HideKillImage()
{
	if (Kill_Image1)
	{
		Kill_Image1->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UUI_Widget::UpdateAmmo(int32 CurrentAmmo, int32 MaxAmmo)
{
	if (Ammo_Text)
	{
		FString AmmoString = FString::Printf(TEXT("%d / %d"), CurrentAmmo, MaxAmmo);
		Ammo_Text->SetText(FText::FromString(AmmoString));
	}
}

void UUI_Widget::ShowHitMarker()
{
	if (HitMarker)
	{
		HitMarker->SetVisibility(ESlateVisibility::HitTestInvisible);
		GetWorld()->GetTimerManager().ClearTimer(HitMarkerTimer);
		GetWorld()->GetTimerManager().SetTimer(
			HitMarkerTimer,
			this,
			&UUI_Widget::HideHitMarker,
			0.8f,
			false
		);
	}
}

void UUI_Widget::HideHitMarker()
{
	if (HitMarker)
	{
		HitMarker->SetVisibility(ESlateVisibility::Hidden);
	}
}
