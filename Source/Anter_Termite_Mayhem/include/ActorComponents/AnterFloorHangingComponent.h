#pragma once

#include "Components/SceneComponent.h"
#include "SceneActors/Platforms/BasePlatform.h"
#include "AnterFloorHangingComponent.generated.h"

class AAnterPaperCharacter;

UCLASS(Blueprintable,BlueprintType)
class ANTER_TERMITE_MAYHEM_API UAnterFloorHangingComponent : public USceneComponent
{
    GENERATED_BODY()

public:

    UAnterFloorHangingComponent();

    virtual void BeginPlay() override;

    UFUNCTION()
    void MoveToUpsideDownPlatform();

    UFUNCTION()
    void DetachFromUpsideDownPlatform();

    //Function called to try to raycast to find vertical platform
    UFUNCTION()
    void AttemptRayCastByLineTrace();

protected:

    UPROPERTY(EditDefaultsOnly)
    float MaxRayTracingDistance = 100.f;

    AAnterPaperCharacter* Anter;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<ABasePlatform> PlatformClass;

};