#include "SceneActors/Managers/EnemyManager.h"
#include "SceneActors/Enemies/BaseEnemy.h"
#include "SceneActors/Enemies/BaseEnemyBoundary.h"
#include "SceneUtilities/SceneStructs.h"
#include "ActorComponents/HealthComponent.h"
#include "AnterGameModes/AnterBaseLevelGameMode.h"
#include "Kismet/GameplayStatics.h"

#include "EngineUtils.h"

void AEnemyManager::BeginPlay()
{
    Super::BeginPlay();
    CreateActor();
    FillActorPositions();
    InjectActorBehavior();
    SetBindings();

    /*
    if(AAnterBaseLevelGameMode* LevelGMode = Cast<AAnterBaseLevelGameMode>(UGameplayStatics::GetGameMode(this)))
    {
        LevelGMode->PlayerRestartedDelegate.AddDynamic(this,&AEnemyManager::SetBindings);
    }
    */

}

void AEnemyManager::FillActorPositions()
{
    if(EnemyPositions.Num() > 0)
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
    if((Enemy != nullptr && Enemy->IsActorBeingDestroyed()) || Enemy == nullptr)
    {
        Enemy = nullptr;
        if(EnemyPositions.Num() > 0)
        {   
            Enemy = GetWorld()->SpawnActor<ABaseEnemy>(EnemyClass,EnemyPositions[0]+GetActorLocation(),GetActorRotation());
        }
        else
        {
            Enemy = GetWorld()->SpawnActor<ABaseEnemy>(EnemyClass,GetActorLocation(),GetActorRotation());
        }
    }

    if(Enemy != nullptr)
    {
        Enemy->SetPivotDistanceThreshold(EnemyPivotDistanceThreshold);
    }
}

void AEnemyManager::RefreshActor()
{
    CreateActor();
    FillActorPositions();
    InjectActorBehavior();
    SetBindings();
    ResetActorMovement();
}

void AEnemyManager::ResetActorMovement()
{
    if(Enemy != nullptr)
    {
        Enemy->ResetMovement();
    }
}