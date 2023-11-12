#include "FSMState/TravelToMapFSMState.h"
#include "AnterGameStates/AnterBaseMenuGameState.h"

UTravelToMapFSMState::UTravelToMapFSMState(const FObjectInitializer& ObjectInitializer)
:Super(ObjectInitializer)
{
}

void UTravelToMapFSMState::StartState_Implementation()
{
    if(GetOwnerFSM() != nullptr && GetOwnerFSM()->GetOwnerComponent() != nullptr)
    {
        if(AAnterBaseMenuGameState* GameState = Cast<AAnterBaseMenuGameState>(GetOwnerFSM()->GetOwnerComponent()))
        {
            GameState->GetWorld()->ServerTravel(LevelName);
        }
    }
}