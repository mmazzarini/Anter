#include "Pawn/AnterPaperCharacter.h"
#include "GameFramework/Character.h"
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
    CharMovement->SetupAttachment(RootComponent);
    */

    Spring = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring"));
    Spring->SetupAttachment(RootComponent);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(Spring);

    AnterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AnterMesh"));
    AnterMesh->SetupAttachment(RootComponent);

    AnterHealth = CreateDefaultSubobject<UHealthComponent>(TEXT("AnterHealth"));
    AnterHealth->SetupAttachment(RootComponent);

}

void AAnterPaperCharacter::Tick(float DeltaTime)
{

}

void AAnterPaperCharacter::OnDeathEvent()
{
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
    if(AnterHealth != nullptr)
    {
        AnterHealth->GetDeathReachedDelegate().AddDynamic(this,&AAnterPaperCharacter::OnDeathEvent);
    }
}

PRAGMA_DISABLE_OPTIMIZATION
void AAnterPaperCharacter::SetupPlayerInputComponent(UInputComponent* InputComponent)
{   
    Super::SetupPlayerInputComponent(InputComponent);
    UCharacterMovementComponent* AnterMovement = Cast<UCharacterMovementComponent>(FindComponentByClass<UCharacterMovementComponent>());
    if((InputComponent != nullptr))// && (AnterMovement != nullptr))
    {
        InputComponent->BindAxis("RightMovement",this,&AAnterPaperCharacter::HandleRightMovement);
        InputComponent->BindAction("Jump",IE_Pressed,this,&AAnterPaperCharacter::HandleJump);
        //InputComponent->BindAction("RightMovement",IE_Released,this,&AAnterPaperCharacter::HandleRightMovement);
    }

}
PRAGMA_ENABLE_OPTIMIZATION

void AAnterPaperCharacter::HandleRightMovement(float InAxisValue)
{
    UCharacterMovementComponent* AnterMovement = Cast<UCharacterMovementComponent>(FindComponentByClass<UCharacterMovementComponent>());
    if(AnterMovement != nullptr)
    {
        FVector MovementVector = FVector(InAxisValue,0.0f,0.0f);
        if(InAxisValue != 0.0f)
        {
            //MoveRight(InAxisValue*100.0f);
            UE_LOG(LogTemp, Warning,TEXT("Impulse is %f"), MovementVector.X);
            
            AddMovementInput(MovementVector,MovementMultiplier);

            /*
            Qui: guarda come il character gestisce gli input.
            Controlla: Se non ci sono collisioni bloccanti tra piano e cubo. 
            Prima stacca le collisions. E disabilita la gravity.
            Poi vedo

            JUMP: per fare il salto, sfrutta come il CharacterMovement ha già implementata sta cosa.
            */

            //AnterMovement->AddImpulse(MovementVector);
        }
    }
}

void AAnterPaperCharacter::HandleJump()
{
    UCharacterMovementComponent* AnterMovement = Cast<UCharacterMovementComponent>(FindComponentByClass<UCharacterMovementComponent>());
    if(AnterMovement != nullptr)
    {
        FVector JumpVector = FVector(0.0f,0.0f,JumpScale);
        //MoveRight(InAxisValue*100.0f);
        UE_LOG(LogTemp, Warning,TEXT("Jump Impulse is %f"), JumpVector.Z);
        AnterMovement->AddImpulse(JumpVector);
        //AddMovementInput(JumpVector,JumpScale);
    }
}
