#pragma once

#include "GameFramework/Actor.h"
#include "PaperCharacter.h"

#include "BaseEnemy.generated.h"

class UBaseEnemyMovementComponent;

/*
Base Enemy class
An Enemy that is able to move right/left or up/down according to what is 
set by user on Blueprint defaults.
Can be derived to obtain all the specified enemies.
*/

UCLASS(BlueprintType,Blueprintable)
class ANTER_TERMITE_MAYHEM_API ABaseEnemy : public APaperCharacter
{

    GENERATED_BODY()

public:

    ABaseEnemy();

    void BeginPlay() override;

protected:

    /* Enemy components */

    UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
    UBaseEnemyMovementComponent* BaseEnemyMovement;

    void SetBindings();

    UFUNCTION()
    virtual void HandleMovement(FVector2D InMovementDirection, float InMovementSpeed);

};