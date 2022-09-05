#pragma once

#include "AIController.h"

#include "BaseEnemyController.generated.h"

UCLASS(BlueprintType)
class ANTER_TERMITE_MAYHEM_API ABaseEnemyController : public AAIController
{

    void BeginPlay() override;

    void Tick(float DeltaSeconds) override;

    GENERATED_BODY()


protected:

    ABaseEnemy* EnemyPawn;

};

