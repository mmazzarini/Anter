#include "SceneActors/Managers/EnemyManager.h"
#include "SceneActors/Enemies/BaseEnemy.h"
#include "SceneActors/Enemies/BaseEnemyBoundary.h"
#include "EngineUtils.h"

void AEnemyManager::BeginPlay()
{
    Super::BeginPlay();
    if(EnemyPositions.Num() > 0)
    {   
        for(int32 PositionIndex = 0;PositionIndex <EnemyPositions.Num();PositionIndex++)
        {
            GetWorld()->SpawnActor<ABaseEnemyBoundary>(EnemyBoundaryClass,EnemyPositions[PositionIndex],GetActorRotation());
        }
        Enemy = GetWorld()->SpawnActor<ABaseEnemy>(EnemyClass,EnemyPositions[0],GetActorRotation());
    }
    else
    {
        Enemy = GetWorld()->SpawnActor<ABaseEnemy>(EnemyClass,GetActorLocation(),GetActorRotation());
    }
    
    if(EnemyPositions.Num() > 0)
    {
        FillEnemyPositions();
    }
}

void AEnemyManager::FillEnemyPositions()
{
    if(Enemy != nullptr)
    {
        Enemy->FillPositionArrays(EnemyPositions);
    }
}