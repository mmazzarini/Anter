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
            FString MapOption = FString("");
            if (GetWorld() != nullptr)
            {
                if (FString* FoundOptionStr = GetWorld()->URL.Op.FindByKey("GoToMap"))
                {
                    MapOption = *FoundOptionStr;
                }
            }
            AnterMenuFSM->InitializeFSM(this, MapOption);
        }
    }
}