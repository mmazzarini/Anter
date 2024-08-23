#include "SaveGames/LevelProgressionSaveGame.h"
#include "SceneActors/Managers/LevelInfoManager.h"
#include "Kismet/GameplayStatics.h"

ULevelProgressionSaveGame::ULevelProgressionSaveGame()
{
    
}

void ULevelProgressionSaveGame::InitializeSaveGameForSave()
{
    if(ALevelInfoManager* LevelInfoManager = Cast<ALevelInfoManager>(UGameplayStatics::GetActorOfClass(this,ALevelInfoManager::StaticClass())))
    {
        LevelInfoMap = LevelInfoManager->GetLevelsMap();
    }
}

void ULevelProgressionSaveGame::InitializeSaveGameForLoad()
{
    if(ALevelInfoManager* LevelInfoManager = Cast<ALevelInfoManager>(UGameplayStatics::GetActorOfClass(this,ALevelInfoManager::StaticClass())))
    {
        LevelInfoManager->SetLevelsMap(LevelInfoMap);
    }
}