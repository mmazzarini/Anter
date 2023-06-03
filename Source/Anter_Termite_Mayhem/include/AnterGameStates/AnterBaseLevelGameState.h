#pragma once

#include "GameFramework/GameStateBase.h"

#include "AnterBaseLevelGameState.generated.h"

/* Game State base class for platform levels, to represent their state from creation to end
It encapsulates also player condition/state infos, by attaching via ptr to player state
*/

UCLASS(BlueprintType)
class ANTER_TERMITE_MAYHEM_API AAnterBaseLevelGameState : public AGameStateBase
{
    GENERATED_BODY()

    virtual void HandleBeginPlay() override;

};