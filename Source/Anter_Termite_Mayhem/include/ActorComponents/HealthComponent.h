#pragma once

#include "ActorComponents/HealthComponent.h"
#include "UObject/Object.h"

#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathReachedDelegate);

/*
*
* Class that defines the basic properties of Health component attached to a Pawn.
* It uses a CoreHealth variable to repreent health.
* This class can be derived to satisfy the specific behavior of the Pawn class.
*
*/

UCLASS(Blueprintable, BlueprintType)
class ANTER_TERMITE_MAYHEM_API UHealthComponent : public UActorComponent
{
    GENERATED_BODY()

    public:

        UHealthComponent();

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

        float GetCoreHealth(){return CoreHealth;}

        FOnDeathReachedDelegate GetDeathReachedDelegate(){return OnDeathReached;}

    protected:
    
    UPROPERTY(EditDefaultsOnly)
    float MaxHealth = 10.0f;

    UPROPERTY(EditDefaultsOnly)
    float StartingHealth = 3.0f;

    private:

    // The true core health of the Pawn. 
    float CoreHealth = 0.0f;

    //Delegate to use to notify other objects (e.g. the owning Pawn about the death event)
    FOnDeathReachedDelegate OnDeathReached;

    private:

    virtual void SetupHealth();

};