#include "SceneActors/AnterFire.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

AAnterFire::AAnterFire()
{
    FireMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FireMesh"));
    FireMesh->SetupAttachment(RootComponent);   

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
}

void AAnterFire::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    //Super::Tick(DeltaSeconds);
    UpdatePosition();
    CheckScreenLocation(); 
}

void AAnterFire::BeginPlay()
{
    Super::BeginPlay();
    MovementVector.X = MovementX;
    MovementVector.Y = MovementY;
    MovementVector.Z = MovementZ;
    /*
    UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(FindComponentByClass<UCharacterMovementComponent>());
    if(MovementComponent != nullptr)
    {
        MovementComponent->bAutoActivate = true;
    }
    */
    if(ProjectileMovement !=nullptr)
    {
        ProjectileMovement->ProjectileGravityScale = false;
        ProjectileMovement->Velocity.X = 0.0f;
    }

    PlayerController = Cast<AAnterPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void AAnterFire::UpdatePosition()
{   
    //UE_LOG(LogTemp, Warning,TEXT("Fire Impulse is %f"), MovementVector.X);
    //AddMovementInput(MovementVector,MovementMultiplier*RightMovementCorrectionFactor,true);
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
        if(ScreenPos.X>ScreenX*ScreenAppearanceMultiplier || ScreenPos.X < 0.0f)
        {
            Destroy();
        }
    }
}

PRAGMA_DISABLE_OPTIMIZATION
void AAnterFire::SetMovementToRight(FVector InMovementDirection)
{
    ProjectileMovement->Velocity.X = EditableVelocity*InMovementDirection.X;
    ProjectileMovement->Velocity.Z = EditableVelocity*InMovementDirection.Z;
}
PRAGMA_ENABLE_OPTIMIZATION