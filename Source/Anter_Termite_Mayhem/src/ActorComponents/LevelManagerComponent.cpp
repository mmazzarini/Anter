#include "ActorComponents/LevelManagerComponent.h"
#include "SceneUtilities/SceneStructs.h"
#include "SceneActors/Items/LevelCheckpoint.h"
#include "GameFramework/GameMode.h"
#include "SceneActors/Managers/CrateManager.h"
#include "SceneActors/Managers/EnemyManager.h"
#include "Kismet/GameplayStatics.h"

ULevelManagerComponent::ULevelManagerComponent(const FObjectInitializer& Obj)
{

}

void ULevelManagerComponent::SetupLevelElements()
{
    OwnerGameMode = Cast<AGameMode>(GetOwner());
    GenerateCheckpoints();
}

void ULevelManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    DeleteBindings();
    Super::EndPlay(EndPlayReason);
}

void ULevelManagerComponent::GenerateCheckpoints()
{
    if(CheckpointClass != nullptr)
    {
        for(const FVector& CheckpointConfig : CheckpointsConfigurations)
        {
            ALevelCheckpoint* NewCheckpoint = GetWorld()->SpawnActor<ALevelCheckpoint>(CheckpointClass,CheckpointConfig,FRotator(0.0f,0.0f,0.0f)); 
            if(NewCheckpoint != nullptr)
            {
                NewCheckpoint->OnActivatedCheckpointDelegate.AddDynamic(this,&ThisClass::OnCheckpointActivated);
                TPair<ALevelCheckpoint*,bool> CheckpointPair(NewCheckpoint,false);
                LevelCheckpoints.Add(CheckpointPair);
            }          
        }
    }
}

void ULevelManagerComponent::DeleteBindings()
{
    for(const TPair<ALevelCheckpoint*, bool >& Checkpoint : LevelCheckpoints)
    {
        if(Checkpoint.Key != nullptr)
        {
            Checkpoint.Key->OnActivatedCheckpointDelegate.RemoveDynamic(this,&ThisClass::OnCheckpointActivated);
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
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyManager::StaticClass(), ActorEnemyManagers);
    for (auto* ActorEnemyManager : ActorEnemyManagers)
    {
        if(AEnemyManager* TempEnemyManager = Cast<AEnemyManager>(ActorEnemyManager))
        {
            EnemyManagers.Add(TempEnemyManager);
        }
    }

    /*Get all crate managers references*/
    CrateManagers.Empty();
    TArray<AActor*> ActorCrateManagers;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACrateManager::StaticClass(), ActorCrateManagers);
    for (auto* ActorCrateManager : ActorCrateManagers)
    {
        if(ACrateManager* TempCrateManager = Cast<ACrateManager>(ActorCrateManager))
        {
            CrateManagers.Add(TempCrateManager);
        }
    }
}