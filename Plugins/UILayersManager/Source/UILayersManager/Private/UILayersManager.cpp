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
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUILayersManagerModule, UILayersManager)