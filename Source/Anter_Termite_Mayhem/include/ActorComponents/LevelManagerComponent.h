#pragma once

#include "Components/SceneComponent.h"
#include "SceneActors/Items/LevelCheckpoint.h"

#include "LevelManagerComponent.generated.h"

//struct FConfigurablePlaceable;

class AEnemyManager;
class ACrateManager;
class AAntManager;
class AMovingActorManager;
class AAnterBaseLevelGameMode;

/* 
Level manager component class: it controls the flow of execution of levels: 
- it generates checkpoints, 
- it respawns player,
- it triggers enemy/object spawning by means of enemy managers etc.
It can be plugged into the Game Mode class that is specific for levels.  
*/

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOneCheckpointActivatedDelegate, ALevelCheckpoint*, CurrentCheckpointPtr);

UCLASS(Blueprintable,BlueprintType)
class ANTER_TERMITE_MAYHEM_API ULevelManagerComponent : public USceneComponent
{
    GENERATED_BODY()
 
public:

    ULevelManagerComponent();

    /*
    This function is used to setup all elements in the level
    - enemy manager
    - crate manager
    - object initializers etc
    */
    void SetupLevelElements();

    FOneCheckpointActivatedDelegate OnActivatedOneCheckpointDelegate;

    /*Components that define specific behaviors. E.g. Level checkpoints are managed through a specific component*/
    void GetLevelGoalReference();

    virtual void BeginPlay() override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:

    void GenerateCheckpoints();

    void DeleteBindings();

    UFUNCTION()
    void OnCheckpointActivated(ALevelCheckpoint* InCheckpoint);

    //This is used to bind all managers to gamemode
    void BindManagers();

    //Internal map, used to generate and keep track of all checkpoints
    TMap<ALevelCheckpoint*, bool> LevelCheckpoints;

    //External array, configurable on BP
    UPROPERTY(EditInstanceOnly)
    TArray<FVector> CheckpointsConfigurations;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<ALevelCheckpoint> CheckpointClass;  

    ALevelCheckpoint* CurrentCheckpoint;

    AAnterBaseLevelGameMode* OwnerGameMode = nullptr;

    /*We need a Goal object, that represents the logical end of the level*/
    AActor* LevelGoal; 

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<AActor> LevelGoalClass;

    TArray<AEnemyManager*> EnemyManagers;

    TArray<ACrateManager*> CrateManagers;

    TArray<AAntManager*> AntManagers;

    TArray<AMovingActorManager*> PlatformManagers;

    UPROPERTY(EditInstanceOnly)
    FString AnterPlayerStartString = "AnterPlayerStart";

};