#include "ActorComponents/HealthComponent.h"
#include "GameConstants.h"

#include "Health/HealthContainer.h"

UHealthComponent::UHealthComponent()
:
    Health(nullptr)
{

}

UHealthComponent::~UHealthComponent()
{

}

void UHealthComponent::CreateHealth()
{
    Health = NewObject<UHealthContainer>(this,HealthContainerClass); 
    if(Health != nullptr)
    {
        Health->SetHealth(MaxHealth);
    }
}

UHealthContainer* UHealthComponent::GetHealthContainer()
{
    if(Health !=nullptr)
    {   
       return Health;
    }

}

UHealthContainer* UHealthComponent::UpdateHealth(int32 InNewHealth)
{
    if(Health != nullptr)
    if(InNewHealth > MaxHealth)
    {
        Health->SetHealth(MaxHealth);
    }
    else
    {
        Health->SetHealth(InNewHealth);
    }
}

UHealthContainer* UHealthComponent::GetHealth()
