#include "Pawn/AnterPaperCharacter.h"
#include "Components/InputComponent.h"
#include "ActorComponents/AnterInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerControllers/AnterPlayerController.h"
#include "AnterCameras/AnterCameraActor.h"

AAnterPaperCharacter::AAnterPaperCharacter()
{
}

void AAnterPaperCharacter::Tick(float DeltaTime)
{

}

void AAnterPaperCharacter::OnDeathEvent()
{
    UHealthComponent* Health = Cast<UHealthComponent>(FindComponentByClass<UHealthComponent>());
    if(Health != nullptr)
    {
        Health->GetDeathReachedDelegate().RemoveDynamic(this,&AAnterPaperCharacter::OnDeathEvent);
    }
    Destroy();
}


void AAnterPaperCharacter::BeginPlay()
{
    Super::BeginPlay();
    AAnterPlayerController* PlayerController = Cast<AAnterPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (PlayerController != nullptr) // && (Camera != nullptr))
    {
       PlayerController->SetViewTarget(this); 
    }
    SetBindings();
};

void AAnterPaperCharacter::SetBindings()
{
    UHealthComponent* Health = Cast<UHealthComponent>(FindComponentByClass<UHealthComponent>());
    if(Health != nullptr)
    {
        Health->GetDeathReachedDelegate().AddDynamic(this,&AAnterPaperCharacter::OnDeathEvent);
    }
}

PRAGMA_DISABLE_OPTIMIZATION
void AAnterPaperCharacter::SetupPlayerInputComponent(UInputComponent* InputComponent)
{   
    Super::SetupPlayerInputComponent(InputComponent);
    UAnterMovementComponent* AnterMovement = Cast<UAnterMovementComponent>(FindComponentByClass<UAnterMovementComponent>());
    if((InputComponent != nullptr) && (AnterMovement != nullptr))
    {
        InputComponent->BindAxis("RightMovement",AnterMovement,&UAnterMovementComponent::HandleRightMovement);
    }

}
PRAGMA_ENABLE_OPTIMIZATION
