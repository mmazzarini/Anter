#pragma once

#include "FSMState/GameFSMState.h"

#include "LoadGameFSMState.generated.h"

class UAnterSaveGame;

/*
    I put here a snippet of commented code from USaveGame source (c.right Epic)
	 *	@see UGameplayStatics::CreateSaveGameObject
	 *	@see UGameplayStatics::SaveGameToSlot
	 *	@see UGameplayStatics::DoesSaveGameExist
	 *	@see UGameplayStatics::LoadGameFromSlot
	 *	@see UGameplayStatics::DeleteGameInSlot
*/

UCLASS()
class ANTER_TERMITE_MAYHEM_API ULoadGameFSMState : public UGameFSMState
{
    GENERATED_BODY()

public:

    void StartState() override;

protected: 

    void LoadGame();

    UFUNCTION()
    void OnGameLoaded(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedGame);

    UFUNCTION()
    void InitializeSaveGameForLoad(USaveGame* LoadedGame);

    UPROPERTY(EditInstanceOnly)
    TSubclassOf<UAnterSaveGame> AnterSaveGameClass;

    UPROPERTY()
    UAnterSaveGame* AnterSaveGame;

    UPROPERTY(EditInstanceOnly)
    FString ProceedTransitionID = "None";


    UPROPERTY(EditInstanceOnly)
    FString LoadSlotName = "None";

};