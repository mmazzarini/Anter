#pragma once

#include "FSM/AnterFSM/AnterFSM.h"
#include "Components/ActorComponent.h"
#include "Templates/SharedPointer.h"

#include "AnterComponent.generated.h"

class AAnterPawn;

/*
*
*
* Class representing the anter component for the status of the player (jumping, running, etc)
* It has a refernce to the FSM of the pawn and a set of functions to manipulate the FSM of the pawn
*
*/

UCLASS(BlueprintType)
class ANTER_TERMITE_MAYHEM_API UAnterComponent : public UActorComponent
{
    GENERATED_BODY()
    public:

        UAnterComponent();

        ~UAnterComponent();

    UFUNCTION(BlueprintCallable)
    UAnterFSM* GetAnterFSM(){return AnterFSM;}

    

    private:

    UPROPERTY()
    UAnterFSM* AnterFSM;

    UPROPERTY()
    AAnterPawn* Anter;
};