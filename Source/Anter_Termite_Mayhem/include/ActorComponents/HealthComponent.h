#pragma once

#include "UObject/Object.h"
#include "Components/SceneComponent.h"
#include "SceneUtilities/SceneStructs.h"

#include "HealthComponent.generated.h"

//Death reached dyn delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathReachedDelegate);

//Health dyn multicast delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthUpdatedDelegate, float, NewHealthValue);

/*
*
* Class that defines the basic properties of Health component attached to a Pawn.
* It uses a CoreHealth variable to repreent health.
* This class can be derived to satisfy the specific behavior of the Pawn class.
*
*/

UCLASS(Blueprintable, BlueprintType)
class ANTER_TERMITE_MAYHEM_API UHealthComponent : public USceneComponent
{
    GENERATED_BODY()

public:

    UHealthComponent();

    virtual void BeginPlay() override;

    //This function updates the core health of the health container
    UFUNCTION()
    virtual void UpdateHealth(float InNewHealth);

    //This function is used to calculate the new value of health and change it 
    UFUNCTION()
    virtual void IncreaseHealth(float InIncreaseHealth);
    
    //Base class reset
    UFUNCTION()
    virtual void ResetHealth();
        
    UFUNCTION()
    virtual void CheckDeath(); 

    UFUNCTION(BlueprintCallable)
    float GetCoreHealth(){return CoreHealth;}

    //Delegate to notify other objects of death reached
    FOnDeathReachedDelegate OnDeathReached;

    //Delegate to notify of health value updated.
    FOnHealthUpdatedDelegate OnHealthUpdated;

    TArray<EHealthHarmlessType> GetTypesOfDamageAvailable(){return TypesOfDamageIsSensibleTo;}

protected:
    
    UPROPERTY(EditDefaultsOnly)
    float MaxHealth = 10.0f;

    UPROPERTY(EditDefaultsOnly)
    float StartingHealth = 3.0f;

    UPROPERTY(EditDefaultsOnly)
    TArray<EHealthHarmlessType> TypesOfDamageIsSensibleTo; 

private:

    // The true core health of the Pawn. 
    float CoreHealth = 0.0f;

    private:

    virtual void SetupHealth();

};