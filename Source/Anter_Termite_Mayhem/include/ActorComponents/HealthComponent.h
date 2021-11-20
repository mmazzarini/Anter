#pragma once

#include "ActorComponents/HealthComponent.h"
#include "UObject/Object.h"
#include "HealthComponent.generated.h"

/*
*
* Base class representing the structure of Health.
*
*/

UCLASS(Blueprintable)
class ANTER_TERMITE_MAYHEM_API UHealthContainer : public UObject
{
    GENERATED_BODY()

public: 
    FORCEINLINE UHealthContainer(){}

    FORCEINLINE ~UHealthContainer(){}

    //Public setter and getter for Health Component.
    void SetHealth(int32 InHealth){CoreHealth = InHealth;}

    int32 GetCoreHealth(){return CoreHealth}

    UPROPERTY(EditDeafultsOnly)
    int32 MaxHealth;

private:

    int32 CoreHealth = 3.0f;
};

/*
*
* Class that defines the basic properties of Health component attached to a Pawn.
* It uses a Health class to decrease/increase the core health.
* Both the health class and Health component class can be derived to satisfy the specific behavior of the Pawn class.
*
*/

UCLASS(Blueprintable)
class ANTER_TERMITE_MAYHEM_API UHealthComponent : public UActorComponent
{
    GENERATED_BODY()

    public:

        UHealthComponent();

        ~UHealthComponent();

        //This function updates the core health of the health container
        UFUNCTION()
        virtual void UpdateHealth(int32 InNewHealth);

        //This function is used to calculate the new value of health and change it 
        UFUNCTION()
        virtual void IncreaseHealth(int32 InIncrease);

    UHealthContainer* GetHealthContainer();

    protected:

    UPROPERTY(EditAnywhere)
    TSubclassOf<UHealthContainer> HealthContainerClass;

    UPROPERTY(EditDefaultsOnly)
    UHealthContainer* Health;
    
    UPROPERTY(EditDefaultsOnly)
    int32 MaxHealth;

    private:
    //This function instantiates the health container
    virtual void CreateHealth();

};