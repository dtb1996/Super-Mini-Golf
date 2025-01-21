// Copyright 2025, Bit Gaming Studio. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameplayTagContainer.h"
#include "PrimaryLayout.h"
#include "UIHUD.generated.h"

UCLASS(Abstract)
class UILAYERSMANAGER_API AUIHUD : public AHUD
{
	GENERATED_BODY()

public:
	//Make BlueprintCallable for testing
	UFUNCTION(BlueprintCallable)
	void ShowMainMenu();

	UFUNCTION(BlueprintCallable)
	void HideMainMenu();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UPrimaryLayout* GetPrimaryLayout() const;

	UFUNCTION(BlueprintCallable)
	bool RegisterLayer(FGameplayTag LayerName, UUILayer* LayerWidget);

	UFUNCTION(BlueprintCallable)
	bool UnregisterLayer(FGameplayTag LayerName);

	UFUNCTION(BlueprintCallable)
	UUserWidget* PushContentToLayer(FGameplayTag LayerName, TSoftClassPtr<class UUserWidget> WidgetClass);

	UFUNCTION(BlueprintCallable)
	void PopContentFromLayer(FGameplayTag LayerName);

	UFUNCTION(BlueprintCallable)
	void ClearAllLayers();

	UFUNCTION(BlueprintCallable)
	void ClearAllLayersExcept(FGameplayTag LayerName);

	UFUNCTION(BlueprintCallable)
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
