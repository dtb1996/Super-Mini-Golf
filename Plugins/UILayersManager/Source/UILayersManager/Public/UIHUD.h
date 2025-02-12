// Copyright 2025, Bit Gaming Studio. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameplayTagContainer.h"
#include "PrimaryLayout.h"
#include "UILayersManagerFunctionLibrary.h"
#include "UIHUD.generated.h"

UCLASS(Abstract)
class UILAYERSMANAGER_API AUIHUD : public AHUD
{
	GENERATED_BODY()

public:
	void ShowMainMenu();

	void HideMainMenu();

	UPrimaryLayout* GetPrimaryLayout() const;

	bool RegisterLayer(FGameplayTag LayerName, UUILayer* LayerWidget);

	bool UnregisterLayer(FGameplayTag LayerName);

	UUserWidget* PushContentToLayer(FGameplayTag LayerName, TSoftClassPtr<class UUserWidget> WidgetClass);

	void PushContentToLayer(FGameplayTag LayerName, TSoftClassPtr<class UUserWidget> WidgetClass, FWidgetLoaderOnCompleteBP Callback);

	void PopContentFromLayer(FGameplayTag LayerName);

	void ClearAllLayers();

	void ClearAllLayersExcept(FGameplayTag LayerName);

	void ClearLayer(FGameplayTag LayerName);

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> MainMenuClass;

	// Keep a pointer to be able to hide it
	UPROPERTY()
	TObjectPtr<UUserWidget> MainMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	UPrimaryLayout* PrimaryLayout;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	TMap<FGameplayTag, TSoftClassPtr<class UUserWidget>> InitialScreens;
};
