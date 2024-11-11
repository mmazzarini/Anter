#include "ActorComponents/AnterWeaponComponent.h"
#include "Pawn/AnterPaperCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"

UAnterWeaponComponent::UAnterWeaponComponent()
: 
bCanShoot(true),
InternalSequentialShotCounter(0)
{
    //void at the moment
}

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
            if(!ShouldUseWeaponCounter || (ShouldUseWeaponCounter && InternalAmmosCounter > 0.0f))
            {
                AAnterFire* Fire = GetWorld()->SpawnActor<AAnterFire>(LaserSubClass,FirePosition,AnterRotation);
                if(Fire != nullptr)
                {
                    Fire->SetMovementToRight(LaserDirection); 
                    Fire->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(Fire->GetActorLocation(),Fire->GetActorLocation()+LaserDirection),ETeleportType::None);
                    if(ShouldUseWeaponCounter)
                    {
                        UpdateAmmos(-Fire->GetConsumptionAmount());
                    }
                }
                SetCanShoot(false);
                InternalSequentialShotCounter++;
                if(InternalSequentialShotCounter < MaxSequentialShots)
                {
                    OwnerActor->GetWorldTimerManager().SetTimer(SequentialFireTimerHandle, this, &UAnterWeaponComponent::OnTimerEnded, InSequentialFireRate, false, InSequentialFireRate);
                }
                else
                {
                    InternalSequentialShotCounter = 0;
                    OwnerActor->GetWorldTimerManager().SetTimer(FireTimerHandle, this, &UAnterWeaponComponent::OnTimerEnded, InFireRate, false, InFireRate);
                }
            }
        }
    }
}

void UAnterWeaponComponent::SetCanShoot(bool InCanShoot)
{
    bCanShoot = InCanShoot;
}

void UAnterWeaponComponent::OnTimerEnded()
{
    GetOwner()->GetWorldTimerManager().ClearTimer(FireTimerHandle);
    GetOwner()->GetWorldTimerManager().ClearTimer(SequentialFireTimerHandle);
    SetCanShoot(true);

}

void UAnterWeaponComponent::UpdateAmmos(float InAmmosDifference)
{
    if(InAmmosDifference > 0.0f || (InternalAmmosCounter >= abs(InAmmosDifference)))
    {
        InternalAmmosCounter += InAmmosDifference;
        if(GEngine != nullptr)
        {
            GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,TEXT("New Weapon number: ") + FString::SanitizeFloat(InternalAmmosCounter));
        }
    }

    float ClampedCounter = FMath::Clamp<float>(InternalAmmosCounter,0.0f,MaxWeaponCounter);
}

float UAnterWeaponComponent::GetAmmosCounter() const
{
    return InternalAmmosCounter;
}
