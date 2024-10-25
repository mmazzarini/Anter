#include "AnterGameModes/AnterBaseLevelGameMode.h"
#include "AnterGameStates/AnterBaseLevelGameState.h"
#include "SceneActors/Managers/EnemyManager.h"
#include "SceneActors/Managers/CrateManager.h"
#include "ActorComponents/LevelManagerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerController.h"
#include "SceneActors/Items/LevelGoal.h"
#include "SceneActors/Items/LevelCheckpoint.h"
#include "SceneActors/Managers/LevelInfoManager.h"

AAnterBaseLevelGameMode::AAnterBaseLevelGameMode()
{
    LevelManager = CreateDefaultSubobject<ULevelManagerComponent>(TEXT("LevelManager"));
    LevelManager->SetupAttachment(RootComponent);
}

void AAnterBaseLevelGameMode::OnLevelFinished()
{
    GetWorldTimerManager().ClearTimer(EndLevelTimerHandle);
    UE_LOG(LogTemp, Warning, TEXT("GameMode: Started timer because of level finished!"));
    UpdateLevelStats();
    GetWorldTimerManager().SetTimer(EndLevelTimerHandle, this, &AAnterBaseLevelGameMode::OnLevelFinishedTimerEnded, NumSecondsForLevelEnd, false, NumSecondsForLevelEnd);
}

void AAnterBaseLevelGameMode::OnEndLevelTimerEnded()
{
    GetWorldTimerManager().ClearTimer(EndLevelTimerHandle);
    //Quit Game
    APlayerController* CurrentPC;
    TArray<AActor*> CurrentPCs;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerController::StaticClass(), CurrentPCs);
    if(CurrentPCs.Num() > 0)
    {
        CurrentPC = Cast<APlayerController>(CurrentPCs[0]);
        if(CurrentPC != nullptr)
        {
            UKismetSystemLibrary::QuitGame(this, CurrentPC ,EQuitPreference::Quit, true);
        }
    }
}

void AAnterBaseLevelGameMode::OnLevelFinishedTimerEnded()
{
    UE_LOG(LogTemp, Warning, TEXT("GameMode: Broadcasting level finished!"));
    LevelFinishedDelegate.Broadcast();
}

void AAnterBaseLevelGameMode::OnLevelGameOver()
{
    GetWorldTimerManager().ClearTimer(EndLevelTimerHandle);
    GetWorldTimerManager().SetTimer(EndLevelTimerHandle, this, &AAnterBaseLevelGameMode::OnLevelGameOverTimerEnded, NumSecondsForLevelEnd, false, NumSecondsForLevelEnd);
}

void AAnterBaseLevelGameMode::OnLevelGameOverTimerEnded()
{
    GetWorldTimerManager().ClearTimer(EndLevelTimerHandle);
    //Quit Game
    APlayerController* CurrentPC;
    TArray<AActor*> CurrentPCs;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerController::StaticClass(), CurrentPCs);
    if(CurrentPCs.Num() > 0)
    {
        CurrentPC = Cast<APlayerController>(CurrentPCs[0]);
        if(CurrentPC != nullptr)
        {
            RestartPlayer(CurrentPC);
        }
    }
}

void AAnterBaseLevelGameMode::StartPlay()
{
    Super::StartPlay();
    /*After calling start play on super (GameModeBase::StartPlay()), calls Start Level
    -- evaluate if using a timer in order to start level.
    */   
    StartLevel();
    //GetLevelGoalReference();
    /*
    if (GEngine != nullptr)
    {
        GEngine->SetMaxFPS(60);
    }
    */
}

void AAnterBaseLevelGameMode::StartLevel()
{
    /* Call component to start level elements */
    if(LevelManager != nullptr)
    {
        LevelManager->OnActivatedOneCheckpointDelegate.AddDynamic(this,&AAnterBaseLevelGameMode::UpdatePlayerStartPosition);
    }

    ALevelGoal* LevelGoal;
    TArray<AActor*> LevelGoals;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALevelGoal::StaticClass(), LevelGoals);
    if(LevelGoals.Num() > 0)
    {
        LevelGoal = Cast<ALevelGoal>(LevelGoals[0]);
        if(LevelGoal != nullptr)
        {
            LevelGoal->LevelGoalReached.AddDynamic(this,&AAnterBaseLevelGameMode::OnLevelFinished);
        }

    }

    if(GameState != nullptr)
    {
        if(AAnterBaseLevelGameState* CastedGameState = Cast<AAnterBaseLevelGameState>(GameState))
        {
            CastedGameState->OnDeathReached.AddDynamic(this,&AAnterBaseLevelGameMode::OnLevelGameOver); 
        }
    }
}

void AAnterBaseLevelGameMode::UpdatePlayerStartPosition(ALevelCheckpoint* InCheckpoint)
{
    //TODO may be deprecated
}

void AAnterBaseLevelGameMode::RestartPlayer(AController* NewPlayer)
{
    Super::RestartPlayer(NewPlayer);

    //Methodology: we just broadcast the restart, who needs to bind to this does that before.
    //Advantage: we leave the different objects and components (GState, LevelManager) independent and flexibly organised.
    PlayerRestartedDelegate.Broadcast();
}

void AAnterBaseLevelGameMode::TravelToMap(FString InMapString, TArray<FString> InOptions)
{
    UE_LOG(LogTemp, Warning, TEXT("GameMode: Travelling to map!"));
    GetWorldTimerManager().ClearTimer(EndLevelTimerHandle);
    FURL MapToTravelURL(*InMapString);
    for (FString Opt : InOptions)
    {
        MapToTravelURL.AddOption(*Opt);
    }
    GetWorld()->ServerTravel(MapToTravelURL.ToString());
}

void AAnterBaseLevelGameMode::UpdateLevelStats()
{
    if (ULevelInfoManager* LevelInfoManager = ULevelInfoManager::Get(this))
    {
        LevelInfoManager->SetCurrentLevelComplete();
    }
}
