#include "SceneActors/Managers/SceneActorManagerBase.h"
#include "AnterGameModes/AnterBaseLevelGameMode.h"
#include "Kismet/GameplayStatics.h"

void ASceneActorManagerBase::BindActorCreation(const UObject* WorldContextObject)
{
    if(AAnterBaseLevelGameMode* LevelGMode = Cast<AAnterBaseLevelGameMode>(UGameplayStatics::GetGameMode(WorldContextObject)))
    {
        LevelGMode->PlayerRestartedDelegate.AddDynamic(this,&ASceneActorManagerBase::ProceedToActorRefresh);
    }
}

void ASceneActorManagerBase::ProceedToActorRefresh()
{
    //Implement refresh in specific class
    RefreshActor();
    //This must be overridden
    //CreateActor();
    //FillActorPositions();
    //InjectActorBehavior();
    //SetupActor();
}