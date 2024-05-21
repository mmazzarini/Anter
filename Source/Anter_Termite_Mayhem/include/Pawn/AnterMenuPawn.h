#pragma once

#include "PaperCharacter.h"

#include "AnterMenuPawn.generated.h"

UCLASS(BlueprintType)
class ANTER_TERMITE_MAYHEM_API AAnterMenuPawn : public APaperCharacter
{
    GENERATED_BODY()
    
public:

    AAnterMenuPawn();

    void BeginPlay() override;

    void SetupPlayerInputComponent(UInputComponent* InInputComponent) override;

    void MoveToLevelPivot();

    void GetTargetPivot();

    //Movement functions
    void HandleMoveDown();

    //Movement functions
    void HandleMoveUp();

    //Movement functions
    void HandleMoveLeft();

    //Movement functions
    void HandleMoveRight();


private:

    UPROPERTY(EditDefaultsOnly)
    float ThresholdToNextPivot = 10.0f;

};
