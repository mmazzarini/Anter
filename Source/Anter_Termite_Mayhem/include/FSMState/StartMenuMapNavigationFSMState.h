#pragma once

#include "FSMState/GameFSMState.h"

#include "StartMenuMapNavigationFSMState.generated.h"

class UAnterMapNavigationPage;
class AAnterMenuPawn;

UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class ANTER_TERMITE_MAYHEM_API UStartMenuMapNavigationFSMState : public UGameFSMState 
{

    GENERATED_BODY()

public:

    void StartState() override;
    
    void EndState() override;

protected:

    void StartMenuMapNavigation();

    UFUNCTION()
    void OnMapLevelHit(FString InLevelName);

    UFUNCTION()
    void OnMapLevelSelected(FString InLevelName);

    UPROPERTY(EditInstanceOnly)
    FString StartingLevelLabel;

    TWeakObjectPtr<AAnterMenuPawn> AnterPawn;
};
