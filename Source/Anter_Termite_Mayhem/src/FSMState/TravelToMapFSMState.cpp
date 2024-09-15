#include "FSMState/TravelToMapFSMState.h"
#include "GameFramework/GameStateBase.h"
#include "FSM/GameFSM.h"
#include "AnterGameModes/AnterBaseLevelGameMode.h"

UTravelToMapFSMState::UTravelToMapFSMState(const FObjectInitializer& ObjectInitializer)
:Super(ObjectInitializer)
{
}

void UTravelToMapFSMState::StartState()
{ 
    Super::StartState();
    
    if(GetOwnerFSM() != nullptr && GetOwnerFSM()->GetOwnerComponent() != nullptr)
    {
        if (AGameStateBase* GameState = Cast<AGameStateBase>(GetOwnerFSM()->GetOwnerComponent()))
        {
            AAnterBaseLevelGameMode* LevelGameMode = Cast<AAnterBaseLevelGameMode>(GameState->AuthorityGameMode);
            {
                if (LevelGameMode != nullptr)
                {
                    UE_LOG(LogTemp, Warning, TEXT("FSMState: Im calling travel on GameMode!"));
                    LevelGameMode->TravelToMap(LevelName, TravelOptions);
                }
            }
        }
    }
}