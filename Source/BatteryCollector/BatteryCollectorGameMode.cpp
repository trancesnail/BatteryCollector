// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "BatteryCollector.h"
#include "BatteryCollectorGameMode.h"
#include "BatteryCollectorCharacter.h"
#include "Kismet/GameplayStatics.h"

ABatteryCollectorGameMode::ABatteryCollectorGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	DecayRate = 0.01f;
}

void ABatteryCollectorGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	ABatteryCollectorCharacter * MyCharacter = Cast<ABatteryCollectorCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if ( MyCharacter && MyCharacter->GetCurrentPower() > 0 )
	{
		MyCharacter->UpdatePower(-DeltaSeconds * DecayRate * (MyCharacter->GetInitialPower()));
	}

}