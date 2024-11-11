#include "ActorComponents/BaseEnemyMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Actor.h"
#include <ActorComponents/JumpComponent.h>

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

void UBaseEnemyMovementComponent::BeginPlay()
{
    Super::BeginPlay();
    if (UJumpComponent* OwnerJumpComponent = Cast<UJumpComponent>(GetOwner()->GetComponentByClass(UJumpComponent::StaticClass())))
    {
        OwnerJumpComponent->OnJumpRequested.AddUObject(this, &UBaseEnemyMovementComponent::AddImpulse);
    }
}

void UBaseEnemyMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
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

void UBaseEnemyMovementComponent::PauseSpeed()
{
    BackupSpeed = InternalMovementSpeed;
    SetSpeed(0.0f);
} 

void UBaseEnemyMovementComponent::ResetSpeed()
{
    SetSpeed(BackupSpeed);
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

void UBaseEnemyMovementComponent::AddImpulse(FVector InInpulse)
{
    if (OwnerEnemy != nullptr)
    {
        if (UCharacterMovementComponent* EnemyMovement = Cast<UCharacterMovementComponent>(OwnerEnemy->GetComponentByClass(UCharacterMovementComponent::StaticClass())))
        {
            EnemyMovement->AddImpulse(InInpulse);
        }
    }
}

void UBaseEnemyMovementComponent::InvertSpeed()
{
    InternalMovementSpeed = -InternalMovementSpeed;
}

void UBaseEnemyMovementComponent::DestroyComponent(bool bPromoteChildren)
{
    if (UJumpComponent* OwnerJumpComponent = Cast<UJumpComponent>(GetOwner()->GetComponentByClass(UJumpComponent::StaticClass())))
    {
        OwnerJumpComponent->OnJumpRequested.RemoveAll(this);
    }
    Super::DestroyComponent(bPromoteChildren);
}