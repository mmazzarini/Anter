#include "SceneActors/Managers/MovingActorManager.h"
#include "SceneActors/Platforms/MovingPlatform.h"
#include "ActorComponents/MovingActorMovementSupportComponent.h"
#include "SceneUtilities/SceneStructs.h"
#include "EngineUtils.h"

void AMovingActorManager::BeginPlay()
{
    Super::BeginPlay();

    CreateActor();
    FillActorPositions();
    InjectActorBehavior();
    SetupActor();
    SetBindings();
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
    if((MovingActor != nullptr && MovingActor->IsActorBeingDestroyed()) || MovingActor == nullptr)
    {
        MovingActor = nullptr;
        if(MovingActorPositions.Num() > 0)
        {   
            MovingActor = GetWorld()->SpawnActor<AActor>(MovingActorClass,MovingActorPositions[0]+GetActorLocation(),GetActorRotation());
        }
        else
        {
            MovingActor = GetWorld()->SpawnActor<AActor>(MovingActorClass,GetActorLocation(),GetActorRotation());
        }
    }

    if(MovingActorPositions.Num() > 0)
    {   
        MovingActor->SetActorLocation(MovingActorPositions[0]+GetActorLocation());
    }
    else
    {
        MovingActor->SetActorLocation(GetActorLocation());
    }

    if(MovingActor != nullptr)
    {
        MovingActorMovementSupport = Cast<UMovingActorMovementSupportComponent>(MovingActor->FindComponentByClass(UMovingActorMovementSupportComponent::StaticClass()));
        if(MovingActorMovementSupport != nullptr)
        {
            MovingActorMovementSupport->SetPivotDistanceThreshold(MovingActorPivotDistanceThreshold);
        }
    }
}

void AMovingActorManager::SetupActor()
{
    if(AMovingPlatform* MyPlatform = Cast<AMovingPlatform>(MovingActor))
    {
        MyPlatform->Setup();
    }
}

void AMovingActorManager::RefreshActor()
{
    CreateActor();
    FillActorPositions();
    InjectActorBehavior();
    SetupActor();
    SetBindings();
}
void AMovingActorManager::SetBindings()
{
    if(MovingActor != nullptr)
    {
        UHealthComponent* MovingActorHealth = Cast<UHealthComponent>(MovingActor->FindComponentByClass(UHealthComponent::StaticClass()));
        if(MovingActorHealth != nullptr)
        {
            MovingActorHealth->OnDeathReached.AddDynamic(this,&AMovingActorManager::OnActorDeath);
        }
    }  
}

void AMovingActorManager::OnActorDeath()
{
    if(MovingActor != nullptr)
    {
        UHealthComponent* MovingActorHealth = Cast<UHealthComponent>(MovingActor->FindComponentByClass(UHealthComponent::StaticClass()));
        if(MovingActorHealth != nullptr)
        {
            if(MovingActorHealth->OnDeathReached.IsBound())
            {
                MovingActorHealth->OnDeathReached.RemoveDynamic(this,&AMovingActorManager::OnActorDeath);
            }
        }
        MovingActor->Destroy();
    }    
}