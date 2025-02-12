// Copyright 2025, Bit Gaming Studio. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "UILayer.h"
#include "UILayersManagerFunctionLibrary.h"
#include "PrimaryLayout.generated.h"

UCLASS(Abstract)
class UILAYERSMANAGER_API UPrimaryLayout : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"), Category = "Config")
	TMap<FGameplayTag, TSoftClassPtr<class UUserWidget>> InitialScreens;

	UFUNCTION(BlueprintCallable)
	bool RegisterLayer(FGameplayTag LayerName, UUILayer* LayerWidget);

	UFUNCTION(BlueprintCallable)
	bool UnregisterLayer(FGameplayTag LayerName);

	UUserWidget* PushContentToLayer(FGameplayTag LayerName, TSoftClassPtr<class UUserWidget> WidgetClass);

	void PushContentToLayer(FGameplayTag LayerName, TSoftClassPtr<class UUserWidget> WidgetClass, FWidgetLoaderOnCompleteBP Callback);

	void PopContentFromLayer(FGameplayTag LayerName);

	void ClearAllLayers();

	void ClearAllLayersExcept(FGameplayTag LayerToKeep);

	void ClearLayer(FGameplayTag LayerName);

	void PushInitialScreens();

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UOverlay* Overlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	TMap<FGameplayTag, UUILayer*> Layers;
};
