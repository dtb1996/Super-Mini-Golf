// Copyright 2025, Bit Gaming Studio. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PlayerStats.h"
#include "GolfGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInputHardwareChanged, bool, bIsGamepadInput);

UCLASS(Abstract)
class SUPERMINIGOLF_API UGolfGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;

	virtual void Shutdown() override;

	UPROPERTY(BlueprintAssignable, Category = "Player Input")
	FOnInputHardwareChanged OnInputHardwareChanged;

	UFUNCTION(BlueprintCallable, Category = "Player Input")
	const bool GetIsGamepadInput(const FPlatformUserId UserId) const;

	UFUNCTION(BlueprintCallable, Category = "Player Info")
	void SetPlayerStats(const int PlayerId, const FPlayerStats NewPlayerStats);

	UFUNCTION(BlueprintCallable, Category = "Player Info")
	const FPlayerStats GetPlayerStats(const int PlayerId, bool& Found) const;

	UFUNCTION(BlueprintCallable, Category = "Player Info")
	void ClearPlayerStats();

	UFUNCTION(BlueprintCallable, Category = "Player Info")
	void SetPlayerScore(const int PlayerId, const int NewPlayerScore);

private:
	UFUNCTION()
	void OnInputTypeChanged(const FPlatformUserId UserId, const FInputDeviceId DeviceId);

	FDelegateHandle InputDeviceChangedHandle;

	bool bIsGamepadInput = false;

	TMap<int, FPlayerStats> PlayerStats;
};
