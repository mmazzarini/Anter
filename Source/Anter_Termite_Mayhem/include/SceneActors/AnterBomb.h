#pragma once

#include "SceneActors/AnterFire.h"
/*
#include "PaperCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "PlayerControllers/AnterPlayerController.h"
#include "GameFramework/ProjectileMovementComponent.h"
*/
#include "TimerManager.h"
#include "SceneActors/SceneActorInterface.h"

#include "AnterBomb.generated.h"

class UCollisionSupportComponent;

UCLASS(BlueprintType,Blueprintable)
class ANTER_TERMITE_MAYHEM_API AAnterBomb : public AAnterFire, public ISceneActorInterface
{
    GENERATED_BODY()
    
public:
    
    AAnterBomb();

    virtual void BeginPlay() override;

    UFUNCTION()
    virtual void HandleCollision(const FCollisionGeometry& InCollisionGeometry, AActor* OtherActor) override;

    UFUNCTION()
    virtual void UpdateWeaponDirection(float InLaserDirection) override {};

    virtual void SetMovementToRight(FVector InMovementDirection);

    void HandleDamage(AActor* OtherActor) override {};

    virtual void ProcessRayCastGeometry(const FGeometron& InGeometron, bool bHitVertically, bool bHitHorizontallyFront, bool bHitHorizontallyBack) {}

protected:

    UFUNCTION()
    void OnTimerEnded();

    UPROPERTY(EditDefaultsOnly)
    float BombGravityScale = 2.0f;

    UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
    UCollisionSupportComponent* BombCollisionSupport;

    void BindToBombCollision();

    FTimerHandle ExplosionHandle;

    FTimerDelegate ExplosionDelegate;   

    /*Explosion timer, to be set by design*/
    UPROPERTY(EditDefaultsOnly)
    float ExplosionTimer = 3.0f;
};
