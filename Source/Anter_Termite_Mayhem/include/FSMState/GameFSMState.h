#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Containers/Map.h"


#include "GameFSMState.generated.h"

class UGameFSM;

/*
*
* Game FSM Base class for defining FSM machines. This is an abstract implementation.
*
*/

UCLASS(Blueprintable)
class ANTER_TERMITE_MAYHEM_API UGameFSMState : public UObject 
{
public:

    GENERATED_BODY()
    //Constructor
    UGameFSMState();
 
  //State getter 
    UFUNCTION()
    UGameFSM* GetOwnerFSM(){return OwnerFSM;}

    UFUNCTION()
    void SetOwnerFSM(UGameFSM* InOwnerFSM);

    UFUNCTION()
    virtual void TransitionToState(FString InState);

    UFUNCTION()
    virtual void OnActionExecuted();
   
    //Getter function for the identifier of the FSMState
    UFUNCTION()
    FString GetFSMStateID();

protected:

    UPROPERTY(BlueprintReadWrite)    
    TMap<FString, TSubclassOf<UGameFSM> > MapOfStateTransitions; //default empty array of states

    UFUNCTION()
    void KillState();

    UPROPERTY(BlueprintReadWrite)
    UGameFSM* OwnerFSM = nullptr;

    UPROPERTY(EditDefaultsOnly)
    FString FSMStateID = "GameFSMStateID"; //GameFSMStateID is the default identifier for the GameFSMStates. For each subclass,
    //this can be redefined via blueprint defaults. 

};
