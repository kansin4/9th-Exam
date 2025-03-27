#include "BBPlayerController.h"
#include "BGameMode.h"
#include "BBGameHUD.h"

void ABBPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // 클라이언트에서만 HUD 생성
    if (IsLocalController() && GameHUDClass)
    {
        GameHUD = CreateWidget<UBBGameHUD>(this, GameHUDClass);
        if (GameHUD)
        {
            GameHUD->AddToViewport();
        }
    }
}

void ABBPlayerController::Client_ReceiveMessage_Implementation(const FString& Message)
{
    UE_LOG(LogTemp, Log, TEXT("Client_ReceiveMessage: Received message=%s on %s"), *Message, *GetName());
    if (GameHUD)
    {
        GameHUD->UpdateTurn(CurrentPlayerIndex == MyPlayerIndex, Message);
        UE_LOG(LogTemp, Log, TEXT("Client_ReceiveMessage: Updated GameHUD with message=%s"), *Message);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Client_ReceiveMessage: GameHUD is null on %s!"), *GetName());
    }
}

void ABBPlayerController::Client_ReceiveTurnInfo_Implementation(int32 NewCurrentPlayerIndex, int32 MyIndex)
{
    CurrentPlayerIndex = NewCurrentPlayerIndex;
    MyPlayerIndex = MyIndex;
    UE_LOG(LogTemp, Log, TEXT("Client_ReceiveTurnInfo: My Index=%d, Current Player Index=%d on %s"), MyPlayerIndex, CurrentPlayerIndex, *GetName());

    if (GameHUD)
    {
        GameHUD->UpdateTurn(CurrentPlayerIndex == MyPlayerIndex, TEXT(""));
        UE_LOG(LogTemp, Log, TEXT("Client_ReceiveTurnInfo: Updated GameHUD on %s"), *GetName());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Client_ReceiveTurnInfo: GameHUD is null on %s!"), *GetName());
    }
}
void ABBPlayerController::SendGuess(const FString& Guess)
{
    ProcessGuess(Guess);
}

void ABBPlayerController::HandleInvalidGuess()
{
    Server_HandleInvalidGuess();
}

void ABBPlayerController::ProcessGuess_Implementation(const FString& Guess)
{
    ABGameMode* GameMode = Cast<ABGameMode>(GetWorld()->GetAuthGameMode());
    if (GameMode)
    {
        GameMode->ProcessGuess(this, Guess);
    }
}

void ABBPlayerController::Server_HandleInvalidGuess_Implementation()
{
    ABGameMode* GameMode = Cast<ABGameMode>(GetWorld()->GetAuthGameMode());
    if (GameMode)
    {
        GameMode->HandleInvalidGuess(this);
    }
}