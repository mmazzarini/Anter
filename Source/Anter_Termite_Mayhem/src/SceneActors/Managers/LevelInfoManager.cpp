#include "SceneActors/Managers/LevelInfoManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameInstance/AnterGameInstance.h"

void ULevelInfoManager::Init()
{
    // do something to initialize manager, in case
}

void ULevelInfoManager::SetCurrentLevelComplete()
{
    if (FLevelInfo* CurrentLevelInfo = LevelInfoMap.Find(CurrentLevelName))
    {
        CurrentLevelInfo->bLevelComplete = true;
    }
}

ULevelInfoManager* ULevelInfoManager::Get(UObject* WorldContextObject)
{
    if (UAnterGameInstance* GameInstance = Cast<UAnterGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject)))
    {
        return GameInstance->GetLevelInfoManager();
    }
    return nullptr;
}

const TMap<FString, FLevelInfo>& ULevelInfoManager::GetLevelsMap() const
{
    return LevelInfoMap;
}

void ULevelInfoManager::SetLevelsMap(const TMap<FString, FLevelInfo>& InLevelsMap)
{
    LevelInfoMap = InLevelsMap;
}

void ULevelInfoManager::SetCurrentLevelName(const FString& InLevelName)
{
    CurrentLevelName = InLevelName;
}