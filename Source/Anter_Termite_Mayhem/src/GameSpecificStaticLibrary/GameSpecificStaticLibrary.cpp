#include "GameSpecificStaticLibrary/GameSpecificStaticLibrary.h"
#include "ActorComponents/HealthComponent.h"
#include "ActorComponents/DamageComponent.h"
#include "GameFramework/Actor.h"

bool UGameSpecificStaticLibrary::IsHealthDamageType(AActor* InDamagedActor, AActor* InDamagingActor)
{
    UHealthComponent* Health = (InDamagedActor != nullptr) ? Cast<UHealthComponent>(InDamagedActor->FindComponentByClass(UHealthComponent::StaticClass())) : nullptr;
    UDamageComponent* Damage = (InDamagingActor != nullptr) ? Cast<UDamageComponent>(InDamagingActor->FindComponentByClass(UDamageComponent::StaticClass())) : nullptr;
    if(Health != nullptr && Damage != nullptr)
    {
        return (Health->GetTypesOfDamageAvailable().Contains(Damage->GetTypeOfDamage()));
    }
    //Default return: false
    return false;
}