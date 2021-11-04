#include "Health/HealthContainer.h"

#define MAX_HEALTH (int32)100

UHealthContainer::UHealthContainer()
:
CoreHealth(0)
{
    SetHealth(MAX_HEALTH)
}

UHealthContainer::~UHealthContainer()
{

}

void UHealthContainer::Tick(float DeltaTime)
{
    UpdateHealth();
}

/*
* Dummy update, to be modified for implementation-specific classes. It avoids having health == 0 at ctor, 
* which would be implying that the character may be destroyed.
* The UpdateHealth is a separate function from SetHealth, because, depending on object-specific implementation,
* The UpdateHealth may do different processes that simply setting health to a specific value, i.e. implementation
* of algorithms e.g. to introduce an increase of health.
*/

void UHealthContainer::UpdateHealth()
{
    SetHealth(MAX_HEALTH);
}

void UHealthContainer::SetHealth(int32 InHealth)
{
    CoreHealth = InHealth;
}