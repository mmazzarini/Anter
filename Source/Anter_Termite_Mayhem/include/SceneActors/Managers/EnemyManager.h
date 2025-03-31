#pragma once 

#include "SceneUtilities/SceneStructs.h"
#include "SceneActors/Managers/SceneActorManagerBase.h"
#include "EnemyManager.generated.h"

/*
Manager class. It is used to manage enemy spawning and attribution of positions to make the enemy move iteratively
along an array of positions 
*/

class ABaseEnemy;
class ABaseEnemyBoundary;

UCLASS(Blueprintable,BlueprintType)
class ANTER_TERMITE_MAYHEM_API AEnemyManager : public ASceneActorManagerBase
{
    GENERATED_BODY()

public:

    void BeginPlay() override;

    void FillActorPositions() override;

    void InjectActorBehavior() override;

    void ResetActorMovement() override;

    UFUNCTION()
    virtual void SetBindings() override;

    //We handle Enemy death through this manager function
    UFUNCTION()
    void OnEnemyDeath();

    void CreateActor() override;

    void SetupActor() override{}

    void RefreshActor() override;

protected:

    void UnbindFromEnemy();

    //EnemyPositions represents the array of enemy pivot positions, calculated relative to the Manager ActorLocation 
    UPROPERTY(EditAnywhere)
    TArray<FVector> EnemyPositions;

    UPROPERTY()
    ABaseEnemy* Enemy;

    UPROPERTY(EditAnywhere)
    TSubclassOf<ABaseEnemy> EnemyClass;

    UPROPERTY(EditAnywhere)
    TSubclassOf<ABaseEnemyBoundary> EnemyBoundaryClass;

    UPROPERTY(EditInstanceOnly)
    float EnemyPivotDistanceThreshold;

    UPROPERTY(EditInstanceOnly)
    EEnemyLoopBehavior EnemyLoopBehavior;

};
