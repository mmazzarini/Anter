#pragma once

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Actor.h"
#include "ActorComponents/BaseEnemyMovementComponent.h"
#include "SceneActors/SceneActorInterface.h"
#include "SceneUtilities/SceneStructs.h"

#include "MovingActorMovementSupportComponent.generated.h"

/*
This class is used to handle movement of moving actors, can be attached to any moving instance, except for 
enemies, that alreadty have integrated movement support. So this is a generalization for future moving-actor implementations.
*/

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMovementCompletedDelegate);

UCLASS(Blueprintable, BlueprintType)
class ANTER_TERMITE_MAYHEM_API UMovingActorMovementSupportComponent : public USceneComponent
{
    GENERATED_BODY()

public:

    UMovingActorMovementSupportComponent();

    void BeginPlay() override;

    void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction);

    void StartToMove();

    //The following two methods are getter and setter of pivot state enum
    EEnemyPivotState GetPivotState(){return PivotState;}

    void SetPivotState(EEnemyPivotState InPivotState){PivotState = InPivotState;}
    
    void MoveToNextPivot();

    void UpdateMovement();

    void SwitchOrientation();

    void AdjustVelocity();

    //virtual void HandleCollision(const FCollisionGeometry& CollisionGeometry, AActor* OtherActor) override; //float InMovementSpeed){} 

    //Setter method for PivotDistanceThreshold
    void SetPivotDistanceThreshold(float InPivotDistanceThreshold){PivotDistanceThreshold = InPivotDistanceThreshold;}

    UFUNCTION(BlueprintCallable)
    void FillPositionArrays(TArray<FVector> InPositions);

    void SetLoopBehavior(EEnemyLoopBehavior InLoopBehavior);

    void SetIsMovementActive(bool InMovementActive){bIsMovementActive = InMovementActive;}

    bool GetIsMovementActive(){return bIsMovementActive;}

    FOnMovementCompletedDelegate OnMovementCompleted;

protected:

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

    //Distance from pivot to trigger movement to next pivot
    UPROPERTY(EditAnywhere)
    float PivotDistanceThreshold = 0.0f;

    EEnemyLoopBehavior LoopBehavior;

    UBaseEnemyMovementComponent* OwnerMovement;

    //Internal reference to owning actor
    AActor* OwningActor;

    //Used to block or activate movement update
    bool bIsMovementActive = true;

};
