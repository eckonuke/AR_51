// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"
#include "Components/Button.h"
#include "ARBlueprintLibrary.h"
#include "AR_Player.h"


void UMainWidget::NativeConstruct()
{
	btn_left->OnClicked.AddDynamic(this, &UMainWidget::DecreaseNumber);
	btn_right->OnClicked.AddDynamic(this, &UMainWidget::IncreaseNumber);
	btn_GroundSession->OnClicked.AddDynamic(this, &UMainWidget::ChangeSessionGround);
	btn_FaceSession->OnClicked.AddDynamic(this, &UMainWidget::ChangeSessionFace);
	btn_ImageSession->OnClicked.AddDynamic(this, &UMainWidget::ChangeSessionImage);
}

void UMainWidget::IncreaseNumber()
{
	faceNum = ++faceNum % 3;
}

void UMainWidget::DecreaseNumber()
{
	faceNum = (faceNum + 2) % 3;
}

void UMainWidget::ChangeSessionGround()
{
	UARBlueprintLibrary::StopARSession();
	UARBlueprintLibrary::StartARSession(player->arSessions[0]);
	player->detectionType = EDetectionType::Ground;
}

void UMainWidget::ChangeSessionFace()
{
	UARBlueprintLibrary::StopARSession();
	UARBlueprintLibrary::StartARSession(player->arSessions[1]);
	player->detectionType = EDetectionType::Face;
}

void UMainWidget::ChangeSessionImage()
{
	UARBlueprintLibrary::StopARSession();
	UARBlueprintLibrary::StartARSession(player->arSessions[2]);
	player->detectionType = EDetectionType::Image;
}
