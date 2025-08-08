# SpiderLilyTavern - GuildManager Module

This repository contains the **GuildManager** system from the **SpiderLilyTavern** Unreal Engine 5 project. The GuildManager handles core gameplay systems such as adventurer generation, quest generation, quest rank scaling, and stat assignment logic.

---

## Overview

**GuildManager** is a central gameplay manager that:

- Generates randomized quests with scalable difficulty (Rank F to S)
- Assigns adventurer stats based on quest requirements and stat priority
- Handles quest timing, XP rewards, and stat difficulty scaling via DataTables
- Implements custom logic for quest rank probability based on guild rank
- Interfaces with quest data and rank data stored in Unreal Engine DataTables

---

## Key Features

- **Quest Generation:** Picks random quest data and assigns quest rank based on guild rank with weighted chances.
- **Stat Assignment:** Generates random stat values and assigns them to quest stats according to priority strings, including randomization within groups.
- **Scalable Difficulty:** Uses DataTables to control quest duration, XP, and stat difficulty ranges per rank.
- **Blueprint Friendly:** Several functions are exposed to Blueprint for easy UI integration and iteration.

---

## Usage

- The system is implemented in C++ for Unreal Engine 5.
- Requires Unreal Engine DataTables for `QuestDetails` and `QuestRankData` (not included here).
- Functions like `GenerateQuest()` and `AssignStatsWithPriority()` are ready to be integrated into your project’s game flow.

---

## Files Included

- **GuildManager.h / GuildManager.cpp:** Core system implementation handling quests, stats, and rank logic.
- **Supporting Enums and Structs:** Includes custom enums like `EQuestRank` and `EStatTypeEnum` for flexible stat and rank handling.

---

## Future Work

- Adventurer assignment and success/failure calculations
- Guild leveling and adventurer leveling
- Quest completion logic with injury and death outcomes
- Integration with UI and gameplay systems for a full playable experience

---

## About

This is a personal project by Marco Alba aiming to demonstrate systems design and UE5 gameplay programming skills.

Feel free to explore, fork, or contribute!

---

## License

This project is licensed under the MIT License. See the LICENSE file for details.
