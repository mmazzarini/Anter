#pragma once

#include "Components/ActorComponent.h"
#include "LevelManagerComponent.generated.h"

struct FConfigurablePlaceable;
class ALevelCheckpoint;

/* 
Level manager component class: it controls the flow of execution of levels: gneerates checkpoints, respawns 
player, triggers enemy spawning by enemy managers etc.
It can be plugged into the Game Mode class specific for levels.  
*/

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOneCheckpointActivatedDelegate)

UCLASS(Blueprintable,BlueprintType)
class ANTER_TERMITE_MAYHEM_API ULevelManagerComponent : public UActorComponent
{
    GENERATED_BODY()
 
public:

    ULevelManagerComponent(){}

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

    AGameMode* OwnerGameMode = nullptr;

};