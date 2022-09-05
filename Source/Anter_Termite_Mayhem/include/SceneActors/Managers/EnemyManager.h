#include "GameFramework/Actor.h"

#include "EnemyManager.generated.h"

/*
Manager class. It is used to manage enemy spawning and attribution of positions to make the enemy move iteratively
along an array of positions 
*/

class ABaseEnemy;

UCLASS(Blueprintable,BlueprintType)
class ANTER_TERMITE_MAYHEM_API AEnemyManager : public AActor
{
    GENERATED_BODY()

    AEnemyManager(){}

public:

    void BeginPlay() override;

    UFUNCTION()
    void FillEnemyPositions();

    //This function will be changed. Is it to test communications with enemy
    UFUNCTION()
    void RegisterEnemy();

protected:

    UPROPERTY(EditDefaultsOnly)
    TArray<FVector> EnemyPositions;

    UPROPERTY()
    ABaseEnemy* Enemy;

};
