#include "ActorComponents/LevelManagerComponent.h"
#include "SceneUtilities/SceneStructs.h"
#include "SceneActors/Items/LevelCheckpoint.h"


void ULevelManagerComponent::SetupLevelElements()
{
    OwnerGameMode = Cast<AGameMode>(GetOwner());
    GenerateCheckpoints();
}

void ULevelManagerComponent::EndPlay()
{
    DeleteBindings();
}

void ULevelManagerComponent::GenerateCheckpoints()
{
    if(CheckpointClass != nullptr)
    {
        for(FConfigurablePlaceable CheckpointConfig : CheckpointsConfigurations)
        {
            ALevelCheckpoint* NewCheckpoint = GetWorld()->SpawnActor<ALevelCheckpoint>(CheckpointClass,CheckpointConfig.PlaceablePosition,FQuat::Identity); 
            if(NewCheckpoint != nullptr)
            {
                NewCheckpoint->OnActivatedCheckpointDelegate.AddDynamic(this,&ALevelManager::OnCheckpointActivated);
                TPair<ALevelCheckpoint*,bool> CheckpointPair(NewCheckpoint,false);
                LevelCheckpoints.AddUnique(CheckpointPair);
            }          
        }
    }
}

void ULevelManagerComponent::DeleteBindings()
{
    for(auto* Checkpoint : LevelCheckpoints)
    {
        if(Checkpoint != nullptr)
        {
            Checkpoint->OnActivatedCheckpointDelegate.RemoveDynamic(this,&ALevelManager::OnCheckpointActivated);
        }
    }
}

void ULevelManagerComponent::OnCheckpointActivated(ALevelCheckpoint* InCheckpoint)
{
    /* Update current checkpoint ptr */
    if(InCheckpoint != nullptr)
    {
        CurrentCheckpoint = InCheckpoint;
    }
    
    /*If checkpoint was not activated before (bool == false), we activate it*/
    bool* CheckpointBoolPtr = LevelCheckpoints.Find(InCheckpoint);
    if(CheckpointBoolPtr != nullptr && *CheckpointBoolPtr == false)
    {
        *CheckpointBoolPtr = true;
        //Activate delegate to notify about checkpoint activation
        OnActivatedOneCheckpointDelegate.Broadcast();
    }    
}

void ULevelManagerComponent::GetLevelGoalReference()
{
    /*Get the level goal reference*/
    TArray<AActor*> LevelGoalsArray;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), LevelGoalClass, LevelGoalsArray);
    if(LevelGoalsArray.Num() > 0)
    {
        LevelGoal = LevelGoalsArray[0];
    }
}

void ULevelManagerComponent::BindToManagers()
{
    /*Get all enemy managers reference*/
    EnemyManagers.Empty();
    TArray<AActor*> ActorEnemyManagers;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyManager, ActorEnemyManagers);
    for (auto* ActorEnemyManager : ActorEnemyManagers)
    {
        if(AEnemyManager* TempEnemyManager = Cast<AEnemyManger*>(ActorEnemyManager))
        {
            EnemyManagers.Add(TempEnemyManager);
        }
    }

    /*Get all crate managers references*/
    CrateManagers.Empty();
    TArray<AActor*> ActorCrateManagers;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyManager, ActorCrateManagers);
    for (auto* ActorCrateManager : ActorCrateManagers)
    {
        if(AEnemyManager* TempCrateManager = Cast<AEnemyManger*>(ActorCrateManager))
        {
            CrateManagers.Add(TempCrateManager);
        }
    }
}