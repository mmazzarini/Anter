#include "ActorComponents/AnterFloorHangingComponent.h"
#include "Pawn/AnterPaperCharacter.h"
#include "Math/Vector.h"
#include "Engine/EngineTypes.h"
#include "CollisionQueryParams.h"

//#include "UObject/WeakObjectPtrTemplates.h"

UAnterFloorHangingComponent::UAnterFloorHangingComponent()
{

}

void UAnterFloorHangingComponent::BeginPlay()
{
    Anter = Cast<AAnterPaperCharacter>(GetOwner());
}

void UAnterFloorHangingComponent::MoveToUpsideDownPlatform()
{
    if(Anter != nullptr)
    {
        if(UCharacterMovementComponent* AnterMovement = Cast<UCharacterMovementComponent>(Anter->FindComponentByClass(UCharacterMovementComponent::StaticClass())))
        {
            //Reset Gravity
            AnterMovement->GravityScale = 0.0f;
        }
        Anter->SetVerticalMotionStatus(EAnterVerticalMotionStatus::MovingTowardsUp);
    }
}

void UAnterFloorHangingComponent::DetachFromUpsideDownPlatform()
{
    if(Anter != nullptr)
    {
        if(UCharacterMovementComponent* AnterMovement = Cast<UCharacterMovementComponent>(Anter->FindComponentByClass(UCharacterMovementComponent::StaticClass())))
        {
            if(!Anter->GetCanJump())
            {
                //Reset Gravity
                AnterMovement->GravityScale = Anter->GetInputGravityScale();
            }
        }
        Anter->SetVerticalMotionStatus(EAnterVerticalMotionStatus::NormalStatus);
    }
}

void UAnterFloorHangingComponent::AttemptRayCastByLineTrace()
{
    if(Anter != nullptr && !Anter->GetCanJump())
    {
        if(Anter->GetWorld() != nullptr)
        {
            TArray<FHitResult> ObjectsHit;
            //const FCollisionObjectQueryParams& ObjectQueryParams, const FCollisionQueryParams& Params /* = FCollisionQueryParams::DefaultQueryParam */) const
            FVector RayTraceStart = Anter->GetActorLocation();
            FVector RayTraceEnd = RayTraceStart + FVector::UpVector*MaxRayTracingDistance;
            FCollisionObjectQueryParams ObjectParams(ECollisionChannel::ECC_WorldDynamic);
            //Check raycast
            if(Anter->GetWorld()->LineTraceMultiByObjectType(ObjectsHit,RayTraceStart,RayTraceEnd,ObjectParams))
            {
                for(FHitResult ObjectHit : ObjectsHit)
                {
                    //Check if hit object was of specified type 
                    if(ObjectHit.Actor.IsValid() && ObjectHit.Actor.Get() != nullptr && ObjectHit.Actor.Get() != Anter)
                    {
                        if(ABasePlatform* UpSidePlatform = Cast<ABasePlatform>(ObjectHit.Actor.Get()))
                        {
                            MoveToUpsideDownPlatform();
                            break;
                        }
                    }
                }
            }
        }
    }
}