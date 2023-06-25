#include "ActorComponents/AnterMovementSupportComponent.h"
#include "Pawn/AnterPaperCharacter.h"

UAnterMovementSupportComponent::UAnterMovementSupportComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;  
}

void UAnterMovementSupportComponent::BeginPlay() 
{
    Super::BeginPlay();
    Anter = Cast<AAnterPaperCharacter>(GetOwner());
}

void UAnterMovementSupportComponent::HandleRightMovement(float InMovementAmount)
{
    
}

void UAnterMovementSupportComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if(Anter != nullptr) //&& Anter->GetMovingStatus!!!!
    {
        Anter->AddActorWorldOffset(FVector::UpVector*VerticalShift);
    }
}
