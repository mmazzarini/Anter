#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "LevelInfoManager.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FLevelInfo
{
    GENERATED_USTRUCT_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
    bool bLevelComplete = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
    int32 NumberCollectibles = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
    int32 NumDeaths = 0;
};

/*
* Level Info Manager class: instanced to handle information about the levels in the game, about the name, the collectibles etc
*/

UCLASS(Blueprintable,BlueprintType)
class ANTER_TERMITE_MAYHEM_API ULevelInfoManager : public UObject 
{
    GENERATED_BODY()

public:
    
    //Static public getter method for Level Manager. 
    static ULevelInfoManager* Get(UObject* WorldContextObject);

    //Initialization method for Level Info Manager. Right now, it may be useless
    void Init();

    //Sets completion info about a level.
    void SetCurrentLevelComplete();

    const TMap<FString,FLevelInfo>& GetLevelsMap() const;

    void SetLevelsMap(const TMap<FString,FLevelInfo>& InLevelsMap);

    void SetCurrentLevelName(const FString& InLevelName);

protected:

    UPROPERTY(EditAnywhere, SaveGame)
    TMap<FString,FLevelInfo> LevelInfoMap;

private:

    FString CurrentLevelName;
};