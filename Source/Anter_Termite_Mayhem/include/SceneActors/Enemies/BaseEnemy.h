#pragma once

#include "GameFramework/Actor.h"
#include "PaperCharacter.h"
#include "ActorComponents/AnterFSMComponent.h"
#include "ActorComponents/HealthComponent.h"
#include "ActorComponents/BaseEnemyMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "ActorComponents/CollisionSupportComponent.h"
#include "ActorComponents/AnterWeaponComponent.h"
#include "SceneActors/SceneActorInterface.h"
#include "SceneActors/Enemies/EnemyInterface.h"
#include "SceneUtilities/SceneStructs.h"
#include "BaseEnemy.generated.h"

/*
Enum to control the pivot arrays filling state of the enemy
*/
UENUM()
enum class EEnemyPivotState : uint8
{
    IsWaitingToBeFilled,
    HasBeenFilled,
    HasStartedMoving
};

/*
Enum to represent the running orientation of the enemy
*/
UENUM()
enum class EEnemyRunningOrientation : uint8
{
    IsRunningForward,
    IsRunningBackward
};

/*
Base Enemy class
An Enemy that is able to move right/left or up/down according to what is 
set by user on Blueprint defaults.
Can be derived to obtain all the specified enemies.
*/

UCLASS(BlueprintType)
class ANTER_TERMITE_MAYHEM_API ABaseEnemy : public APaperCharacter, public ISceneActorInterface, public IEnemyInterface
{

    GENERATED_BODY()

public:

    ABaseEnemy();

    void BeginPlay() override;

    void Tick(float DeltaSeconds) override;

    void StartToMove();

    //The following two methods are getter and setter of pivot state enum
    EEnemyPivotState GetPivotState(){return PivotState;}

    void SetPivotState(EEnemyPivotState InPivotState){PivotState = InPivotState;}
    
    void MoveToNextPivot();

    void SwitchOrientation();

    virtual void HandleCollision(const FCollisionGeometry& CollisionGeometry, AActor* OtherActor) override; //float InMovementSpeed){} 

    virtual void UpdateWeaponDirection(float InLaserDirection) override{}

    //Setter method for PivotDistanceThreshold
    void SetPivotDistanceThreshold(float InPivotDistanceThreshold){PivotDistanceThreshold = InPivotDistanceThreshold;}

    UFUNCTION(BlueprintCallable)
    void FillPositionArrays(TArray<FVector> InPositions);

    UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
    UBaseEnemyMovementComponent* BaseEnemyMovement;

    UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
    UStaticMeshComponent* BaseEnemyMesh;

    UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
    UHealthComponent* BaseEnemyHealth;

    UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
    UBoxComponent* BaseEnemyBox;

    UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
    UCollisionSupportComponent* BaseEnemyCollisionSupport;

    //UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
    //UAnterWeaponComponent* BaseEnemyWeaponComponent;

    //Interface overridden functions
    void UpdateAttack();
    
    void UpdateMovement();

    void SetLoopBehavior(EEnemyLoopBehavior InLoopBehavior);

    void SetCanShoot(bool InCanShoot){bCanShoot = InCanShoot;}

protected:

    void SetBindings();

    void AdjustVelocity();

    UPROPERTY(BlueprintReadWrite)
    TArray<FVector> PivotPositions;

    UPROPERTY(BlueprintReadWrite)
    TArray<FVector> ReversePivotPositions; 

    TArray<FVector> GetCurrentPivotPositionsArray(){return CurrentPivotPositions;}

    //Must be set equal to PivotPositions or ReversePivotPositions
    TArray<FVector> CurrentPivotPositions;

    EEnemyPivotState PivotState = EEnemyPivotState::IsWaitingToBeFilled;

    EEnemyRunningOrientation RunningOrientation = EEnemyRunningOrientation::IsRunningForward;

    uint8 PivotArrayIndex;

    UPROPERTY(EditDefaultsOnly)
    FVector EnemySpeed;

    //Distance from pivot to trigger movement to next pivot
    UPROPERTY(EditAnywhere)
    float PivotDistanceThreshold = 0.0f;

    EEnemyLoopBehavior LoopBehavior;

    //Set externally by manager to configure if enemy shoots
    UPROPERTY(EditDefaultsOnly)
    bool bCanShoot;
};
