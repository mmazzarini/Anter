#include "SceneActors/Items/LevelGoal.h"
#include "Pawn/AnterPaperCharacter.h"


ALevelGoal::ALevelGoal()
{
    LevelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LevelMesh"));
    LevelMesh->SetupAttachment(RootComponent);

    LevelBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LevelBox"));
    LevelBox->SetupAttachment(RootComponent);
}

void ALevelGoal::BeginPlay()
{
    Super::BeginPlay();
    SetBindings();
}

void ALevelGoal::SetBindings()
{
    if(LevelMesh !=nullptr)
    {
        LevelMesh->OnComponentBeginOverlap.AddDynamic(this,&ALevelGoal::OnCollidedWithActor);
    }
}

void ALevelGoal::OnCollidedWithActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    if(AAnterPaperCharacter* Anter = Cast<AAnterPaperCharacter>(OtherActor))
    {
        Anter->Destroy();
        LevelGoalReached.Broadcast();
    }
}