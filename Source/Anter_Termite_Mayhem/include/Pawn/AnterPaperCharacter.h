#pragma once

//#include "FSM/AnterFSM/AnterFSM.h"

#include "PaperCharacter.h"
#include "ActorComponents/AnterFSMComponent.h"
#include "ActorComponents/HealthComponent.h"
#include "ActorComponents/AnterCameraComponent.h"
#include "ActorComponents/AnterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/MovementComponent.h"

#include "AnterPaperCharacter.generated.h"

class UInputComponent;

/*
*
* Anter Paper Character class, representing the entity of the main character in the game
* Components are attached and refernced by the Pawn
*
*/

UCLASS(BlueprintType)
class ANTER_TERMITE_MAYHEM_API AAnterPaperCharacter : public APaperCharacter
{
    GENERATED_BODY()

public:

    AAnterPaperCharacter();

    //Basic ticker
    void Tick(float DeltaTime) override;

    void OnDeathEvent();

    virtual void BeginPlay() override;

    void SetBindings();

    virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

    void HandleRightMovement(float InAxisValue);

//UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
//UAnterMovementComponent* AnterMovement;

UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
USpringArmComponent* Spring;

UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
UCameraComponent* Camera;

UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
UStaticMeshComponent* AnterMesh;

UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
UHealthComponent* AnterHealth;

protected:

UPROPERTY(EditAnywhere)
float Multiplier = 100.0f;

private:

//UAnterComponent* AnterFSMComponent;

};

