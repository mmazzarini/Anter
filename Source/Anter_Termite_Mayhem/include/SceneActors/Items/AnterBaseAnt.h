#pragma once

#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

#include "AnterBaseAnt.generated.h"

class UPrimitiveComponent;
struct FHitResult;

UENUM(BlueprintType)
enum class EAntCollectibleType : uint8
{
    NoType,
    HealthBarRecharge,
    WeaponRecharge,
    SpecialPowerRecharge
};

UCLASS(Blueprintable,BlueprintType)
class ANTER_TERMITE_MAYHEM_API AAnterBaseAnt : public AActor
{
    GENERATED_BODY()

public:

    AAnterBaseAnt(){}

    void BeginPlay() override;

    //UFUNCTION()
    //void OnAntCollided(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    const EAntCollectibleType GetAntType() const {return AntType;}

    const float GetAntCollectiblePoints() const {return AntCollectiblePoints;}

protected:

    USphereComponent* AntSphere;

    UPROPERTY(EditDefaultsOnly)
    UStaticMeshComponent* AntMesh;

    UPROPERTY(EditDefaultsOnly)
    EAntCollectibleType AntType = EAntCollectibleType::NoType;

    UPROPERTY(EditDefaultsOnly)
    float AntCollectiblePoints = 0.0f;

};
