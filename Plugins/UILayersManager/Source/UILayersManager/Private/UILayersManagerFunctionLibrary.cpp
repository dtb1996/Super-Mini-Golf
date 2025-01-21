// // Copyright 2025, Bit Gaming Studio. All Rights Reserved

#include "UILayersManagerFunctionLibrary.h"
#include "UIHUD.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

bool UUILayersManagerFunctionLibrary::RegisterLayer(APlayerController* OwningPlayer, FGameplayTag LayerName, UUILayer* LayerWidget)
{
	if (!OwningPlayer)
	{
		return false;
	}

	AUIHUD* PlayerHUD = Cast<AUIHUD>(OwningPlayer->GetHUD());

	if (!PlayerHUD)
	{
		return false;
	}

	return PlayerHUD->RegisterLayer(LayerName, LayerWidget);
}

UUserWidget* UUILayersManagerFunctionLibrary::PushContentToLayer(APlayerController* OwningPlayer, FGameplayTag LayerName, TSoftClassPtr<UUserWidget> WidgetClass)
{
	if (!OwningPlayer)
	{
		return nullptr;
	}

	AUIHUD* PlayerHUD = Cast<AUIHUD>(OwningPlayer->GetHUD());

	if (!PlayerHUD)
	{
		return nullptr;
	}

	return PlayerHUD->PushContentToLayer(LayerName, WidgetClass);
}

void UUILayersManagerFunctionLibrary::PopContentFromLayer(APlayerController* OwningPlayer, FGameplayTag LayerName)
{
	if (!OwningPlayer)
	{
		return;
	}

	AUIHUD* PlayerHUD = Cast<AUIHUD>(OwningPlayer->GetHUD());

	if (!PlayerHUD)
	{
		return;
	}

	PlayerHUD->PopContentFromLayer(LayerName);
}

void UUILayersManagerFunctionLibrary::ClearAllLayers(APlayerController* OwningPlayer)
{
	if (!OwningPlayer)
	{
		return;
	}

	AUIHUD* PlayerHUD = Cast<AUIHUD>(OwningPlayer->GetHUD());

	if (!PlayerHUD)
	{
		return;
	}

	PlayerHUD->ClearAllLayers();
}

void UUILayersManagerFunctionLibrary::ClearLayer(APlayerController* OwningPlayer, FGameplayTag LayerName)
{
	if (!OwningPlayer)
	{
		return;
	}

	AUIHUD* PlayerHUD = Cast<AUIHUD>(OwningPlayer->GetHUD());

	if (!PlayerHUD)
	{
		return;
	}

	PlayerHUD->ClearLayer(LayerName);
}

void UUILayersManagerFunctionLibrary::ClearAllLayersExcept(APlayerController* OwningPlayer, FGameplayTag ExceptionLayerName)
{
	if (!OwningPlayer)
	{
		return;
	}

	AUIHUD* PlayerHUD = Cast<AUIHUD>(OwningPlayer->GetHUD());

	if (!PlayerHUD)
	{
		return;
	}

	PlayerHUD->ClearAllLayersExcept(ExceptionLayerName);
}

void UUILayersManagerFunctionLibrary::FocusGame(APlayerController* OwningPlayer)
{
	if (!OwningPlayer)
	{
		return;
	}

	if (!OwningPlayer)
	{
		return;
	}

	OwningPlayer->SetShowMouseCursor(false);

	UWidgetBlueprintLibrary::SetInputMode_GameOnly(OwningPlayer, true);

	UWidgetBlueprintLibrary::SetFocusToGameViewport();

	OwningPlayer->SetPause(false);
}

void UUILayersManagerFunctionLibrary::FocusModal(APlayerController* OwningPlayer, UWidget* WidgetToFocus, bool PauseGame, bool ShowMouseCursor, bool UIOnlyInput)
{
	if (!OwningPlayer)
	{
		return;
	}

	if (!OwningPlayer)
	{
		return;
	}

	OwningPlayer->SetShowMouseCursor(false);

	UWidgetBlueprintLibrary::SetInputMode_GameOnly(OwningPlayer, true);

	UWidgetBlueprintLibrary::SetFocusToGameViewport();

	OwningPlayer->SetPause(false);
}
