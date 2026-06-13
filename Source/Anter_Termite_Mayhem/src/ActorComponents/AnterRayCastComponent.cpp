#include "ActorComponents/AnterRayCastComponent.h"
#include "Pawn/AnterPaperCharacter.h"

//DEBUG
#include "Engine/Canvas.h"
#include "DisplayDebugHelpers.h"

UAnterRayCastComponent::UAnterRayCastComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UAnterRayCastComponent::DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& YL, float& YPos)
{
    if (DebugDisplay.IsDisplayOn("PLATFORMRAYCAST"))
    {
        FDisplayDebugManager& DisplayDebugManager = Canvas->DisplayDebugManager;
        DisplayDebugManager.DrawString(FString::Printf(TEXT(" + + + + + DEBUG RAYCAST + + + + +")));
        DisplayDebugManager.SetDrawColor(FColor::Red);
        DisplayDebugManager.DrawString(FString::Printf(TEXT("HIT FRONT: %s"), (bHitHorizontallyFront ? TEXT("YES") : TEXT("NO"))));
        DisplayDebugManager.SetDrawColor(FColor::White);
        DisplayDebugManager.DrawString(FString::Printf(TEXT("HIT BACK: %s"),  (bHitHorizontallyBack  ? TEXT("YES") : TEXT("NO"))));
        DisplayDebugManager.SetDrawColor(FColor::Red);
        DisplayDebugManager.DrawString(FString::Printf(TEXT("HIT VERTICALLY: %s"), (bHitVertically ? TEXT("YES") : TEXT("NO"))));
        DisplayDebugManager.SetDrawColor(FColor::White);
        DisplayDebugManager.DrawString(FString::Printf(TEXT("VERTICAL SIGN: %f"), (VerticalRayCastSignCorrection*1.0f)));
    }
}

void UAnterRayCastComponent::BeginPlay()
{
    Super::BeginPlay();
    RegisterInterfaceOwnerCharacter();
    //We tick after pawn tick - we dont interfere with tick ops at pawn.
    PrimaryComponentTick.AddPrerequisite(OwnerActorWeakRef.Get(), OwnerActorWeakRef->PrimaryActorTick);
}

void UAnterRayCastComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    RayCast();
}

void UAnterRayCastComponent::RegisterInterfaceOwnerCharacter()
{
    OwnerActorInterfacePtr = Cast<ISceneActorInterface>(GetOwner());
    OwnerActorWeakRef = Cast<AActor>(OwnerActorInterfacePtr);
}

void UAnterRayCastComponent::RayCast()
{
    ResetRayCastFlags();

    if (OwnerActorInterfacePtr != nullptr && OwnerActorWeakRef.IsValid())
    {
        CachedOwningActorVerticalMotionStatus = OwnerActorInterfacePtr->GetVerticalMotionStatus();

        VerticalRayCast();
        HorizontalFrontRayCast();
        HorizontalBackRayCast();
        SendGeometryAndFlags();
    }
}

void UAnterRayCastComponent::ResetRayCastFlags()
{
    bBlockedHorizontallyFront = false;
    bBlockedHorizontallyBack = false;
    bHitVertically = false;
    bHitHorizontallyFront = false;
    bHitHorizontallyBack = false;
    RotatedNormal = FVector(1.0f, 0.0f, 0.0f);
}

void UAnterRayCastComponent::VerticalRayCast()
{
    //#TODO Optimization - maybe caching OwnerActor at begin play?
    TArray<FHitResult> ObjectsHit;
    RayTraceStart = OwnerActorWeakRef->GetActorLocation();
    OwnerActorWeakRef->GetActorBounds(true, OwnerActorCentre, OwnerActorSize, false);
    VerticalRayCastSignCorrection = (CachedOwningActorVerticalMotionStatus == EAnterVerticalMotionStatus::NormalStatus) ?
        -1.f : +1.f;                
    RayTraceEnd = RayTraceStart + VerticalRayCastSignCorrection * FVector::UpVector * OwnerActorSize.Z*VerticalSizeCorrection; //Some size we get from owner
    
    FCollisionObjectQueryParams ObjectParams(ECollisionChannel::ECC_WorldDynamic);
    //Check raycast
    if (OwnerActorWeakRef->GetWorld()->LineTraceMultiByObjectType(ObjectsHit, RayTraceStart, RayTraceEnd, ObjectParams))
    {
        ProcessVerticallyBlockingGeometry(ObjectsHit);
    }
    //If was not hit vertically, process from the front border
    if (!bHitVertically)
    {
        RayTraceStart += FrontOffset;
        RayTraceEnd += FrontOffset;
        if (OwnerActorWeakRef->GetWorld()->LineTraceMultiByObjectType(ObjectsHit, RayTraceStart, RayTraceEnd, ObjectParams))
        {
            ProcessVerticallyBlockingGeometry(ObjectsHit);
        }
    }
    //If it was still not hit vertically, process from the back 
    if (!bHitVertically)
    {
        RayTraceStart -= 2.f*FrontOffset;
        RayTraceEnd -= 2.f*FrontOffset;
        if (OwnerActorWeakRef->GetWorld()->LineTraceMultiByObjectType(ObjectsHit, RayTraceStart, RayTraceEnd, ObjectParams))
        {
            ProcessVerticallyBlockingGeometry(ObjectsHit);
        }
    }
}

