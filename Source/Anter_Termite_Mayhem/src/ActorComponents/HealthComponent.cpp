#include "ActorComponents/HealthComponent.h"
#include "Health/HealthContainer.h"

AHealthComponent::AHealthComponent()
Health(nullptr)
{

}

~AHealthComponent::AHealthComponent()
{

}

void AHealthComponent::CreateHealth()
{
    Health = NewObject<UHealthContainer>(this,HealthContainerClass); 
    if(Health)
    {
        Health->SetHealth(MAX_HEALTH)
    }
}

UHealthContainer* AHealthComponent::GetHealth()
{
    return Health;
}

/*
    UPROPERTY(EditAnywhere)
    TSubclassOf<UHealthContainer> HealthContainerClass;

    UPROPERTY()
    UHealthContainer* HealthContainer;
*/