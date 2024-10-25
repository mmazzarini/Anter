#include "SaveGames/LevelProgressionSaveGame.h"
#include "SceneActors/Managers/LevelInfoManager.h"
#include "Kismet/GameplayStatics.h"

ULevelProgressionSaveGame::ULevelProgressionSaveGame()
{
    
}

void ULevelProgressionSaveGame::InitializeSaveGameForSave()
{
    if(ULevelInfoManager* LevelInfoManager = ULevelInfoManager::Get(this))
    {
        LevelInfoMap = LevelInfoManager->GetLevelsMap();
    }
}

void ULevelProgressionSaveGame::InitializeSaveGameForLoad()
{
    if(ULevelInfoManager* LevelInfoManager = ULevelInfoManager::Get(this))
    {
        LevelInfoManager->SetLevelsMap(LevelInfoMap);
    }
}