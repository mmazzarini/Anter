#include "FSMState/LevelFSMStates/LevelGameplayFSMState.h"
#include "GameFramework/GameStateBase.h"
#include "FSM/GameFSM.h"
#include "AnterGameModes/AnterBaseLevelGameMode.h"

void ULevelGameplayFSMState::StartState()
{
    Super::StartState();
    
    if (GetOwnerFSM() != nullptr && GetOwnerFSM()->GetOwnerComponent() != nullptr)
    {
        if (AGameStateBase* GameState = Cast<AGameStateBase>(GetOwnerFSM()->GetOwnerComponent()))
        {
            LevelGameMode = Cast<AAnterBaseLevelGameMode>(GameState->AuthorityGameMode);
            {
                if (LevelGameMode != nullptr)
                {
                    UE_LOG(LogTemp, Warning, TEXT("GameMode: Bound to gamemode!"));
                    LevelGameMode->LevelFinishedDelegate.AddDynamic(this, &ULevelGameplayFSMState::OnLevelFinished);
                }
            }
        }
    }
}

void ULevelGameplayFSMState::EndState()
{
    Super::EndState();

    if (LevelGameMode != nullptr)
    {
        LevelGameMode->LevelFinishedDelegate.RemoveDynamic(this, &ULevelGameplayFSMState::OnLevelFinished);
    }
}

void ULevelGameplayFSMState::OnLevelFinished()
{
    UE_LOG(LogTemp, Warning, TEXT("Level FSMState: travelling to map!"));
    TransitionToState(LevelFinishedStateID);
}