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
	FString PlayerNameString;
};