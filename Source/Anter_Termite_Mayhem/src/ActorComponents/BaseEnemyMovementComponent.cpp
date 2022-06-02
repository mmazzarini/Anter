#include "ActorComponents/BaseEnemyMovementComponent.h"
#include ""

UBaseEnemyMovementComponent::UBaseMovementComponent()
{

}

void UBaseEnemyMovement::Initialize()
{
    SetMovement(InitialMovementGeometry, InitialMovementSpeedX, InitialMovementSpeedY);
    OwnerEnemy = Cast<ABaseEnemy>(GetOwner());
}

void UBaseEnemyMovement::SetMovement(FVector2D InGeometryVector, float InSpeed)
{
    InternalMovementGeometry.X = InGeometryVector.X;
    InternalMovementGeometry.Y = InGeometryVector.Y;
    InternalMovementSpeed = InSpeed;
}

void UBaseEnemyMovement::UpdateMovement()
{
    OnMovementUpdated.Broadcast(InternalMovementGeometry,InternalMovementSpeed);
}
