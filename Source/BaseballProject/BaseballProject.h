// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class BBFunctionLibrary
{
public:
	// static인 이유는 class MyPrintString 객체가 없더라도 함수를 호출할 수 있도록
	static void MyPrintString(
		const AActor* InWorldContextActor, // GetWorld를 통해 어떤 World에 있는지를 가져올만할 객체
		const FString& InString, 
		float InTimeToDisplay = 1.f, 
		FColor InColor = FColor::Cyan)
	{
		if (IsValid(GEngine) == true && IsValid(InWorldContextActor) == true)
		{
			if (InWorldContextActor->GetNetMode() == NM_Client || InWorldContextActor->GetNetMode() == NM_ListenServer)
			{
				GEngine->AddOnScreenDebugMessage(-1, InTimeToDisplay, InColor, InString);
			}
			else // NM_Client가 아니고 NM_ListenServer도 아닌 경우 = Dedicated Server
			{
				UE_LOG(LogTemp, Log, TEXT("%s"), *InString);
				// Dedicated Server는 Viewport가 없기 때문에 UE_LOG 사용
			}
		}
	}

	// static인 이유는 class MyPrintString 객체가 없더라도 함수를 호출할 수 있도록
	static FString GetNetModeString(const AActor* InWorldContextActor)
	{
		FString NetModeString = TEXT("None");

		if (IsValid(InWorldContextActor) == true)
		{
			ENetMode NetMode = InWorldContextActor->GetNetMode();
			if (NetMode == NM_Client)
			{
				NetModeString = TEXT("Client");
			}
			else
			{
				if (NetMode == NM_Standalone)
				{
					NetModeString = TEXT("StandAlone");
				}
				else
				{
					NetModeString = TEXT("Server");
				}
			}
		}

		return NetModeString;
	}

};