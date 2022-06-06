#pragma once

#include "GameFramework/CharacterMovementComponent.h"

#include "AnterMovementSupportComponent.generated.h"

UCLASS(Blueprintable, BlueprintType)
class ANTER_TERMITE_MAYHEM_API UAnterMovementSupportComponent : public USceneComponent
{

    GENERATED_BODY()

public:

    UAnterMovementSupportComponent(){}

    void HandleRightMovement(float InMovementAmount);    

protected:
    UPROPERTY(EditDefaultsOnly)
    FVector PositionVector = FVector(0.0f,0.0f,0.0f);

};