// BBGameModeBase.cpp

#include "BBGameModeBase.h"
#include "BBGameStateBase.h"
#include "Player/BBPlayerController.h"
#include "EngineUtils.h"
#include "Player/BBPlayerState.h"				// Player 이름 구분 

void ABBGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	// 정답 String 저장
	SecretNumberString = GenerateSecretNumber();
	UE_LOG(LogTemp, Warning, TEXT("%s"), *SecretNumberString);

	GetWorldTimerManager().SetTimer(
		MainTimerHandle,
		this,
		&ABBGameModeBase::OnMainTimerElapsed,
		1.0f,
		true
	);
}

void ABBGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	ABBPlayerController* BBPlayerController = Cast<ABBPlayerController>(NewPlayer);
	if (IsValid(BBPlayerController) == true)
	{
		// PlayerController는 Server에 있기 때문에 Replication
		// GameMode(Server)에 접속한 PlayerController가 유효할 때 NotificationText 변경
		BBPlayerController->NotificationText = FText::FromString(TEXT("Connected to the game server."));

		AllPlayerControllers.Add(BBPlayerController);

		ABBPlayerState* BBPS = BBPlayerController->GetPlayerState<ABBPlayerState>();
		if (IsValid(BBPS) == true)
		{
			// Server에 있는 PlayerState의 속성을 변경하면 Replication이 이루어짐
			BBPS->PlayerNameString = TEXT("Player") + FString::FromInt(AllPlayerControllers.Num());
		}

		ABBGameStateBase* BBGameStateBase = GetGameState<ABBGameStateBase>();
		if (IsValid(BBGameStateBase) == true)
		{
			BBGameStateBase->MulticastRPCBroadcastLoginMessage(BBPS->PlayerNameString);
		}
	}
}

FString ABBGameModeBase::GenerateSecretNumber()
{
	TArray<int32> Numbers;
	for (int32 i = 1; i <= 9; ++i)
	{
		Numbers.Add(i);
	}

	FMath::RandInit(FDateTime::Now().GetTicks());
	Numbers = Numbers.FilterByPredicate([](int32 Num) { return Num > 0; });

	FString Result;
	for (int32 i = 0; i < 3; ++i)
	{
		int32 Index = FMath::RandRange(0, Numbers.Num() - 1);
		Result.Append(FString::FromInt(Numbers[Index]));
		Numbers.RemoveAt(Index);
	}

	return Result;
}

bool ABBGameModeBase::IsGuessNumberString(const FString& InNumberString)
{
	bool bCanPlay = false;

	do {

		if (InNumberString.Len() != 3)
		{
			break;
		}

		bool bIsUnique = true;
		TSet<TCHAR> UniqueDigits;
		for (TCHAR C : InNumberString)
		{
			if (FChar::IsDigit(C) == false || C == '0')
			{
				bIsUnique = false;
				break;
			}

			UniqueDigits.Add(C);
		}

		if (bIsUnique == false)
		{
			break;
		}

		ABBPlayerState* CurrentGuessPlayerState = AllPlayerControllers[CurrentGuessPlayerIndex]->GetPlayerState<ABBPlayerState>();

		// 시간이 다 되었으면 플레이 불가
		if (CurrentGuessPlayerState->CurrentGuessTime <= KINDA_SMALL_NUMBER)
		{
			break; // false 반환
		}

		bCanPlay = true;

		CurrentGuessPlayerState->bDidParticipateThisTurn = true;

	} while (false);

	return bCanPlay;
}

FString ABBGameModeBase::JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString)
{
	int32 StrikeCount = 0, BallCount = 0;

	for (int32 i = 0; i < 3; ++i)
	{
		if (InSecretNumberString[i] == InGuessNumberString[i])
		{
			StrikeCount++;
		}
		else
		{
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), InGuessNumberString[i]);
			if (InSecretNumberString.Contains(PlayerGuessChar))
			{
				BallCount++;
			}
		}
	}

	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}

	return FString::Printf(TEXT("%dS %dB"), StrikeCount, BallCount);
}

