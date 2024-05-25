#pragma once

#include "FSMState/GameFSMState.h"

#include "MenuPlaceAnterAndCameraFSMState.generated.h"

class AAnterMenuPawn;
class ACameraActor;

UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class ANTER_TERMITE_MAYHEM_API UMenuPlaceAnterAndCameraFSMState : public UGameFSMState 
{

    GENERATED_BODY()

public:

    virtual void StartState() override;

protected:

    UPROPERTY(EditInstanceOnly)
    TSubclassOf<ACameraActor> AnterCameraClass;

    UPROPERTY(EditInstanceOnly)
    FName AnterTargetMarkerTag;

    UPROPERTY(EditInstanceOnly)
    FName CameraTargetMarkerTag;

    UPROPERTY(EditInstanceOnly)
    FName CameraActorTag;

    UPROPERTY(EditInstanceOnly)
    bool bShouldActivateCamera = false;

    UPROPERTY(EditInstanceOnly)
    bool bEnableAnterInput = false;

    UPROPERTY(EditInstanceOnly)
    FString ProceedTransitionLabel;

    void CreateAnter();

    void PlaceAnter();

    UPROPERTY(EditInstanceOnly)
    bool bProceedAfterPlacement = false;

private:

    TWeakObjectPtr<AAnterMenuPawn> AnterMenuPawnRef;

    TWeakObjectPtr<ACameraActor> AnterCameraActorRef;
};