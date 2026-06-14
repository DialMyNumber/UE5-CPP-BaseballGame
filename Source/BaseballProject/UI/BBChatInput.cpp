// BBChatInput.cpp

#include "UI/BBChatInput.h"
#include "Components/EditableTextBox.h"
#include "Player/BBPlayerController.h"

void UBBChatInput::NativeConstruct()
{
	Super::NativeConstruct();

	// OnTextCommitted가 바인딩되어있지 않으면 바인딩
	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == false)
	{ // OnTextCommitted가 바인딩 되어 있으면 델리게이트에 등록
		EditableTextBox_ChatInput->OnTextCommitted.AddDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UBBChatInput::NativeDestruct()
{
	Super::NativeDestruct();

	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == true)
	{
		EditableTextBox_ChatInput->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UBBChatInput::OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{	// 엔터를 친 플레이어 컨트롤러를 가져옴
		APlayerController* OwningPlayerController = GetOwningPlayer();
		if (IsValid(OwningPlayerController) == true)	// 해당 플레이어 컨트롤러가 유효하면
		{
			ABBPlayerController* OwningBBPlayerController = Cast<ABBPlayerController>(OwningPlayerController);
			if (IsValid(OwningBBPlayerController) == true)
			{	// 메세지 설정
				OwningBBPlayerController->SetChatMessageString(Text.ToString());

				EditableTextBox_ChatInput->SetText(FText());	// 빈 텍스트 하나를 바로 입력해서 메세지 창 초기화
			}
		}
	}
}