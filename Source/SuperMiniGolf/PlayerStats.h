// Copyright 2024, Bit Gaming Studio. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "PlayerStats.generated.h"

USTRUCT(BlueprintType)
struct FPlayerStats : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentCoins;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PlayerScore;
};