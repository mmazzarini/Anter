#include "ActorComponents/AnterWeaponComponent.h"
#include "Pawn/AnterPaperCharacter.h"

UAnterWeaponComponent::UAnterWeaponComponent()
: 
bCanShoot(true)
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
        if(bCanShoot == true)
        {
            FVector AnterPosition = Anter->GetTransform().GetTranslation();
            FRotator AnterRotation = Anter->GetTransform().Rotator();
            FVector FirePosition = AnterPosition;
            FirePosition.X = AnterPosition.X + WeaponSpawnRange*MovementDirectionScaleFactor;
            AAnterFire* Fire = GetWorld()->SpawnActor<AAnterFire>(LaserSubClass,FirePosition,AnterRotation);
            if(Fire != nullptr)
            {
                Fire->SetMovementToRight((MovementDirectionScaleFactor > 0.0f));
            }
            SetCanShoot(false);
            GetOwner()->GetWorldTimerManager().SetTimer(FireTimerHandle, this, &UAnterWeaponComponent::OnTimerEnded, InFireRate, false, InFireRate);
        }
    }
}
PRAGMA_ENABLE_OPTIMIZATION

void UAnterWeaponComponent::SetCanShoot(bool InCanShoot)
{
    bCanShoot = InCanShoot;
}

void UAnterWeaponComponent::OnTimerEnded()
{
    GetOwner()->GetWorldTimerManager().ClearTimer(FireTimerHandle);
    SetCanShoot(true);
}

void UAnterWeaponComponent::OnOwnerMoving(float InAxisValue)
{
    if(InAxisValue > 0.0f)
    {
        MovementDirectionScaleFactor = 1.0f;
    }
    else if(InAxisValue < 0.0f)
    {
        MovementDirectionScaleFactor = -1.0f;
    }
}