#include "SceneActors/Items/AnterBaseCrate.h"
#include "Components/PrimitiveComponent.h"
#include "SceneActors/AnterFire.h"
#include "GameSpecificStaticLibrary/GameSpecificStaticLibrary.h"

AAnterBaseCrate::AAnterBaseCrate()
{
    BaseCrateCollisionSupport = CreateDefaultSubobject<UCollisionSupportComponent>(TEXT("CrateCollisionSupport"));
    BaseCrateCollisionSupport->SetupAttachment(RootComponent);

    //rateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CrateMesh"));
    //ateMesh->SetupAttachment(RootComponent);
}

void AAnterBaseCrate::BeginPlay()
{
    Super::BeginPlay();
    SetBindings();
}

void AAnterBaseCrate::SetBindings()
{
    if(BaseCrateCollisionSupport != nullptr)
    {
        BaseCrateCollisionSupport->RegisterInterfaceOwnerCharacter(this);
        CrateMesh = Cast<UStaticMeshComponent>(FindComponentByClass(UStaticMeshComponent::StaticClass()));
        if(CrateMesh != nullptr)
        {
            CrateMesh->OnComponentBeginOverlap.AddDynamic(BaseCrateCollisionSupport,&UCollisionSupportComponent::ProcessCollisionGeometry);
        }
    }
}

void AAnterBaseCrate::HandleCollision(const FCollisionGeometry& InCollisionGeometry, AActor* OtherActor) 
{
    if(UGameSpecificStaticLibrary::IsHealthDamageType(this,OtherActor))
    {
        HandleDamage(OtherActor);
    }
}

void AAnterBaseCrate::HandleDamage(AActor* OtherActor)
{
    if(AAnterFire* AnterFire = Cast<AAnterFire>(OtherActor))
    {  
        AnterFire->Destroy();
        Destroy();
    }
}

void AAnterBaseCrate::OnVerticallyHit()
{
    this->Destroy();
}