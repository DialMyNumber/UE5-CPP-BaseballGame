// BBPlayerController.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BBPlayerController.generated.h"

class UBBChatInput;

UCLASS()
class BASEBALLPROJECT_API ABBPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	void SetChatMessageString(const FString& InChatMessageString);

	void PrintChatMessageString(const FString& InChatMessageString);

	// Reliable을 통해 메세지 신뢰성 설정
	UFUNCTION(Client, Reliable)
	void ClientRPCPrintChatMessageString(const FString& InChatMessageString);

	// Reliable을 통해 메세지 신뢰성 설정
	UFUNCTION(Server, Reliable)
	void ServerRPCPrintChatMessageString(const FString& InChatMessageString);

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UBBChatInput> ChatInputWidgetClass;

	UPROPERTY()
	TObjectPtr<UBBChatInput> ChatInputWidgetInstance;

	// 입력할 채팅 문자열
	FString ChatMessageString;

};