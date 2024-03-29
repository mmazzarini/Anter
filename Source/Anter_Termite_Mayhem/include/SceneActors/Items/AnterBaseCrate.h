#pragma once

#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ActorComponents/CollisionSupportComponent.h"
#include "SceneActors/SceneActorInterface.h"

#include "AnterBaseCrate.generated.h"

class UPrimitiveComponent;
struct FHitResult;

UCLASS(Blueprintable,BlueprintType)
class ANTER_TERMITE_MAYHEM_API AAnterBaseCrate : public AActor, public ISceneActorInterface
{
    GENERATED_BODY()

public:

    AAnterBaseCrate();

    void BeginPlay() override;

    virtual void SetBindings();

    /* Pure virtual functions */
    UFUNCTION()
    virtual void HandleCollision(const FCollisionGeometry& InCollisionGeometry, AActor* OtherActor) override;

    UFUNCTION()
    virtual void UpdateWeaponDirection(float InLaserDirection) override {};

    void OnVerticallyHit();

protected:

    UPROPERTY(EditDefaultsOnly)
    UStaticMeshComponent* CrateMesh;

    UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
    UCollisionSupportComponent* BaseCrateCollisionSupport;

    UFUNCTION()
    virtual void HandleDamage(AActor* OtherActor) override;

};