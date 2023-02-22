// Fill out your copyright notice in the Description page of Project Settings.


#include "FaceComponent.h"
#include "ARBlueprintLibrary.h"
#include "ARTrackable.h"
#include "AR_Player.h"
#include "MainWidget.h"
#include "Components/TextBlock.h"
#include "GoogleARCoreAugmentedFace.h"


UFaceComponent::UFaceComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UFaceComponent::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<AAR_Player>(GetOwner());
}


void UFaceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (player->detectionType != EDetectionType::Face)
	{
		return;
	}

	//FaceCenterDetection();
	partsNumber = player->mainUI->faceNum;
	Face3PointsDectection(partsNumber);
}

void UFaceComponent::FaceCenterDetection()
{
	// 추적된 얼굴 데이터를 가져온다.
	TArray<UARFaceGeometry*> faces = UARBlueprintLibrary::GetAllGeometriesByClass<UARFaceGeometry>();

	player->mainUI->logText1->SetText(FText::AsNumber(faces.Num()));

	for (int32 i = 0; i < faces.Num(); i++)
	{
		FTransform trans = faces[0]->GetLocalToWorldTransform();

		if (spawnedFace == nullptr)
		{
			spawnedFace = GetWorld()->SpawnActor<AActor>(faceActor, trans);
		}
		else
		{
			spawnedFace->SetActorTransform(trans);
		}
	}
}

void UFaceComponent::Face3PointsDectection(int32 part)
{
	TArray<UARFaceGeometry*> faces = UARBlueprintLibrary::GetAllGeometriesByClass<UARFaceGeometry>();

	if (faces.Num() > 0)
	{
		UGoogleARCoreAugmentedFace* faceParts = Cast<UGoogleARCoreAugmentedFace>(faces[0]);

		EGoogleARCoreAugmentedFaceRegion region = EGoogleARCoreAugmentedFaceRegion::NoseTip;

		switch (part)
		{
		case 0:
			region = EGoogleARCoreAugmentedFaceRegion::NoseTip;
			break;
		case 1:
			region = EGoogleARCoreAugmentedFaceRegion::ForeheadLeft;
			break;
		case 2:
			region = EGoogleARCoreAugmentedFaceRegion::ForeheadRight;
			break;
		default:
			break;
		}

		// 지정한 부위의 트랜스폼 정보를 가져온다.
		FTransform trans = faceParts->GetLocalToWorldTransformOfRegion(region);

		if (spawnedFace == nullptr)
		{
			spawnedFace = GetWorld()->SpawnActor<AActor>(faceActor, trans);
		}
		else
		{
			spawnedFace->SetActorTransform(trans);
		}
	}
}

