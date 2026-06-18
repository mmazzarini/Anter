#include "SceneActors/AnterFire.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameSpecificStaticLibrary/GameSpecificStaticLibrary.h"

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
        if (!UGameSpecificStaticLibrary::IsInScreen(PlayerController, GetActorLocation(), ScreenAppearanceMultiplier))
        {
            Destroy();
        }
    }
}

void AAnterFire::SetMovementToRight(FVector InMovementDirection)
{
    ProjectileMovement->Velocity.X = EditableVelocity*InMovementDirection.X;
    ProjectileMovement->Velocity.Z = EditableVelocity*InMovementDirection.Z;
}