#pragma once

#include "FSMState/GameFSMState.h"

#include "SaveGameFSMState.generated.h"

class UAnterSaveGame;

UCLASS()
class ANTER_TERMITE_MAYHEM_API USaveGameFSMState : public UGameFSMState
{
    GENERATED_BODY()

public:

    void StartState() override;

protected: 

    void SaveGame();

    void InitializeSaveGameForSave();

    UFUNCTION()
    void OnGameSaved(const FString& SlotName, const int32 UserIndex, bool bSuccess);

    UPROPERTY(EditInstanceOnly)
    TSubclassOf<UAnterSaveGame> AnterSaveGameClass;

    UPROPERTY()
    UAnterSaveGame* AnterSaveGame;

    UPROPERTY(EditInstanceOnly)
    FString SaveSlotName = "None";

    UPROPERTY(EditInstanceOnly)
    FString SuccessTransitionID = "None";

    UPROPERTY(EditInstanceOnly)
    FString ErrorTransitionID = "None";

};