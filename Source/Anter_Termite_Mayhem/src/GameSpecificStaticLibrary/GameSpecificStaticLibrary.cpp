#include "GameSpecificStaticLibrary/GameSpecificStaticLibrary.h"
#include "ActorComponents/HealthComponent.h"
#include "ActorComponents/DamageComponent.h"


bool UGameSpecificStaticLibrary::IsHealthDamageType(UHealthComponent* Health, UDamageComponent* Damage)
{
    if(Health != nullptr && Damage != nullptr)
    {
        return (Health->GetTypesOfDamageAvailable().Contains(Damage->GetTypeOfDamage()));
    }
    //Default return: false
    return false;
}