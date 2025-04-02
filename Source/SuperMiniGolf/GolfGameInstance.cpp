// Copyright 2025, Bit Gaming Studio. All Rights Reserved

#include "GolfGameInstance.h"

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
