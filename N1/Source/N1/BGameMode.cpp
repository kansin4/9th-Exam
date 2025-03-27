#include "BGameMode.h"
#include "BBPlayerController.h"

ABGameMode::ABGameMode()
{
    PlayerControllerClass = ABBPlayerController::StaticClass();
    CurrentPlayerIndex = 0;
}

void ABGameMode::BeginPlay()
{
    Super::BeginPlay();
    FMath::SRandInit(static_cast<int32>(FDateTime::Now().GetTicks()));
    GenerateRandomNumber();

    Players.Empty();
    PlayerAttempts.Empty();
    CurrentPlayerIndex = 0;

    // 호스트 플레이어를 Players 배열에 추가
    for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
    {
        APlayerController* PlayerController = Iterator->Get();
        if (PlayerController)
        {
            ABBPlayerController* BBPlayer = Cast<ABBPlayerController>(PlayerController);
            if (BBPlayer)
            {
                Players.Add(BBPlayer);
                PlayerAttempts.Add(3);
                UE_LOG(LogTemp, Log, TEXT("BeginPlay: Added host player to Players. Total Players=%d"), Players.Num());
            }
        }
    }

    UE_LOG(LogTemp, Log, TEXT("BeginPlay: Game initialized."));
}

void ABGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    UE_LOG(LogTemp, Log, TEXT("PostLogin: NewPlayer=%s"), *GetNameSafe(NewPlayer));

    ABBPlayerController* BBPlayer = Cast<ABBPlayerController>(NewPlayer);
    if (BBPlayer)
    {
        Players.Add(BBPlayer);
        PlayerAttempts.Add(3);
        UE_LOG(LogTemp, Log, TEXT("PostLogin: Player added. Total Players=%d"), Players.Num());
        if (Players.Num() == 2)
        {
            BroadcastMessage(TEXT("Game starts now!"));
            BroadcastTurnInfo();
            UE_LOG(LogTemp, Log, TEXT("PostLogin: Game started with 2 players."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("PostLogin: Failed to cast NewPlayer to ABBPlayerController!"));
    }
}

void ABGameMode::BroadcastMessage(const FString& Message)
{
    UE_LOG(LogTemp, Log, TEXT("BroadcastMessage: Sending message=%s to %d players"), *Message, Players.Num());
    for (int32 i = 0; i < Players.Num(); i++)
    {
        if (Players[i])
        {
            UE_LOG(LogTemp, Log, TEXT("BroadcastMessage: Sending to Player %d: %s"), i, *GetNameSafe(Players[i]));
            Players[i]->Client_ReceiveMessage(Message);
        }
    }
}

void ABGameMode::BroadcastTurnInfo()
{
    UE_LOG(LogTemp, Log, TEXT("BroadcastTurnInfo: CurrentPlayerIndex=%d"), CurrentPlayerIndex);
    for (int32 i = 0; i < Players.Num(); i++)
    {
        if (Players[i])
        {
            UE_LOG(LogTemp, Log, TEXT("BroadcastTurnInfo: Sending to Player %d: %s"), i, *GetNameSafe(Players[i]));
            Players[i]->Client_ReceiveTurnInfo(CurrentPlayerIndex, i);
        }
    }
}

void ABGameMode::GenerateRandomNumber()
{
    RandomNumber.Empty();
    while (RandomNumber.Len() < 3)
    {
        FString Digit = FString::FromInt(FMath::RandRange(0, 9));
        if (!RandomNumber.Contains(Digit))
        {
            RandomNumber += Digit;
        }
    }
    UE_LOG(LogTemp, Log, TEXT("GenerateRandomNumber: RandomNumber=%s"), *RandomNumber);
}

void ABGameMode::ProcessGuess(ABBPlayerController* Player, const FString& Guess)
{
    if (Players[CurrentPlayerIndex] != Player)
    {
        Player->Client_ReceiveMessage(TEXT("It's not your turn!"));
        return;
    }

    if (Guess.Len() != 3)
    {
        Player->Client_ReceiveMessage(TEXT("Invalid guess! Please enter 3 digits."));
        return;
    }

    int32 Strikes = 0, Balls = 0;
    for (int32 i = 0; i < 3; i++)
    {
        FString GuessDigit = Guess.Mid(i, 1);
        if (RandomNumber[i] == TCHAR(GuessDigit[0]))
        {
            Strikes++;
        }
        else if (RandomNumber.Contains(GuessDigit))
        {
            Balls++;
        }
    }

    PlayerAttempts[CurrentPlayerIndex]--;
    FString Result = FString::Printf(TEXT("Result: %d Strikes, %d Balls"), Strikes, Balls);
    BroadcastMessage(Result);

    if (Strikes == 3)
    {
        BroadcastMessage(FString::Printf(TEXT("Player %d wins!"), CurrentPlayerIndex + 1));
        for (int32 i = 0; i < Players.Num(); i++)
        {
            Players[i]->Client_ReceiveTurnInfo(CurrentPlayerIndex, i);
        }
        return;
    }

    if (PlayerAttempts[CurrentPlayerIndex] <= 0)
    {
        BroadcastMessage(FString::Printf(TEXT("Player %d ran out of attempts! Player %d wins!"), CurrentPlayerIndex + 1, (CurrentPlayerIndex == 0) ? 2 : 1));
        return;
    }

    CurrentPlayerIndex = (CurrentPlayerIndex + 1) % Players.Num();
    BroadcastTurnInfo();
}

void ABGameMode::HandleInvalidGuess(ABBPlayerController* Player)
{
    Player->Client_ReceiveMessage(TEXT("Invalid input! Please start with / followed by 3 digits (e.g., /123)"));
}