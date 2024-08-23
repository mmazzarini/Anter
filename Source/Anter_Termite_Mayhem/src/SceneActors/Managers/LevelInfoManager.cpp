#include "SceneActors/Managers/LevelInfoManager.h"

TMap<FString, FLevelInfo> ALevelInfoManager::GetLevelsMap() const
{
    return LevelInfoMap;
}

void ALevelInfoManager::SetLevelsMap(const TMap<FString, FLevelInfo>& InLevelsMap)
{
    LevelInfoMap = InLevelsMap;
}