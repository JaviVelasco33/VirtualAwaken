// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class VirtualAwaken : ModuleRules
{
	public VirtualAwaken(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "CommonInput" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        // Uncomment if you are using Slate UI
        //PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true

        // Esto permite que el compilador encuentre las carpetas Core, Characters, etc. 
        // sin tener que usar rutas relativas complejas.
        PublicIncludePaths.Add(ModuleDirectory);
    }
}
