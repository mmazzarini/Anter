#include "ActorComponents/CollisionSupportComponent.h"

UCollisionSupportComponent::UCollisionSupportComponent()
{

}

FCollisionGeometry UCollisionSupportComponent::ProcessCollisionGeometry(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    FCollisionGeometry CollisionGeometry;
    AActor* Owner = GetOwner();
    if(OtherActor != nullptr && OtherActor->GetName().Contains("Floor"))
    {
        OtherActor->GetActorBounds(true,CollisionGeometry.PlatformCentre,CollisionGeometry.PlatformSize,false);
        
        UStaticMeshComponent* PlatformMeshComponent = Cast<UStaticMeshComponent>(OtherActor->FindComponentByClass<UStaticMeshComponent>());

        if(PlatformMeshComponent != nullptr)
        {
            UStaticMesh* PlatformStaticMesh = PlatformMeshComponent->GetStaticMesh();
            if(PlatformStaticMesh != nullptr)
            {
                FVector IntrinsicSize = PlatformStaticMesh->GetBounds().GetBox().GetExtent();
                FVector PlatformScale = OtherActor->GetTransform().GetScale3D();
                CollisionGeometry.PlatformLength = IntrinsicSize.X*PlatformScale.X;   
                CollisionGeometry.PlatformHeight =  IntrinsicSize.Z*PlatformScale.Z;    
            }
        }

        CollisionGeometry.PlatformSurfaceCentre = CollisionGeometry.PlatformCentre + FVector::UpVector*CollisionGeometry.PlatformHeight; 
        CollisionGeometry.PlatformBottomCentre =  CollisionGeometry.PlatformCentre - FVector::UpVector*CollisionGeometry.PlatformHeight;
        float PlatformAngle = OtherActor->GetTransform().GetRotation().GetAngle();
        float PlatformCosine = 1.0f;
        float PlatformSine = 0.0f;
        FMath::SinCos(&PlatformSine,&PlatformCosine,PlatformAngle);
        //Pawn geometrical references
        FVector AnterSize = FVector(0.0f,0.0f,0.0f);
        FVector AnterCentre = FVector(0.0f,0.0f,0.0f);
        Owner->GetActorBounds(true,AnterCentre,AnterSize,false);
        CollisionGeometry.TopDist = Owner->GetActorLocation()-CollisionGeometry.PlatformSurfaceCentre;
        CollisionGeometry.BottomDist = Owner->GetActorLocation()-CollisionGeometry.PlatformBottomCentre;
        //Check geometry of collision to decide which impact     
        CollisionGeometry.RotatedNormal = OtherActor->GetTransform().GetRotation().RotateVector(FVector::UpVector);
        CollisionGeometry.PlatformRightSideCentre = CollisionGeometry.PlatformCentre + FVector::XAxisVector*CollisionGeometry.PlatformLength;
        CollisionGeometry.PlatformLeftSideCentre = CollisionGeometry.PlatformCentre - FVector::XAxisVector*CollisionGeometry.PlatformHeight;
        CollisionGeometry.SideDist = Owner->GetActorLocation()-CollisionGeometry.PlatformCentre;
    }\
    //we return TopDist, RotatedNormal, PlatformRightSideCentre, PlatformLeftSideCentre, SideDist
    return CollisionGeometry;
}

/*
            if(FVector::DotProduct(TopDist,RotatedNormal) >= VerticalTolerance)
            {
                // Impact was from top: pawn is standing on platform. 

                //Register impact with vertical colliding platform
                RegisterPlatformCollision(OtherActor,EPlatformCollisionType::IsVeritcallyColliding);

                UCharacterMovementComponent* AnterMovement = Cast<UCharacterMovementComponent>(FindComponentByClass<UCharacterMovementComponent>());
                if(AnterMovement != nullptr)
                {
                    SetIsFalling(false);
                    SetCanJump(true);
                    AnterMovement->GravityScale = 0.0f;
                    ImposeGeometry(PlatformAngle);
                }
                //Floor impenetrability condition
                float ProjectedPlatformZ = PlatformCentre.Z + UKismetMathLibrary::DegTan(PlatformAngle/UKismetMathLibrary::GetPI()*180.)*(GetActorLocation().X - PlatformCentre.X);

                FVector NewLocation = FVector(GetActorLocation().X ,GetActorLocation().Y,ProjectedPlatformZ + AnterSize.Z/VerticalImpenetrabilityFactor + PlatformHeight/PlatformCosine);
                SetActorLocation(NewLocation);
                GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,TEXT("Platform centre Z: ") + FString::SanitizeFloat(PlatformCentre.Z));
                GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,TEXT("Platform proj Z: ") + FString::SanitizeFloat(ProjectedPlatformZ));
            } 
            else
            {
                if(FVector::DotProduct(BottomDist,-RotatedNormal) < VerticalTolerance)
                {

                    /* Impact was from side or bottom 
                    FVector PlatformRightSideCentre = PlatformCentre + FVector::XAxisVector*PlatformLength;
                    FVector PlatformLeftSideCentre = PlatformCentre - FVector::XAxisVector*PlatformLength;
                    FVector SideDist = this->GetActorLocation()-PlatformCentre;
                    if(SideDist.X > 0.0f)
                    {
                        //Impact coming from right
                        if((FVector::DotProduct(SideDist,FVector::XAxisVector) > HorizontalTolerance))
                        {
                            //Impenetrability
                            SetLeftMovementFree(false);
                            RegisterPlatformCollision(OtherActor,EPlatformCollisionType::IsCollidingFromRight);
                        }
                    }
                    else if(SideDist.X < 0.0f)
                    {
                        //Impact coming from left
                        if((FVector::DotProduct(SideDist,-FVector::XAxisVector) > HorizontalTolerance))
                        {
                            //Impenetrability
                            SetRightMovementFree(false);
                            RegisterPlatformCollision(OtherActor,EPlatformCollisionType::IsCollidingFromLeft);
                        }
                    }
                }
            }
        }
    }
}
*/
