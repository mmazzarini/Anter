#pragma once

#include "GameFramework/PlayerState.h"

#include "AnterLevelPlayerState.generated.h"

/* Level Player state class controlling player pawn state, in terms of score and health mostly, or additional achievements.
Has a number of delegates we bind GameState to, in order to communicate the player situation and notify of changes in the state.
*/

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerStateDeathReached);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerLevelGoalReached);


UCLASS(BlueprintType)
class ANTER_TERMITE_MAYHEM_API AAnterLevelPlayerState : public APlayerState
{
    GENERATED_BODY()

public:

    void BindToPawnDelegates();

    UFUNCTION()
    void OnDeathReached();

    UFUNCTION()
    void OnHealthUpdated(float InNewHealth);

    UFUNCTION()
    void OnLevelGoalReached();

    FOnPlayerStateDeathReached OnPlayerStateDeathDelegate;

    FOnPlayerLevelGoalReached OnPlayerLevelGoalReachedDelegate;

private:

    /*private Pawn reference to bind to its delegates*/
    APawn* PlayerPawn;

};