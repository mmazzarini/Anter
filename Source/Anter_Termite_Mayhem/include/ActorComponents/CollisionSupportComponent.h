#pragma once

#include "Components/SceneComponent.h"

#include "CollisionSupportComponent.generated.h"

USTRUCT()
struct FCollisionGeometry
{
    FCollisionGeometry()
    {}

    FVector PlatformCentre = FVector(0.0f,0.0f,0.0f);
    FVector PlatformSurfaceCentre = FVector(0.0f,0.0f,0.0f);
    FVector PlatformBottomCentre = FVector(0.0f,0.0f,0.0f);
    FVector PlatformRightSideCentre = FVector(0.0f,0.0f,0.0f);
    FVector PlatformLeftSideCentre = FVector(0.0f,0.0f,0.0f);
    FVector SideDist = FVector(0.0f,0.0f,0.0f);
    FVector TopDist = FVector(0.0f,0.0f,0.0f);
    FVector BottomDist = FVector(0.0f,0.0f,0.0f);
    FVector RotatedNormal = FVector(0.0f,0.0f,0.0f);

    float PlatformLength = 0.0f;
    float PlatformHeight = 0.0f;
}

UCLASS(BlueprintType)
class ANTER_TERMITE_MAYHEM_API UCollisionSupportComponent : public USceneComponent
{
    GENERATED_BODY()

public:

    UCollisionSupportComponent();

    UFUNCTION()
    FCollisionGeometry ProcessCollisionGeometry(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

};