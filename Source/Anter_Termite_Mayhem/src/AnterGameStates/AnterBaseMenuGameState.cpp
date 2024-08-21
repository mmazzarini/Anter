#include "AnterGameStates/AnterBaseMenuGameState.h"

#include "GameFramework/GameUserSettings.h"

void AAnterBaseMenuGameState::HandleBeginPlay()
{
    Super::HandleBeginPlay();

    if(GEngine != nullptr)
    {
      GEngine->GetGameUserSettings()->SetScreenResolution(FIntPoint(640, 480));
      GEngine->GetGameUserSettings()->SetFullscreenMode(EWindowMode::Windowed);
      FIntPoint MyResolution = GEngine->GetGameUserSettings()->GetScreenResolution();
      FIntPoint MyNewResolution = MyResolution;

    }
    if(AnterMenuFSMClass != nullptr)
    {
        AnterMenuFSM = NewObject<UGameFSM>(this,AnterMenuFSMClass);
        if(AnterMenuFSM != nullptr)
        {
            AnterMenuFSM->InitializeFSM(this);
        }
    }
}