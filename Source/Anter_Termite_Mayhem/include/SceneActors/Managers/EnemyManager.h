#include "GameFramework/Actor.h"

#include "EnemyManager.generated.h"

/*
Manager class. It is used to manage enemy spawning and attribution of positions to make the enemy move iteratively
along an array of positions 
*/

class ABaseEnemy;
class ABaseEnemyBoundary;

UCLASS(Blueprintable,BlueprintType)
class ANTER_TERMITE_MAYHEM_API AEnemyManager : public AActor
{
    GENERATED_BODY()

    AEnemyManager(){}

public:

    void BeginPlay() override;

    UFUNCTION()
    void FillEnemyPositions();


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

};
