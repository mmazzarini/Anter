#pragma once

#include "GameFramework/GameState.h"
#include "FSM/GameFSM.h"

#include "AnterBaseLevelGameState.generated.h"

/* Game State base class for platform levels, to represent their state from creation to end
It encapsulates also player condition/state infos, by attaching via ptr to player state
*/

UENUM(BlueprintType)
enum class ELevelCompletionState : uint8
{
    LevelWaitingToStart,
    LevelStarted,
    LevelGameOver,
    LevelCompleted
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameStateDeathReached);

UCLASS(BlueprintType)
class ANTER_TERMITE_MAYHEM_API AAnterBaseLevelGameState : public AGameStateBase
{
    GENERATED_BODY()

public:

    virtual void HandleBeginPlay() override;

    UFUNCTION()
    void OnGameOver();

    UFUNCTION()
    void OnLevelCompleted();

    void SetLevelCompletionState(ELevelCompletionState InLevelCompletion){LevelCompletion = InLevelCompletion;}

    UFUNCTION()
    void BindToPlayerStates();

    ELevelCompletionState GetLevelCompletionState(){return LevelCompletion;}

    FOnGameStateDeathReached OnDeathReached;

protected:

    void BindToPlayerStatesInternal();

    void UnbindFromPawnDelegates();

    //private internal reference to MenuFSM.
    //The FSM must contain states for menu - options - map

    UGameFSM* LevelMenuFSM;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UGameFSM> LevelMenuFSMClass;

private:

    AActor* GameOverCameraActor;

    ELevelCompletionState LevelCompletion = ELevelCompletionState::LevelWaitingToStart;

};