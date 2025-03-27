// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BBFuncLibrary.generated.h"

USTRUCT(BlueprintType)
struct FBaseballGuessResult {
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    int32 Strikes = 0;
    UPROPERTY(BlueprintReadOnly)
    int32 Balls = 0;
    UPROPERTY(BlueprintReadOnly)
    bool bIsOut = false;
};

UCLASS()
class N1_API UBBFuncLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable)
    static FString GenerateRandomNumber();

    // 입력값과 목표 숫자를 비교해 S/B/OUT 계산
    UFUNCTION(BlueprintCallable)
    static FBaseballGuessResult CalculateGuessResult(const FString& TargetNumber, const FString& Guess);
};
