// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AR_Player.generated.h"

UENUM(BlueprintType)
enum class EDetectionType : uint8
{
	Ground	UMETA(DisplayName = "GroundType"),
	Face	UMETA(DisplayName = "FaceType"),
	Image	UMETA(DisplayName = "ImageType"),
};


UCLASS()
class AR_51_API AAR_Player : public APawn
{
	GENERATED_BODY()

public:
	AAR_Player();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = ARSettings)
	class USceneComponent* rootComp;

	UPROPERTY(EditAnywhere, Category = ARSettings)
	class UCameraComponent* cameraComp;

	UPROPERTY(EditAnywhere, Category=ARSettings)
	TArray<class UARSessionConfig*> arSessions; 

	UPROPERTY(EditAnywhere, Category = ARSettings)
	TSubclassOf<class UMainWidget> mainWidget_Source;

	UPROPERTY(EditAnywhere, Category = ARSettings)
	EDetectionType detectionType = EDetectionType::Ground;

	UPROPERTY(EditAnywhere, Category = ARSettings)
	class UGroundDetectionComponent* groundComp;

	UPROPERTY(EditAnywhere, Category = ARSettings)
	class UFaceComponent* faceComp;

	UPROPERTY(EditAnywhere, Category = ARSettings)
	class UImageDectectionComponent* imageComp;

	class UMainWidget* mainUI;

	class APlayerController* playerCon;
	
private:
	void GetViewportSize();
};
