#pragma once

#include "GameFramework/GameMode.h"

#include "AnterBaseLevelGameMode.generated.h"

/*
Game mode class for defining the game properties of levels played as classical platform levels.
In this class we must define:
- Rules to setup and start levels
- Rules to decide when to finish or restart a level
- Rules to decide the game progression of the player through the level (e.g. binding to checkpoints via component...)
- Other similar properties and behaviors
*/

class AEnemyManager;
class ACrateManager;

UCLASS(Blueprintable, BlueprintType)
class ANTER_TERMITE_MAYHEM_API AAnterBaseLevelGameMode : public AGameMode
{
    GENERATED_BODY()

public:

    AAnterBaseLevelGameMode(){}

    void BeginPlay() override;

protected:

    /*Components that define specific behaviors. E.g. Level checkpoints are managed through a specific component*/
    void GetLevelGoalReference();

    void BindToEnemyManagers();


    /*Destroyables include boxes, enemies and items. We manage them through their managers*/

    /*We need a Goal object, that represents the logical end of the level*/
    AActor* LevelGoal; 

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<AActor> LevelGoalClass;

    TArray<AEnemyManager*> EnemyManagers;

    TArray<ACrateManager*> CrateManagers;
};