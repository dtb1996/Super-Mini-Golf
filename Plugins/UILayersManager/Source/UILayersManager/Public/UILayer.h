// Copyright 2025, Bit Gaming Studio. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UILayersManagerFunctionLibrary.h"
#include "UILayer.generated.h"

UCLASS(Abstract)
class UILAYERSMANAGER_API UUILayer : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName LayerName;

	UUserWidget* PushContent(TSoftClassPtr<class UUserWidget> WidgetClass);

	void PushContent(TSoftClassPtr<class UUserWidget> WidgetClass, FWidgetLoaderOnCompleteBP Callback);

	void PopContent();

	void ClearStack();

	UUserWidget* Peek() const;

	void CollapseTop();

	void ShowTop();

	UFUNCTION(BlueprintCallable)
	FText GetStackListNames();

	bool IsEmpty() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UBorder* Border;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UUserWidget* PushedWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	TArray<UUserWidget*> Stack;

	UFUNCTION(BlueprintImplementableEvent)
	void RequestAsyncLoadWidget(const TSoftClassPtr<class UUserWidget>& WidgetClass);

	UFUNCTION(BlueprintCallable)
	void OnWidgetLoaded(UUserWidget* LoadedWidget);

	FWidgetLoaderOnCompleteBP CallbackRef;
};
