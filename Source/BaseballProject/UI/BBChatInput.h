// BBChatInput.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BBChatInput.generated.h"

class UEditableTextBox;		// 전방선언

UCLASS()
class BASEBALLPROJECT_API UBBChatInput : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

protected:
	UFUNCTION()
	void OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> EditableTextBox_ChatInput;
	// BindWidget : 순수 가상함수처럼 
	// "EditableTextBox_ChatInput"라는 이름의 UEditableTextBox를 구현할 것을 요구(강제)함
};
