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
            FVector SlideImpulse = FVector::XAxisVector*InternalMovementDirection*SlideMovementMultiplier;
            SlideStopThreshold = AnterMovement->Velocity.X + SlideImpulse;
            AnterMovement->AddImpulse(FVector::XAxisVector*InternalMovementDirection*SlideMovementMultiplier);
        }
        Anter->GetWorldTimerManager().SetTimer(SlideTimerHandle, this, &UAnterMovementSupportComponent::EndSlide, SlideMovementDurationTime, false, SlideMovementDurationTime);
        bCanSlideInternal = false;
    }
}

void UAnterMovementSupportComponent::EndSlide()
{
    
    if(Anter != nullptr && AnterMovement != nullptr)
    {

        // TODO: MUST decide which threshold to evaluate: design-tuned? character mechanics-tuned?
        if(Anter->GetCanJump() || AnterMovement->Velocity.X <= SlideStopThreshold)
        {
            // Completely remove slide.
            //Evaluate: if there is no input then velocity is set to 0, else divide below threshold.
            if(InternalMovementDirection == 0.0f)
            {
                AnterMovement->Velocity.X = 0.0f;
            }
            else if(AnterMovement->Velocity.X*InternalMovementDirection > 0.0f)
            {
                //I.e. if we are moving in the direction of initial slide impulse
                AnterMovement->Velocity.X /= SlideMovementMultiplier;
            }

            // We can reset the slide movement boolean
            bCanSlideInternal = true;
        }
        else
        {
            //Remove slide partially, because it is in air, but reset timer
            AnterMovement->Velocity.X /= SlideMovementMultiplier;
            Anter->GetWorldTimerManager().SetTimer(SlideTimerHandle, this, &UAnterMovementSupportComponent::EndSlide, SlideMovementAirDurationTime, false, SlideMovementAirDurationTime);
        }
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
