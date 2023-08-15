#include "SceneActors/Items/LevelCheckpoint.h"
#include "GameSpecificStaticLibrary/GameSpecificStaticLibrary.h"
#include "Pawn/AnterPaperCharacter.h"

ALevelCheckpoint::ALevelCheckpoint()
{
    LevelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LevelMesh"));
    LevelMesh->SetupAttachment(RootComponent);

    LevelBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LevelBox"));
    LevelBox->SetupAttachment(RootComponent);
}

void ALevelCheckpoint::BeginPlay()
{
    Super::BeginPlay();
    SetBindings();
}

void ALevelCheckpoint::OnCheckpointActivated()
{
    bIsActivated = true;
    OnActivatedCheckpointDelegate.Broadcast(this);
}

void ALevelCheckpoint::SetBindings()
{
    if(LevelMesh !=nullptr)
    {
        LevelMesh->OnComponentBeginOverlap.AddDynamic(this,&ALevelCheckpoint::OnCollidedWithActor);
    }
}

void ALevelCheckpoint::OnCollidedWithActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    if(AAnterPaperCharacter* Anter = Cast<AAnterPaperCharacter>(OtherActor))
    {
        OnCheckpointActivated();
    }
}