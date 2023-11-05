#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Containers/Map.h"

#include "GameFSMState.generated.h"

class UGameFSM;
class UAnterBasePage;

/*
*
* Game FSM Base class for defining FSM machines. This is an abstract implementation.
*
*/

UCLASS(Blueprintable, BlueprintType)
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

    virtual void StartState();

    virtual void CreatePage();

    virtual void StartPage();

    UFUNCTION()
    virtual void TransitionToState(FString InState);

    //This function is called when a button or any other bound widget triggers an action.
    //TODO please refactor and put an input parameter, corresponding to the action (e.g. a label, 
    //e.g. to help transition to)
    UFUNCTION()
    virtual void OnActionExecuted(FString InTriggerName);
   
    //Getter function for the identifier of the FSMState
    UFUNCTION()
    FString GetFSMStateID();

    void SetFSMStateID(const FString& InStateID){FSMStateID = InStateID;}

    UFUNCTION()
    void EndState();

protected:

    UPROPERTY(EditInstanceOnly)    
    TMap<FString, FString > MapOfStateTransitions; //default empty array of states

    UPROPERTY(BlueprintReadWrite)
    UGameFSM* OwnerFSM = nullptr;

    UPROPERTY()
    FString FSMStateID = "GameFSMStateID"; //GameFSMStateID is the default identifier for the GameFSMStates. For each subclass,
    //this can be redefined via blueprint defaults. 

    //Reference to Widget for the current state. 
    //MainWidget may be a MenuPage, a HUDPage...therefore this is generic and must be specified in the derived classes
    UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Page Config")
    UAnterBasePage* MainPage;

    UPROPERTY(EditDefaultsOnly, Category = "Page Config")
    TSubclassOf<UAnterBasePage> MainPageClass;

    UPROPERTY(EditDefaultsOnly,Category = "Page Config")
    FName MainPageName;
};
