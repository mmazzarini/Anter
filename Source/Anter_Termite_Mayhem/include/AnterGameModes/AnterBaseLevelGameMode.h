#pragma once

#include "GameFramework/GameModeBase.h"

#include "AnterBaseLevelGameMode.generated.h"

/*
Game mode class for defining the game properties of levels played as classical platform levels.
In this class we must define:
- Rules to setup and start levels
- Rules to decide when to finish or restart a level
- Rules to decide the game progression of the player through the level (e.g. binding to checkpoints via component...)
- Other similar properties and behaviors
*/

/*#TODO Ci servirà una funzione per fare handling di finish di livello
Dovrà esseree fatto così:
- al goal raggiunto, parte una transizione verso il post-goal
- al post goal, il giocatore può decidere se fare restart, prosegui, o vedi stats
- si ricomincia il livello o si prosegue
- al prosegui le stats vengono salvate e si fa transition to map o to next level
:: queste info verranno poi inserite in inglese in tale documentzione in-code
::  e soprattutto per il postgoal aspettiamo.
*/

class AEnemyManager;
class ACrateManager;
class ULevelManagerComponent;

UCLASS(Blueprintable, BlueprintType)
class ANTER_TERMITE_MAYHEM_API AAnterBaseLevelGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:

    AAnterBaseLevelGameMode(){}

    /*Handles level completion for a series of operations:
    - restart level
    - migrate to next level or map
    - calculate scores and rewards + savestate
    */
    UFUNCTION()
    void OnLevelFinished();

    /*
    Override basic startplay function to call a series of initializations 
    to start the level
    */
    virtual void StartPlay() override;

    /*Executes the operations to start the level and spawn the first elements in the scene*/
    UFUNCTION()
    virtual void StartLevel() override;

    //override base function from GameModeBase. We need to understand how to develop it
    virtual void RestartPlayer(AController* NewPlayer) override;

    /*Starts the Level FSM --- #TODO needs to be defined!!!! */
    UFUNCTION()
    void StartFSM();

protected:

    /*Protected variable for LevelManager*/
    ULevelManagerComponent* LevelManager

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<ULevelManagerComponent> LevelManagerClass;

};