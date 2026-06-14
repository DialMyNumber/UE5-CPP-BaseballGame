// BBPlayerState.cpp

#include "Player/BBPlayerState.h"

#include "Net/UnrealNetwork.h"

ABBPlayerState::ABBPlayerState()
	: PlayerNameString(TEXT("None"))
	, CurrentGuessCount(0)
	, MaxGuessCount(7)
{
	// 1. Replication을 위해 이 Actor의 bReplicates를 true로 설정
	bReplicates = true;
}

void ABBPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 3. DOREPLIFETIME 매크로를 사용해 복제할 속성을 명시
	// "Net/UnrealNetwork.h" 헤더파일 필요
	DOREPLIFETIME(ThisClass, PlayerNameString);
	DOREPLIFETIME(ThisClass, CurrentGuessCount);
	DOREPLIFETIME(ThisClass, MaxGuessCount); 

	// 변수 중에서 변경될 여지가 있으면 Replication에 등록,
	// 변경될 여지 없이 고정 값이라면 Replication에 등록할 필요가 없음
}

FString ABBPlayerState::GetPlayerInfoString()
{
	FString PlayerInfoString = PlayerNameString + TEXT("(") + FString::FromInt(CurrentGuessCount) + TEXT(" / ") + FString::FromInt(MaxGuessCount) + TEXT(")");
	return PlayerInfoString;
}