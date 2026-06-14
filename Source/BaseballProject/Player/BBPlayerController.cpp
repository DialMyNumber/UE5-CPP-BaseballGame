// BBPlayerController.cpp

#include "BBPlayerController.h"
#include "UI/BBChatInput.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BaseballProject.h"			// NetMode와 Message만 출력할 수 있도록
#include "EngineUtils.h"				// TActorIterator를 위한 헤더
#include "Kismet/GameplayStatics.h"
#include "Game/BBGameModeBase.h"


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
	// PrintChatMessageString(ChatMessageString);


	if (IsLocalController() == true)
	{
		ServerRPCPrintChatMessageString(InChatMessageString);
	}
}

void ABBPlayerController::PrintChatMessageString(const FString& InChatMessageString)
{
	// 좌상단에 Log처럼 메세지를 띄우는 코드
	//UKismetSystemLibrary::PrintString(
	//	this,						// World Context, this = PlayerController
	//	ChatMessageString,			// 메세지
	//	true,						// 스크린에 출력할지 여부
	//	true,						// 로그에 출력할지 여부
	//	FLinearColor::Red,			// 색깔
	//	5.0f);						// 유지 시간

	/* 디버깅용 코드 블록
	FString NetModeString = BBFunctionLibrary::GetNetModeString(this);	// 현재 NetMode를 String으로 알려주는 함수
	FString CombinedMessageString = FString::Printf(TEXT("%s: %s"), *NetModeString, *InChatMessageString);
	BBFunctionLibrary::MyPrintString(this, CombinedMessageString, 10.f);
	// NetMode와 Message만 출력하는 함수, BaseballProject.h에 구현함
	*/
	BBFunctionLibrary::MyPrintString(this, InChatMessageString, 10.f);
}

// RPC를 구현할 때에는 _Implementation 를 붙여야함
void ABBPlayerController::ClientRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	PrintChatMessageString(InChatMessageString);
}

// RPC를 구현할 때에는 _Implementation 를 붙여야함
// Server에서 실행될 RPC
// Server에는 모든 PlayerController가 존재함
void ABBPlayerController::ServerRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	// BBGameModeBase.cpp에서 구현하는 것으로 리팩토링
	//for (TActorIterator<ABBPlayerController> It(GetWorld()); It; ++It)
	//{
	//	ABBPlayerController* BBPlayerController = *It;
	//	if (IsValid(BBPlayerController) == true)
	//	{
	//		BBPlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
	//	}
	//}

	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (IsValid(GM) == true)
	{
		ABBGameModeBase* BBGM = Cast<ABBGameModeBase>(GM);
		if (IsValid(BBGM) == true)
		{
			BBGM->PrintChatMessageString(this, InChatMessageString);
		}
	}
}