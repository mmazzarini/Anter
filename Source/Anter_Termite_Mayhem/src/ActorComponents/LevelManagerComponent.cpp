#include "ActorComponents/LevelManagerComponent.h"
#include "SceneUtilities/SceneStructs.h"
#include "GameFramework/GameMode.h"
#include "SceneActors/Managers/CrateManager.h"
#include "SceneActors/Managers/EnemyManager.h"
#include "Kismet/GameplayStatics.h"

ULevelManagerComponent::ULevelManagerComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;  
}

void ULevelManagerComponent::BeginPlay() 
{
    Super::BeginPlay();
    SetupLevelElements();
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
    TArray<AActor*> Checkpoints;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALevelCheckpoint::StaticClass(), Checkpoints);
    for(auto* Checkpoint : Checkpoints)
    {
        if(ALevelCheckpoint* CastedCheckpoint = Cast<ALevelCheckpoint>(Checkpoint))
        {
            CastedCheckpoint->OnActivatedCheckpointDelegate.AddDynamic(this,&ThisClass::OnCheckpointActivated);
            TPair<ALevelCheckpoint*,bool> CheckpointPair(CastedCheckpoint,false);
            LevelCheckpoints.Add(CheckpointPair);       
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
        OnActivatedOneCheckpointDelegate.Broadcast(InCheckpoint);
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