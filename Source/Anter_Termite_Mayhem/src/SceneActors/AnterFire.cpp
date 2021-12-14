#include "SceneActors/AnterFire.h"
#include "Kismet/GameplayStatics.h"

AAnterFire::AAnterFire()
{

}

void AAnterFire::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    UpdatePosition();
    CheckScreenLocation();
}

void AAnterFire::BeginPlay()
{
    Super::BeginPlay();
    MovementVector.X = MovementX;
    MovementVector.Y = MovementY;
    MovementVector.Z = MovementZ;
    PlayerController = Cast<AAnterPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void AAnterFire::UpdatePosition()
{   
    AddMovementInput(MovementVector);
}

void AAnterFire::CheckScreenLocation()
{
    if (PlayerController != nullptr) // && (Camera != nullptr))
    {
        //Get the screen location
        int32 ScreenX, ScreenY;
        PlayerController->GetViewportSize(ScreenX,ScreenY);
        //Get the projection of world location on the screen
        FVector2D ScreenPos = FVector2D(0.0f,0.0f);
        PlayerController->ProjectWorldLocationToScreen(GetActorLocation(), ScreenPos);
        if(ScreenPos.X>ScreenX || ScreenPos.X < 0.0f)
        {
            Destroy();
        }
    }
}