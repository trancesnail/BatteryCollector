// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "BatteryCollectorGameMode.generated.h"

UCLASS(minimalapi)
class ABatteryCollectorGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ABatteryCollectorGameMode();
	virtual void Tick(float DeltaSeconds) override;
protected: 
	//Power˥����
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category = "Power")
	float DecayRate;
};



