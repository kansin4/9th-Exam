#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BBPlayerController.generated.h"

class UBBGameHUD;

UCLASS()
class N1_API ABBPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    // 클라이언트에서 추측 전송
    UFUNCTION(BlueprintCallable, Category = "Baseball")
    void SendGuess(const FString& Guess);

    // 잘못된 입력 처리
    UFUNCTION(BlueprintCallable, Category = "Baseball")
    void HandleInvalidGuess();

    // 서버에서 메시지 수신
    UFUNCTION(Client, Reliable)
    void Client_ReceiveMessage(const FString& Message);

    // 서버에서 턴 정보 수신
    UFUNCTION(Client, Reliable)
    void Client_ReceiveTurnInfo(int32 NewCurrentPlayerIndex, int32 MyIndex);

    // 서버로 추측 전송
    UFUNCTION(Server, Reliable)
    void ProcessGuess(const FString& Guess);

    // 서버로 잘못된 입력 전송
    UFUNCTION(Server, Reliable)
    void Server_HandleInvalidGuess();

protected:

    virtual void BeginPlay() override;

    int32 CurrentPlayerIndex; // 현재 턴의 플레이어 인덱스
    int32 MyPlayerIndex; // 내 플레이어 인덱스

    // HUD 위젯
    UPROPERTY(BlueprintReadWrite, Category = "Baseball")
    UBBGameHUD* GameHUD;

    // HUD 위젯 클래스 (블루프린트에서 설정)
    UPROPERTY(EditAnywhere, Category = "Baseball")
    TSubclassOf<UBBGameHUD> GameHUDClass;
};