#pragma once

#include "FSMState/GameFSMState.h"

#include "StartMenuMapNavigationFSMState.generated.h"

UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class ANTER_TERMITE_MAYHEM_API UStartMenuMapNavigationFSMState : public UGameFSMState 
{

    GENERATED_BODY()

public:

    void StartState() override;

protected:

    UPROPERTY(EditInstanceOnly)
    FString StartingLevelLabel;

    void StartMenuMapNavigation();

};
