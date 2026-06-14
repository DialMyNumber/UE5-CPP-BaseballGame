// BBPawn.cpp

#include "Player/BBPawn.h"
#include "BaseballProject.h"

void ABBPawn::BeginPlay()
{
	Super::BeginPlay();
	
	FString NetRoleString = BBFunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(
		TEXT("BBPawn::BeginPlay() %s [%s]"), *BBFunctionLibrary::GetNetModeString(this), *NetRoleString);
	BBFunctionLibrary::MyPrintString(this, CombinedString, 10.f);
}

void ABBPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	FString NetRoleString = BBFunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(
		TEXT("BBPawn::PossessedBy() %s [%s]"), *BBFunctionLibrary::GetNetModeString(this), *NetRoleString);
	BBFunctionLibrary::MyPrintString(this, CombinedString, 10.f);
}