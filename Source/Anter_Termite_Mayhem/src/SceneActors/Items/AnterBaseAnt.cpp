#include "SceneActors/Items/AnterBaseAnt.h"
#include "Components/PrimitiveComponent.h"

void AAnterBaseAnt::BeginPlay()
{
/*
    if(AnterSphere != nullptr)
    {
        AnterMesh->OnComponentBeginOverlap.AddDynamic(this,&UCollisionSupportComponent::Process);
    }
*/
}

/*
void AAnterBaseAnt::OnAntCollided(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if(Other != nullptr)
    {
        if(AAnterPaperCharacter* Anter = Cast<AAnterPaperCharacter>(Other))
        {
            OnAnterCollided.Broadcast();
            this->Destroy();
        }
    }
}
*/