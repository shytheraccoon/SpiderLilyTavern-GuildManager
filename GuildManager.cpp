// Fill out your copyright notice in the Description page of Project Settings.


#include "GuildManager.h"
#include "AdventurerInfo.h"
#include "QuestRankData.h"
#include "QuestRankChance.h"
#include "GuildRank.h"
#include "StatEnum.h"
#include "Algo/RandomShuffle.h"

void GuildManager::GenerateAdventurerGroup(int32 AdCount) // Generate a group of adventurers based on the specified count
{
	Adventurers.Empty();
	for (int i = 0; i < AdCount; i++)
	{
		FAdventurerInfo NewAdventurer = GenerateAdventurer();
		Adventurers.Add(NewAdventurer);
	}
}

FAdventurerInfo GuildManager::GenerateAdventurer() // Generates a single adventurer with random attributes
{
	FAdventurerInfo NewAdventurer;
	bool bFoundName = false;

	// Try picking name from TwitchNames
	for (const FCharacterNameEntry& Entry : TwitchNames)
	{
		if (!IsNameUsed(Entry.Name))
		{
			NewAdventurer.Name = Entry.Name;
			NewAdventurer.Gender = Entry.Gender;
			bFoundName = true;
			break;
		}
	}

	// If not found, pick from CustomNames
	if (!bFoundName)
	{
		for (const FCharacterNameEntry& Entry : CustomNames)
		{
			if (!IsNameUsed(Entry.Name))
			{
				NewAdventurer.Name = Entry.Name;
				NewAdventurer.Gender = Entry.Gender;
				bFoundName = true;
				break;
			}
		}
	}


	// If still not found, pick from CharacterNameTable DataTable
	if (!bFoundName && CharacterNameTable)
	{
		static const FString ContextString(TEXT("Getting Character Names"));
		TArray<FCharacterNameEntry*> AllRows;
		CharacterNameTable->GetAllRows<FCharacterNameEntry>(ContextString, AllRows);

		if (AllRows.Num() > 0)
		{
			// Shuffle the list once to ensure randomness
			Algo::RandomShuffle(AllRows);

			// Pick the first unused name from the shuffled list
			for (FCharacterNameEntry* Entry : AllRows)
			{
				if (Entry && !IsNameUsed(Entry->Name))
				{
					NewAdventurer.Name = Entry->Name;
					NewAdventurer.Gender = Entry->Gender;
					bFoundName = true;
					break;
				}
			}
		}
	}
	
	// Initialize stats (assign default or randomized values as needed) TODO: Implement proper stat generation logic
	NewAdventurer.MaxHealth;
	NewAdventurer.CurrentHealth;
	NewAdventurer.Strength;
	NewAdventurer.Intelligence;
	NewAdventurer.Dexterity;
	NewAdventurer.Wisdom;
	NewAdventurer.Charisma;
	NewAdventurer.Constitution;

	return NewAdventurer;
}

bool GuildManager::IsNameUsed(const FString& Name) // Check if the specified name is already in use
{
	for (const FAdventurerInfo& Adventurer : Adventurers)
	{
		if (Adventurer.Name == Name)
		{
			return true;
		}
	}
	return false;
}


//Quest Generation
void GuildManager::GenerateQuestGroup(int32 QCount) // Generates a group of quests based on the specified count
{
	QuestList.Empty();
	for (int i = 0; i < QCount; i++)
	{
		FQuestInstance NewQuest = GenerateQuest();
		QuestList.Add(NewQuest);
		// Add the new quest to a collection or process it as needed
	}
}

FQuestInstance GuildManager::GenerateQuest() // Generates a single quest
{
	FQuestInstance NewQuest;

	if (!QuestDetailsTable)
	{
		return NewQuest; // Early return if no DataTable
	}

	static const FString ContextString(TEXT("Getting Quest Data"));
	TArray<FQuestDataRow*> AllRows;
	QuestDetailsTable->GetAllRows<FQuestDataRow>(ContextString, AllRows);
	if (AllRows.Num() == 0)
	{
		return NewQuest; // No quests available
	}

	// Pick random quest data
	Algo::RandomShuffle(AllRows);
	NewQuest.QuestData = *AllRows[0];

	// Choose quest rank based on guild rank
	NewQuest.QuestRank = ChooseQuestRank(CurrentGuildRank);

	// Get RankData for this rank (you need a function or DataTable lookup)
	FQuestRankData* RankData = GetRankData(NewQuest.QuestRank);
	if (!RankData)
	{
		return NewQuest; // Fail safe
	}

	// Randomize Duration and XPReward within RankData ranges
	NewQuest.Duration = FMath::RandRange(RankData->MinDuration, RankData->MaxDuration);
	NewQuest.XPReward = FMath::RandRange(RankData->MinXP, RankData->MaxXP);

	// Generate and assign stats based on priority string and min/max stat difficulty
	AssignStatsWithPriority(NewQuest, RankData->MinStatDifficulty, RankData->MaxStatDifficulty, NewQuest.QuestData.StatPriorityString);

	NewQuest.bIsCompleted = false;

	return NewQuest;
}

EQuestRank GuildManager::ChooseQuestRank(EGuildRank GuildRank) // Determine quest rank based on guild rank's chance distribution
{
	const FGuildRankQuestChancesRow* ChancesRow = QuestChances->FindRow<FGuildRankQuestChancesRow>(FName(*GuildRankToString(GuildRank)), TEXT(""));

	if (!ChancesRow)
	{
		return EQuestRank::F; // Default to F rank if no chances row found
	}

	//Roll random float
	float Roll = FMath::FRandRange(0.0f, 100.0f);
	float Accumulated = 0.0f;

	// Iterate through the rank chances to find the appropriate quest rank
	for (const FQuestRankChance& Chance : ChancesRow->RankChances)
	{
		Accumulated += Chance.Chance;
		if (Roll <= Accumulated)
		{
			return Chance.QuestRank; // Return the matched quest rank
		}
	}
	
	return ChancesRow->RankChances.Last().QuestRank;
}

