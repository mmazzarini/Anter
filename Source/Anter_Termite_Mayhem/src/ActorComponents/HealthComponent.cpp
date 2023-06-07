#include "ActorComponents/HealthComponent.h"

UHealthComponent::UHealthComponent()
{
    SetupHealth();
}

void UHealthComponent::SetupHealth()
{
    CoreHealth = StartingHealth;
}

void UHealthComponent::UpdateHealth(float InNewHealth)
{ 
    if(InNewHealth > MaxHealth)
    {
        CoreHealth = MaxHealth;
    }
    else
    {
        CoreHealth = InNewHealth;
    }
    //Broadcast new health
    if(OnHealthUpdated.IsBound())
    {
        OnHealthUpdated.Broadcast(CoreHealth);
    }
    if(GEngine != nullptr)
    {
        GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,TEXT("New Health: ") + FString::SanitizeFloat(CoreHealth));
    }
    //Then check if pawn is dead
    CheckDeath();
}

void UHealthComponent::IncreaseHealth(float InIncreaseHealth)
{
    float NewHealth = CoreHealth + InIncreaseHealth;
    UpdateHealth(NewHealth);
}

// The base version of this class just exectues a dumb call to the Setup method. But subclass overrides
// may call more complex methods, such as doing actions before actually restoring health.
void UHealthComponent::ResetHealth()
{
    SetupHealth();
}

void UHealthComponent::CheckDeath()
{
    if (CoreHealth <= 0.0f)
    {
        //Notify death reached with a broadcast event
        if(OnDeathReached.IsBound())
        {
            OnDeathReached.Broadcast();
        }
    }
}
