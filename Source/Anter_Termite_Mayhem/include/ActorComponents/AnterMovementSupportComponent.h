#pragma once

#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

#include "AnterMovementSupportComponent.generated.h"

class AAnterPaperCharacter;

UCLASS(Blueprintable, BlueprintType)
class ANTER_TERMITE_MAYHEM_API UAnterMovementSupportComponent : public USceneComponent
{

    GENERATED_BODY()

public:

    UAnterMovementSupportComponent();

    void HandleRightMovement(float InMovementAmount);    

    UFUNCTION()
    void HandleSlide();

    void EndSlide();

    void EndSecondSlide();

    void SetMovementDirection(float InAxisValue);

    virtual void BeginPlay() override;

    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction) override;

    
protected:

    UFUNCTION()
    void OnAnterGeometryChanged(float XGeom, float ZGeom);

    UPROPERTY(EditDefaultsOnly)
    FVector PositionVector = FVector(0.0f,0.0f,0.0f);

    //Controls upside down movement
    bool bIsMovingToFloor;

    UPROPERTY(EditDefaultsOnly)
    float VerticalShift = 10.0f;

    AAnterPaperCharacter* Anter;

    //Slide movement properties

    UPROPERTY(EditDefaultsOnly, Category="Slide Movement")
    float SlideMovementDurationTime = 1.0f;
    
    UPROPERTY(EditDefaultsOnly, Category="Slide Movement")
    float SlideMovementAirDurationTime = 0.5f;

    UPROPERTY(EditDefaultsOnly, Category="Slide Movement")
    float SlideMovementMultiplier = 3.0f;

    UPROPERTY(EditDefaultsOnly, Category="Slide Movement")
    float SlideMovementDivider = 2.0f;

    UPROPERTY(EditDefaultsOnly, Category="Slide Movement")
    float SlideMovementAirDivider = 2.0f;

    UPROPERTY(EditDefaultsOnly, Category="Slide Movement", meta=(ClampMin=0, ClampMax=1))
    float SlideStopThresholdFraction = 0.8f;

    //Internal, not exposable to BP, needed for mathematical corrections to horizontal trajectory
    float SlideStopThreshold = 0.0f;

    UCharacterMovementComponent* AnterMovement = nullptr;

    FVector InternalMovementDirection;

    FTimerDelegate SlideTimerDelegate;

    FTimerHandle SlideTimerHandle;

    //boolean to control slide internal start with true
    bool bCanSlideInternal = true;

    //Slide value
    FVector SlideImpulse;

    float GeometryX = 1.0f;

	float GeometryZ = 0.0f;

};