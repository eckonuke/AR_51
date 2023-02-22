// Fill out your copyright notice in the Description page of Project Settings.


#include "GroundDetectionComponent.h"
#include "ARBlueprintLibrary.h"
#include "AR_Player.h"
#include "MainWidget.h"
#include "Components/TextBlock.h"


UGroundDetectionComponent::UGroundDetectionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UGroundDetectionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	player = Cast<AAR_Player>(GetOwner());
}


void UGroundDetectionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 감지 타입이 Gound일 때에만 실행
	if (player->detectionType != EDetectionType::Ground)
	{
		return;
	}

	// 감지된 모든 평면 정보를 가져온다.
	planes = UARBlueprintLibrary::GetAllGeometries();
	DetectGround();
}


void UGroundDetectionComponent::DebugGroundDetection()
{
	// 면 감지를 하고 있는지 디버깅을 해본다.
	

	for (UARTrackedGeometry* plane : planes)
	{
		UARBlueprintLibrary::DebugDrawTrackedGeometry(plane, GetWorld(), FLinearColor(1.0f, 1.0f, 0, 1.0f));
	}
}

void UGroundDetectionComponent::CarControl()
{
	if (IsValid(spawnedCar))
	{
		bool isFirstTouch, isSecondTouch;
		FVector2D first, second;

		// 두 손가락 터치를 체크한다.
		player->playerCon->GetInputTouchState(ETouchIndex::Touch1, first.X, first.Y, isFirstTouch);
		player->playerCon->GetInputTouchState(ETouchIndex::Touch2, second.X, second.Y, isSecondTouch);

		FString firstTouchResult = isFirstTouch ? FString("True") : FString("False");
		FString secondTouchResult = isSecondTouch ? FString("True") : FString("False");

		player->mainUI->logText2->SetText(FText::FromString(FString::Printf(TEXT("%s, %s"), *firstTouchResult, *secondTouchResult)));

		// 두 손가락 사이의 거리에 따라 자동차 모델링의 크기를 늘리거나 줄이고 싶다!
		float currentDist = FVector2D::Distance(first, second);

		if (isFirstTouch && isSecondTouch)
		{
			// 첫 번째 투터치라면 그 거리 값을 저장해둔다.
			if (prev_dist < 0)
			{
				prev_dist = currentDist;
			}
			else
			{
				// 손가락 거리의 변화량만큼 자동차 모델링의 크기를 변경한다.
				float delta = currentDist - prev_dist;
				FVector newScale = spawnedCar->GetActorScale3D() + FVector(delta * 0.001f);

				// 크기 변화는 0.5배 ~ 2배 사이로 제한한다.
				newScale.X = FMath::Clamp(newScale.X, 0.5f, 2.0f);
				newScale.Y = FMath::Clamp(newScale.Y, 0.5f, 2.0f);
				newScale.Z = FMath::Clamp(newScale.Z, 0.5f, 2.0f);

				spawnedCar->SetActorScale3D(newScale);
			}
		}
		else
		{
			// 손가락을 뗐을 때는 prev_dist 값을 -1로 다시 초기화한다.
			prev_dist = -1;
		}
	}
}

void UGroundDetectionComponent::DetectGround()
{
	// 스크린 중앙(해상도 기준)에서 AR 현실 화면에 라인 트레이스를 발사한다.
	float touch_x, touch_y;
	bool bIsTouchFirst = false;

	GetWorld()->GetFirstPlayerController()->GetInputTouchState(ETouchIndex::Touch1, touch_x, touch_y, bIsTouchFirst);


	// 라인 트레이스에 닿은 지점이 감지했던 면이라면...
	TArray<FARTraceResult> hitInfos = UARBlueprintLibrary::LineTraceTrackedObjects(FVector2D(vSize.X / 2, vSize.Y / 2));

	if (hitInfos.Num() > 0)
	{
		if (spawnedIndicator == nullptr)
		{
			// 그 위치에 인디케이터 액터를 생성한다.
			FActorSpawnParameters param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			spawnedIndicator = GetWorld()->SpawnActor<AActor>(indicator, hitInfos[0].GetLocalToWorldTransform(), param);

		}
		else
		{
			spawnedIndicator->SetActorTransform(hitInfos[0].GetLocalToWorldTransform());
		}

		// 인디케이터가 있는 상태에서 화면을 터치했을 때 인디케이터 위치에 자동차 액터를 생성한다.
		if (bIsTouchFirst)
		{
			FActorSpawnParameters param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			// 자동차 모델이 월드에 없을 때에만 모델링 액터를 생성한다.
			if (spawnedCar == nullptr)
			{
				spawnedCar = GetWorld()->SpawnActor<AActor>(carModel, spawnedIndicator->GetTransform(), param);
			}
		}

		CarControl();
	}
	else
	{
		// 인디케이터의 모습이 보이지 않도록 처리한다.
		if (spawnedIndicator != nullptr)
		{
			spawnedIndicator->Destroy();
			spawnedIndicator = nullptr;
		}
	}
}