#include "SceneActors/Managers/SceneActorManagerBase.h"
#include "AnterGameModes/AnterBaseLevelGameMode.h"
#include "Kismet/GameplayStatics.h"

void ASceneActorManagerBase::BindActorCreation(const UObject* WorldContextObject)
{
    if(AAnterBaseLevelGameMode* LevelGMode = Cast<AAnterBaseLevelGameMode>(UGameplayStatics::GetGameMode(WorldContextObject)))
    {
        LevelGMode->PlayerRestartedDelegate.AddDynamic(this,&ASceneActorManagerBase::ProceedToActorCreation);
    }
}

void ASceneActorManagerBase::ProceedToActorCreation()
{
    //This must be overridden
    CreateActor();
    FillActorPositions();
    InjectActorBehavior();
}