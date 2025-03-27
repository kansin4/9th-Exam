#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BGameMode.generated.h"

class ABBPlayerController;

UCLASS()
class N1_API ABGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ABGameMode();

    virtual void BeginPlay() override;
    virtual void PostLogin(APlayerController* NewPlayer) override;

    // 플레이어의 추측을 처리
    void ProcessGuess(ABBPlayerController* Player, const FString& Guess);

    // 잘못된 추측 처리
    void HandleInvalidGuess(ABBPlayerController* Player);

protected:
    // 무작위 숫자 생성
    void GenerateRandomNumber();

    // 모든 플레이어에게 메시지 브로드캐스트
    void BroadcastMessage(const FString& Message);

    // 턴 정보 브로드캐스트
    void BroadcastTurnInfo();

    // 현재 플레이어 인덱스
    int32 CurrentPlayerIndex;

    // 정답 숫자
    FString RandomNumber;

    // 플레이어 목록
    TArray<ABBPlayerController*> Players;

    // 각 플레이어의 남은 시도 횟수
    TArray<int32> PlayerAttempts;
};