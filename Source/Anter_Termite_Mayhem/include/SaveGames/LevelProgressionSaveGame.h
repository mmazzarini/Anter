#pragma once

#include "SaveGames/AnterSaveGame.h"
#include "SceneActors/Managers/LevelInfoManager.h"

#include "LevelProgressionSaveGame.generated.h"

/**
 *
 */
UCLASS()
class ANTER_TERMITE_MAYHEM_API ULevelProgressionSaveGame : public UAnterSaveGame
{
    GENERATED_BODY()

public:

    ULevelProgressionSaveGame();

    virtual void InitializeSaveGameForSave() override;

    virtual void InitializeSaveGameForLoad() override;

    //UPROPERTY(VisibleAnywhere, Category = Basic)
    //ALevelInfoManager LevelInfo;

protected:

    UPROPERTY(SaveGame)
    TMap<FString, FLevelInfo> LevelInfoMap;
};