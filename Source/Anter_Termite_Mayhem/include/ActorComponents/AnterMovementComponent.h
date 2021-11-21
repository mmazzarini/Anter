#pragma once

#include "GameFramework/PawnMovementComponent.h"

#include "AnterMovementComponent.generated.h"

UCLASS(Blueprintable, BlueprintType)
class ANTER_TERMITE_MAYHEM_API UAnterMovementComponent : public UPawnMovementComponent
{

    GENERATED_BODY()

public:

    UAnterMovementComponen(){}

    void HandleRightMovement(float InMovementAmount);    

private:

    FVector PositionVector = (0.0f,0.0f,0.0f);

};