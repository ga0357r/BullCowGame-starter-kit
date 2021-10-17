// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
//#include "Math/UnrealMathUtility.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if (isGameOver == true)
    {
        ClearScreen();
        SetupGame();
    }

    else ProcessGuess(Input);
}

void UBullCowCartridge::ProcessGuess(const FString& guess)
{
    if (guess == hiddenWord)
    {
        WinGame();
        return;
    }

    if (guess.Len() != hiddenWord.Len())
    {
        PrintLine(TEXT("The hidden word is %i letters long"), hiddenWord.Len());
        return;
    }

    if (!IsIsogram(guess))
    {
        PrintLine(TEXT("Guess again, no repeating letters"));
        return;
    }
      
    lives--;
    PrintLine(TEXT("You lost a life \n%i lives left"), lives);

    if (lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You have no lives left"));
        PrintLine(TEXT("The hidden word was: %s"), *hiddenWord);
        EndGame();
        return;    
    }

    bullCowCount = GetBullCows(guess);
    PrintLine(TEXT("You have %i bulls and %i cows"), bullCowCount.bullCount, bullCowCount.cowCount);
    PrintLine(TEXT("Guess again, you have %i lives left"), lives);
}

void UBullCowCartridge::LoseGame()
{
    PrintLine(TEXT("You have no lives left! \nYou lose!"));
    EndGame();
}

void UBullCowCartridge::WinGame()
{
    PrintLine(TEXT("You have won!"));
    EndGame();
}

void UBullCowCartridge::SetupGame()
{
    ExtractWordsFromFile();
    validWords = GetValidWords(possibleWords);
    hiddenWord = validWords[FMath::RandRange(0, validWords.Num() - 1)];
    lives = hiddenWord.Len() * 2;
    isGameOver = false;
    PrintLine(TEXT("Welcome to Bull Cows!"));
    PrintLine(TEXT("Guess the %i letter word"), hiddenWord.Len());
    PrintLine(TEXT("You have %i lives"), lives);
    PrintLine(TEXT("Type in your guess and \npress enter to continue"));
}

void UBullCowCartridge::ExtractWordsFromFile()
{
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArray(possibleWords, *WordListPath);
}

void UBullCowCartridge::EndGame()
{
    isGameOver = true;
    PrintLine(TEXT("Press enter to continue..."));
}

BullCowCount UBullCowCartridge::GetBullCows(const FString &guess) const
{
    BullCowCount bullCowCount;

    for (int32 guessIndex = 0; guessIndex < guess.Len(); guessIndex++)
    {
        if (guess[guessIndex] == hiddenWord[guessIndex])
        {
            bullCowCount.bullCount++;
            continue;
        }

        for (int32 hiddenIndex = 0; hiddenIndex < hiddenWord.Len(); hiddenIndex++)
        {
            if (guess[guessIndex] == hiddenWord[hiddenIndex])
            {
                bullCowCount.cowCount++;
                break;
            }
        }
    }

    return bullCowCount;
}

bool UBullCowCartridge::IsIsogram(const FString& guess) const
{
    for (int index = 0; index < guess.Len(); index++)
    {
        for (int comparison = index + 1; comparison < guess.Len(); comparison++)
        {
            if (guess[index] == guess[comparison]) return false;
        }
    }

    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& possibleWords)
{
    maxWordNumber = possibleWords.Num();
    TArray<FString> validWords;

    for(FString word : possibleWords)
    {
        if (IsIsogram(word)) if (word.Len() >= 4 && word.Len() <= 8) validWords.Emplace(word);
    }

    return validWords;
}

