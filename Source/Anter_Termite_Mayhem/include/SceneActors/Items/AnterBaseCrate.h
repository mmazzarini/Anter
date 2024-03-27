#pragma once

#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

#include "AnterBaseCrate.generated.h"

class UPrimitiveComponent;
struct FHitResult;

UCLASS(Blueprintable,BlueprintType)
class ANTER_TERMITE_MAYHEM_API AAnterBaseCrate : public AActor
{
    GENERATED_BODY()

protected:

    UPROPERTY(EditDefaultsOnly)
    UStaticMeshComponent* CrateMesh;

};
