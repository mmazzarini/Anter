#include "ActorComponents/CollisionSupportComponent.h"

UCollisionSupportComponent::UCollisionSupportComponent()
{

}

void UCollisionSupportComponent::RegisterInterfaceOwnerCharacter(ISceneActorInterface* ActorOwnerInterface)
{
    OwnerActorInterfacePtr = ActorOwnerInterface;
}

void UCollisionSupportComponent::ProcessCollisionGeometry(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    if (bTurnOnSystem)
    {
        FCollisionGeometry CollisionGeometry;
        AActor* OwningActor = GetOwner();
        if (OtherActor != nullptr && OtherActor != OwningActor)
        {
            OtherActor->GetActorBounds(true, CollisionGeometry.PlatformCentre, CollisionGeometry.PlatformSize, false);
            CollisionGeometry.PlatformSurfaceCentre = CollisionGeometry.PlatformCentre + FVector::UpVector * CollisionGeometry.PlatformSize.Z;
            CollisionGeometry.PlatformBottomCentre = CollisionGeometry.PlatformCentre - FVector::UpVector * CollisionGeometry.PlatformSize.Z;
            CollisionGeometry.PlatformRightSideCentre = CollisionGeometry.PlatformCentre + FVector::XAxisVector * CollisionGeometry.PlatformSize.X;
            CollisionGeometry.PlatformLeftSideCentre = CollisionGeometry.PlatformCentre - FVector::XAxisVector * CollisionGeometry.PlatformSize.X;
            //Pawn geometrical references
            FVector OwnerSize = FVector(0.0f, 0.0f, 0.0f);
            FVector OwnerCentre = FVector(0.0f, 0.0f, 0.0f);
            OwningActor->GetActorBounds(true, OwnerCentre, OwnerSize, false);
            FVector OwningActorLocation = OwningActor->GetActorLocation();
            CollisionGeometry.TopDist = OwningActorLocation - CollisionGeometry.PlatformSurfaceCentre;
            CollisionGeometry.BottomDist = OwningActorLocation - CollisionGeometry.PlatformBottomCentre;
            //Check geometry of collision to decide which impact     
            CollisionGeometry.RotatedNormal = OtherActor->GetTransform().GetRotation().RotateVector(FVector::UpVector);
            CollisionGeometry.SideDist = OwningActor->GetActorLocation() - CollisionGeometry.PlatformCentre;
            //We need to use interface ptr to call the interface method on the scene actor owning this component
            if (OwnerActorInterfacePtr != nullptr)
            {
                OwnerActorInterfacePtr->HandleCollision(CollisionGeometry, OtherActor);
            }
        }

    }
}
