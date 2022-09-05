#include "SceneActors/Managers/EnemyManager.h"
#include "SceneActors/Enemies/BaseEnemy.h"
#include "EngineUtils.h"

void AEnemyManager::BeginPlay()
{
    Super::BeginPlay();
    RegisterEnemy();
    if(Enemy != nullptr && EnemyPositions.Num() > 0)
    {
        FillEnemyPositions(Enemy);
    }
}

void AEnemyManager::RegisterEnemy()
{
    for(TActorIterator<ABaseEnemy> EnemyItr(GetWorld()); EnemyItr; ++EnemyItr)
    {
        Enemy = EnemyItr;
    }
}

void AEnemyManager::FillEnemyPositions()
{
    if(Enemy != nullptr)
    {
        Enemy->FillPositionArrays(EnemyPositions);
    }
}