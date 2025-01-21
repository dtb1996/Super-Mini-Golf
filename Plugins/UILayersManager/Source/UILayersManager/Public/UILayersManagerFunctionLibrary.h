// // Copyright 2025, Bit Gaming Studio. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UILayersManagerFunctionLibrary.generated.h"

UCLASS()
class UILAYERSMANAGER_API UUILayersManagerFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	static bool RegisterLayer(APlayerController* OwningPlayer, FGameplayTag LayerName, UUILayer* LayerWidget);

	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	static UUserWidget* PushContentToLayer(APlayerController* OwningPlayer, FGameplayTag LayerName, TSoftClassPtr<UUserWidget> WidgetClass);

	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	static void PopContentFromLayer(APlayerController* OwningPlayer, FGameplayTag LayerName);

	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	static void ClearAllLayers(APlayerController* OwningPlayer);

	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	static void ClearLayer(APlayerController* OwningPlayer, FGameplayTag LayerName);

	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	static void ClearAllLayersExcept(APlayerController* OwningPlayer, FGameplayTag ExceptionLayerName);

	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	static void FocusGame(APlayerController* OwningPlayer);

	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	static void FocusModal(APlayerController* OwningPlayer, UWidget* WidgetToFocus, bool PauseGame, bool ShowMouseCursor, bool UIOnlyInput);	
};
