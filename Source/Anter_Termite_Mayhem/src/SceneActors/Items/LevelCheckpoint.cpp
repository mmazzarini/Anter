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

void ALevelCheckpoint::ActivateCheckpoint()
{
    bIsActivated = true;
    OnActivatedCheckpointDelegate.Broadcast(this);
}

bool ALevelCheckpoint::GetIsActivated() const
{
    return bIsActivated;
}
