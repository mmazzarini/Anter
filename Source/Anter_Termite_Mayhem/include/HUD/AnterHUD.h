#pragma once

#include "GameFramework/HUD.h"

#include "AnterHUD.generated.h"

class AAnterBaseLevelGameState;

UCLASS(Blueprintable, BlueprintType)
class ANTER_TERMITE_MAYHEM_API AAnterHUD : public AHUD
{
	GENERATED_BODY()

public:

	void BeginPlay() override;

	void ShowDebugInfo(float& YL, float& YPos) override;

protected:

	TWeakObjectPtr<AAnterBaseLevelGameState> GameState;

};