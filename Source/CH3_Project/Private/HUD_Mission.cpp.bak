//HUD_Mission.cpp


#include "HUD_Mission.h"
#include "MainCharacter.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UHUD_Mission::NativeConstruct()
{
	Super::NativeConstruct();
	MyCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (MyCharacter)
	{
		MyCharacter->MissionUpdateBoard.AddUniqueDynamic(this, &UHUD_Mission::GetMission);
	}


}

void UHUD_Mission::GetMission(FString MissionName, int32 CurrentScore, int32 MaxScore)
{
	if (Mission_Text)
	{
		FString MissionString = FString::Printf(TEXT(""), *MissionName, CurrentScore, MaxScore);
		Mission_Text->SetText(FText::FromString(MissionString));
	}
}
