// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class IVAN : ModuleRules
{
	public IVAN(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "MotionTrajectory" });
	}
}
