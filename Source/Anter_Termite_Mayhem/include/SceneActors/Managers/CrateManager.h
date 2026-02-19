#pragma once

#include "GameFramework/Actor.h"

#include "CrateManager.generated.h"

/*
#include "SceneUtilities/SceneStructs.h"
Manager class for crates. It is used to manage crate spawning and destruction state in between checkpoints 
*/

class ABaseEnemy;
class ABaseEnemyBoundary;

UCLASS(Blueprintable,BlueprintType)
class ANTER_TERMITE_MAYHEM_API ACrateManager : public AActor
{
    GENERATED_BODY()

public:

    virtual void BeginPlay() override;

    void CreateActor(){}

    void FillActorPositions(){}
    
    void InjectActorBehavior(){}
};