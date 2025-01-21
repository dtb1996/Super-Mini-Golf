// Copyright 2025, Bit Gaming Studio. All Rights Reserved

#include "UILayer.h"
#include "Components/Border.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetStringLibrary.h"

void UUILayer::PushWidget(TSoftClassPtr<UUserWidget> LayerWidget)
{
	// TODO: fix this function (may need to be removed)
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("PushWidget() called"));
}

UUserWidget* UUILayer::PushContent(TSoftClassPtr<class UUserWidget> WidgetClass)
{
	if (WidgetClass.IsNull())
	{
		return nullptr;
	}

	APlayerController* Owner = this->GetOwningPlayer();

	PushedWidget = CreateWidget<UUserWidget>(Owner, UKismetSystemLibrary::LoadClassAsset_Blocking(WidgetClass)); //TODO: update to use async load

	CollapseTop();

	Border->ClearChildren();

	Stack.Add(PushedWidget);

	Border->AddChild(PushedWidget);

	ShowTop();

	return PushedWidget;
}

void UUILayer::PopContent()
{
	if (IsEmpty())
	{
		return;
	}

	UUserWidget* TopWidget = Peek();

	if (!TopWidget)
	{
		return;
	}

	TopWidget->RemoveFromParent();

	Stack.Pop();

	Border->ClearChildren();

	TopWidget = Peek();

	if (!TopWidget)
	{
		return;
	}

	TopWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	Border->AddChild(TopWidget);
}

void UUILayer::ClearStack()
{
	for (int i = 0; i < Stack.Num(); i++)
	{
		if (Stack[i])
		{
			Stack[i]->RemoveFromParent();
		}
	}

	Stack.Empty();

	Border->ClearChildren();
}

UUserWidget* UUILayer::Peek() const
{
	return !IsEmpty() ? Stack.Last() : nullptr;
}

void UUILayer::CollapseTop()
{
	if (Peek())
	{
		Peek()->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUILayer::ShowTop()
{
	if (Peek())
	{
		Peek()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

FText UUILayer::GetStackListNames()
{
	TArray<FString>StackNames;

	for (int i = 0; i < Stack.Num(); i++)
	{
		StackNames.Push(UKismetSystemLibrary::GetDisplayName(Stack[i]));
	}

	return FText::FromString(UKismetStringLibrary::JoinStringArray(StackNames, "\n"));
}

bool UUILayer::IsEmpty() const
{
	return Stack.IsEmpty();
}
