// Copyright 2025, Bit Gaming Studio. All Rights Reserved

#pragma once

#include "UIHUD.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void AUIHUD::ShowMainMenu()
{
	// Make widget owned by our PlayerController
	APlayerController* PC = Cast<APlayerController>(GetOwner());
	MainMenu = CreateWidget<UUserWidget>(PC, MainMenuClass);

	MainMenu->AddToViewport();
}

void AUIHUD::HideMainMenu()
{
	if (MainMenu)
	{
		MainMenu->RemoveFromViewport();
		MainMenu = nullptr;
	}
}

UPrimaryLayout* AUIHUD::GetPrimaryLayout() const
{
	return PrimaryLayout;
}

bool AUIHUD::RegisterLayer(FGameplayTag LayerName, UUILayer* LayerWidget)
{
	UPrimaryLayout* Layout = GetPrimaryLayout();

	return Layout->RegisterLayer(LayerName, LayerWidget);
}

bool AUIHUD::UnregisterLayer(FGameplayTag LayerName)
{
	UPrimaryLayout* Layout = GetPrimaryLayout();

	return Layout->UnregisterLayer(LayerName);
}

UUserWidget* AUIHUD::PushContentToLayer(FGameplayTag LayerName, TSoftClassPtr<class UUserWidget> WidgetClass)
{
	UPrimaryLayout* Layout = GetPrimaryLayout();

	return Layout->PushContentToLayer(LayerName, WidgetClass);
}

void AUIHUD::PopContentFromLayer(FGameplayTag LayerName)
{
	UPrimaryLayout* Layout = GetPrimaryLayout();

	Layout->PopContentFromLayer(LayerName);
}

void AUIHUD::ClearAllLayers()
{
	UPrimaryLayout* Layout = GetPrimaryLayout();

	Layout->ClearAllLayers();
}

void AUIHUD::ClearAllLayersExcept(FGameplayTag LayerName)
{
	UPrimaryLayout* Layout = GetPrimaryLayout();

	Layout->ClearAllLayersExcept(LayerName);
}

void AUIHUD::ClearLayer(FGameplayTag LayerName)
{
	UPrimaryLayout* Layout = GetPrimaryLayout();

	Layout->ClearLayer(LayerName);
}