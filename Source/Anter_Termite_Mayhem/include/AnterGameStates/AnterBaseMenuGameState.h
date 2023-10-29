#pragma once

#include "GameFramework/GameState.h"
#include "FSM/GameFSM.h"

#include "AnterBaseMenuGameState.generated.h"

/* Game State base class for platform levels, to represent their state from creation to end
It encapsulates also player condition/state infos, by attaching via ptr to player state
*/

//#TODO_REFACTORING please refactor the class to add a FSMComponent which will include the FSM (State machine)

UCLASS(BlueprintType)
class ANTER_TERMITE_MAYHEM_API AAnterBaseMenuGameState : public AGameStateBase
{
    GENERATED_BODY()

public:

    //TODO Implement it!!
    virtual void HandleBeginPlay() override;

private:

    //private internal reference to MenuFSM.
    //The FSM must contain states for menu - options - map
    
    UGameFSM* AnterMenuFSM;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UGameFSM> AnterMenuFSMClass;

};