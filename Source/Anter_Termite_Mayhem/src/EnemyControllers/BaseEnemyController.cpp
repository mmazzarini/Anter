#include "EnemyControllers/BaseEnemyController.h"
#include "SceneActors/Enemies/BaseEnemy.h"
#include "EngineUtils.h"

void ABaseEnemyController::BeginPlay()
{
    APawn* ControlledPawn = GetPawn();
    if(ControlledPawn != nullptr)
    {
        EnemyPawn = Cast<ABaseEnemy>(ControlledPawn);
    }
    if(EnemyPawn != nullptr && EnemyPawn->GetPivotState() == EEnemyPivotState::HasBeenFilled)       
    {
        EnemyPawn->MoveToNextPivot();
    }
}

void ABaseEnemyController::Tick(float DeltaSeconds)
{

}

