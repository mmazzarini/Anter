#include "SceneActors/Managers/EnemyManager.h"
#include "SceneActors/Enemies/BaseEnemy.h"
#include "SceneActors/Enemies/BaseEnemyBoundary.h"
#include "SceneUtilities/SceneStructs.h"
#include "ActorComponents/HealthComponent.h"

#include "EngineUtils.h"

void AEnemyManager::BeginPlay()
{
    Super::BeginPlay();
    CreateActor();
    //The following has to be moved into CreateEnemy
    
    if(Enemy != nullptr)
    {
        Enemy->SetPivotDistanceThreshold(EnemyPivotDistanceThreshold);
    }

    if(EnemyPositions.Num() > 0)
    {
        FillActorPositions();
    }
    
    InjectActorBehavior();

    SetBindings();
    
}

void AEnemyManager::FillActorPositions()
{
    TArray<FVector> CorrectedPositions;
    for(FVector EnemyPosition : EnemyPositions)
    {
        CorrectedPositions.Add(EnemyPosition + this->GetActorLocation());
    } 

    if(Enemy != nullptr)
    {
        Enemy->FillPositionArrays(CorrectedPositions);
    }
}

void AEnemyManager::InjectActorBehavior()
{
    if(Enemy != nullptr)
    {
        Enemy->SetLoopBehavior(EnemyLoopBehavior);
    }
}

void AEnemyManager::SetBindings()
{
    UHealthComponent* EnemyHealth = Cast<UHealthComponent>(Enemy->FindComponentByClass(UHealthComponent::StaticClass()));
    if(EnemyHealth != nullptr)
    {
        EnemyHealth->OnDeathReached.AddDynamic(this,&AEnemyManager::OnEnemyDeath);
    }
}

void AEnemyManager::OnEnemyDeath()
{

    if(Enemy != nullptr)
    {
        UHealthComponent* EnemyHealth = Cast<UHealthComponent>(Enemy->FindComponentByClass(UHealthComponent::StaticClass()));
        if(EnemyHealth != nullptr)
        {
            if(EnemyHealth->OnDeathReached.IsBound())
            {
                EnemyHealth->OnDeathReached.RemoveDynamic(this,&AEnemyManager::OnEnemyDeath);
            }
        }
        Enemy->Destroy();
    }    
}

void AEnemyManager::CreateActor()
{
    if(Enemy == nullptr)
    {
        if(EnemyPositions.Num() > 0)
        {   
            Enemy = GetWorld()->SpawnActor<ABaseEnemy>(EnemyClass,EnemyPositions[0]+GetActorLocation(),GetActorRotation());
        }
        else
        {
            Enemy = GetWorld()->SpawnActor<ABaseEnemy>(EnemyClass,GetActorLocation(),GetActorRotation());
        }
    }
}