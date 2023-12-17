#include "SceneActors/Platforms/MovingPlatform.h"
#include "ActorComponents/MovingActorMovementSupportComponent.h"
#include "Kismet/GameplayStatics.h"

AMovingPlatform::AMovingPlatform()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bTickEvenWhenPaused = true;

}

void AMovingPlatform::BeginPlay()
{
    Super::BeginPlay();
    Anter = Cast<AAnterPaperCharacter>(UGameplayStatics::GetActorOfClass(this,AAnterPaperCharacter::StaticClass()));
    OldPosition = GetActorLocation();
    NewPosition = GetActorLocation();
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
    if(Anter != nullptr)
    {
        TArray<TPair<AActor*,EPlatformCollisionType>> PlatformCollisions = Anter->GetResigsteredPlatformCollisions();
        CandidateCollision = PlatformCollisions.FindByPredicate([this](TPair<AActor*,EPlatformCollisionType> TPAEColl)
        {
            return (TPAEColl.Key == this);
        });
        if(CandidateCollision != nullptr && CandidateCollision->Value == EPlatformCollisionType::IsVerticallyColliding)
        {
            Anter->AddActorWorldOffset(NewPosition-OldPosition);
        }
    }
}

void AMovingPlatform::Setup()
{
    if(UMovingActorMovementSupportComponent* ActorMovement = FindComponentByClass<UMovingActorMovementSupportComponent>())
    {
        Anter = Cast<AAnterPaperCharacter>(UGameplayStatics::GetActorOfClass(this,AAnterPaperCharacter::StaticClass()));
        OldPosition = GetActorLocation();
        NewPosition = GetActorLocation();
        ActorMovement->ResetMovement();
    }
}