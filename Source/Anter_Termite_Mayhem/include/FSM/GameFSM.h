#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Containers/Array.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"

#include "GameFSM.generated.h"


class UGameFSMState;
class UAnterFSMComponent;

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
  
UCLASS(BlueprintType)
class ANTER_TERMITE_MAYHEM_API UGameFSM : public UObject 
{

    GENERATED_BODY()

public:

    //Base function initializer
    virtual void AnterInitializer(UAnterFSMComponent* ContextObject);

    //State getter 
    UFUNCTION()
    virtual UGameFSMState* GetCurrentState(){return CurrentState;}

    //State setter
    UFUNCTION()
    virtual void SetCurrentState(FString InState);

    //This function is called at construction time to create instances of the states
    UFUNCTION()
    void CreateFSMStates();

    void AddFSMState(const TSubclassOf<UGameFSMState>& InSubclass);

    UFUNCTION()
    virtual void GetFSMStateIdentifiers();

    UFUNCTION()
    virtual void SetupFSMStates();



protected:

    UPROPERTY()
    UGameFSMState* CurrentState = nullptr;

    UPROPERTY(BlueprintReadWrite)
    UGameFSMState* PreviousState = nullptr;

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
    UAnterFSMComponent* OwnerComponent;  
};
