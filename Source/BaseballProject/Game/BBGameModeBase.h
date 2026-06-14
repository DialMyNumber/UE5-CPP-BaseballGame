// BBGameModeBase.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BBGameModeBase.generated.h"

class ABBPlayerController;

UCLASS()
class BASEBALLPROJECT_API ABBGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void OnPostLogin(AController* NewPlayer) override;

	// 랜덤한 정답 숫자를 만드는 함수
	FString GenerateSecretNumber();

	// 유효한 입력인지 확인하는 함수
	bool IsGuessNumberString(const FString& InNumberString);

	// GuessNumber와 비교하여 결과를 알려주는 함수
	FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString);

	virtual void BeginPlay() override;

	void PrintChatMessageString(ABBPlayerController* InChattingPlayerController, const FString& InChatMessageString);

	// 해당 PlayerController의 시도 횟수를 늘려주는 함수
	void IncreaseGuessCount(ABBPlayerController* InChattingPlayerController);

protected:
	FString SecretNumberString;

	TArray<TObjectPtr<ABBPlayerController>> AllPlayerControllers;
};