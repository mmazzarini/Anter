#pragma once

#include "Components/SceneComponent.h"
#include "SceneUtilities/SceneStructs.h"
#include "SceneActors/SceneActorInterface.h"

#include "CollisionSupportComponent.generated.h"

UCLASS(BlueprintType)
class ANTER_TERMITE_MAYHEM_API UCollisionSupportComponent : public USceneComponent
{
    GENERATED_BODY()

public:

    UCollisionSupportComponent();

    void RegisterInterfaceOwnerCharacter(ISceneActorInterface* ActorOwnerInterface);

    UFUNCTION()
    void ProcessCollisionGeometry(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
    
protected:

    UPROPERTY(EditDefaultsOnly)
    bool bTurnOnSystem = true;

private: 

    ISceneActorInterface* OwnerActorInterfacePtr = nullptr;

};