#pragma once

//#include "FSM/AnterFSM/AnterFSM.h"
#include "GameFramework/Pawn.h"
#include "ActorComponents/AnterFSMComponent.h"

#include "AnterPawn.generated.h"

/*
*
* Anter Pawn class, representing the entity of the main character in the game
* Components are attached and refernced by the Pawn
*
*/

UCLASS(BlueprintType)
class ANTER_TERMITE_MAYHEM_API AAnterPawn : public APawn
{
    GENERATED_BODY()

public:

    AAnterPawn();

    //Basic ticker
    void Tick(float DeltaTime) override;

    void OnDeathEvent();

    void SetBindings();

protected:

//UPROPERTY()
//TSubclassOf<UAnterFSMComponent> AnterFSMComponentClass;


private:

//UAnterComponent* AnterFSMComponent;



};