FQuestRankData* GuildManager::GetRankData(EQuestRank QuestRank) // Retrieves the rank data for a given quest rank
{
	if (!QuestRankDataTable)
	{
		return nullptr; // Early return if no DataTable
	}

	FString RankRowName = QuestRankToString(QuestRank);

	FName RowName(*RankRowName);

	return QuestRankDataTable->FindRow<FQuestRankData>(RowName, TEXT("Getting Quest Rank Data"));
}

void GuildManager::AssignStatsWithPriority(FQuestInstance& Quest, int32 MinStat, int32 MaxStat, const FString& StatPriorityString) // Assigns stats to a quest based on the specified priority string
{
	// 1. Generate 6 random stat values
	TArray<int32> StatValues;
	for (int i = 0; i < 6; i++)
	{
		StatValues.Add(FMath::RandRange(MinStat, MaxStat));
	}
	StatValues.Sort([](int32 A, int32 B) { return A > B; }); // Descending order

	// 2. Parse StatPriorityString into groups
	TArray<FString> Groups;
	StatPriorityString.ParseIntoArray(Groups, TEXT("|"), true);

	// 3. For each group, parse individual stats (split by comma)
	int32 ValueIndex = 0;
	for (const FString& Group : Groups)
	{
		TArray<FString> StatsInGroup;
		Group.ParseIntoArray(StatsInGroup, TEXT(","), true);

		// Shuffle stats order within group
		StatsInGroup.Sort([](const FString& A, const FString& B) { return FMath::RandBool(); });

		// Assign stat values from StatValues to these stats
		for (const FString& StatNameStr : StatsInGroup)
		{
			if (ValueIndex >= StatValues.Num()) break;

			int32 StatValue = StatValues[ValueIndex++];
			EStatTypeEnum StatEnum = ConvertStringToStatEnum(StatNameStr);

			// Skip invalid stats
			if (StatEnum == EStatTypeEnum::None)
			{
				UE_LOG(LogTemp, Warning, TEXT("AssignStatsWithPriority: Skipping unknown stat '%s'"), *StatNameStr);
				continue;
			}

			// Assign to the correct stat on Quest
			switch (StatEnum)
			{
			case EStatTypeEnum::Strength: Quest.Strength = StatValue; break;
			case EStatTypeEnum::Constitution: Quest.Constitution = StatValue; break;
			case EStatTypeEnum::Dexterity: Quest.Dexterity = StatValue; break;
			case EStatTypeEnum::Intelligence: Quest.Intelligence = StatValue; break;
			case EStatTypeEnum::Wisdom: Quest.Wisdom = StatValue; break;
			case EStatTypeEnum::Charisma: Quest.Charisma = StatValue; break;
			default: break; // 'None' already handled above
			}
		}
	}
}

EStatTypeEnum GuildManager::ConvertStringToStatEnum(const FString& StatName) // Converts a string representation of a stat name to the corresponding EStatTypeEnum
{
	FString CleanName = StatName.TrimStartAndEnd();

	if (CleanName.Equals(TEXT("Strength"), ESearchCase::IgnoreCase))
	{
		return EStatTypeEnum::Strength;
	}
	else if (CleanName.Equals(TEXT("Intelligence"), ESearchCase::IgnoreCase))
	{
		return EStatTypeEnum::Intelligence;
	}
	else if (CleanName.Equals(TEXT("Dexterity"), ESearchCase::IgnoreCase))
	{
		return EStatTypeEnum::Dexterity;
	}
	else if (CleanName.Equals(TEXT("Wisdom"), ESearchCase::IgnoreCase))
	{
		return EStatTypeEnum::Wisdom;
	}
	else if (CleanName.Equals(TEXT("Charisma"), ESearchCase::IgnoreCase))
	{
		return EStatTypeEnum::Charisma;
	}
	else if (CleanName.Equals(TEXT("Constitution"), ESearchCase::IgnoreCase))
	{
		return EStatTypeEnum::Constitution;
	}

	UE_LOG(LogTemp, Warning, TEXT("ConvertStringToStatEnum: Unknown stat name '%s'"), *StatName);
	return EStatTypeEnum::None; 
}



FString GuildManager::QuestRankToString(EQuestRank Rank) // Converts EQuestRank to a string representation
{
	switch (Rank)
	{
		case EQuestRank::F: return TEXT("F");
		case EQuestRank::E: return TEXT("E");
		case EQuestRank::D: return TEXT("D");
		case EQuestRank::C: return TEXT("C");
		case EQuestRank::B: return TEXT("B");
		case EQuestRank::A: return TEXT("A");
		case EQuestRank::S: return TEXT("S");
		default: return TEXT("Unknown");
	}
}

FString GuildManager::GuildRankToString(EGuildRank Rank) // Converts EGuildRank to a string representation
{
	switch (Rank)
	{
	case EGuildRank::F: return TEXT("F");
	case EGuildRank::E: return TEXT("E");
	case EGuildRank::D: return TEXT("D");
	case EGuildRank::C: return TEXT("C");
	case EGuildRank::B: return TEXT("B");
	case EGuildRank::A: return TEXT("A");
	case EGuildRank::S: return TEXT("S");
	default: return TEXT("Unknown");
	}
}