#pragma once

#include "GameFramework/Actor.h"

#include "LevelInfoManager.generated.h"

USTRUCT()
struct FLevelInfo
{
    GENERATED_BODY()
    
    UPROPERTY(SaveGame)
    bool bLevelComplete = false;

    UPROPERTY(SaveGame)
    int32 NumberCollectibles = 0;

    UPROPERTY(SaveGame)
    int32 NumDeaths = 0;
};

/*
* Level Info Manager class: instanced to handle information about the levels in the game, about the name, the collectibles etc
*/

UCLASS(Blueprintable,BlueprintType)
class ANTER_TERMITE_MAYHEM_API ALevelInfoManager : public AActor 
{
    GENERATED_BODY()

public:

    TMap<FString,FLevelInfo> GetLevelsMap() const;

    void SetLevelsMap(const TMap<FString,FLevelInfo>& InLevelsMap);

protected:

    UPROPERTY(SaveGame)
    TMap<FString,FLevelInfo> LevelInfoMap;

};