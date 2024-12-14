#include "FSMState/LevelFSMStates/LevelPauseFSMState.h"
#include "Kismet/GameplayStatics.h"

void ULevelPauseFSMState::StartState()
{
    Super::StartState();
    
    if (!UGameplayStatics::IsGamePaused(this))
    {
        UGameplayStatics::SetGamePaused(this, true);
    }
}

void ULevelPauseFSMState::EndState()
{
    Super::EndState();
}

void ULevelPauseFSMState::OnActionExecuted(FString InTriggerName)
{
    if (InTriggerName.Equals(BackToGameName))
    {
        UGameplayStatics::SetGamePaused(this, false);
    }
    Super::OnActionExecuted(InTriggerName);
}