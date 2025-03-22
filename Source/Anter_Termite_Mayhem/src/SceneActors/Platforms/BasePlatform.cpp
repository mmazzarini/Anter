#include "SceneActors/Platforms/BasePlatform.h"
#include "Components/CapsuleComponent.h"

ABasePlatform::ABasePlatform()
{
    //PlatformBox = CreateDefaultSubobject<UBoxComponent>(TEXT("PlatformBox"));
    //RootComponent = PlatformBox;

    PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
    RootComponent = PlatformMesh;

    UCapsuleComponent* Capsule = FindComponentByClass<UCapsuleComponent>();
    if(Capsule != nullptr)
    {
        Capsule->DestroyComponent();
    }    
}

void ABasePlatform::BeginPlay()
{   
    UStaticMesh* InnerMesh = PlatformMesh->GetStaticMesh();
    if(InnerMesh != nullptr)
    {
        UBodySetup* PlatformBody = InnerMesh->GetBodySetup();
        
        if(PlatformBody != nullptr)
        {
            FKAggregateGeom AggregateGeom = PlatformBody->AggGeom;
            FKBoxElem PlatformMeshBox = AggregateGeom.BoxElems[0];
            const FName MyName = PlatformMeshBox.GetName();
            PlatformMeshBox.X = PlatformMesh->Bounds.BoxExtent.X;
            if(MyName == "pippo")
            {

            }
        }
        
    }

    Super::BeginPlay();
}
