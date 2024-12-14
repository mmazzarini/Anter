#pragma once

#include "FSMState/GameFSMState.h"

#include "LevelPauseFSMState.generated.h"

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
class ANTER_TERMITE_MAYHEM_API ULevelPauseFSMState : public UGameFSMState
{
    GENERATED_BODY()

public:

    void StartState() override;

	void OnActionExecuted(FString InTriggerName) override;

	void EndState() override;

protected:

	UPROPERTY(EditDefaultsOnly)
	FString BackToGameName = "Back";
};