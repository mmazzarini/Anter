#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Containers/Map.h"
#include "FSMState/GameFSMState.h"

#include "TravelToMapFSMState.generated.h"

/*
*
* Game FSM Base class for defining FSM machines. This is an abstract implementation.
*
*/

UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class ANTER_TERMITE_MAYHEM_API UTravelToMapFSMState : public UGameFSMState 
{
public:

    GENERATED_BODY()
    //Constructor
    UTravelToMapFSMState(const FObjectInitializer& ObjectInitializer);
 
  //State getter 

    void StartState() override;


protected:

    //Internal map of state transitions. Filled by FSM, read to communicate transition to FSM
    UPROPERTY(EditInstanceOnly)
    FString LevelName ="";

    UPROPERTY(EditInstanceOnly)
    TArray<FString> TravelOptions;
};
