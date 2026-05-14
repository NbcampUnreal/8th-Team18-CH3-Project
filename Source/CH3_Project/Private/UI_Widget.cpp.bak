//UI_Widget.cpp


#include "UI_Widget.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


void UUI_Widget::NativeConstruct()
{
	Super::NativeConstruct();
	MyCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (MyCharacter)
	{
		MyCharacter->HealthDamaged.AddUniqueDynamic(this, &UUI_Widget::UpdateHP);
		UpdateHP(MyCharacter->GetHealth(), 100.f, 100.0f);
	}
	

}

void UUI_Widget::UpdateHP(float UpdatePlayerHP, float PlayerMaxHP, float CurrentPlayerHP)
{
	if (HP_Bar)
	{
		if (PlayerMaxHP > 0.0f)
		{
			float UpdatePlayerHP = CurrentPlayerHP / PlayerMaxHP;
			HP_Bar->SetPercent(UpdatePlayerHP);
		}
		
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
