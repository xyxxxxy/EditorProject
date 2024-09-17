// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class EditorProject : ModuleRules
{
	public EditorProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicIncludePaths.AddRange(new string[] {
            "EditorProject"
        });

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"Slate", 
			"SlateCore", 
			"LyraInventory"
		});

		PrivateDependencyModuleNames.AddRange( new string[] {
			"OnlineSubsystem",
			"Slate",
			"SlateCore",
			"CommonGame",
			"CommonInput",
			"CommonUI",
			"GameplayTags",
			"ModularGameplay",
			"ModularGameplayActors",
			"UMG",
			"EnhancedInput"
		});

	}
}
