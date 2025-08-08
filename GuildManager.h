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
	UFUNCTION(BlueprintCallable)
	void GenerateAdventurerGroup(int32 AdCount);

	UFUNCTION(BlueprintCallable)
	FAdventurerInfo GenerateAdventurer();

	UFUNCTION(BlueprintCallable)
	FQuestInstance GenerateQuest();

	UFUNCTION(BlueprintCallable)
	void GenerateQuestGroup(int32 QCount);

	UFUNCTION(BlueprintCallable)
	EQuestRank ChooseQuestRank(EGuildRank GuildRank);

	UFUNCTION(BlueprintCallable)
	static FString GuildRankToString(EGuildRank Rank);

	UFUNCTION(BlueprintCallable)
	EStatTypeEnum ConvertStringToStatEnum(const FString& StatName);

	UFUNCTION(BlueprintCallable)
	FQuestRankData* GetRankData(EQuestRank QuestRank);

	UFUNCTION(BlueprintCallable)
	static FString QuestRankToString(EQuestRank Rank);

	UFUNCTION(BlueprintCallable)
	void AssignStatsWithPriority(FQuestInstance& Quest, int32 MinStat, int32 MaxStat, const FString& StatPriorityString);

	bool IsNameUsed(const FString& Name);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FAdventurerInfo> Adventurers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FAdventurerInfo> GuildMembers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FQuestInstance> QuestList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FQuestInstance> AcceptedQuest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCharacterNameEntry> TwitchNames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCharacterNameEntry> CustomNames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGuildRank CurrentGuildRank;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Names")
	UDataTable* CharacterNameTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quests")
	UDataTable* QuestDetailsTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest Rank Chances")
	UDataTable* QuestChances;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Guild Ranks")
	UDataTable* QuestRankDataTable;

};
