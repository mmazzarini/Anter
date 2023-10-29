#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Containers/Array.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"
#include "UObject/WeakObjectPtrTemplates.h"

#include "GameFSM.generated.h"


class UGameFSMState;
// #TODO_REFACTORING Please attach The FSM to a FSMComponent in the game state
//class UAnterFSMComponent;

USTRUCT()
struct ANTER_TERMITE_MAYHEM_API FFSMStateSpecifier 
{

    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly)
    FString StateID;

    UPROPERTY(EditDefaultsOnly)
    UGameFSMState* FSMState;
};

/*
*
* Game FSM Base class for defining FSM machines. This is an abstract implementation.
*
*/
  
UCLASS(BlueprintType,Blueprintable)
class ANTER_TERMITE_MAYHEM_API UGameFSM : public UObject 
{

    GENERATED_BODY()

public:

    //Base function initializer
    virtual void InitializeFSM(UObject* ContextObject);

    //State getter 
    UFUNCTION()
    virtual UGameFSMState* GetCurrentState() const {return CurrentState.Get();}

    //State setter
    UFUNCTION()
    virtual void SetCurrentState(FString InState);

    //This function is called at construction time to create instances of the states
    UFUNCTION()
    void CreateFSMStates();

    void AddFSMState(const TSubclassOf<UGameFSMState>& InSubclass);

    UFUNCTION()
    virtual void SetupFSMStates();

    //This is used to transition to new state
    virtual void TransitionToState(FString InNewState);

    virtual void StartFSM();

protected:

    UPROPERTY()
    TWeakObjectPtr<UGameFSMState> CurrentState;

    UPROPERTY(EditAnywhere)
    FString InitialStateString;

    //This array can be filled by designers in BP to create the set of states to build the array
    UPROPERTY(EditAnywhere)
    TArray<TSubclassOf<UGameFSMState>> ArrayOfTSubclassStates;

    //Actual Array of FSM States. This is the core property.
    UPROPERTY()
    TArray<FFSMStateSpecifier> InternalArrayOfStates; //array of FSM states

    //Pointer to the actor component that owns this FSM state
    UPROPERTY()
    UObject* OwnerComponent;  
    //Refactor with AnterFSMComponent
};
