#pragma once

#include "ActorComponents/HealthComponent.h"
#include "Health/HealthContainer.h"


UCLASS(Blueprintable)
class ANTER_TERMITE_MAYHEM_API AHealthComponent : public AActorComponent
{
    GENERATED_BODY()

    public:

        AHealthComponent();

        ~AHealthComponent();

        //This function instantiates the health container
        UFUNCTION()
        virtual void CreateHealth());

    UHealthContainer* GetHealth();

    protected:

    UPROPERTY(EditAnywhere)
    TSubclassOf<UHealthContainer> HealthContainerClass;

    UPROPERTY()
    UHealthContainer* Health;


}