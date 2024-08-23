#include "FSMState/LoadGameFSMState.h"
#include "SaveGames/AnterSaveGame.h"
#include "Kismet/GameplayStatics.h"

void ULoadGameFSMState::StartState()
{
    Super::StartState();
    LoadGame();
}

void ULoadGameFSMState::LoadGame()
{
    FAsyncLoadGameFromSlotDelegate LoadedDelegate;
    LoadedDelegate.BindUObject(this, &ULoadGameFSMState::OnGameLoaded);
    UGameplayStatics::AsyncLoadGameFromSlot(LoadSlotName, 0, LoadedDelegate);
}

void ULoadGameFSMState::OnGameLoaded(const FString &SlotName, const int32 UserIndex, USaveGame *LoadedGame)
{
    InitializeSaveGameForLoad(LoadedGame);
    TransitionToState(ProceedTransitionID);
}
void ULoadGameFSMState::InitializeSaveGameForLoad(USaveGame *LoadedGame)
{
    AnterSaveGame = Cast<UAnterSaveGame>(LoadedGame);
    if(AnterSaveGame != nullptr)
    {
        AnterSaveGame->InitializeSaveGameForLoad();
    }
}