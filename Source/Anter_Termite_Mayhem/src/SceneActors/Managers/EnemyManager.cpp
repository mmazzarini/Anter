#include "SceneActors/Managers/EnemyManager.h"
#include "SceneActors/Enemies/BaseEnemy.h"
#include "SceneActors/Enemies/BaseEnemyBoundary.h"
#include "SceneUtilities/SceneStructs.h"

#include "EngineUtils.h"

void AEnemyManager::BeginPlay()
{
    Super::BeginPlay();
    if(EnemyPositions.Num() > 0)
    {   
        /*
        for(int32 PositionIndex = 0;PositionIndex <EnemyPositions.Num();PositionIndex++)
        {
            GetWorld()->SpawnActor<ABaseEnemyBoundary>(EnemyBoundaryClass,EnemyPositions[PositionIndex],GetActorRotation());
        }
        */
        Enemy = GetWorld()->SpawnActor<ABaseEnemy>(EnemyClass,EnemyPositions[0]+GetActorLocation(),GetActorRotation());
    }
    else
    {
        Enemy = GetWorld()->SpawnActor<ABaseEnemy>(EnemyClass,GetActorLocation(),GetActorRotation());
    }
    
    if(Enemy != nullptr)
    {
        Enemy->SetPivotDistanceThreshold(EnemyPivotDistanceThreshold);
    }

    if(EnemyPositions.Num() > 0)
    {
        FillEnemyPositions();
    }
    
    InjectEnemyLoopBehavior();
}

void AEnemyManager::FillEnemyPositions()
{
    TArray<FVector> CorrectedPositions;
    for(FVector EnemyPosition : EnemyPositions)
    {
        CorrectedPositions.Add(EnemyPosition + this->GetActorLocation());
    } 

    if(Enemy != nullptr)
    {
        Enemy->FillPositionArrays(CorrectedPositions);
    }
}

void AEnemyManager::InjectEnemyLoopBehavior()
{
    if(Enemy != nullptr)
    {
        Enemy->SetLoopBehavior(EnemyLoopBehavior);
    }
}