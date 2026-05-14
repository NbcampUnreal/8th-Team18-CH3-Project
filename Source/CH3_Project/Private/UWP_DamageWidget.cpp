//UWP_DamageWidget.cpp

#include "UWP_DamageWidget.h"
#include "Components/TextBlock.h"

void UUWP_DamageWidget::ViewDamageText(float DamageAmount)
{
	if (DamageText)
	{
		int32 RoundedDamage = FMath::RoundToInt(DamageAmount);
		DamageText->SetText(FText::AsNumber(RoundedDamage));
	}
}
