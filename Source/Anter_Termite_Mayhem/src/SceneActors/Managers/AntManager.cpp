#include "SceneActors/Managers/AntManager.h"
#include "SceneActors/Items/AnterBaseAnt.h"

#include "EngineUtils.h"

void AAntManager::BeginPlay()
{
    Super::BeginPlay();
    CreateActor();
}

void AAntManager::CreateActor()
{
    if((Ant != nullptr && Ant->IsActorBeingDestroyed()) || Ant == nullptr)
    {
        Ant = nullptr;
        Ant = GetWorld()->SpawnActor<AAnterBaseAnt>(AntClass,GetActorLocation(),GetActorRotation());
    }
}
