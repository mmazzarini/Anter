#include "Pawn/AnterPaperCharacter.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ActorComponents/AnterMovementComponent.h"
#include "ActorComponents/AnterInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerControllers/AnterPlayerController.h"
#include "AnterCameras/AnterCameraActor.h"

AAnterPaperCharacter::AAnterPaperCharacter()
{

    /*
    //AnterMovement = CreateDefaultSubobject<UAnterMovementComponent>(TEXT("AnterMovement"));
    UCharacterMovementComponent = Cast<UHealthComponent>(FindComponentByClass<UHealthComponent>());
    UCharacterMovementComponent* CharMovement = Cast<UCharacterMovementComponent>(AnterMovement);
    if()
    CharMovement->SetupAttachment(RootComponent);
    */

    Spring = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring"));
    Spring->SetupAttachment(RootComponent);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(Spring);

    AnterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AnterMesh"));
    AnterMesh->SetupAttachment(RootComponent);

    //AnterHealth = CreateDefaultSubobject<UHealthComponent>(TEXT("AnterMovement"));
    //AnterHealth->SetupAttachment(RootComponent);

}

void AAnterPaperCharacter::Tick(float DeltaTime)
{

}

void AAnterPaperCharacter::OnDeathEvent()
{
    AnterHealth = Cast<UHealthComponent>(FindComponentByClass<UHealthComponent>());
    if(AnterHealth != nullptr)
    {
        AnterHealth->GetDeathReachedDelegate().RemoveDynamic(this,&AAnterPaperCharacter::OnDeathEvent);
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
    AnterHealth = Cast<UHealthComponent>(FindComponentByClass<UHealthComponent>());
    if(AnterHealth != nullptr)
    {
        AnterHealth->GetDeathReachedDelegate().AddDynamic(this,&AAnterPaperCharacter::OnDeathEvent);
    }
}

PRAGMA_DISABLE_OPTIMIZATION
void AAnterPaperCharacter::SetupPlayerInputComponent(UInputComponent* InputComponent)
{   
    Super::SetupPlayerInputComponent(InputComponent);
    //UAnterMovementComponent* AnterMovement = Cast<UAnterMovementComponent>(FindComponentByClass<UAnterMovementComponent>());
    if((InputComponent != nullptr))// && (AnterMovement != nullptr))
    {
        InputComponent->BindAxis("RightMovement",this,&AAnterPaperCharacter::HandleRightMovement);
    }

}
PRAGMA_ENABLE_OPTIMIZATION

void AAnterPaperCharacter::HandleRightMovement(float InAxisValue)
{
    UAnterMovementComponent* AnterMovement = Cast<UAnterMovementComponent>(FindComponentByClass<UAnterMovementComponent>());
    if(AnterMovement != nullptr)
    {
        FVector MovementVector = FVector(InAxisValue,0.0f,0.0f);
        if(InAxisValue != 0.0f)
        {
            //MoveRight(InAxisValue*100.0f);
            UE_LOG(LogTemp, Warning,TEXT("Impulse is %f"), MovementVector.X);
            this->AddMovementInput(MovementVector,Multiplier);
            //AnterMovement->AddImpulse(MovementVector);
        }
    }
}