// BBGameStateBase.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BBGameStateBase.generated.h"


UCLASS()
class BASEBALLPROJECT_API ABBGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	// NetMulticast RPC
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCBroadcastLoginMessage(const FString& InNameString = FString(TEXT("Default")));

};
