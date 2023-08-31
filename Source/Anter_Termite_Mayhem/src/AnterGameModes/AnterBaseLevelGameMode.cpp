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

AAnterBaseLevelGameMode::AAnterBaseLevelGameMode()
{
    LevelManager = CreateDefaultSubobject<ULevelManagerComponent>(TEXT("LevelManager"));
    LevelManager->SetupAttachment(RootComponent);
}

void AAnterBaseLevelGameMode::OnLevelFinished()
{
    GetWorldTimerManager().SetTimer(EndLevelTimerHandle, this, &AAnterBaseLevelGameMode::OnEndLevelTimerEnded, NumSecondsForLevelEnd, false, NumSecondsForLevelEnd);
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

void AAnterBaseLevelGameMode::OnLevelGameOver()
{
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
    if(InCheckpoint != nullptr)
    {
        APlayerController* CurrentPC;
        TArray<AActor*> CurrentPCs;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerController::StaticClass(), CurrentPCs);
        if(CurrentPCs.Num() > 0)
        {
            CurrentPC = Cast<APlayerController>(CurrentPCs[0]);
            if(CurrentPC != nullptr)
            {
                AActor* LevelStart = FindPlayerStart(CurrentPC,AnterPlayerStartString);
                if(LevelStart != nullptr)
                {
                    FVector OldLocation = LevelStart->GetActorLocation();
                    LevelStart->SetActorLocation(InCheckpoint->GetActorLocation() + FVector::UpVector*100.0f);
                    FVector NewLocation = LevelStart->GetActorLocation();
                    FVector DiffLocation = OldLocation-NewLocation;
                }
            }
        }
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
