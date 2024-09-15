#pragma once

#include "FSMState/GameFSMState.h"

#include "LevelGameplayFSMState.generated.h"

class AAnterBaseLevelGameMode;

/*
    I put here a snippet of commented code from USaveGame source (c.right Epic)
	 *	@see UGameplayStatics::CreateSaveGameObject
	 *	@see UGameplayStatics::SaveGameToSlot
	 *	@see UGameplayStatics::DoesSaveGameExist
	 *	@see UGameplayStatics::LoadGameFromSlot
	 *	@see UGameplayStatics::DeleteGameInSlot
*/

UCLASS()
class ANTER_TERMITE_MAYHEM_API ULevelGameplayFSMState : public UGameFSMState
{
    GENERATED_BODY()

public:

    void StartState() override;

	void EndState() override;

protected:

	UFUNCTION()
	void OnLevelFinished();

	UPROPERTY(EditInstanceOnly)
	FString LevelFinishedStateID = "";

private:

	AAnterBaseLevelGameMode* LevelGameMode;
};