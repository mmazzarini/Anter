#include "Pawn/AnterPaperCharacter.h"
#include "ActorComponents/HealthComponent.h"
#include "Components/InputComponent.h"
#include "ActorComponents/AnterMovementComponent.h"

AAnterPaperCharacter::AAnterPaperCharacter()
{
    SetBindings();
}

void AAnterPaperCharacter::Tick(float DeltaTime)
{
    const FVector NewLoc;
    SetActorLocation(NewLoc);
}

void AAnterPaperCharacter::OnDeathEvent()
{
    UHealthComponent* HealthComponent = Cast<UHealthComponent>(FindComponentByClass<UHealthComponent>());
    if(HealthComponent != nullptr)
    {
        HealthComponent->GetDeathReachedDelegate().RemoveDynamic(this,&AAnterPaperCharacter::OnDeathEvent);
    }
    Destroy();
}

void AAnterPaperCharacter::SetBindings()
{
    UHealthComponent* HealthComponent = Cast<UHealthComponent>(FindComponentByClass<UHealthComponent>());
    if(HealthComponent != nullptr)
    {
        HealthComponent->GetDeathReachedDelegate().AddDynamic(this,&AAnterPaperCharacter::OnDeathEvent);
    }
    
    UInputComponent* InputComponent = Cast<UInputComponent>(FindComponentByClass<UInputComponent>());
    UAnterMovementComponent* MovementComponent = Cast<UAnterMovementComponent>(FindComponentByClass<UAnterMovementComponent>());
    if((InputComponent != nullptr) && (MovementComponent != nullptr))
    {
        InputComponent->BindAxis("RightMovement",MovementComponent,&UAnterMovementComponent::HandleRightMovement);
    }

    
}