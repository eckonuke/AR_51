// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FaceComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AR_51_API UFaceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFaceComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> faceActor;

	UPROPERTY(EditAnywhere)
	int32 partsNumber = 0;

private:
	class AAR_Player* player;
	class AActor* spawnedFace;
	
	void FaceCenterDetection();
	void Face3PointsDectection(int32 part);
};
