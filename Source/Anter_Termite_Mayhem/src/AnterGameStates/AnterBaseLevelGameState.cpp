#include "AnterGameStates/AnterBaseLevelGameState.h"
#include "AnterPlayerStates/AnterLevelPlayerState.h"
#include "PlayerControllers/AnterPlayerController.h"
#include "Kismet/GameplayStatics.h"

void AAnterBaseLevelGameState::HandleBeginPlay()
{
    Super::HandleBeginPlay();

    /*override: after calling Super:: we bind the player state to the Pawn delegates*/
    if(PlayerArray.Num() > 0)
    {
        if(AAnterLevelPlayerState* CurrPlayerState = Cast<AAnterLevelPlayerState>(PlayerArray[0]))
        {
            uint64 MyPlayerAddress = (uint64)CurrPlayerState;
            CurrPlayerState->BindToPawnDelegates();
            CurrPlayerState->OnPlayerStateDeathDelegate.AddDynamic(this,&AAnterBaseLevelGameState::OnGameOver);
            CurrPlayerState->OnPlayerLevelGoalReachedDelegate.AddDynamic(this,&AAnterBaseLevelGameState::OnLevelCompleted);
        }
    }

    SetLevelCompletionState(ELevelCompletionState::LevelStarted);
}

void AAnterBaseLevelGameState::OnGameOver()
{
    AAnterPlayerController* PlayerController = Cast<AAnterPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (PlayerController != nullptr)
    {
       TArray<AActor*> GameOverActors;
       UGameplayStatics::GetAllActorsWithTag(GetWorld(),"GameOver",GameOverActors);
       if(GameOverActors.Num() > 0)
       {
            GameOverCameraActor = GameOverActors[0];
       }
       if(GameOverCameraActor != nullptr)
       {
           PlayerController->SetViewTarget(GameOverCameraActor); 
       }
    }
    SetLevelCompletionState(ELevelCompletionState::LevelGameOver);
}

void AAnterBaseLevelGameState::OnLevelCompleted()
{
    AAnterPlayerController* PlayerController = Cast<AAnterPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (PlayerController != nullptr)
    {
       TArray<AActor*> GameOverActors;
       UGameplayStatics::GetAllActorsWithTag(GetWorld(),"LevelCompleted",GameOverActors);
       if(GameOverActors.Num() > 0)
       {
           GameOverCameraActor = GameOverActors[0];
       }
       if(GameOverCameraActor != nullptr)
       {
           PlayerController->SetViewTarget(GameOverCameraActor); 
       }
    }
    SetLevelCompletionState(ELevelCompletionState::LevelCompleted);
}