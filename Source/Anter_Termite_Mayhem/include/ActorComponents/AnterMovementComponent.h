#pragma once

#include "GameFramework/CharacterMovementComponent.h"
//#include "Components/SceneComponent.h"

#include "AnterMovementComponent.generated.h"

UCLASS(Blueprintable, BlueprintType)
class ANTER_TERMITE_MAYHEM_API UAnterMovementComponent : public UCharacterMovementComponent
{

    GENERATED_BODY()

public:

    UAnterMovementComponent(){}

    void HandleRightMovement(float InMovementAmount);    

private:

    FVector PositionVector = FVector(0.0f,0.0f,0.0f);

};