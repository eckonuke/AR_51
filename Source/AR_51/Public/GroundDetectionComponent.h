// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GroundDetectionComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class AR_51_API UGroundDetectionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGroundDetectionComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, Category = ARSettings)
	TSubclassOf<class AActor> indicator;

	UPROPERTY(EditAnywhere, Category = ARSettings)
	TSubclassOf<class AActor> carModel;

private:
	class AActor* spawnedIndicator;
	FIntPoint vSize;
	class AActor* spawnedCar;
	float prev_dist = -1;
	TArray<class UARTrackedGeometry*> planes;
	class AAR_Player* player;

	
	void DebugGroundDetection();
	void CarControl();
	void DetectGround();

};
