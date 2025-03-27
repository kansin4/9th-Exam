// Fill out your copyright notice in the Description page of Project Settings.


#include "BBFuncLibrary.h"
#include "Kismet/KismetMathLibrary.h"

FString UBBFuncLibrary::GenerateRandomNumber()
{
    TArray<int32> Numbers = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    FString Result;

    // 3자리 숫자 무작위 선택
    for (int32 i = 0; i < 3; i++) {
        int32 Index = UKismetMathLibrary::RandomIntegerInRange(0, Numbers.Num() - 1);
        Result.Append(FString::FromInt(Numbers[Index]));
        Numbers.RemoveAt(Index);
    }
    return Result;
}
FBaseballGuessResult UBBFuncLibrary::CalculateGuessResult(const FString& TargetNumber, const FString& Guess)
{
    FBaseballGuessResult Result;

    // 유효성 검사: 3자리 숫자인지, 중복 없어야 함
    if (Guess.Len() != 3 || Guess.Contains("0")) {
        Result.bIsOut = true;
        return Result;
    }
    TSet<TCHAR> GuessSet;
    for (TCHAR Char : Guess)
    {
        if (GuessSet.Contains(Char)) { // 중복 체크
            Result.bIsOut = true;
            return Result;
        }
        GuessSet.Add(Char);
    }

    // S/B 계산
    for (int32 i = 0; i < 3; i++) {
        if (Guess[i] == TargetNumber[i]) {
            Result.Strikes++;
        }
        else if (TargetNumber.Contains(FString::Chr(Guess[i]))) {
            Result.Balls++;
        }
    }
    Result.bIsOut = (Result.Strikes == 0 && Result.Balls == 0);
    return Result;
}