// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "BatteryCollectorGameMode.generated.h"

UENUM(BlueprintType)
enum class EBatteryPlaystate
{
	EPlaying,
	EGameOver,
	EWon,
	EUnknow
};

UCLASS(minimalapi)
class ABatteryCollectorGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ABatteryCollectorGameMode();
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintPure, Category = "Power")
	float GetPowerToWin() const;
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure, Category = "Power")
	EBatteryPlaystate GetCurrentState() const;

	void SetCurrentState(EBatteryPlaystate NewState);

protected: 
	//PowerË¥¼õÂÊ
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category = "Power ",Meta = (BlueprintProtected = "true") )
	float DecayRate;
	//the power needed to win the game
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power ", Meta = (BlueprintProtected = "true"))
	float PowerToWin;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power ", Meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	class UUserWidget* CurrentWidget;

private:
	EBatteryPlaystate CurrentState;
	TArray<class ASpawnVolume*> SpawnVolumeActors;

	void HandleNewsState(EBatteryPlaystate NewState);
};



