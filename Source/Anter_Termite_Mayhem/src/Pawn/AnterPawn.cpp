#include "Pawn/AnterPawn.h"
#include "ActorComponents/HealthComponent.h"
#include "Components/InputComponent.h"
#include "ActorComponents/AnterMovementComponent.h"

AAnterPawn::AAnterPawn()
{
    SetBindings();
}

void AAnterPawn::Tick(float DeltaTime)
{
    const FVector NewLoc;
    SetActorLocation(NewLoc);
}

void AAnterPawn::OnDeathEvent()
{
    Destroy();
}

void AAnterPawn::SetBindings()
{
    UHealthComponent* HealthComponent = Cast<UHealthComponent>(FindComponentByClass<UHealthComponent>());
    if(HealthComponent != nullptr)
    {
        HealthComponent->GetDeathReachedDelegate().AddDynamic(this,&AAnterPawn::OnDeathEvent);
    }
    
    UInputComponent* InputComponent = Cast<UInputComponent>(FindComponentByClass<UInputComponent>());
    UAnterMovementComponent* MovementComponent = Cast<UAnterMovementComponent>(FindComponentByClass<UAnterMovementComponent>());
    if((InputComponent != nullptr) && (MovementComponent != nullptr))
    {
        InputComponent->BindAxis("RightMovement",MovementComponent,&UAnterMovementComponent::HandleRightMovement);
    }
}