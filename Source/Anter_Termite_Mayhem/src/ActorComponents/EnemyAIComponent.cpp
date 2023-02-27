#include "ActorComponents/EnemyAIComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

UEnemyAIComponent::UEnemyAIComponent()
:AnterPtr(nullptr),
TargetLocation(FVector(0.0f,0.0f,0.0f)),
EnemyWeapon(nullptr)
{
    
}

void UEnemyAIComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
    Super::TickComponent(DeltaTime,TickType,ThisTickFunction);
    SetTargetLocation();
    EvaluateShoot();
}

void UEnemyAIComponent::BeginPlay()
{
    Super::BeginPlay();
    SetAnterPointer();
    if(GetOwner() != nullptr)
    {
        EnemyWeapon = Cast<UAnterWeaponComponent>(GetOwner()->FindComponentByClass(UAnterWeaponComponent::StaticClass()));
    }

}

void UEnemyAIComponent::SetTargetLocation()
{
    if(AnterPtr != nullptr && GetOwner() != nullptr)
    {
        TargetLocation = FVector(AnterPtr->GetActorLocation().X-GetOwner()->GetActorLocation().X,0.0f,GetOwner()->GetActorLocation().Z);
    }
}

void UEnemyAIComponent::EvaluateShoot()
{
    if(EnemyWeapon != nullptr && GetOwner() != nullptr)
    {
        /*
        float RandomNumber = FMath::RandRange(0.0f,1.0f);
        if(RadomNumber >= RandomThresholdLimitForShooting)
        {
            */
        if(EnemyWeapon->CanShoot())
        {
            EnemyWeapon->SetLaserDirection(TargetLocation-GetOwner()->GetActorLocation());
            EnemyWeapon->ShootLaser();
        }
        //}
    }

}

void UEnemyAIComponent::SetAnterPointer()
{
    //This is required to check for the main character. 
    AnterPtr = Cast<AAnterPaperCharacter>(UGameplayStatics::GetActorOfClass(this,AAnterPaperCharacter::StaticClass()));
}