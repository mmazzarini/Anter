#include "AnterGameModes/AnterBaseLevelGameMode.h"
#include "SceneActors/Managers/EnemyManager.h"
#include "SceneActors/Managers/CrateManager.h"

void AAnterBaseLevelGameMode::BeginPlay()
{
    Super::BeginPlay();
    GetLevelGoalReference();


}

void AAnterBaseLevelGameMode::GetLevelGoalReference()
{
    /*Get the level goal reference*/
    TArray<AActor*> LevelGoalsArray;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), LevelGoalClass, LevelGoalsArray);
    if(LevelGoalsArray.Num() > 0)
    {
        LevelGoal = LevelGoalsArray[0];
    }   
}

void AAnterBaseLevelGameMode::BindToManagers()
{
    /*Get all enemy managers reference*/
    EnemyManagers.Empty();
    TArray<AActor*> ActorEnemyManagers;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyManager, ActorEnemyManagers);
    for (auto* ActorEnemyManager : ActorEnemyManagers)
    {
        if(AEnemyManager* TempEnemyManager = Cast<AEnemyManger*>(ActorEnemyManager))
        {
            EnemyManagers.Add(TempEnemyManager);
        }
    }

    /*Get all crate managers refernce*/
    CrateManagers.Empty();
    TArray<AActor*> ActorCrateManagers;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyManager, ActorCrateManagers);
    for (auto* ActorCrateManager : ActorCrateManagers)
    {
        if(AEnemyManager* TempCrateManager = Cast<AEnemyManger*>(ActorCrateManager))
        {
            CrateManagers.Add(TempCrateManager);
        }
    }
}