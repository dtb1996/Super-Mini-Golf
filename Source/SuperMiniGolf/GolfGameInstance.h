// Copyright 2025, Bit Gaming Studio. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PlayerStats.h"
#include "GolfGameInstance.generated.h"

UCLASS(Abstract)
class SUPERMINIGOLF_API UGolfGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Player Info")
	void SetPlayerStats(const int PlayerId, const FPlayerStats NewPlayerStats);

	UFUNCTION(BlueprintCallable, Category = "Player Info")
	const FPlayerStats GetPlayerStats(const int PlayerId, bool& Found) const;

	UFUNCTION(BlueprintCallable, Category = "Player Info")
	void ClearPlayerStats();

	UFUNCTION(BlueprintCallable, Category = "Player Info")
	void SetPlayerScore(const int PlayerId, const int NewPlayerScore);

private:
	TMap<int, FPlayerStats> PlayerStats;
};
