#include "SceneActors/Managers/EnemyManager.h"
#include "SceneActors/Enemies/BaseEnemy.h"
#include "EngineUtils.h"

void AEnemyManager::BeginPlay()
{
    Super::BeginPlay();
    Enemy = GetWorld()->SpawnActor<ABaseEnemy>(EnemyClass,GetActorLocation(),GetActorRotation());
    
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