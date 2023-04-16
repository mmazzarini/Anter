#pragma once

#include "GameFramework/Actor.h"
#include "LevelManager.generated.h"

struct FConfigurablePlaceable;
class ALevelCheckpoint;

/* 
Level manager class: it controls the flow of execution of levels: gneerates checkpoints, respawns 
player, triggers enemy spawning by enemy managers etc.
It has a level fo relation to the GameMode and the GameState. 

#TODO 16-04-2023, This level of relation to Game Mode&State has to be defined. We do not have a 
game mode and state that are custom to the game levels, yet, therefore this will be a future task
*/

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOneCheckpointActivatedDelegate)

UCLASS(Blueprintable,BlueprintType)
class ANTER_TERMITE_MAYHEM_API ALevelManager : public public AActor
{
    GENERATED_BODY()
 
public:

    ALevelManager(){}

    void BeginPlay() override;

    void EndPlay() override;

    FOneCheckpointActivatedDelegate OnActivatedOneCheckpointDelegate;

protected:

    void GenerateCheckpoints();

    void DeleteBindings();

    UFUNCTION()
    void OnCheckpointActivated(ALevelCheckpoint* InCheckpoint);

    //Internal map, used to generate and keep track of all checkpoints
    TMap<ALevelCheckpoint*, bool> LevelCheckpoints;

    //External array, configurable on BP
    UPROPERTY(EditInstanceOnly)
    TArray<FConfigurablePlaceable> CheckpointsConfigurations;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<ALevelCheckpoint> CheckpointClass;  

    ALevelCheckpoint* CurrentCheckpoint;

};