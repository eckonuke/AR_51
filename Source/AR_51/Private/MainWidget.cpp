// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"
#include "Components/Button.h"


void UMainWidget::NativeConstruct()
{
	btn_left->OnClicked.AddDynamic(this, &UMainWidget::DecreaseNumber);
	btn_right->OnClicked.AddDynamic(this, &UMainWidget::IncreaseNumber);
}

void UMainWidget::IncreaseNumber()
{
	faceNum = ++faceNum % 3;
}

void UMainWidget::DecreaseNumber()
{
	faceNum = (faceNum + 2) % 3;
}
