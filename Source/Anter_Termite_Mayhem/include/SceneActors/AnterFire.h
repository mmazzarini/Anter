#pragma once

#include "GameFramework/Actor.h"
//#include "PaperSpriteActor.h"
#include "PaperCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "PlayerControllers/AnterPlayerController.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "AnterFire.generated.h"


UCLASS(BlueprintType,Blueprintable)
class AAnterFire : public APaperCharacter
{
    GENERATED_BODY()

    public:

    AAnterFire();

    void Tick(float DeltaSeconds) override;

    virtual void BeginPlay() override;

    virtual void UpdatePosition();

    void CheckScreenLocation();

    void SetMovementToRight(bool InIsMovingRight);

    protected:

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
    float EditableVelocityX = 1000.0f;
 
    UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Laser Visibility")
    float ScreenAppearanceMultiplier = 1.2f;

    AAnterPlayerController* PlayerController; 

    UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
    UStaticMeshComponent* FireMesh;

    UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
    UProjectileMovementComponent* ProjectileMovement;

    UPROPERTY(EditAnywhere, Category = "Fire Movement")
    float MovementMultiplier = 100.0f;

    //Right movement correction
    float RightMovementCorrectionFactor = 1.0f;

};