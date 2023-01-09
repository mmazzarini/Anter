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
    AActor* OwnerActor = GetOwner(); //AAnterPaperCharacter* Anter = Cast<AAnterPaperCharacter>(GetOwner());
    if(OwnerActor != nullptr)
    {
        if(bCanShoot == true)
        {
            FVector AnterPosition = OwnerActor->GetTransform().GetTranslation();
            FRotator AnterRotation = OwnerActor->GetTransform().Rotator();
            FVector FirePosition = AnterPosition;
            FirePosition.X = AnterPosition.X + WeaponSpawnRange*LaserDirection.X;
            AAnterFire* Fire = GetWorld()->SpawnActor<AAnterFire>(LaserSubClass,FirePosition,AnterRotation);
            if(Fire != nullptr)
            {
                Fire->SetMovementToRight(LaserDirection); 
            }
            SetCanShoot(false);
            OwnerActor->GetWorldTimerManager().SetTimer(FireTimerHandle, this, &UAnterWeaponComponent::OnTimerEnded, InFireRate, false, InFireRate);
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

/*
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
*/