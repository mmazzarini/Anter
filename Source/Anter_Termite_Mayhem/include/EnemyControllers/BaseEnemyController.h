#pragma once

#include "AIController.h"
#include "SceneActors/Enemies/BaseEnemy.h"

#include "BaseEnemyController.generated.h"

UCLASS(BlueprintType)
class ANTER_TERMITE_MAYHEM_API ABaseEnemyController : public AAIController
{
    GENERATED_BODY()

public:

    ABaseEnemyController();

    void BeginPlay() override;

    void Tick(float DeltaSeconds) override;

protected:

    ABaseEnemy* EnemyPawn;

};

