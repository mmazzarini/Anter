#pragma once

#include "GameFramework/MovementComponent.h"
#include "Components/SceneComponent.h"

#include "BaseEnemyMovementComponent.generated.h"

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

    void Initialize();

    virtual void SetMovement(FVector2D InGeometryVector, float InSpeed);

    virtual void UpdateMovement();

    FOnMovementUpdatedDelegate OnMovementUpdated;

protected:

    UPROPERTY(EditDefaultsOnly)
    float InitialMovementSpeed;

    UPROPERTY(EditDefaultsOnly)
    FVector2D InitialMovementGeometry;

    float InternalMovementSpeedX;

    FVector2D InternalMovementGeometry;

    // Reference to owner enemy
    UPROPERTY()
    ABaseEnemy* OwnerEnemy;

};