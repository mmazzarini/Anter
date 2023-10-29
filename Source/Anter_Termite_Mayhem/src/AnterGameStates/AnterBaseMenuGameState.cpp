#include "AnterGameStates/AnterBaseMenuGameState.h"

void AAnterBaseMenuGameState::HandleBeginPlay()
{
    Super::HandleBeginPlay();
    if(AnterMenuFSMClass != nullptr)
    {
        AnterMenuFSM = NewObject<UGameFSM>(this,AnterMenuFSMClass);
        if(AnterMenuFSM != nullptr)
        {
            AnterMenuFSM->InitializeFSM(this);
        }
    }
}