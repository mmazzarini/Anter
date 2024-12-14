#include "PlayerControllers/AnterPlayerController.h"
#include "Kismet/GameplayStatics.h"

void AAnterPlayerController::InitInputSystem()
{
    //Just call the super: for debug purposes right now.
    Super::InitInputSystem();
}

void AAnterPlayerController::SetPawn(APawn* InPawn)
{
     //Just call the super: for debug purposes right now.
    Super::SetPawn(InPawn);
}

void AAnterPlayerController::OnPossess(APawn* PawnToPossess)
{
    Super::OnPossess(PawnToPossess);
}

void AAnterPlayerController::OnGamePausePressed()
{
    if (!UGameplayStatics::IsGamePaused(this))
    {
        UGameplayStatics::SetGamePaused(this,true);
        OnLevelPaused.Broadcast();
    }
}

void AAnterPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    if (InputComponent != nullptr)
    {
        InputComponent->BindAction("PauseAction", EInputEvent::IE_Released, this, &AAnterPlayerController::OnGamePausePressed);
    }
}
