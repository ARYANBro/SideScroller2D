// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class SideScroller2DTarget : TargetRules
{
	public SideScroller2DTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
		ExtraModuleNames.Add("SideScroller2D");

		if (bBuildEditor)
		{
			ExtraModuleNames.Add("SideScroller2DEditor");
		}
	}
}
