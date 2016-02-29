// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "BatteryCollector.h"
#include "BatteryCollectorGameMode.h"
#include "BatteryCollectorCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "SpawnVolume/SpawnVolume.h"

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

void ABatteryCollectorGameMode::BeginPlay()
{
	Super::BeginPlay();

	//获取Actor
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundActors);
	for (auto Actor : FoundActors)
	{
		ASpawnVolume * SpawnVolumeActor = Cast<ASpawnVolume>(Actor);
		if (SpawnVolumeActor)
		{
			SpawnVolumeActors.AddUnique(SpawnVolumeActor);
		}
	}

	SetCurrentState(EBatteryPlaystate::EPlaying);
	ABatteryCollectorCharacter * MyCharacter = Cast<ABatteryCollectorCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (MyCharacter )
	{
		PowerToWin = (MyCharacter->GetInitialPower())*1.25f;
	}
	if ( HUDWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(),HUDWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}
}

void ABatteryCollectorGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	ABatteryCollectorCharacter * MyCharacter = Cast<ABatteryCollectorCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if ( MyCharacter  )
	{
		if ( MyCharacter->GetCurrentPower() > PowerToWin)
		{
			SetCurrentState(EBatteryPlaystate::EWon);
		}
		else if (MyCharacter->GetCurrentPower() > 0)
		{
			MyCharacter->UpdatePower(-DeltaSeconds * DecayRate * (MyCharacter->GetInitialPower()));
		}
		else
		{
			SetCurrentState(EBatteryPlaystate::EGameOver); 
		}
	}

}

float ABatteryCollectorGameMode::GetPowerToWin() const
{
	return PowerToWin;
}
EBatteryPlaystate ABatteryCollectorGameMode::GetCurrentState() const
{
	return CurrentState;
}

void ABatteryCollectorGameMode::SetCurrentState(EBatteryPlaystate NewState)
{
	CurrentState = NewState;
	HandleNewsState(CurrentState);
}

void ABatteryCollectorGameMode::HandleNewsState(EBatteryPlaystate NewState)
{
	switch (NewState)
	{
	case  EBatteryPlaystate::EPlaying:
		{
			for ( ASpawnVolume * volume : SpawnVolumeActors)
			{
				volume->SetSpawningActive(true);
			}
		}
		break;
	case  EBatteryPlaystate::EWon:
		{
			for (ASpawnVolume * volume : SpawnVolumeActors)
			{
				volume->SetSpawningActive(false);
			}
		}
		break;
	case  EBatteryPlaystate::EGameOver:
		{
			for (ASpawnVolume * volume : SpawnVolumeActors)
			{
				volume->SetSpawningActive(false);
			}
			//屏蔽玩家输入
			APlayerController * PlayerController = UGameplayStatics::GetPlayerController(this,0);
			if (PlayerController)
			{
				PlayerController->SetCinematicMode(true, false, false, true, true);
			}
			ACharacter * MyCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
			ABatteryCollectorCharacter * BatteryCollectorCharacter = Cast<ABatteryCollectorCharacter>(MyCharacter);
			if (MyCharacter)
			{
				MyCharacter->GetMesh()->SetSimulatePhysics(true);
				MyCharacter->GetMovementComponent()->MovementState.bCanJump = false;
			}

		}
		break;
	case  EBatteryPlaystate::EUnknow:
	default:
		{

		}
		break;
	}
}