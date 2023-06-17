#include "SceneActors/Managers/MovingActorManager.h"
#include "SceneActors/Platforms/MovingPlatform.h"
#include "ActorComponents/MovingActorMovementSupportComponent.h"
#include "SceneUtilities/SceneStructs.h"

#include "EngineUtils.h"

void AMovingActorManager::BeginPlay()
{
    Super::BeginPlay();
    if(MovingActorPositions.Num() > 0)
    {   
        MovingActor = GetWorld()->SpawnActor<AActor>(MovingActorClass,MovingActorPositions[0]+GetActorLocation(),GetActorRotation());
    }
    else
    {
        MovingActor = GetWorld()->SpawnActor<AActor>(MovingActorClass,GetActorLocation(),GetActorRotation());
    }
    
    if(MovingActor != nullptr)
    {
        MovingActorMovementSupport = Cast<UMovingActorMovementSupportComponent>(MovingActor->FindComponentByClass(UMovingActorMovementSupportComponent::StaticClass()));
        if(MovingActorMovementSupport != nullptr)
        {
            MovingActorMovementSupport->SetPivotDistanceThreshold(MovingActorPivotDistanceThreshold);
        }
    }

    if(MovingActorPositions.Num() > 0)
    {
        FillMovingActorPositions();
    }
    
    InjectMovingActorBehavior();

}

void AMovingActorManager::FillMovingActorPositions()
{
    TArray<FVector> CorrectedPositions;
    for(FVector MovingActorPosition : MovingActorPositions)
    {
        CorrectedPositions.Add(MovingActorPosition + this->GetActorLocation());
    } 

    if(MovingActorMovementSupport != nullptr)
    {
        MovingActorMovementSupport->FillPositionArrays(CorrectedPositions);
    }

}

void AMovingActorManager::InjectMovingActorBehavior()
{
    
    if(MovingActorMovementSupport != nullptr)
    {
        MovingActorMovementSupport->SetLoopBehavior(MovingActorLoopBehavior);
    }
}