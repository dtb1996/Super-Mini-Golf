// Copyright 2025, Bit Gaming Studio. All Rights Reserved

#include "GolfGameInstance.h"
#include "GameFramework/InputDeviceSubsystem.h"
#include "Engine/Engine.h"

void UGolfGameInstance::Init()
{
	Super::Init();

	UInputDeviceSubsystem* InputDeviceSubsystem = GEngine->GetEngineSubsystem<UInputDeviceSubsystem>();
	if (InputDeviceSubsystem)
	{
		InputDeviceSubsystem->OnInputHardwareDeviceChanged.AddDynamic(this, &UGolfGameInstance::OnInputTypeChanged);
	}
}

void UGolfGameInstance::Shutdown()
{
	UInputDeviceSubsystem* InputDeviceSubsystem = GEngine->GetEngineSubsystem<UInputDeviceSubsystem>();
	if (InputDeviceSubsystem)
	{
		InputDeviceSubsystem->OnInputHardwareDeviceChanged.RemoveDynamic(this, &UGolfGameInstance::OnInputTypeChanged);
	}

	Super::Shutdown();
}

const bool UGolfGameInstance::GetIsGamepadInput(const FPlatformUserId UserId) const
{
	UInputDeviceSubsystem* InputDeviceSubsystem = GEngine->GetEngineSubsystem<UInputDeviceSubsystem>();
	if (!InputDeviceSubsystem)
	{
		return false;
	}

	FHardwareDeviceIdentifier HardwareDevice = InputDeviceSubsystem->GetMostRecentlyUsedHardwareDevice(UserId);
	if (!HardwareDevice.IsValid())
	{
		return false;
	}

	FString DeviceTypeString = UEnum::GetValueAsString(HardwareDevice.PrimaryDeviceType);

	return DeviceTypeString.Contains(TEXT("Gamepad"));
}

void UGolfGameInstance::SetPlayerStats(const int PlayerId, const FPlayerStats NewPlayerStats)
{
	PlayerStats.Add(PlayerId, NewPlayerStats);
}

const FPlayerStats UGolfGameInstance::GetPlayerStats(const int PlayerId, bool& Found) const
{
	Found = PlayerStats.Contains(PlayerId);

	if (!Found)
	{
		return FPlayerStats();
	}

	return *PlayerStats.Find(PlayerId);
}

void UGolfGameInstance::ClearPlayerStats()
{
	PlayerStats.Empty();
}

void UGolfGameInstance::SetPlayerScore(const int PlayerId, const int NewPlayerScore)
{
	if (!PlayerStats.Contains(PlayerId))
	{
		FPlayerStats NewStats = FPlayerStats();
		NewStats.PlayerScore = NewPlayerScore;

		PlayerStats.Add(PlayerId, NewStats);
	}

	FPlayerStats* FoundStats = PlayerStats.Find(PlayerId);
	FoundStats->PlayerScore = NewPlayerScore;

	PlayerStats.Add(PlayerId, *FoundStats);
}

void UGolfGameInstance::OnInputTypeChanged(const FPlatformUserId UserId, const FInputDeviceId DeviceId)
{
	bIsGamepadInput = GetIsGamepadInput(UserId);

	OnInputHardwareChanged.Broadcast(bIsGamepadInput);
}
