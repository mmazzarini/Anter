#include "EnemyControllers/BaseEnemyController.h"
#include "SceneActors/Enemies/BaseEnemy.h"
#include "EngineUtils.h"

void ABaseEnemyController::BeginPlay()
{
    for(TActorIterator<ABaseEnemy> EnemyItr(GetWorld()); EnemyItr; ++EnemyItr)
    {
        EnemyPawn = EnemyItr;
    }
    SetPawn(EnemyPawn);
    if(EnemyPawn != nullptr)
    {
        if(EnemyPawn->GetPivotState() == EEnemyPivotState::HasBeenFilled)       
        {
            EnemyPawn->MoveToNextPivot();
        } 
    }
}

void Tick(float DeltaSeconds)
{

}

