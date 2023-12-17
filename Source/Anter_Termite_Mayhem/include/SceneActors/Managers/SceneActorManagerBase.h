#pragma once

#include "GameFramework/Actor.h"
#include "SceneActorManagerBase.generated.h"

//Base class, abstract

UCLASS(Abstract)
class ANTER_TERMITE_MAYHEM_API ASceneActorManagerBase : public AActor
{
    GENERATED_BODY()
public:

    virtual void FillActorPositions() PURE_VIRTUAL(ASceneActorManagerBase::FillActorPositions,);

    virtual void InjectActorBehavior() PURE_VIRTUAL(ASceneActorManagerBase::InjectActorBehavior,);

    virtual void CreateActor() PURE_VIRTUAL(ASceneActorManagerBase::CreateActor,);

    //This is used at beginplay to bind manager to gamemode
    virtual void BindActorCreation(const UObject* WorldContextObject);

    //This is bound to GMode restart event
    UFUNCTION()
    virtual void ProceedToActorCreation();

    virtual void SetupActor() PURE_VIRTUAL(ASceneActorManagerBase::SetupActor,);
};