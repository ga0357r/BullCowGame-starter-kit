// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "CoreMinimal.h"
#include "Console/Cartridge.h"
#include <BullCowGame/BullCowCount.h>
#include "BullCowCartridge.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BULLCOWGAME_API UBullCowCartridge : public UCartridge
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override; //when the game starts
	virtual void OnInput(const FString& playerInput) override; //when the player hits enter

private:
	FString hiddenWord;
	int32 lives;
	bool isGameOver;
	TArray<FString> possibleWords;
	TArray<FString> validWords;
	int32 maxWordNumber;
	BullCowCount bullCowCount;
	
private:
	void SetupGame();
	void ExtractWordsFromFile();
	void EndGame();
	bool IsIsogram(const FString& guess) const;
	TArray<FString> GetValidWords(const TArray<FString>& possibleWords);
	void LoseGame();
	void WinGame();
	void ProcessGuess(const FString& guess);
	BullCowCount GetBullCows(const FString& guess) const;
};
