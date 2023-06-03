#include "AnterGameStates/AnterBaseLevelGameState.h"
#include "AnterPlayerStates/AnterLevelPlayerState.h"


void AAnterBaseLevelGameState::HandleBeginPlay()
{
    Super::HandleBeginPlay();

    /*override: after calling Super:: we bind the player state to the Pawn delegates*/
    if(PlayerArray.Num() > 0)
    {
        if(AAnterLevelPlayerState* CurrPlayerState = Cast<AAnterLevelPlayerState>(PlayerArray[0]))
        {
            CurrPlayerState->BindToPawnDelegates();
        }
    }
}