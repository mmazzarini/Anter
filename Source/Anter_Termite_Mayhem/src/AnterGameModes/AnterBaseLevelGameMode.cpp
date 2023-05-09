#include "AnterGameModes/AnterBaseLevelGameMode.h"
#include "SceneActors/Managers/EnemyManager.h"
#include "SceneActors/Managers/CrateManager.h"
#include "ActorComponents/LevelManagerComponent.h"


void AAnterBaseLevelGameMode::OnLevelFinished()
{

}

void AAnterBaseLevelGameMode::StartPlay()
{
    Super::StartPlay();
    /*After calling start play on super (GameModeBase::StartPlay()), calls Start Level
    -- evaluate if using a timer in order to start level.
    */   
    StartLevel();
    //GetLevelGoalReference();
}

void AAnterBaseLevelGameMode::StartLevel()
{
    /* Call component to start level elements */
    ULevelManagerComponent* LevelManager = Cast<ULevelManagerComponent>(FindComponentByClass(LevelManagerClass));
    if(LevelManager != nullptr)
    {
        LevelManager->SetupLevelElements();
    }
}

void AAnterBaseLevelGameMode::RestartPlayer(AController* NewPlayer)
{
    Super::RestartPlayer(NewPlayer);
}

void AAnterBaseLevelGameMode::StartFSM()
{
    //empty for now
}

