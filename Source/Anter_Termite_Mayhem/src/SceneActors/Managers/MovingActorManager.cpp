#include "SceneActors/Managers/MovingActorManager.h"
#include "SceneActors/Platforms/MovingPlatform.h"
#include "ActorComponents/MovingActorMovementSupportComponent.h"
#include "SceneUtilities/SceneStructs.h"

#include "EngineUtils.h"

void AMovingActorManager::BeginPlay()
{
    Super::BeginPlay();

    CreateActor();
    
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
        FillActorPositions();
    }
    
    InjectActorBehavior();

}

void AMovingActorManager::FillActorPositions()
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

void AMovingActorManager::InjectActorBehavior()
{
    
    if(MovingActorMovementSupport != nullptr)
    {
        MovingActorMovementSupport->SetLoopBehavior(MovingActorLoopBehavior);
    }
}

void AMovingActorManager::CreateActor()
{
    if(MovingActor == nullptr)
    {
        if(MovingActorPositions.Num() > 0)
        {   
            MovingActor = GetWorld()->SpawnActor<AActor>(MovingActorClass,MovingActorPositions[0]+GetActorLocation(),GetActorRotation());
        }
        else
        {
            MovingActor = GetWorld()->SpawnActor<AActor>(MovingActorClass,GetActorLocation(),GetActorRotation());
        }
    }
}