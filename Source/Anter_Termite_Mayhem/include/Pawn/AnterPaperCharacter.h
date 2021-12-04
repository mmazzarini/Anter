#pragma once

//#include "FSM/AnterFSM/AnterFSM.h"

#include "PaperCharacter.h"
#include "ActorComponents/AnterFSMComponent.h"

#include "AnterPaperCharacter.generated.h"

/*
*
* Anter Paper Character class, representing the entity of the main character in the game
* Components are attached and refernced by the Pawn
*
*/

UCLASS(BlueprintType)
class ANTER_TERMITE_MAYHEM_API AAnterPaperCharacter : public APaperCharacter
{
    GENERATED_BODY()

public:

    AAnterPaperCharacter();

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

