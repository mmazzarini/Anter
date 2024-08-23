#include "FSMState/SaveGameFSMState.h"
#include "SaveGames/AnterSaveGame.h"
#include "Kismet/GameplayStatics.h"

void USaveGameFSMState::StartState()
{
    Super::StartState();
    InitializeSaveGameForSave();
}

void USaveGameFSMState::SaveGame()
{
    FAsyncSaveGameToSlotDelegate SavedDelegate;
    SavedDelegate.BindUObject(this, &USaveGameFSMState::OnGameSaved);
    UGameplayStatics::AsyncSaveGameToSlot(AnterSaveGame,SaveSlotName, 0, SavedDelegate);
}

void USaveGameFSMState::InitializeSaveGameForSave()
{
    AnterSaveGame = Cast<UAnterSaveGame>(UGameplayStatics::CreateSaveGameObject(AnterSaveGameClass));
    if(AnterSaveGame != nullptr)
    {
        AnterSaveGame->InitializeSaveGameForSave();
        SaveGame();
    }
}

void USaveGameFSMState::OnGameSaved(const FString &SlotName, const int32 UserIndex, bool bSuccess)
{
    if(bSuccess)
    {
        //TransitionToState(SuccessTransition)
    }
    else
    {
        //TransitionToState(ErrorTransition);
    }
}
