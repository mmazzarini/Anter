#pragma once 

#include "GameFramework/Actor.h"
#include "SceneUtilities/SceneStructs.h"
#include "SceneActors/Managers/EnemyManagerInterface.h"
#include "EnemyManager.generated.h"

/*
Manager class. It is used to manage enemy spawning and attribution of positions to make the enemy move iteratively
along an array of positions 
*/

class ABaseEnemy;
class ABaseEnemyBoundary;

UCLASS(Blueprintable,BlueprintType)
class ANTER_TERMITE_MAYHEM_API AEnemyManager : public AActor, public IEnemyManagerInterface
{
    GENERATED_BODY()

    AEnemyManager(){}

public:

    void BeginPlay() override;

    void FillEnemyPositions() override;

    void InjectEnemyBehavior() override;
protected:

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
