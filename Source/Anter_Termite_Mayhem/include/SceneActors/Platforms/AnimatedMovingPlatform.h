#pragma once 

#include "SceneActors/Platforms/MovingPlatform.h"

#include "AnimatedMovingPlatform.generated.h"

class AAnterPaperCharacter;
class UMovingActorMovementSupportComponent;
class UBaseEnemyMovementComponent;

UCLASS(Blueprintable,BlueprintType)
class ANTER_TERMITE_MAYHEM_API AAnimatedMovingPlatform : public AMovingPlatform
{
    GENERATED_BODY()

public:

    void BeginPlay() override;

    void Tick(float DeltaSeconds) override;

    /* Geometrical configuration of movement */
    TWeakObjectPtr<UMovingActorMovementSupportComponent> ActorMovement;

    /* Dynamical movement component */
    TWeakObjectPtr<UBaseEnemyMovementComponent> ActorBaseMotion;    

    UFUNCTION()
    void OnMovementComplete();

};