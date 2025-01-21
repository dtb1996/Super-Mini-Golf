// Copyright Epic Games, Inc. All Rights Reserved.

#include "UILayersManager.h"
#include "GameplayTagsManager.h"

#define LOCTEXT_NAMESPACE "FUILayersManagerModule"

void FUILayersManagerModule::StartupModule()
{
	UGameplayTagsManager::Get().AddTagIniSearchPath(FPaths::ProjectPluginsDir() / TEXT("UILayersManager/Config/Tags"));
}

void FUILayersManagerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUILayersManagerModule, UILayersManager)