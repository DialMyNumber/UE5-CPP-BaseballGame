// BBPlayerState.cpp

#include "Player/BBPlayerState.h"

#include "Net/UnrealNetwork.h"

ABBPlayerState::ABBPlayerState()
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
}