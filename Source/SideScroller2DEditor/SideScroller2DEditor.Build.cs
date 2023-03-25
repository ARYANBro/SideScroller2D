using UnrealBuildTool;

public class SideScroller2DEditor : ModuleRules
{
    public SideScroller2DEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "SideScroller2D" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UnrealEd" });
        PublicIncludePaths.AddRange(new string[] { "SideScroller2DEditor" });

    }
}
