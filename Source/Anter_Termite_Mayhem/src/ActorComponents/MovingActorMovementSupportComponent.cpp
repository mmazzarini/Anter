#include "ActorComponents/MovingActorMovementSupportComponent.h"

UMovingActorMovementSupportComponent::UMovingActorMovementSupportComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;  
}

void UMovingActorMovementSupportComponent::BeginPlay()
{
    Super::BeginPlay();
    OwningActor = GetOwner();
    OwnerMovement = ( OwningActor ? Cast<UBaseEnemyMovementComponent>(OwningActor->FindComponentByClass(UBaseEnemyMovementComponent::StaticClass())) : nullptr );
    if(OwnerMovement != nullptr)
    {
        OwnerMovement->Initialize();
    }

    //We set this at the beginning
    CurrentPivotPositions = PivotPositions;
}

void UMovingActorMovementSupportComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
    UpdateMovement();
}

void UMovingActorMovementSupportComponent::StartToMove()
{
    PivotState = EEnemyPivotState::HasStartedMoving; 
    MoveToNextPivot();
}

void UMovingActorMovementSupportComponent::MoveToNextPivot()
{
    //We first switch orientation if necessary, then we move to next pivot by updating the velocity.
    if(PivotArrayIndex == GetCurrentPivotPositionsArray().Num()-1)
    {
        SwitchOrientation();
    }

    PivotArrayIndex++;
    AdjustVelocity();
}

void UMovingActorMovementSupportComponent::UpdateMovement()
{
    if(bIsMovementActive)
    {
        if(PivotState == EEnemyPivotState::HasBeenFilled)
        {
            StartToMove();
        }
        if(OwningActor != nullptr)
        {
            FVector CurrentPositionToReach = CurrentPivotPositions[PivotArrayIndex];
            float PivotDist = FVector::Dist(OwningActor->GetActorLocation(),CurrentPositionToReach);
            if(CurrentPivotPositions.Num()>0 && PivotDist < PivotDistanceThreshold)
            {
                MoveToNextPivot();
            }
        }
    }
}

void UMovingActorMovementSupportComponent::SwitchOrientation()
{
    if(LoopBehavior == EEnemyLoopBehavior::GoesBackward)
    {
        if(CurrentPivotPositions == PivotPositions)
        {
            CurrentPivotPositions = ReversePivotPositions;
        }
        else
        {
            CurrentPivotPositions = PivotPositions;
            OnMovementCompleted.Broadcast();
        }
    }
    else
    {
        OnMovementCompleted.Broadcast();
    }
    PivotArrayIndex = 0;
}

void UMovingActorMovementSupportComponent::AdjustVelocity()
{
    if(OwningActor != nullptr)
    {
        FVector CurrentLocation = OwningActor->GetActorLocation();
        FVector NewLocation = CurrentPivotPositions[PivotArrayIndex];
        FVector LocationDistance = NewLocation - CurrentLocation;
        
        // Normalize distance vector
        if(LocationDistance.Size() != 0 && OwnerMovement != nullptr)
        {
            FVector NewGeometry;
            NewGeometry.X = LocationDistance.X/LocationDistance.Size();
            NewGeometry.Y = LocationDistance.Z/LocationDistance.Size();
            FVector2D New2DGeometry(NewGeometry.X,NewGeometry.Y);
            OwnerMovement->SetMovement(New2DGeometry);
        }
    }
}

void UMovingActorMovementSupportComponent::FillPositionArrays(TArray<FVector> InPositions)
{
    PivotPositions.Empty();
    ReversePivotPositions.Empty();
        
    if(InPositions.Num() != 0)
    {
        for(FVector InPosition : InPositions)
        {
            PivotPositions.Add(InPosition);
            CurrentPivotPositions.Add(InPosition);
        }
        //reverse loop
        for(int32 PositionIndex = InPositions.Num()-1; PositionIndex >= 0; PositionIndex--)
        {
            ReversePivotPositions.Add(InPositions[PositionIndex]);
        }
    }
    PivotState = EEnemyPivotState::HasBeenFilled;
}

void UMovingActorMovementSupportComponent::SetLoopBehavior(EEnemyLoopBehavior InLoopBehavior)
{
    LoopBehavior = InLoopBehavior;
}