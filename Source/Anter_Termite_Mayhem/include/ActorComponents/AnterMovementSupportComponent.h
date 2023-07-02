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

    void SetMovementDirection(float InAxisValue);

    virtual void BeginPlay() override;

    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction) override;

    
protected:
    UPROPERTY(EditDefaultsOnly)
    FVector PositionVector = FVector(0.0f,0.0f,0.0f);

    //Controls upside down movement
    bool bIsMovingToFloor;

    UPROPERTY(EditDefaultsOnly)
    float VerticalShift = 10.0f;

    AAnterPaperCharacter* Anter;

    UPROPERTY(EditDefaultsOnly, Category="Slide Movement")
    float SlideMovementDurationTime = 1.0f;

    UPROPERTY(EditDefaultsOnly, Category="Slide Movement")
    float SlideMovementMultiplier = 3.0f;

    UPROPERTY(EditDefaultsOnly, Category="Slide Movement")
    float SlideMovementDivider = 2.0f;

    UCharacterMovementComponent* AnterMovement = nullptr;

    float InternalMovementDirection = 1.0f; //Can assume only -1.0f,0.0f,1.0f

    FTimerDelegate SlideTimerDelegate;

    FTimerHandle SlideTimerHandle;

    //boolean to control slide internal start with true
    bool bCanSlideInternal = true;

};