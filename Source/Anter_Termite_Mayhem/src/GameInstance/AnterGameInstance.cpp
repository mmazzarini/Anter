#include "GameInstance/AnterGameInstance.h"
#include "SceneActors/Managers/LevelInfoManager.h"

void UAnterGameInstance::Init()
{
	Super::Init();

	LevelInfoManager = NewObject<ULevelInfoManager>(this, LevelInfoManagerClass);
	if (LevelInfoManager != nullptr)
	{
		LevelInfoManager->Init();
	}
}

ULevelInfoManager* UAnterGameInstance::GetLevelInfoManager()
{
	return LevelInfoManager;
}
