// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdventurerInfo.h"
#include "CharacterNameEntry.h"
#include "QuestInstance.h"
#include "GuildRank.h"
#include "StatEnum.h"
#include "QuestRankData.h"

/**
 * 
 */
class SPIDERLILYTAVERN_API GuildManager
{
public:

	//Adventurer Generation
	UFUNCTION(BlueprintCallable)
	void GenerateAdventurerGroup(int32 AdCount); // Generates a group of adventurers based on the specified count

	UFUNCTION(BlueprintCallable)
	FAdventurerInfo GenerateAdventurer(); // Generates a single adventurer with random attributes

	bool IsNameUsed(const FString& Name); // Checks if a given name is already used by any adventurer in the Adventurers list

	// Name lists from various sources
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCharacterNameEntry> TwitchNames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCharacterNameEntry> CustomNames;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Names")
	UDataTable* CharacterNameTable;

	// Adventurer management
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FAdventurerInfo> Adventurers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FAdventurerInfo> GuildMembers;

	
	//Quest Generation
	UFUNCTION(BlueprintCallable)
	void GenerateQuestGroup(int32 QCount); // Generates a group of quests based on the specified count

	UFUNCTION(BlueprintCallable)
	FQuestInstance GenerateQuest(); // Generates a single quest

	UFUNCTION(BlueprintCallable)
	EQuestRank ChooseQuestRank(EGuildRank GuildRank); // Chooses a quest rank based on the guild rank

	UFUNCTION(BlueprintCallable)
	FQuestRankData* GetRankData(EQuestRank QuestRank); // Retrieves the rank data from DataTable for a given quest rank

	UFUNCTION(BlueprintCallable)
	void AssignStatsWithPriority(FQuestInstance& Quest, int32 MinStat, int32 MaxStat, const FString& StatPriorityString); // Assigns stats to a quest based on the specified priority string

	UFUNCTION(BlueprintCallable)
	EStatTypeEnum ConvertStringToStatEnum(const FString& StatName); //convert stat name string to enum value

	UFUNCTION(BlueprintCallable)
	static FString QuestRankToString(EQuestRank Rank); // Convert quest rank enum to string

	// Data tables for quests and ranks
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quests")
	UDataTable* QuestDetailsTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest Rank Chances")
	UDataTable* QuestChances;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Guild Ranks")
	UDataTable* QuestRankDataTable;
	
	// Quest lists
	UPROPERTY(EditAnywhere, BlueprintReadWrite) // List of all quests available in the guild
	TArray<FQuestInstance> QuestList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) // List of accepted quests by the guild
	TArray<FQuestInstance> AcceptedQuest;

	// Guild Management
	UFUNCTION(BlueprintCallable)
	static FString GuildRankToString(EGuildRank Rank); // Convert guild rank enum to string

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGuildRank CurrentGuildRank;

};
