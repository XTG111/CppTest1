// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class CppTest1Target : TargetRules
{
	public CppTest1Target( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "CppTest1" } );
	}
}
