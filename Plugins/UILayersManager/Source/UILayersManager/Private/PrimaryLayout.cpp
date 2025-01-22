// Copyright 2025, Bit Gaming Studio. All Rights Reserved

#pragma once

#include "PrimaryLayout.h"
#include "UIHUD.h"

void UPrimaryLayout::NativeConstruct()
{
	Super::NativeConstruct();

	PushInitialScreens();
}

bool UPrimaryLayout::RegisterLayer(FGameplayTag LayerName, UUILayer* LayerWidget)
{
	if (!LayerWidget || Layers.Contains(LayerName))
	{
		return false;
	}

	Layers.Add(LayerName, LayerWidget);

	return true;
}

bool UPrimaryLayout::UnregisterLayer(FGameplayTag LayerName)
{
	if (!Layers.Contains(LayerName))
	{
		return false;
	}

	UUILayer* Layer = *Layers.Find(LayerName);

	if (Layer)
	{
		Layer->RemoveFromParent();
	}

	Layers.Remove(LayerName);

	return true;
}

UUserWidget* UPrimaryLayout::PushContentToLayer(FGameplayTag LayerName, TSoftClassPtr<class UUserWidget> WidgetClass)
{
	UUILayer** LayerPtr = Layers.Find(LayerName);
	UUILayer* Layer = *LayerPtr;

	if (!Layer)
	{
		return nullptr;
	}

	return Layer->PushContent(WidgetClass);
}

void UPrimaryLayout::PopContentFromLayer(FGameplayTag LayerName)
{
	UUILayer* Layer = *Layers.Find(LayerName);

	if (!Layer)
	{
		return;
	}

	Layer->PopContent();
}

void UPrimaryLayout::ClearAllLayers()
{
	for (auto& it : Layers)
	{
		it.Value->ClearStack();
	}
}

void UPrimaryLayout::ClearAllLayersExcept(FGameplayTag LayerToKeep)
{
	for (auto& it : Layers)
	{
		if (it.Key != LayerToKeep)
		{
			it.Value->ClearStack();
		}
	}
}

void UPrimaryLayout::ClearLayer(FGameplayTag LayerName)
{
	UUILayer* Layer = *Layers.Find(LayerName);

	if (!Layer)
	{
		return;
	}

	Layer->ClearStack();
}

void UPrimaryLayout::PushInitialScreens()
{
	for (auto& it : Layers)
	{
		APlayerController* Owner = this->GetOwningPlayer();

		if (Owner)
		{
			AUIHUD* HUD = Cast<AUIHUD>(Owner->GetHUD());

			TSoftClassPtr<class UUserWidget>* WidgetClass = InitialScreens.Find(it.Key);

			if (!WidgetClass)
			{
				continue;
			}

			HUD->PushContentToLayer(it.Key, *WidgetClass);
		}
	}
}