void ABBGameModeBase::PrintChatMessageString(ABBPlayerController* InChattingPlayerController, const FString& InChatMessageString)
{
	FString GuessNumberString = InChatMessageString.Right(3);

	if (IsGuessNumberString(GuessNumberString))
	{
		IncreaseGuessCount(InChattingPlayerController);

		ABBPlayerState* BBPS = InChattingPlayerController->GetPlayerState<ABBPlayerState>();

		FString JudgeResultString = JudgeResult(SecretNumberString, GuessNumberString);

		FString CombinedMessageString = BBPS->GetPlayerInfoString() + TEXT(": ") + InChatMessageString + TEXT(" -> ") + JudgeResultString;

		for (TActorIterator<ABBPlayerController> It(GetWorld()); It; ++It)
		{
			ABBPlayerController* PC = *It;

			if (IsValid(PC))
			{
				PC->ClientRPCPrintChatMessageString(CombinedMessageString);
			}
		}

		int32 StrikeCount = FCString::Atoi(*JudgeResultString.Left(1));

		JudgeGame(InChattingPlayerController, StrikeCount);
	}
	else
	{
		ABBPlayerState* BBPS = InChattingPlayerController->GetPlayerState<ABBPlayerState>();

		FString CombinedMessageString = BBPS->PlayerNameString + TEXT(": ") + InChatMessageString;

		for (TActorIterator<ABBPlayerController> It(GetWorld()); It; ++It)
		{
			ABBPlayerController* PC = *It;

			if (IsValid(PC))
			{
				PC->ClientRPCPrintChatMessageString(CombinedMessageString);
			}
		}
	}
}

void ABBGameModeBase::IncreaseGuessCount(ABBPlayerController* InChattingPlayerController)
{
	ABBPlayerState* BBPS = InChattingPlayerController->GetPlayerState<ABBPlayerState>();
	if (IsValid(BBPS) == true)
	{
		BBPS->CurrentGuessCount++;
	}
}

void ABBGameModeBase::ResetGame()
{
	SecretNumberString = GenerateSecretNumber();

	for (const auto& BBPlayerController : AllPlayerControllers)
	{
		ABBPlayerState* BBPS = BBPlayerController->GetPlayerState<ABBPlayerState>();
		if (IsValid(BBPS) == true)
		{
			BBPS->CurrentGuessCount = 0;
			BBPS->bDidParticipateThisTurn = false;
			BBPS->CurrentGuessTime = BBPS->MaxGuessTime;
		}
	}
}

bool ABBGameModeBase::JudgeGame(ABBPlayerController* InChattingPlayerController, int InStrikeCount)
{
	if (InStrikeCount == 3)
	{
		ABBPlayerState* BBPS = InChattingPlayerController->GetPlayerState<ABBPlayerState>();

		if (IsValid(BBPS))
		{
			for (const auto& BBPlayerController : AllPlayerControllers)
			{
				FString CombinedMessageString = BBPS->PlayerNameString + TEXT(" has won the game.");
				BBPlayerController->NotificationText = FText::FromString(CombinedMessageString);
			}

			ResetGame();
			return true;	// 누군가 승리했을때 게임 종료
		}
	}

	bool bIsDraw = true;

	for (const auto& BBPlayerController : AllPlayerControllers)
	{
		ABBPlayerState* BBPS = BBPlayerController->GetPlayerState<ABBPlayerState>();

		if (IsValid(BBPS))
		{
			if (BBPS->CurrentGuessCount < BBPS->MaxGuessCount)
			{
				bIsDraw = false;
				break;
			}
		}
	}

	if (bIsDraw)
	{
		for (const auto& BBPlayerController : AllPlayerControllers)
		{
			BBPlayerController->NotificationText = FText::FromString(TEXT("Draw."));
		}

		ResetGame();
		return true;	// 비겼을때 게임 종료
	}

	return false;	// 아직 게임이 종료되지 않았을 때
}

void ABBGameModeBase::OnMainTimerElapsed()
{
	ABBPlayerState* CurrentGuessPlayerState =
		AllPlayerControllers[CurrentGuessPlayerIndex]->GetPlayerState<ABBPlayerState>();

	if (!CurrentGuessPlayerState)
	{
		return;
	}

	// 남은 시간 감소
	if (CurrentGuessPlayerState->CurrentGuessTime > 0.f)
	{
		CurrentGuessPlayerState->CurrentGuessTime -= 1.f;
	}

	// 시간 종료 처리
	if (CurrentGuessPlayerState->CurrentGuessTime <= 0.f)
	{
		// 이번 턴 미참여 패널티
		if (CurrentGuessPlayerState->bDidParticipateThisTurn == false)
		{
			++CurrentGuessPlayerState->CurrentGuessCount;

			// 타임아웃도 게임 판정에 포함
			bool bGameEnded = JudgeGame(AllPlayerControllers[CurrentGuessPlayerIndex], 0);

			if (bGameEnded)
			{
				return; // 게임 종료
			}
		}

		AdvanceTurn();
	}
}

void ABBGameModeBase::AdvanceTurn()
{
	// 다음 플레이어로 인덱스 변경
	CurrentGuessPlayerIndex = (CurrentGuessPlayerIndex + 1) % AllPlayerControllers.Num();

	// 다음 플레이어의 시간을 최대로 재설정
	ABBPlayerState* NextPlayerState = AllPlayerControllers[CurrentGuessPlayerIndex]->GetPlayerState<ABBPlayerState>();
	if (NextPlayerState)
	{
		NextPlayerState->CurrentGuessTime = NextPlayerState->MaxGuessTime;
	}
}