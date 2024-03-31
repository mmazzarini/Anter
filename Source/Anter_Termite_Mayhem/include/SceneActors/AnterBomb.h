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

protected:

    UFUNCTION()
    void OnTimerEnded();

    UPROPERTY(EditDefaultsOnly)
    float BombGravityScale = 2.0f;

    UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
    UCollisionSupportComponent* BombCollisionSupport;

    void BindToBombCollision();

    /*
    FVector MovementVector;

    UPROPERTY(EditDefaultsOnly, Category="Laser Movement")
    float MovementX = 10.0f;

    UPROPERTY(EditDefaultsOnly, Category="Laser Movement")
    float MovementY = 0.0f;

    UPROPERTY(EditDefaultsOnly, Category="Laser Movement")
    float MovementZ = 0.0f;

    UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Laser Physics")
    float EditableMass = 0.0f;

    UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Projectile Kinematics")
    float EditableVelocity = 1000.0f;
 
    UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Laser Visibility")
    float ScreenAppearanceMultiplier = 1.2f;

    AAnterPlayerController* PlayerController; 

    UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
    UStaticMeshComponent* FireMesh;

    UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
    UProjectileMovementComponent* ProjectileMovement;

    UPROPERTY(EditAnywhere, Category = "Fire Movement")
    float MovementMultiplier = 100.0f;

    //Indicates how much weapon recharge this fire weapon consumes
    UPROPERTY(EditAnywhere, Category = "Fire Consumption")
    float ConsumptionAmount = 1.0f;
    */

    FTimerHandle ExplosionHandle;

    FTimerDelegate ExplosionDelegate;   

    /*Explosion timer, to be set by design*/
    UPROPERTY(EditDefaultsOnly)
    float ExplosionTimer = 3.0f;
};
