#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BBGameHUD.generated.h"

class UEditableText;
class UTextBlock;
class ABBPlayerController;

UCLASS()
class N1_API UBBGameHUD : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    // 턴 정보와 메시지를 업데이트하는 함수
    UFUNCTION(BlueprintCallable, Category = "Baseball")
    void UpdateTurn(bool IsMyTurn, const FString& Message);

    // 입력 텍스트 커밋 시 호출되는 함수
    UFUNCTION()
    void OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

protected:
    
    // 입력창 (EditableText)
    UPROPERTY(meta = (BindWidget))
    UEditableText* InputGuess;

    // 결과 텍스트 (TextBlock)
    UPROPERTY(meta = (BindWidget))
    UTextBlock* ResultText;

    // 현재 내 턴인지 여부
    bool bIsMyTurn;

    // 소유 플레이어 컨트롤러
    UPROPERTY()
    ABBPlayerController* MyPlayerController;
};