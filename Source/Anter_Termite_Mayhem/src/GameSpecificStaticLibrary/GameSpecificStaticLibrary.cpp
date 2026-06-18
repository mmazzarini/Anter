#include "GameSpecificStaticLibrary/GameSpecificStaticLibrary.h"
#include "ActorComponents/HealthComponent.h"
#include "ActorComponents/DamageComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"

bool UGameSpecificStaticLibrary::IsHealthDamageType(AActor* InDamagedActor, AActor* InDamagingActor)
{
    UHealthComponent* Health = (InDamagedActor != nullptr) ? Cast<UHealthComponent>(InDamagedActor->FindComponentByClass(UHealthComponent::StaticClass())) : nullptr;
    UDamageComponent* Damage = (InDamagingActor != nullptr) ? Cast<UDamageComponent>(InDamagingActor->FindComponentByClass(UDamageComponent::StaticClass())) : nullptr;
    if(Health != nullptr && Damage != nullptr)
    {
        return (Health->GetTypesOfDamageAvailable().Contains(Damage->GetTypeOfDamage()));
    }

    return false;
}

bool UGameSpecificStaticLibrary::IsInScreen(const APlayerController* InPlayerController, const FVector& InLocation, float InMultiplier)
{
    bool bIsInScreen = false;
    if (InPlayerController != nullptr)
    {
        FVector2D ScreenPos = FVector2D(0.0f, 0.0f);
        int32 ScreenX, ScreenY;
        InPlayerController->GetViewportSize(ScreenX, ScreenY);
        InPlayerController->ProjectWorldLocationToScreen(InLocation, ScreenPos);
        return (ScreenPos.X < ScreenX * InMultiplier &&
            ScreenPos.X > -1.f * InMultiplier &&
            ScreenPos.Y < ScreenY * InMultiplier &&
            ScreenPos.Y > -1.f * InMultiplier);            
    }

    return bIsInScreen;
}
