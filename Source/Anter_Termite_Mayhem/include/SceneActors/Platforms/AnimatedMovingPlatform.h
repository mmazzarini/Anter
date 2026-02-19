#pragma once 

#include "SceneActors/Platforms/MovingPlatform.h"
#include "Animation/AnimInstance.h"
#include "Components/TimelineComponent.h"

#include "AnimatedMovingPlatform.generated.h"

class AAnterPaperCharacter;
class UMovingActorMovementSupportComponent;
class UBaseEnemyMovementComponent;

UCLASS(Abstract, Blueprintable,BlueprintType)
class ANTER_TERMITE_MAYHEM_API AAnimatedMovingPlatform : public AMovingPlatform
{
    GENERATED_BODY()

public:
    
    AAnimatedMovingPlatform();

    void BeginPlay() override;

    void Tick(float DeltaSeconds) override;

    UFUNCTION(BlueprintCallable)
    bool GetIsEngaged();

    void SetIsEngaged(bool bInPlaying);

protected:

    void StartIdleAnimation();

    void StopTimeline();

    void ResetMotionAfterAnimation() const;

    void SetupTimelineAnimation(UTimelineComponent* InTimelineComp, UCurveFloat* InCurve, FOnTimelineFloat& InExecTimeline, FOnTimelineEventStatic& InEndedTimeline);

    void BindTimelineFunctions();

    void UnbindTimelineFunctions(){}

    void ResetActorMovement();

    UFUNCTION()
    void OnVerticalDisplacementFloatUpdated(float InDisplacement);

    UFUNCTION()
    void OnBounceAnimationFinished();

    UFUNCTION()
    void OnIdleDisplacementFloatUpdated(float InDisplacement);

    UFUNCTION()
    void OnIdleAnimationFinished();

    UFUNCTION()
    void OnMovementComplete();

    virtual void OnStartShakeAnimation();

    void StartShakeAnimation();

    /* Geometrical configuration of movement */
    TWeakObjectPtr<UMovingActorMovementSupportComponent> ActorMovement;

    /* Dynamical movement component */
    TWeakObjectPtr<UBaseEnemyMovementComponent> ActorBaseMotion;

    //Curve vector
    UPROPERTY(EditAnywhere)
    UCurveFloat* VerticalCurve;

    //Curve vector
    UPROPERTY(EditAnywhere)
    UCurveFloat* IdleCurve;

    //Timeline component for simulating animated movements.
    UPROPERTY(EditDefaultsOnly)
    UTimelineComponent* BounceTimeLineComp;

    //Timeline component for simulating animated movements.
    UPROPERTY(EditDefaultsOnly)
    UTimelineComponent* IdleTimeLineComp;

    FOnTimelineFloat VerticalTimelineFunc;

    FOnTimelineEventStatic BounceAnimationFinishedFunc;

    FOnTimelineFloat IdleTimelineFunc;

    FOnTimelineEventStatic IdleAnimationFinishedFunc;

    // Bool to register the full animations-movement loop.
    // - - N.B. This is not simply equivalent to some Timeline's Playing state. It is more general instead and it includes the Timeline step.
    bool bIsEngaged = false;

    UPROPERTY(BlueprintReadOnly)
    float ActorVerticalPivot;

protected:

    //Method to call the Shake Animation
    //Call it only from bp, e.g. from any behavior tree attached to this actor.
    UFUNCTION(BlueprintCallable)
    void BlueprintStartShakeAnimation();

    FVector InitialPlatformLocation;
};