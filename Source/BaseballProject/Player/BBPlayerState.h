// BBPlayerState.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BBPlayerState.generated.h"

UCLASS()
class BASEBALLPROJECT_API ABBPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	ABBPlayerState();

	// 3. GetLifetimeReplicatedProps() 함수에 네트워크로 복제할 속성을 추가
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	// 2. Replication을 위해 UPROPERTY에 Replicated 키워드 추가
	UPROPERTY(Replicated)
	FString PlayerNameString;
};