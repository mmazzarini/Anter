#include "SceneActors/Managers/LevelManager.h"
#include "SceneUtilities/SceneStructs.h"
#include "SceneActors/Items/LevelCheckpoint.h"
#include "LevelManager.h"


void ALevelManager::BeginPlay()
{
    GenerateCheckpoints();
}

void ALevelManager::GenerateCheckpoints()
{
    if(CheckpointClass != nullptr)
    {
        for(FConfigurablePlaceable CheckpointConfig : CheckpointsConfigurations)
        {
            ALevelCheckpoint* NewCheckpoint = GetWorld()->SpawnActor<ALevelCheckpoint>(CheckpointClass,CheckpointConfig.PlaceablePosition,FQuat::Identity); 
            if(NewCheckpoint != nullptr)
            {
                NewCheckpoint->OnActivatedCheckpointDelegate.AddDynamic(this,&ALevelManager::OnCheckpointActivated);
                LevelCheckpoints.Add(NewCheckpoint);
            }
            
        }
    }
}

void ALevelManager::DeleteBindings()
{
    for(auto* Checkpoint : LevelCheckpoints)
    {
        if(Checkpoint != nullptr)
        {
            Checkpoint->OnActivatedCheckpointDelegate.RemoveDynamic(this,&ALevelManager::OnCheckpointActivated);
        }
    }
}

void ALevelManager::OnCheckpointActivated(ALevelCheckpoint* InCheckpoint)
{
    CurrentCheckpointIndex = LevelCheckpoints.IndexOfByPredicate([=InCheckpoint](ALevelCheckpoint* LC)
    {
        return LC == InCheckpoint;
    });
}
