#pragma once

#include "CoreMinimal.h"
#include "FSM/GameFSM.h"
#include "UObject/Object.h"
#include "Containers/Map.h"

#include "AnterFSM.generated.h"


/*
*
* Anter Jump FSM class for defining FSM. This is an implementation of the base class
* 12 OCT 2023 - this class may be deprecated!! Use GameFSM!
*/

UCLASS(Blueprintable, BlueprintType)
class ANTER_TERMITE_MAYHEM_API UAnterFSM : public UGameFSM 
{
public:

    GENERATED_BODY()
    //Constructor
    //UAnterFSM(const FObjectInitializer& ObjectInitializer);
 
    void SetupFSMStates() override;

/*
  //State getter 
   UFUNCTION()
   UGameFSMState* GetCurrentState(){return CurrentState;}

   //State setter. Set to pure virtual to make UGameFSM an abstract class. Implementations needed 
   UFUNCTION()
   virtual void SetCurrentState(FString InState){} //{CsurrentState = MapOfStates[InState];}
 
protected:

    UPROPERTY(EditAnywhere)    
    TMap<FString, TSubclassOf<UGameFSM> > MapOfStates = {}; //default empty array of states

    UPROPERTY()
    UGameFSMState* CurrentState = nullptr;

    UPROPERTY(BlueprintReadWrite)
    UGameFSMState* PreviousState = nullptr;
*/
};
