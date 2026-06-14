// BBPawn.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BBPawn.generated.h"

UCLASS()
class BASEBALLPROJECT_API ABBPawn : public APawn
{
	GENERATED_BODY()

public:

protected:
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;
public:	

};
