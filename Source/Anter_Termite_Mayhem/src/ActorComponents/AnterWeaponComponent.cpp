#include "ActorComponents/AnterWeaponComponent.h"
#include "Pawn/AnterPaperCharacter.h"

UAnterWeaponComponent::UAnterWeaponComponent()
{
    //void at the moment
}

PRAGMA_DISABLE_OPTIMIZATION
void UAnterWeaponComponent::ShootLaser()
{
    //AAnterFire* FireShot = NewObject<AAnterFire>(dGetTransientPackage(),LaserSubClass);
    AAnterPaperCharacter* Anter = Cast<AAnterPaperCharacter>(GetOwner());
    if(Anter != nullptr)
    {
        FVector AnterPosition = Anter->GetTransform().GetTranslation();
        FRotator AnterRotation = Anter->GetTransform().Rotator();
        AnterPosition.X = AnterPosition.X + 30.0f;
        GetWorld()->SpawnActor<AAnterFire>(LaserSubClass,AnterPosition,AnterRotation);
        //FireShot->SetActorLocation(AnterPosition);
    }
}
PRAGMA_ENABLE_OPTIMIZATION