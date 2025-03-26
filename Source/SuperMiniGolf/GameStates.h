// Copyright 2025, Bit Gaming Studio. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EGameStates : uint8
{
	Countdown,
	Active,
	GoalReached,
	Failed,
	ResultsScreen,
	ChangingLevel,
};
