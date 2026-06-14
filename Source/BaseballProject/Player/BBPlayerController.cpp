// BBPlayerController.cpp

#include "BBPlayerController.h"
#include "UI/BBChatInput.h"
#include "Kismet/KismetSystemLibrary.h"

void ABBPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 다른 Client의 PlayerController가 영향을 주지 않게 하기 위한 코드
	if (IsLocalController() == false)
	{
		return;
	}
	// -----------------------------------------
	// IsLocalController()가 true 일때 = Owning Client일 때 수행됨

	FInputModeUIOnly InputModeUIOnly;	// Input모드를 UI에만 사용하도록
	SetInputMode(InputModeUIOnly);

	if (IsValid(ChatInputWidgetClass) == true)	// ChatInputWidgetClass가 있을 때
	{
		ChatInputWidgetInstance = CreateWidget<UBBChatInput>(this, ChatInputWidgetClass);
		if (IsValid(ChatInputWidgetInstance) == true)
		{
			ChatInputWidgetInstance->AddToViewport();	// Viewport에 추가
		}
	}
}

void ABBPlayerController::SetChatMessageString(const FString& InChatMessageString)
{
	// 입력 값을 대입하는 간단한 기능
	ChatMessageString = InChatMessageString;

	// 입력 값을 출력하는 함수
	PrintChatMessageString(ChatMessageString);
}

void ABBPlayerController::PrintChatMessageString(const FString& InChatMessageString)
{
	// 좌상단에 Log처럼 메세지를 띄우는 코드
	UKismetSystemLibrary::PrintString(
		this,						// World Context, this = PlayerController
		ChatMessageString,			// 메세지
		true,						// 스크린에 출력할지 여부
		true,						// 로그에 출력할지 여부
		FLinearColor::Red,			// 색깔
		5.0f);						// 유지 시간
}