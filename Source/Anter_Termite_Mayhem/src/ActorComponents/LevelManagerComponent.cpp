#include "ActorComponents/LevelManagerComponent.h"
#include "SceneUtilities/SceneStructs.h"
#include "SceneActors/Items/LevelCheckpoint.h"
#include "LevelManager.h"


void ALevelManagerComponent::BeginPlay()
{
    OwnerGameMode = GetOwner();
    GenerateCheckpoints();
}

void ALevelManagerComponent::EndPlay()
{
    DeleteBindings();
}

void ALevelManagerComponent::GenerateCheckpoints()
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

void ALevelManagerComponent::DeleteBindings()
{
    for(auto* Checkpoint : LevelCheckpoints)
    {
        if(Checkpoint != nullptr)
        {
            Checkpoint->OnActivatedCheckpointDelegate.RemoveDynamic(this,&ALevelManager::OnCheckpointActivated);
        }
    }
}

void ALevelManagerComponent::OnCheckpointActivated(ALevelCheckpoint* InCheckpoint)
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
