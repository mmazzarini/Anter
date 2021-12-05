#include "ActorComponents/AnterMovementComponent.h"

void UAnterMovementComponent::HandleRightMovement(float InMovementAmount)
{
    AddInputVector(FVector(InMovementAmount,0.0f,0.0f));
}