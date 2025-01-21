// Copyright 2025, Bit Gaming Studio. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UILayer.generated.h"

UCLASS(Abstract)
class UILAYERSMANAGER_API UUILayer : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName LayerName;

	UFUNCTION(BlueprintCallable) //TODO: remove
	void PushWidget(TSoftClassPtr<UUserWidget> LayerWidget);

	UFUNCTION(BlueprintCallable)
	UUserWidget* PushContent(TSoftClassPtr<class UUserWidget> WidgetClass);

	UFUNCTION(BlueprintCallable)
	void PopContent();

	UFUNCTION(BlueprintCallable)
	void ClearStack();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UUserWidget* Peek() const;

	UFUNCTION(BlueprintCallable)
	void CollapseTop();

	UFUNCTION(BlueprintCallable)
	void ShowTop();

	UFUNCTION(BlueprintCallable)
	FText GetStackListNames();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsEmpty() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UBorder* Border;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UUserWidget* PushedWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	TArray<UUserWidget*> Stack;
};
