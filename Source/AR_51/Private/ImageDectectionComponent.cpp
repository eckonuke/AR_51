// Fill out your copyright notice in the Description page of Project Settings.


#include "ImageDectectionComponent.h"
#include "AR_Player.h"
#include "ARBlueprintLibrary.h"
#include "MainWidget.h"
#include "Components/TextBlock.h"
#include "ARTrackable.h"
#include "Containers/Map.h"

UImageDectectionComponent::UImageDectectionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UImageDectectionComponent::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<AAR_Player>(GetOwner());
	/*FOnTrackableRemovedDelegate dele;
	dele.BindUObject(this, &UImageDectectionComponent::TestFunc);
	UARBlueprintLibrary::AddOnTrackableRemovedDelegate_Handle(dele);*/
}


void UImageDectectionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 이미지 디텍션 중에만 실행되도록 처리
	if (player != nullptr)
	{
		if (player->detectionType != EDetectionType::Image)
		{
			return;
		}
	}

	DetectImages();

}

void UImageDectectionComponent::DetectImages()
{
	TArray<UARTrackedImage*> trackedImages = UARBlueprintLibrary::GetAllGeometriesByClass<UARTrackedImage>();

	player->mainUI->logText1->SetText(FText::AsNumber(trackedImages.Num()));

	// 만일, 추적된 이미지가 있다면...
	if (trackedImages.Num() > 0)
	{
		FString fName;

		// 해당 이미지의 Friendly Name을 로그로 출력해본다.
		for (UARTrackedImage* image : trackedImages)
		{
			EARTrackingState curState = image->GetTrackingState();
			FString trackingState = UEnum::GetValueAsString(curState);

			//player->mainUI->logText3->SetText(FText::FromString(TEXT("I've tracking state...")));
			player->mainUI->logText3->SetText(FText::FromString(trackingState));
			UARCandidateImage* myImage = image->GetDetectedImage();

			if (myImage == nullptr)
			{
				player->mainUI->logText2->SetText(FText::FromString("Image NULL"));

			}
			else
			{
				// 이름을 로그로 출력해본다.
				fName.Append(myImage->GetFriendlyName());
				fName.Append(TEXT(", "));
				player->mainUI->logText2->SetText(FText::FromString(fName));

				// 그 이미지에 해당하는 모델링을 생성한다.
				if (trackingTable.Contains(myImage))
				{
					FString friendlyName = myImage->GetFriendlyName();
					FTransform trans = image->GetLocalToWorldTransform();

					if (spawnedData.Contains(friendlyName) == false && curState == EARTrackingState::Tracking)
					{
						TSubclassOf<AActor> model = trackingTable[myImage];
						AActor* spawnedModel = GetWorld()->SpawnActor<AActor>(model, trans);

						// 이름과 생성된 모델을 spawnedData 변수에 넣는다.
						spawnedData.Add(friendlyName, spawnedModel);
					}
					else if(spawnedData.Contains(friendlyName) && curState == EARTrackingState::Tracking)
					{
						// 그 이름에 해당하는 모델링의 트랜스폼을 갱신한다.
						spawnedData[friendlyName]->SetActorTransform(trans);
					}
					// 현재 생성되어 있고, 트래킹 상태는 아닐때
					else if (spawnedData.Contains(friendlyName) && curState == EARTrackingState::NotTracking)
					{
						// TMap에서 제거하고, 모델링 액터도 삭제한다.
						AActor* removeActor = spawnedData[friendlyName];
						spawnedData.Remove(friendlyName);
						removeActor->Destroy();
					}
				}
			}
		}
	}
	// 추적을 놓쳤다면...
	else
	{
		player->mainUI->logText3->SetText(FText::FromString(TEXT("miss tracking")));

		for (TPair<FString, AActor*> actor : spawnedData)
		{
			AActor* spawnedActor = actor.Value;

			//player->mainUI->logText2->SetText(FText::FromString(FString::Printf(TEXT("%s"), *spawnedActor->GetName())));

			spawnedActor->Destroy();
		}
		spawnedData.Empty();
	}
}

void UImageDectectionComponent::TestFunc(UARTrackedGeometry* geo)
{
	UARTrackedImage* tImage = Cast<UARTrackedImage>(geo);
	FString name = tImage->GetDetectedImage()->GetFriendlyName();

	player->mainUI->logText2->SetText(FText::FromString(FString::Printf(TEXT("%s"), *name)));
	spawnedData[name]->Destroy();
}

