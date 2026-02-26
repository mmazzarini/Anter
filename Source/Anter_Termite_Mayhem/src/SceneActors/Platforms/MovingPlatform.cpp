#include "SceneActors/Platforms/MovingPlatform.h"
#include "ActorComponents/MovingActorMovementSupportComponent.h"
#include "Kismet/GameplayStatics.h"

AMovingPlatform::AMovingPlatform()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bTickEvenWhenPaused = true;
}

void AMovingPlatform::Tick(float DeltaTime)
{   
    Super::Tick(DeltaTime);
    UpdateMovement();
}

void AMovingPlatform::UpdateMovement()
{
    OldPosition = NewPosition;
    NewPosition = GetActorLocation();
    if(AnterRef.IsValid())
    {
        TArray<TPair<AActor*,EPlatformCollisionType>> PlatformCollisions = AnterRef->GetResigsteredPlatformCollisions();
        CandidateCollision = PlatformCollisions.FindByPredicate([this](TPair<AActor*,EPlatformCollisionType> TPAEColl)
        {
            return (TPAEColl.Key == this);
        });
        if(CandidateCollision != nullptr && CandidateCollision->Value == EPlatformCollisionType::IsVerticallyColliding)
        {
            //FIX ME!!
            // :)
            AnterRef->AddActorWorldOffset(NewPosition-OldPosition);
        }
    }
}

void AMovingPlatform::Setup()
{
    if(UMovingActorMovementSupportComponent* ActorMovement = FindComponentByClass<UMovingActorMovementSupportComponent>())
    {
        AnterRef = Cast<AAnterPaperCharacter>(UGameplayStatics::GetActorOfClass(this,AAnterPaperCharacter::StaticClass()));
        OldPosition = GetActorLocation();
        NewPosition = GetActorLocation();
        ActorMovement->ResetMovement();
    }
}
