#pragma once 

#include "GameFramework/Actor.h"
#include "SceneActors/Managers/SceneActorManagerBase.h"

#include "AntManager.generated.h"

/*
Manager class for crates. It is used to manage crate spawning and destruction state in between checkpoints 
*/

class AAnterBaseAnt;

UCLASS(Blueprintable,BlueprintType)
class ANTER_TERMITE_MAYHEM_API AAntManager : public ASceneActorManagerBase
{
    GENERATED_BODY()

public:

    virtual void BeginPlay() override;
    
    void CreateActor() override;

    void FillActorPositions() override {}

    void InjectActorBehavior()override {}

    void SetupActor() override {}

    void RefreshActor() override{}

protected:

    UPROPERTY()
    AAnterBaseAnt* Ant;

    UPROPERTY(EditAnywhere)
    TSubclassOf<AAnterBaseAnt> AntClass;
};