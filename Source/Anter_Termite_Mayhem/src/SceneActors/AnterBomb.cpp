#include "SceneActors/AnterBomb.h"
#include "ActorComponents/CollisionSupportComponent.h"
#include "SceneActors/Platforms/BasePlatform.h"

AAnterBomb::AAnterBomb()
:
Super()
{
    BombCollisionSupport = CreateDefaultSubobject<UCollisionSupportComponent>(TEXT("BombCollisionSupport"));
    BombCollisionSupport->SetupAttachment(RootComponent);
}

void AAnterBomb::HandleCollision(const FCollisionGeometry &InCollisionGeometry, AActor *OtherActor)
{
    if(ABasePlatform* HitPlatform = Cast<ABasePlatform>(OtherActor))
    {
        if(ProjectileMovement != nullptr)
        {
            ProjectileMovement->Velocity.X = 0.0f;
            ProjectileMovement->Velocity.Z = 0.0f;
            ProjectileMovement->ProjectileGravityScale = 0.0f;
            GetWorldTimerManager().SetTimer(ExplosionHandle, this, &AAnterBomb::OnTimerEnded, ExplosionTimer, false, ExplosionTimer);
        }
    }
}
void AAnterBomb::SetMovementToRight(FVector InMovementDirection)
{
    Super::SetMovementToRight(InMovementDirection);

    if(ProjectileMovement != nullptr)
    {
        ProjectileMovement->Velocity.Z = abs(ProjectileMovement->Velocity.Z);
        ProjectileMovement->ProjectileGravityScale = BombGravityScale;
    }
    

}
void AAnterBomb::OnTimerEnded()
{
    Destroy();
}

void AAnterBomb::BindToBombCollision() 
{
    if (FireMesh != nullptr && BombCollisionSupport != nullptr)
    {
        FireMesh->OnComponentBeginOverlap.AddDynamic(BombCollisionSupport, &UCollisionSupportComponent::ProcessCollisionGeometry);
    }
}

void AAnterBomb::BeginPlay()
{
    Super::BeginPlay();

    BindToBombCollision();
}

