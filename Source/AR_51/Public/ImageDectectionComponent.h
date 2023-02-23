// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ImageDectectionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AR_51_API UImageDectectionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UImageDectectionComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, Category="AR Settings")
	TMap<class UARCandidateImage*, TSubclassOf<AActor>> trackingTable;

private:
	class AAR_Player* player;
	TMap<FString, AActor*> spawnedData;


	void DetectImages();

	UFUNCTION()
	void TestFunc(class UARTrackedGeometry* geo);
};
