#include "SceneActors/Platforms/MovingPlatform.h"
#include "Pawn/AnterPaperCharacter.h"
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
        OldPosition = FVector(0.0f,0.0f,0.0f);
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
        TPair<AActor*,EPlatformCollisionType>* CandidateCollision = PlatformCollisions.FindByPredicate([this](TPair<AActor*,EPlatformCollisionType> TPAEColl)
        {
            return (TPAEColl.Key == this);
        });
        if(CandidateCollision != nullptr && CandidateCollision->Value == EPlatformCollisionType::IsVerticallyColliding)
        {
            Anter->AddActorWorldOffset(NewPosition-OldPosition);
        }
    }
}