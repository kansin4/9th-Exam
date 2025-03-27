#include "BBGameHUD.h"
#include "BBPlayerController.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"

void UBBGameHUD::NativeConstruct()
{
    Super::NativeConstruct();

    // 플레이어 컨트롤러 가져오기
    APlayerController* OwningPlayer = GetOwningPlayer();
    if (OwningPlayer && OwningPlayer->GetClass()->IsChildOf(ABBPlayerController::StaticClass()))
    {
        MyPlayerController = Cast<ABBPlayerController>(OwningPlayer);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("UBBGameHUD::NativeConstruct: Failed to get PlayerController!"));
    }

    // 입력창에 이벤트 연결
    if (InputGuess && MyPlayerController)
    {
        InputGuess->OnTextCommitted.AddDynamic(this, &UBBGameHUD::OnTextCommitted);
        InputGuess->SetIsEnabled(false); // 처음에는 입력 비활성화
        UE_LOG(LogTemp, Log, TEXT("UBBGameHUD::NativeConstruct: InputGuess bound successfully."));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("UBBGameHUD::NativeConstruct: InputGuess or MyPlayerController is null!"));
    }

    if (ResultText)
    {
        ResultText->SetText(FText::FromString(TEXT("Waiting for game to start...")));
        UE_LOG(LogTemp, Log, TEXT("UBBGameHUD::NativeConstruct: ResultText bound successfully."));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("UBBGameHUD::NativeConstruct: ResultText is null!"));
    }

    bIsMyTurn = false;
}

void UBBGameHUD::UpdateTurn(bool IsMyTurn, const FString& Message)
{
    bIsMyTurn = IsMyTurn;
    if (InputGuess)
    {
        InputGuess->SetIsEnabled(bIsMyTurn); // 내 턴일 때만 입력 활성화
        if (bIsMyTurn)
        {
            // 입력 포커스 설정
            InputGuess->SetKeyboardFocus();
        }
    }

    // 기존 텍스트를 지우고 새 텍스트로 대체
    FString DisplayMessage;
    if (!Message.IsEmpty())
    {
        DisplayMessage = Message + TEXT("\n");
    }

    if (bIsMyTurn)
    {
        DisplayMessage += TEXT("Your Turn!\nEnter / followed by 3 digits (e.g., /123)");
    }
    else
    {
        DisplayMessage += TEXT("Waiting for opponent...");
    }

    if (ResultText)
    {
        ResultText->SetText(FText::FromString(DisplayMessage));
        UE_LOG(LogTemp, Log, TEXT("UBBGameHUD::UpdateTurn: DisplayMessage=%s"), *DisplayMessage);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("UBBGameHUD::UpdateTurn: ResultText is null!"));
    }
}

void UBBGameHUD::OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
    if (CommitMethod == ETextCommit::OnEnter && MyPlayerController)
    {
        FString Guess = Text.ToString();
        if (Guess.StartsWith(TEXT("/")))
        {
            Guess = Guess.Mid(1); // '/' 제거
            MyPlayerController->SendGuess(Guess);
            InputGuess->SetText(FText::GetEmpty()); // 입력창 초기화
        }
        else
        {
            MyPlayerController->HandleInvalidGuess();
        }
    }
}