#include "ActorComponents/BaseEnemyMovementComponent.h"
#include "SceneActors/Enemies/BaseEnemy.h"

UBaseEnemyMovementComponent::UBaseEnemyMovementComponent()
{

}

void UBaseEnemyMovementComponent::Initialize()
{
    SetMovement(InitialMovementGeometry, InitialMovementSpeed);
    OwnerEnemy = Cast<ABaseEnemy>(GetOwner());
}

void UBaseEnemyMovementComponent::SetMovement(FVector2D InGeometryVector, float InSpeed)
{
    InternalMovementGeometry.X = InGeometryVector.X;
    InternalMovementGeometry.Y = InGeometryVector.Y;
    InternalMovementSpeed = InSpeed;
}

void UBaseEnemyMovementComponent::UpdateMovement()
{
    OnMovementUpdated.Broadcast(InternalMovementGeometry,InternalMovementSpeed);
}
