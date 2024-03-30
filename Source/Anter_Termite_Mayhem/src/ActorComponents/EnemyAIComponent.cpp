#include "ActorComponents/EnemyAIComponent.h"
#include "AnterGameModes/AnterBaseLevelGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

UEnemyAIComponent::UEnemyAIComponent()
:AnterPtr(nullptr),
TargetLocation(FVector(0.0f,0.0f,0.0f)),
EnemyWeapon(nullptr)
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;  
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
    if(AAnterBaseLevelGameMode* LevelGMode = Cast<AAnterBaseLevelGameMode>(UGameplayStatics::GetGameMode(GetOwner())))
    {
        LevelGMode->PlayerRestartedDelegate.AddDynamic(this,&UEnemyAIComponent::SetAnterPointer);
    }
}

void UEnemyAIComponent::SetTargetLocation()
{
    if(AnterPtr != nullptr && GetOwner() != nullptr)
    {
        TargetLocation = FVector(AnterPtr->GetActorLocation().X-GetOwner()->GetActorLocation().X,0.0f,AnterPtr->GetActorLocation().Z-GetOwner()->GetActorLocation().Z);
    }
}

void UEnemyAIComponent::EvaluateShoot()
{
    if(EnemyWeapon != nullptr && GetOwner() != nullptr)
    {
        if(EnemyWeapon->CanShoot())
        {
            FVector OwnerPosition = GetOwner()->GetActorLocation();
            EnemyWeapon->SetLaserDirection(TargetLocation/TargetLocation.Size());
            EnemyWeapon->ShootLaser();
        }
    }
}

void UEnemyAIComponent::SetAnterPointer()
{
    //This is required to check for the main character. 
    AnterPtr = Cast<AAnterPaperCharacter>(UGameplayStatics::GetActorOfClass(this,AAnterPaperCharacter::StaticClass()));
}