#pragma once

#include "GameFramework/Actor.h"
#include "LevelManager.generated.h"

struct FConfigurablePlaceable;
class ALevelCheckpoint;

/* 
Level manager class: it controls the flow of execution of levels: gneerates checkpoints, respawns 
player, triggers enemy spawning by enemy managers etc.
*/

UCLASS(Blueprintable,BlueprintType)
class ANTER_TERMITE_MAYHEM_API ALevelManager : public public AActor
{
    GENERATED_BODY()
 
public:

    ALevelManager(){}

    void BeginPlay() override;


protected:

    void GenerateCheckpoints();

    void DeleteBindings();

    UFUNCTION()
    void OnCheckpointActivated(ALevelCheckpoint* InCheckpoint);

    //Internal array, used to generate and keep track of all checkpoints
    TArray<ALevelCheckpoint*> LevelCheckpoints;

    //External array, configurable on BP
    UPROPERTY(EditInstanceOnly)
    TArray<FConfigurablePlaceable> CheckpointsConfigurations;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<ALevelCheckpoint> CheckpointClass;  

    int32 CurrentCheckpointIndex;

};