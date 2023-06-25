#pragma once

#include "GameFramework/CharacterMovementComponent.h"

#include "AnterMovementSupportComponent.generated.h"

class AAnterPaperCharacter;

UCLASS(Blueprintable, BlueprintType)
class ANTER_TERMITE_MAYHEM_API UAnterMovementSupportComponent : public USceneComponent
{

    GENERATED_BODY()

public:

    UAnterMovementSupportComponent();

    void HandleRightMovement(float InMovementAmount);    

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

};