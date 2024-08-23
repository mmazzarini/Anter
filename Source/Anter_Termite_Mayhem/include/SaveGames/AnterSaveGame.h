#pragma once

#include "GameFramework/SaveGame.h"

#include "AnterSaveGame.generated.h"

/**
 *
 */
UCLASS(Abstract)
class ANTER_TERMITE_MAYHEM_API UAnterSaveGame : public USaveGame
{
    GENERATED_BODY()

public:

    virtual void InitializeSaveGameForSave() PURE_VIRTUAL(UAnterSaveGame::InitializeSaveGameForSave,);

    virtual void InitializeSaveGameForLoad() PURE_VIRTUAL(UAnterSaveGame::InitializeSaveGameForLoad,);

};