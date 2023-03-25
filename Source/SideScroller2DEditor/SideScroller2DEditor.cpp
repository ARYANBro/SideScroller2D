#include "SideScroller2DEditor.h"


#include <UnrealEd.h>

IMPLEMENT_GAME_MODULE(FSideScroller2DEditorModule, SideScroller2DEditor)

void FSideScroller2DEditorModule::StartupModule()
{
    UE_LOG(LogTemp, Log, TEXT("SideScroller2DEditorModule Loaded"));
}

void FSideScroller2DEditorModule::ShutdownModule()
{
    UE_LOG(LogTemp, Log, TEXT("SideScroller2DEditorModule Unloaded"));
}
