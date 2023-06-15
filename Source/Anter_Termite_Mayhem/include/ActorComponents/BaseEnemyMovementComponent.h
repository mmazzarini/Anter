#pragma once

#include "GameFramework/CharacterMovementComponent.h"

#include "BaseEnemyMovementComponent.generated.h"

class AActor;

/*
*
* Base Enemy Class that gives inputs to movements, based on movement set by interaction with the environment
*
*/

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMovementUpdatedDelegate, FVector2D, MovementGeometry, float, MovemementSpeed);

UCLASS(Blueprintable, BlueprintType)
class ANTER_TERMITE_MAYHEM_API UBaseEnemyMovementComponent : public USceneComponent
{
    GENERATED_BODY()

public:

    UBaseEnemyMovementComponent();

    void Initialize();

    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction) override;

    virtual void SetMovement(FVector2D InGeometryVector);

    virtual void SetSpeed(float InSpeed);

    virtual void UpdateMovement();

    /*
    Wrapper function to handle collisions notified by external components.
    Can be bound to a Broadcaster to be executed.
    */
    UFUNCTION()
    void OnCollided(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

    FOnMovementUpdatedDelegate OnMovementUpdated;

    const float GetInnerSpeed() const {return InternalMovementSpeed;}

protected:

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
    float InitialMovementSpeed;

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
    FVector2D InitialMovementGeometry;

    float InternalMovementSpeed;

    FVector2D InternalMovementGeometry;

    // Reference to owner enemy
    UPROPERTY()
    AActor* OwnerEnemy;

    /* This is used to invert speed */
    void InvertSpeed();

};