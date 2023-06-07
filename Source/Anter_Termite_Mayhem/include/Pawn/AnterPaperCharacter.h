#pragma once

//#include "FSM/AnterFSM/AnterFSM.h"

#include "PaperCharacter.h"
#include "ActorComponents/AnterFSMComponent.h"
#include "ActorComponents/HealthComponent.h"
#include "ActorComponents/AnterCameraComponent.h"
#include "ActorComponents/AnterMovementSupportComponent.h"
#include "ActorComponents/AnterWeaponComponent.h"
#include "ActorComponents/CollisionSupportComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "SceneActors/SceneActorInterface.h"
#include "TimerManager.h"

#include "AnterPaperCharacter.generated.h"

class UInputComponent;
class AController;
class UCharacterMovementComponent;

UENUM(BlueprintType)
enum class EPlatformCollisionType : uint8
{
    IsVerticallyColliding,
    IsCollidingFromRight,
    IsCollidingFromLeft
};

UENUM(BlueprintType)
enum class EAnterHitableStatus : uint8
{
    CanBeHit,
    CannotBeHit
};


//Little structure to be referenced by Pawn in order to impose its local geometry while moving

struct FGeometron
{
    float X = 1.0f;
    float Z = 0.0f;
};

/*
*
* Anter Paper Character class, representing the entity of the main character in the game
* Components are attached and refernced by the Pawn
*
*/

UCLASS(BlueprintType)
class ANTER_TERMITE_MAYHEM_API AAnterPaperCharacter : public APaperCharacter, public ISceneActorInterface
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

    //UFUNCTION()
    //void OnColliderHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
    UFUNCTION()
    void OnColliderUnhit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UFUNCTION()
    void SetCanJump(bool InCanJump);

    bool GetCanJump(){return bCanAnterJump;}

    //This function is used to prevent player from jumping when falling too fast
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

    void ImposeGeometry(float InAngle);

    void ResetGeometron();

    void HandleCollision(const FCollisionGeometry& InCollisionGeometry, AActor* OtherActor) override;

    virtual void UpdateWeaponDirection() override;

    void HandlePlatform(const FCollisionGeometry& InCollisionGeometry, AActor* Platform);

    void HandleEnemy(AActor* Enemy);

    virtual void PossessedBy(AController* NewController) override;

    UFUNCTION()
    void OnUnhittableTimerEnded();

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

UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
UCollisionSupportComponent* AnterCollisionSupport;

protected:

    //Actual jump processing method
    void ProcessJump(float InJumpValue, UCharacterMovementComponent* InAnterMovement);

    //Kick for collding an actor that damages Anter
    void HandleKick(FVector InKickToReceive, UCharacterMovementComponent* InAnterMovement);

UPROPERTY(EditAnywhere, Category = "Anter Movement")
float MovementMultiplier = 100.0f;

UPROPERTY(EditAnywhere, Category = "Anter Movement")
float ZAscendingMultiplier = 100.0f;

UPROPERTY(EditAnywhere, Category = "Anter Movement")
float ZDiscendingMultiplier = 60.0f;

UPROPERTY(EditAnywhere, Category = "Anter Movement")
float ZBrake = 1000.0f;

UPROPERTY(EditAnywhere, Category = "Anter Jump")
float JumpScale = 100.0f;

/*Jump scale corrector to handle jump correction when rising*/
UPROPERTY(EditAnywhere, Category = "Anter Jump")
float AscendingJumpScaleMultiplier = 3.0f;

/*Jump scale corrector to handle jump correction when falling*/
UPROPERTY(EditAnywhere, Category = "Anter Jump")
float DescendingJumpScaleMultiplier = 6.0f;

/*Kick additional vertical corrector to avoid jump side-effects if hit horizontally*/
UPROPERTY(EditAnywhere, Category = "Anter Jump")
float KickVerticalScaleAddition = 10.0f;

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

UPROPERTY()
bool bCanAnterJump = false;

UPROPERTY()
bool bIsFalling = true;

UPROPERTY()
bool bIsLeftUnlocked = true;

UPROPERTY()
bool bIsRightUnlocked = true;

UPROPERTY()
float LastVelocityX;

//Array of information about vertical collisions with platforms
TArray<TPair<AActor*,EPlatformCollisionType>> RegisteredVerticalPlatformCollisions;


FGeometron AnterGeometron;

UPROPERTY()
FVector AnterCentre = FVector(0.0f,0.0f,0.0f);
UPROPERTY()
FVector AnterSize = FVector(0.0f,0.0f,0.0f);

UPROPERTY()
bool bIsAnterColliding = false;

//Default: can be hit by enemies. After being hit, it changes to cannot until a timer changes
EAnterHitableStatus AnterHitStatus = EAnterHitableStatus::CanBeHit;

UPROPERTY(EditDefaultsOnly)
float UnhittableTimerDuration = 1.0f;

FTimerDelegate UnhittableTimerDelegate;

FTimerHandle UnhittableTimerHandle;

};

