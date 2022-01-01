#pragma once

//#include "FSM/AnterFSM/AnterFSM.h"

#include "PaperCharacter.h"
#include "ActorComponents/AnterFSMComponent.h"
#include "ActorComponents/HealthComponent.h"
#include "ActorComponents/AnterCameraComponent.h"
#include "ActorComponents/AnterMovementSupportComponent.h"
#include "ActorComponents/AnterWeaponComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

#include "AnterPaperCharacter.generated.h"

class UInputComponent;

UENUM(BlueprintType)
enum class EPlatformCollisionType : uint8
{
    IsVeritcallyColliding,
    IsCollidingFromRight,
    IsCollidingFromLeft
};

/*
*
* Anter Paper Character class, representing the entity of the main character in the game
* Components are attached and refernced by the Pawn
*
*/

UCLASS(BlueprintType)
class ANTER_TERMITE_MAYHEM_API AAnterPaperCharacter : public APaperCharacter
{
    GENERATED_BODY()

public:

    AAnterPaperCharacter();

    //Basic ticker
    void Tick(float DeltaTime) override;

    void OnDeathEvent();

    virtual void BeginPlay() override;

    void SetBindings();

    void SetupGravity();

    virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

    void HandleRightMovement(float InAxisValue);

    void HandleStop();

    void HandleJump();

    void SetLeftMovementFree(bool InUnlocked){bIsLeftUnlocked = InUnlocked;}

    void SetRightMovementFree(bool InUnlocked){bIsRightUnlocked = InUnlocked;}

    UFUNCTION()
    void OnColliderHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
    UFUNCTION()
    void OnColliderUnhit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UFUNCTION()
    void SetCanJump(bool InCanJump);

    bool GetCanJump(){return bCanAnterJump;}

    void ConstrainJump();

    UFUNCTION()
    void SetIsFalling(bool InIsFalling);

    UFUNCTION()
    void RegisterPlatformCollision(AActor* InPlatformToAdd, EPlatformCollisionType InPlatformCollisionType);

    UFUNCTION()
    void DeregisterPlatformCollision(AActor* InPlatformToRemove);

    UFUNCTION()
    bool FindAnyCollisionOfType(EPlatformCollisionType InPlatformCollisionTypeToFind);

    void AdjustVelocity();

/* Anter Components */

UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
UAnterMovementSupportComponent* AnterMovementSupport;

UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
USpringArmComponent* Spring;

UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
UCameraComponent* Camera;

UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
UStaticMeshComponent* AnterMesh;

UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
UHealthComponent* AnterHealth;

UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
UAnterWeaponComponent* AnterWeapon;

UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
UBoxComponent* AnterBox;

protected:

UPROPERTY(EditAnywhere, Category = "Anter Movement")
float MovementMultiplier = 100.0f;

UPROPERTY(EditAnywhere, Category = "Anter Jump")
float JumpScale = 100.0f;

UPROPERTY(EditAnywhere, Category = "Anter Movement")
float FrictionScale = 0.5f;

UPROPERTY(EditAnywhere, Category = "Anter Movement")
float VelocityThreshold = 0.1f;

UPROPERTY(EditAnywhere, Category = "Anter Movement")
float InputGravityScale = 1.2f;

UPROPERTY(EditAnywhere, Category = "Anter Jump")
float ZVelocityThresholdToJump = -1.0f;

UPROPERTY(EditAnywhere, Category = "Platform collision")
float VerticalImpenetrabilityFactor = 3.0f;

UPROPERTY(EditDefaultsOnly, Category = "Platform collision")
float VerticalTolerance = 0.0f;

UPROPERTY(EditDefaultsOnly, Category = "Platform collision")
float HorizontalTolerance = 0.0f;

private:

bool bCanAnterJump = false;

bool bIsFalling = true;


bool bIsLeftUnlocked = true;

bool bIsRightUnlocked = true;

//Array of information about vertical collisions with platforms
TArray<TPair<AActor*,EPlatformCollisionType>> RegisteredVerticalPlatformCollisions;

};

