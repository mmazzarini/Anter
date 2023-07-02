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
    if(Anter != nullptr)
    {
       AnterMovement = Cast<UCharacterMovementComponent>(Anter->FindComponentByClass(UCharacterMovementComponent::StaticClass()));
    }   
}

void UAnterMovementSupportComponent::HandleRightMovement(float InMovementAmount)
{
    
}

void UAnterMovementSupportComponent::HandleSlide()
{
    if(Anter != nullptr && Anter->GetCanJump() && bCanSlideInternal)
    {
        if(AnterMovement != nullptr)
        {
            AnterMovement->AddImpulse(FVector::XAxisVector*InternalMovementDirection*SlideMovementMultiplier);
        }
        Anter->GetWorldTimerManager().SetTimer(SlideTimerHandle, this, &UAnterMovementSupportComponent::EndSlide, SlideMovementDurationTime, false, SlideMovementDurationTime);
        bCanSlideInternal = false;
    }
}

void UAnterMovementSupportComponent::EndSlide()
{
    
    if(Anter != nullptr)
    {
        if(AnterMovement != nullptr)
        {
            AnterMovement->Velocity.X /= SlideMovementDivider;
        }
        bCanSlideInternal = true;
    }
}

void UAnterMovementSupportComponent::SetMovementDirection(float InAxisValue)
{
    InternalMovementDirection = (InAxisValue > 0.0f) ? (1.0f) : ((InAxisValue < 0.0f) ? -1.0f : 0.0f);
}

void UAnterMovementSupportComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if(Anter != nullptr && Anter->GetVerticalMotionStatus() == EAnterVerticalMotionStatus::MovingTowardsUp)
    {
        if(AnterMovement != nullptr)
        {
            AnterMovement->Velocity.Z = 0.0f;
        }

        Anter->AddActorWorldOffset(FVector::UpVector*VerticalShift);
    }
}
