// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class BATTERYCOLLECTOR_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	//��ȡ�������
	FORCEINLINE class UStaticMeshComponent * GetMesh() const { return PickupMesh; }

	UFUNCTION(BlueprintPure, Category = "Pickup")
	bool IsActive();
	UFUNCTION(BlueprintCallable, Category = "Pickup")
	void SetActive(bool NewPickupState);

	UFUNCTION(BlueprintNativeEvent, Category = "Pickup")
	void WasCollected();
	virtual void WasCollected_Implementation();

protected:
	//trueΪ���ã�falseΪ��Ч
	bool bIsActive;
private:

	//�ؿ��������
	UPROPERTY( VisibleAnywhere,BlueprintReadOnly,Category = "Pickup",Meta = (AllowPrivateAccess = "true") )
	class UStaticMeshComponent * PickupMesh;
	
};
