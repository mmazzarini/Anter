#include "AnterGameStates/AnterBaseLevelGameState.h"
#include "AnterPlayerStates/AnterLevelPlayerState.h"
#include "PlayerControllers/AnterPlayerController.h"
#include "AnterGameModes/AnterBaseLevelGameMode.h"
#include "Kismet/GameplayStatics.h"

void AAnterBaseLevelGameState::HandleBeginPlay()
{
    Super::HandleBeginPlay();

    /*override: after calling Super:: we bind the player state to the Pawn delegates*/
    BindToPlayerStatesInternal();

    SetLevelCompletionState(ELevelCompletionState::LevelStarted);
    
    if(AAnterBaseLevelGameMode* LevelGMode = Cast<AAnterBaseLevelGameMode>(AuthorityGameMode))
    {
        LevelGMode->PlayerRestartedDelegate.AddDynamic(this,&AAnterBaseLevelGameState::BindToPlayerStates);
    }

    //FSM initialization
    if (LevelMenuFSMClass != nullptr)
    {
        LevelMenuFSM = NewObject<UGameFSM>(this, LevelMenuFSMClass);
        if (LevelMenuFSM != nullptr)
        {
            LevelMenuFSM->InitializeFSM(this);
        }
    }

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
    OnDeathReached.Broadcast();
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

void AAnterBaseLevelGameState::BindToPlayerStates()
{
    UnbindFromPawnDelegates();

    if(PlayerArray.Num() > 0)
    {
        if(AAnterLevelPlayerState* CurrPlayerState = Cast<AAnterLevelPlayerState>(PlayerArray[0]))
        {
            CurrPlayerState->BindToPawnDelegates();
            if(!CurrPlayerState->OnPlayerStateDeathDelegate.IsBound())
            {
                CurrPlayerState->OnPlayerStateDeathDelegate.AddDynamic(this,&AAnterBaseLevelGameState::OnGameOver);
                CurrPlayerState->OnPlayerLevelGoalReachedDelegate.AddDynamic(this,&AAnterBaseLevelGameState::OnLevelCompleted);
            }
        }
    }
}

void AAnterBaseLevelGameState::BindToPlayerStatesInternal()
{
    if (PlayerArray.Num() > 0)
    {
        if (AAnterLevelPlayerState* CurrPlayerState = Cast<AAnterLevelPlayerState>(PlayerArray[0]))
        {
            CurrPlayerState->BindToPawnDelegates();
            if (!CurrPlayerState->OnPlayerStateDeathDelegate.IsBound())
            {
                CurrPlayerState->OnPlayerStateDeathDelegate.AddDynamic(this, &AAnterBaseLevelGameState::OnGameOver);
                CurrPlayerState->OnPlayerLevelGoalReachedDelegate.AddDynamic(this, &AAnterBaseLevelGameState::OnLevelCompleted);
            }
        }
    }
}

void AAnterBaseLevelGameState::UnbindFromPawnDelegates()
{
    if (PlayerArray.Num() > 0)
    {
        if (AAnterLevelPlayerState* CurrPlayerState = Cast<AAnterLevelPlayerState>(PlayerArray[0]))
        {
            CurrPlayerState->UnbindFromPawnDelegates();
        }
    }
}
