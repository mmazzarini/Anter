#pragma once

#include "FSMState/GameFSMState.h"
#include "Engine/GameInstance.h"

#include "AnterGameInstance.generated.h"

class ULevelInfoManager;

UCLASS()
class ANTER_TERMITE_MAYHEM_API UAnterGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:

    void Init() override;

    ULevelInfoManager* GetLevelInfoManager();

protected:

    UPROPERTY()
    ULevelInfoManager* LevelInfoManager;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<ULevelInfoManager> LevelInfoManagerClass;


};