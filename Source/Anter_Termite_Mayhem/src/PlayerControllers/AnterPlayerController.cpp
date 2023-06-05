#include "PlayerControllers/AnterPlayerController.h"

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