#include "SceneActors/Managers/AntManager.h"
#include "SceneActors/Items/AnterBaseAnt.h"

#include "EngineUtils.h"

void AAntManager::BeginPlay()
{
    Super::BeginPlay();
    Ant = GetWorld()->SpawnActor<AAnterBaseAnt>(AntClass,GetActorLocation(),GetActorRotation());
}
