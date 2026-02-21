#include "ActorComponents/AnterRayCastComponent.h"
#include "Pawn/AnterPaperCharacter.h"

UAnterRayCastComponent::UAnterRayCastComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UAnterRayCastComponent::BeginPlay()
{
    Super::BeginPlay();
    RegisterInterfaceOwnerCharacter();
}

void UAnterRayCastComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    RayCast();
}

void UAnterRayCastComponent::RegisterInterfaceOwnerCharacter()
{
    OwnerActorInterfacePtr = Cast<ISceneActorInterface>(GetOwner());
}

void UAnterRayCastComponent::RayCast()
{
    ResetRayCastFlags();

    if (OwnerActorInterfacePtr != nullptr)
    {
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
    if (AActor* OwnerActor = Cast<AActor>(OwnerActorInterfacePtr))
    {
        TArray<FHitResult> ObjectsHit;
        FVector RayTraceStart = OwnerActor->GetActorLocation();
        OwnerActor->GetActorBounds(true, OwnerActorCentre, OwnerActorSize, false);
        FVector RayTraceEnd = RayTraceStart - FVector::UpVector * OwnerActorSize.Z*VerticalSizeCorrection; //Some size we get from owner
        FCollisionObjectQueryParams ObjectParams(ECollisionChannel::ECC_WorldDynamic);
        //Check raycast
        if (OwnerActor->GetWorld()->LineTraceMultiByObjectType(ObjectsHit, RayTraceStart, RayTraceEnd, ObjectParams))
        {
            ProcessVerticallyBlockingGeometry(ObjectsHit);
        }
    }
}

void UAnterRayCastComponent::HorizontalBackRayCast()
{
    if (AActor* OwnerActor = Cast<AActor>(OwnerActorInterfacePtr))
    {
        TArray<FHitResult> ObjectsHit;
        FVector RayTraceStart = OwnerActor->GetActorLocation();
        OwnerActor->GetActorBounds(true, OwnerActorCentre, OwnerActorSize, false);
        FVector RayTraceEnd = RayTraceStart - FVector(OwnerActorSize.X * HorizontalSizeCorrection, 0.0f, 0.0f); //Some size we get from owner
        FCollisionObjectQueryParams ObjectParams(ECollisionChannel::ECC_WorldDynamic);
        //Check raycast
        if (OwnerActor->GetWorld()->LineTraceMultiByObjectType(ObjectsHit, RayTraceStart, RayTraceEnd, ObjectParams))
        {
            ProcessHorizontallyBlockingGeometry(ObjectsHit, false);
        }
    }
}

void UAnterRayCastComponent::HorizontalFrontRayCast()
{
    if (AActor* OwnerActor = Cast<AActor>(OwnerActorInterfacePtr))
    {
        TArray<FHitResult> ObjectsHit;
        FVector RayTraceStart = OwnerActor->GetActorLocation();
        OwnerActor->GetActorBounds(true, OwnerActorCentre, OwnerActorSize, false);
        FVector RayTraceEnd = RayTraceStart + FVector(OwnerActorSize.X * HorizontalSizeCorrection, 0.0f, 0.0f); //Some size we get from owner
        FCollisionObjectQueryParams ObjectParams(ECollisionChannel::ECC_WorldDynamic);
        //Check raycast
        if (OwnerActor->GetWorld()->LineTraceMultiByObjectType(ObjectsHit, RayTraceStart, RayTraceEnd, ObjectParams))
        {
            ProcessHorizontallyBlockingGeometry(ObjectsHit, true);
        }
    }
}

void UAnterRayCastComponent::SendGeometryAndFlags()
{
    FGeometron OwnerGeometry{ RotatedNormal.X, RotatedNormal.Z };
    OwnerActorInterfacePtr->ProcessRayCastGeometry(bHitVertically, bHitHorizontallyFront, bHitHorizontallyBack, OwnerGeometry, ImpactPoint);
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
                    //RotatedNormal = OtherActor->GetTransform().GetRotation().RotateVector(FVector::ForwardVector);
                    bHitVertically = true;
                    break;
                }
            }
        }
    }
}
