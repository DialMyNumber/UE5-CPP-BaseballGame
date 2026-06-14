// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BaseballProject : ModuleRules
{
	public BaseballProject(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
			// Initial Dependencies
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput",

            // UI,    채팅 입력 모듈
            "UMG", "Slate", "SlateCore",
        });

        PrivateDependencyModuleNames.AddRange(new string[] { });

        PublicIncludePaths.AddRange(new string[] { "BaseballProject" });
    }
}
