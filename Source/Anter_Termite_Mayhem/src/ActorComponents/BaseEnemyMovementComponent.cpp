#include "ActorComponents/BaseEnemyMovementComponent.h"
#include "SceneActors/Enemies/BaseEnemy.h"

UBaseEnemyMovementComponent::UBaseEnemyMovementComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;  
    RegisterComponent();
}

void UBaseEnemyMovementComponent::Initialize()
{
    SetMovement(InitialMovementGeometry, InitialMovementSpeed);
    OwnerEnemy = Cast<ABaseEnemy>(GetOwner());
}

void UBaseEnemyMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
    UpdateMovement();
}

void UBaseEnemyMovementComponent::SetMovement(FVector2D InGeometryVector, float InSpeed)
{
    InternalMovementGeometry.X = InGeometryVector.X;
    InternalMovementGeometry.Y = InGeometryVector.Y;
    InternalMovementSpeed = InSpeed;
}

void UBaseEnemyMovementComponent::UpdateMovement()
{
    if(OwnerEnemy != nullptr)
    {
        FVector InputMovement3D = FVector(InternalMovementGeometry.X,0.0f,InternalMovementGeometry.Y);
        OwnerEnemy->AddMovementInput(InputMovement3D,InternalMovementSpeed);
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