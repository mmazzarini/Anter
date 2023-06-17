#include "ActorComponents/BaseEnemyMovementComponent.h"
#include "GameFramework/Actor.h"

UBaseEnemyMovementComponent::UBaseEnemyMovementComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;  
    //RegisterComponent();
}

void UBaseEnemyMovementComponent::Initialize()
{
    SetMovement(InitialMovementGeometry);
    SetSpeed(InitialMovementSpeed);
    OwnerEnemy = GetOwner();
}

void UBaseEnemyMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
    UpdateMovement();
}

void UBaseEnemyMovementComponent::SetMovement(FVector2D InGeometryVector)
{
    InternalMovementGeometry.X = InGeometryVector.X;
    InternalMovementGeometry.Y = InGeometryVector.Y;
}

void UBaseEnemyMovementComponent::SetSpeed(float InSpeed)
{
    InternalMovementSpeed = InSpeed;
}

void UBaseEnemyMovementComponent::UpdateMovement()
{
    if(OwnerEnemy != nullptr)
    {
        FVector InputMovement3D = FVector(InternalMovementGeometry.X,0.0f,InternalMovementGeometry.Y);
        FVector NewPosition = OwnerEnemy->GetActorLocation() + InputMovement3D*InternalMovementSpeed;
        OwnerEnemy->SetActorLocation(NewPosition);
    }
    //    OnMovementUpdated.Broadcast(InternalMovementGeometry,InternalMovementSpeed);
}

void UBaseEnemyMovementComponent::OnCollided(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

}

void UBaseEnemyMovementComponent::InvertSpeed()
{
    InternalMovementSpeed = -InternalMovementSpeed;
}