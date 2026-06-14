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

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UBBChatInput> ChatInputWidgetClass;

	UPROPERTY()
	TObjectPtr<UBBChatInput> ChatInputWidgetInstance;

	// 입력할 채팅 문자열
	FString ChatMessageString;

};