void UAnterRayCastComponent::HorizontalBackRayCast()
{
    TArray<FHitResult> ObjectsHit;
    RayTraceStart = OwnerActorWeakRef->GetActorLocation();
    OwnerActorWeakRef->GetActorBounds(true, OwnerActorCentre, OwnerActorSize, false);
    RayTraceEnd = RayTraceStart - FVector(OwnerActorSize.X * HorizontalSizeCorrection, 0.0f, 0.0f); //Some size we get from owner
    FCollisionObjectQueryParams ObjectParams(ECollisionChannel::ECC_WorldDynamic);
    //Check raycast
    if (OwnerActorWeakRef->GetWorld()->LineTraceMultiByObjectType(ObjectsHit, RayTraceStart, RayTraceEnd, ObjectParams))
    {
        ProcessHorizontallyBlockingGeometry(ObjectsHit, false);
    }
}

void UAnterRayCastComponent::HorizontalFrontRayCast()
{
    TArray<FHitResult> ObjectsHit;
    RayTraceStart = OwnerActorWeakRef->GetActorLocation();
    OwnerActorWeakRef->GetActorBounds(true, OwnerActorCentre, OwnerActorSize, false);
    RayTraceEnd = RayTraceStart + FVector(OwnerActorSize.X * HorizontalSizeCorrection, 0.0f, 0.0f); //Some size we get from owner
    FCollisionObjectQueryParams ObjectParams(ECollisionChannel::ECC_WorldDynamic);
    //Check raycast
    if (OwnerActorWeakRef->GetWorld()->LineTraceMultiByObjectType(ObjectsHit, RayTraceStart, RayTraceEnd, ObjectParams))
    {
        ProcessHorizontallyBlockingGeometry(ObjectsHit, true);
    }
}

void UAnterRayCastComponent::SendGeometryAndFlags()
{
    FGeometron OwnerGeometry{ RotatedNormal.X, RotatedNormal.Z };
    OwnerActorInterfacePtr->ProcessRayCastGeometry(bHitVertically, bHitHorizontallyFront, bHitHorizontallyBack, OwnerGeometry, ImpactPoint, HitActorRef.Get());
}

void UAnterRayCastComponent::ProcessHorizontallyBlockingGeometry(const TArray<FHitResult>& ObjectsHit, bool bFront)
{
    for (const FHitResult& ObjectHit : ObjectsHit)
    {
        if (AActor* OtherActor = ObjectHit.Actor.Get())
        {
            for (const TSubclassOf<AActor>& CollidingActorClass : CollidingActorClassesToCheck)
            {
                if (OtherActor->IsA(CollidingActorClass))
                {
                    FVector ImpactVector = ObjectHit.ImpactNormal;
                    //FVector RotatedVector = OtherActor->GetTransform().GetRotation().RotateVector(FVector::UpVector);
                    bool bBlock = !(FVector::DotProduct(ImpactVector, FVector::UpVector) > GeometryScalarProductTolerance);
                    bFront ? (bHitHorizontallyFront = bBlock) : (bHitHorizontallyBack = bBlock);
                    break;
                }
            }
        }
    }
}

void UAnterRayCastComponent::ProcessVerticallyBlockingGeometry(const TArray<FHitResult>& ObjectsHit)
{
    for (const FHitResult& ObjectHit : ObjectsHit)
    {
        if (AActor* OtherActor = ObjectHit.Actor.Get())
        {
            for (const TSubclassOf<AActor>& CollidingActorClass : CollidingActorClassesToCheck)
            {
                if (OtherActor->IsA(CollidingActorClass))
                {
                    RotatedNormal = ObjectHit.ImpactNormal.RotateAngleAxis(90, FVector::RightVector);
                    ImpactPoint = ObjectHit.ImpactPoint;
                    HitActorRef = ObjectHit.Actor;
                    bHitVertically = true;
                    break;
                }
            }
        }
    }
}
