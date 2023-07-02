#include "ActorComponents/AnterMovementSupportComponent.h"
#include "Pawn/AnterPaperCharacter.h"

UAnterMovementSupportComponent::UAnterMovementSupportComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;  
}

void UAnterMovementSupportComponent::BeginPlay() 
{
    Super::BeginPlay();
    Anter = Cast<AAnterPaperCharacter>(GetOwner());
}

void UAnterMovementSupportComponent::HandleRightMovement(float InMovementAmount)
{
    
}

void UAnterMovementSupportComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if(Anter != nullptr && Anter->GetVerticalMotionStatus() == EAnterVerticalMotionStatus::MovingTowardsUp)
    {
        if(UCharacterMovementComponent* AnterMovement = Cast<UCharacterMovementComponent>(Anter->FindComponentByClass(UCharacterMovementComponent::StaticClass())))
        {
            AnterMovement->Velocity.Z = 0.0f;
        }

        Anter->AddActorWorldOffset(FVector::UpVector*VerticalShift);
    }
}
