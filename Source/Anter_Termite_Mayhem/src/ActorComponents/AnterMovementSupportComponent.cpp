#include "ActorComponents/AnterMovementSupportComponent.h"
#include "Pawn/AnterPaperCharacter.h"
#include "GameConstants/GameConstants.h"

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
       Anter->OnActorGeometryCommunication.AddDynamic(this, &UAnterMovementSupportComponent::OnAnterGeometryChanged);
    }   
}

void UAnterMovementSupportComponent::HandleRightMovement(float InMovementAmount)
{
    
}

void UAnterMovementSupportComponent::HandleSlide()
{
    if(Anter != nullptr && Anter->GetCanJump() && bCanSlideInternal)
    {
        SlideImpulse = InternalMovementDirection * SlideMovementMultiplier;
        if(AnterMovement != nullptr)
        {
            //AnterMovement->AddImpulse(FVector::XAxisVector*InternalMovementDirection*SlideMovementMultiplier);
            SlideStopThreshold = AnterMovement->Velocity.Size();
        }
        Anter->GetWorldTimerManager().SetTimer(SlideTimerHandle, this, &UAnterMovementSupportComponent::EndSlide, SlideMovementDurationTime, false, SlideMovementDurationTime);
        bCanSlideInternal = false;
    }
}

void UAnterMovementSupportComponent::EndSlide()
{
    
    if(Anter != nullptr && AnterMovement != nullptr)
    {

        // TODO: MUST decide which threshold to evaluate: design-tuned via fraction-like multiplier to SlideStopThreshold.
        if(Anter->GetCanJump())// || abs(AnterMovement->Velocity.X) <= abs(SlideStopThreshold*SlideStopThresholdFraction))
        {
            bCanSlideInternal = true;
        }
        else
        {
            //Remove slide partially, because it is in air, but reset timer
            SlideImpulse /= SlideMovementAirDivider;
            Anter->GetWorldTimerManager().SetTimer(SlideTimerHandle, this, &UAnterMovementSupportComponent::EndSecondSlide, SlideMovementAirDurationTime, false, SlideMovementAirDurationTime);
        }
    }
}

void UAnterMovementSupportComponent::EndSecondSlide()
{
    bCanSlideInternal = true;
}

void UAnterMovementSupportComponent::SetMovementDirection(float InAxisValue)
{
    InternalMovementDirection.X = (InAxisValue > 0.0f) ? (GeometryX) : ((InAxisValue < 0.0f) ? -GeometryX : 0.0f);
    InternalMovementDirection.Y = 0.0f;
    InternalMovementDirection.Z = (InAxisValue > 0.0f) ? (GeometryZ) : ((InAxisValue < 0.0f) ? -GeometryZ : 0.0f);
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

    if (!bCanSlideInternal)
    {
        Anter->AddActorWorldOffset(SlideImpulse);
    }
}

void UAnterMovementSupportComponent::OnAnterGeometryChanged(float XGeom, float ZGeom)
{
    GeometryX = XGeom;
	GeometryZ = ZGeom;
}
