// BBPlayerController.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BBPlayerController.generated.h"

class UBBChatInput;
class UUserWidget;

UCLASS()
class BASEBALLPROJECT_API ABBPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ABBPlayerController();

	virtual void BeginPlay() override;

	void SetChatMessageString(const FString& InChatMessageString);

	void PrintChatMessageString(const FString& InChatMessageString);

	// Reliable을 통해 메세지 신뢰성 설정
	UFUNCTION(Client, Reliable)
	void ClientRPCPrintChatMessageString(const FString& InChatMessageString);

	// Reliable을 통해 메세지 신뢰성 설정
	UFUNCTION(Server, Reliable)
	void ServerRPCPrintChatMessageString(const FString& InChatMessageString);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UBBChatInput> ChatInputWidgetClass;

	UPROPERTY()
	TObjectPtr<UBBChatInput> ChatInputWidgetInstance;

	// 입력할 채팅 문자열 Widget
	FString ChatMessageString;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> NotificationTextWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> NotificationTextWidgetInstance;

	// 남은 시간 Timer Widget
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> TimerWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> TimerWidgetInstance;

public:
	UPROPERTY(Replicated, BlueprintReadOnly)
	FText NotificationText;
};