#include "HUD/AnterHUD.h"
#include "AnterGameStates/AnterBaseLevelGameState.h"
#include "Kismet/GameplayStatics.h"
#include "DisplayDebugHelpers.h"

void AAnterHUD::BeginPlay()
{
	Super::BeginPlay();

	GameState = Cast<AAnterBaseLevelGameState>(UGameplayStatics::GetGameState(this));
}

void AAnterHUD::ShowDebugInfo(float& YL, float& YPos)
{
	Super::ShowDebugInfo(YL, YPos);
	if (GameState.IsValid() && DebugCanvas != nullptr)
	{
		FDebugDisplayInfo GStateDisplayInfo(DebugDisplay, ToggledDebugCategories);
		GameState->DisplayDebug(DebugCanvas, GStateDisplayInfo, YL, YPos);
	}
}
