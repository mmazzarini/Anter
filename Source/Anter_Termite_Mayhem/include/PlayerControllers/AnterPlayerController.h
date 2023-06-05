#pragma once

#include "GameFramework/PlayerController.h"

#include "AnterPlayerController.generated.h"

UCLASS(BlueprintType)
class ANTER_TERMITE_MAYHEM_API AAnterPlayerController : public APlayerController
{
    GENERATED_BODY()

    AAnterPlayerController(){}

    virtual void InitInputSystem() override;

    virtual void SetPawn(APawn* InPawn) override;

    virtual void OnPossess(APawn* PawnToPossess) override;

};