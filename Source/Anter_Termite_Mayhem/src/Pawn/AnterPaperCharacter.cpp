#include "Pawn/AnterPaperCharacter.h"
#include "GameFramework/Character.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ActorComponents/AnterMovementSupportComponent.h"
#include "ActorComponents/AnterInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerControllers/AnterPlayerController.h"
#include "AnterCameras/AnterCameraActor.h"
#include "Engine/GameEngine.h"
#include "Kismet/KismetMathLibrary.h"

AAnterPaperCharacter::AAnterPaperCharacter()
{

    AnterMovementSupport = CreateDefaultSubobject<UAnterMovementSupportComponent>(TEXT("AnterMovementSupport"));
    AnterMovementSupport->SetupAttachment(RootComponent);

    Spring = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring"));
    Spring->SetupAttachment(RootComponent);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(Spring);

    AnterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AnterMesh"));
    AnterMesh->SetupAttachment(RootComponent);

    AnterHealth = CreateDefaultSubobject<UHealthComponent>(TEXT("AnterHealth"));
    AnterHealth->SetupAttachment(RootComponent);

    AnterWeapon = CreateDefaultSubobject<UAnterWeaponComponent>(TEXT("AnterWeapon"));
    AnterWeapon->SetupAttachment(RootComponent);

    AnterBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AnterBox"));
    AnterBox->SetupAttachment(RootComponent);

}

void AAnterPaperCharacter::Tick(float DeltaTime)
{
    AdjustVelocity();
    //Ensure no displacement from Y = 0 plane
    SetActorLocation(FVector(GetActorLocation().X,0.0f,GetActorLocation().Z));
}

void AAnterPaperCharacter::OnDeathEvent()
{
    if(AnterHealth != nullptr)
    {
        AnterHealth->GetDeathReachedDelegate().RemoveDynamic(this,&AAnterPaperCharacter::OnDeathEvent);
    }
    if(AnterBox != nullptr)
    {
        if(AnterBox->OnComponentBeginOverlap.IsBound())
        {
            AnterBox->OnComponentBeginOverlap.RemoveDynamic(this,&AAnterPaperCharacter::OnColliderHit);
        }
        if(AnterBox->OnComponentEndOverlap.IsBound())
        {
            AnterBox->OnComponentEndOverlap.RemoveDynamic(this,&AAnterPaperCharacter::OnColliderUnhit);
        }
    }
    Destroy();
    RegisteredVerticalPlatformCollisions.Empty();
}


void AAnterPaperCharacter::BeginPlay()
{
    Super::BeginPlay();
    AAnterPlayerController* PlayerController = Cast<AAnterPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (PlayerController != nullptr)
    {
       PlayerController->SetViewTarget(this); 
    }
    SetBindings();
    SetupGravity();
    RegisteredVerticalPlatformCollisions.Empty();
    /*
    if(Camera != nullptr)
    {
        Camera->ProjectionMode = ECameraProjectionMode::Orthographic;
    }
    */

};

void AAnterPaperCharacter::SetBindings()
{
    if(AnterHealth != nullptr)
    {
        AnterHealth->GetDeathReachedDelegate().AddDynamic(this,&AAnterPaperCharacter::OnDeathEvent);
    }

    if(AnterBox != nullptr)
    {
        AnterBox->OnComponentBeginOverlap.AddDynamic(this,&AAnterPaperCharacter::OnColliderHit);
        AnterBox->OnComponentEndOverlap.AddDynamic(this,&AAnterPaperCharacter::OnColliderUnhit);
    }

}

void AAnterPaperCharacter::SetupGravity()
{
    UCharacterMovementComponent* AnterMovement = Cast<UCharacterMovementComponent>(FindComponentByClass<UCharacterMovementComponent>());
    if(AnterMovement != nullptr)
    {
        AnterMovement->GravityScale = InputGravityScale;
    }
}

void AAnterPaperCharacter::SetupPlayerInputComponent(UInputComponent* InputComponent)
{   
    Super::SetupPlayerInputComponent(InputComponent);
    UCharacterMovementComponent* AnterMovement = Cast<UCharacterMovementComponent>(FindComponentByClass<UCharacterMovementComponent>());
    if((InputComponent != nullptr))
    {
        InputComponent->BindAxis("RightMovement",this,&AAnterPaperCharacter::HandleRightMovement);
        InputComponent->BindAction("Jump",IE_Pressed,this,&AAnterPaperCharacter::HandleJump);
        if(AnterWeapon != nullptr)
        {
            InputComponent->BindAxis("RightMovement",AnterWeapon,&UAnterWeaponComponent::OnOwnerMoving);
            InputComponent->BindAction("Fire",IE_Pressed, AnterWeapon, &UAnterWeaponComponent::ShootLaser);   
        }
    }

}

PRAGMA_DISABLE_OPTIMIZATION
void AAnterPaperCharacter::HandleRightMovement(float InAxisValue)
{
    UCharacterMovementComponent* AnterMovement = Cast<UCharacterMovementComponent>(FindComponentByClass<UCharacterMovementComponent>());
    if(AnterMovement != nullptr)
    {
        FVector MovementVector = FVector(InAxisValue,0.0f,0.0f);
        if(InAxisValue != 0.0f)
        {
            UE_LOG(LogTemp, Warning,TEXT("Impulse is %f"), MovementVector.X);
            if((InAxisValue >0.0f && bIsRightUnlocked) || (InAxisValue <0.0f && bIsLeftUnlocked))
            {
                AddMovementInput(MovementVector,MovementMultiplier);
            }
            else
            {
                AnterMovement->Velocity.X = 0.0f;
            }           
        }
        else
        {
            if(abs(AnterMovement->Velocity.X) >= VelocityThreshold)
            {
                AddMovementInput(-1.0f*FVector::XAxisVector*AnterMovement->Velocity.X*FrictionScale);
            }
            else
            {
                AnterMovement->Velocity.X = 0.0f;
            }
        }
    }
}
PRAGMA_ENABLE_OPTIMIZATION

void AAnterPaperCharacter::HandleJump()
{
    UCharacterMovementComponent* AnterMovement = Cast<UCharacterMovementComponent>(FindComponentByClass<UCharacterMovementComponent>());
    if(AnterMovement != nullptr)
    {
        FVector JumpVector = FVector(0.0f,0.0f,JumpScale);
        UE_LOG(LogTemp, Warning,TEXT("Jump Impulse is %f"), JumpVector.Z);
        AnterMovement->AddImpulse(JumpVector);
    }
}

PRAGMA_DISABLE_OPTIMIZATION

void AAnterPaperCharacter::OnColliderHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    if(OtherActor != nullptr)
    {
        if(OtherActor->GetName().Contains("Floor"))
        {
            if(GEngine != nullptr)
            {
                GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,TEXT("Collision begun"));
            }
            //Get distance from platform surface centre to player and compare it with normal
            FVector PlatformCentre = FVector(0.0f,0.0f,0.0f);
            FVector PlatformSize = FVector(0.0f,0.0f,0.0f);
            OtherActor->GetActorBounds(true,PlatformCentre,PlatformSize,false);
            FVector PlatformSurfaceCentre = PlatformCentre + FVector::UpVector*PlatformSize.Z/2.;
            FVector Dist = this->GetActorLocation()-PlatformSurfaceCentre;     
            FVector Height = FVector::UpVector*(GetActorLocation().Z - PlatformSurfaceCentre.Z);  

            //Check geometry of collision to decide which impact       
            if(FVector::DotProduct(Dist,FVector::UpVector) > VerticalTolerance)
            {
                /* Impact was from top: pawn is standing on platform. */ 

                //Register impact with vertical colliding platform
                RegisterPlatformCollision(OtherActor,true);

                UCharacterMovementComponent* AnterMovement = Cast<UCharacterMovementComponent>(FindComponentByClass<UCharacterMovementComponent>());
                if(AnterMovement != nullptr)
                {
                    SetIsFalling(false);
                    SetCanJump(true);
                    AnterMovement->GravityScale = 0.0f;
                }
                //Floor impenetrability condition
                FVector AnterSize = FVector(0.0f,0.0f,0.0f);
                FVector AnterCentre = FVector(0.0f,0.0f,0.0f);
                this->GetActorBounds(true,AnterCentre,AnterSize,false);
                FVector NewLocation = FVector(GetActorLocation().X,GetActorLocation().Y,PlatformSurfaceCentre.Z + (FVector::UpVector*AnterSize.Z/VerticalImpenetrabilityFactor).Z);
                SetActorLocation(NewLocation);
                GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,TEXT("Anter new location: ") + NewLocation.ToString());
            } 
            else
            {
                /* Impact was from side or bottom. */

                //Register impact with vertical colliding platform
                RegisterPlatformCollision(OtherActor,false);

                /* Impact was from side or bottom */
                FVector PlatformRightSideCentre = PlatformCentre + FVector::XAxisVector*PlatformSize.X/2.;
                FVector PlatformLeftSideCentre = PlatformCentre - FVector::XAxisVector*PlatformSize.X/2.;
                FVector RightDist = this->GetActorLocation()-PlatformCentre;
                FVector LeftDist = this->GetActorLocation()-PlatformCentre;
                if(RightDist.X > 0.0f)
                {
                    if((FVector::DotProduct(RightDist,FVector::XAxisVector) > HorizontalTolerance))
                    {
                        SetLeftMovementFree(false);
                    }
                }
                else if(LeftDist.X < 0.0f)
                {
                    if((FVector::DotProduct(LeftDist,-FVector::XAxisVector) > HorizontalTolerance))
                    {
                        SetRightMovementFree(false);
                    }
                }
            }
        }
    }
}
PRAGMA_ENABLE_OPTIMIZATION

PRAGMA_DISABLE_OPTIMIZATION
void AAnterPaperCharacter::OnColliderUnhit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if(OtherActor != nullptr)
    {
        if(OtherActor->GetName().Contains("Floor"))
        {

            //First, Deregister platform from colliding platforms
            DeregisterPlatformCollision(OtherActor);

            if(GEngine != nullptr)
            {
                GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,TEXT("Collision ended"));
            }
             UCharacterMovementComponent* AnterMovement = Cast<UCharacterMovementComponent>(FindComponentByClass<UCharacterMovementComponent>());
            if(AnterMovement != nullptr)
            {
                //If there is no vertically colliding platform in the registered platforms array, then free fall
                if(FindAnyVerticalCollision() == false)
                {
                    SetIsFalling(true);
                    SetCanJump(false);
                    AnterMovement->GravityScale = InputGravityScale;
                }
                
                SetLeftMovementFree(true);
                SetRightMovementFree(true);
            }
        }
    }
}
PRAGMA_ENABLE_OPTIMIZATION

void AAnterPaperCharacter::SetCanJump(bool InCanJump)
{
    bCanAnterJump = InCanJump;
}


void AAnterPaperCharacter::SetIsFalling(bool InIsFalling)
{
    bIsFalling = InIsFalling;
}


PRAGMA_DISABLE_OPTIMIZATION
void AAnterPaperCharacter::RegisterPlatformCollision(AActor* InPlatformToAdd, bool IsVerticallyColliding)
{
    TPair<AActor*,bool> PlatformToAdd = TPair<AActor*,bool>(InPlatformToAdd,IsVerticallyColliding);
    RegisteredVerticalPlatformCollisions.Add(PlatformToAdd);
}
PRAGMA_ENABLE_OPTIMIZATION

PRAGMA_DISABLE_OPTIMIZATION
void AAnterPaperCharacter::DeregisterPlatformCollision(AActor* InPlatformToRemove)
{
    auto PlatformIndex = RegisteredVerticalPlatformCollisions.IndexOfByPredicate([InPlatformToRemove](TPair <AActor*,bool> PlatformPair)
    {
        return PlatformPair.Key == InPlatformToRemove;
    }
    );
    if (PlatformIndex != INDEX_NONE)
    {
        RegisteredVerticalPlatformCollisions.RemoveAt(PlatformIndex);
    }

}
PRAGMA_ENABLE_OPTIMIZATION

PRAGMA_DISABLE_OPTIMIZATION
bool AAnterPaperCharacter::FindAnyVerticalCollision()
{
    TPair <AActor*,bool>* PlatformToFind = RegisteredVerticalPlatformCollisions.FindByPredicate([](TPair <AActor*,bool> PlatformPair)
    {
        return PlatformPair.Value == true;
    }
    );
    return (PlatformToFind != nullptr);
}
PRAGMA_ENABLE_OPTIMIZATION

void AAnterPaperCharacter::AdjustVelocity()
{
    if(bIsFalling == false)
    {
        UCharacterMovementComponent* AnterMovement = Cast<UCharacterMovementComponent>(FindComponentByClass<UCharacterMovementComponent>());
        if(AnterMovement != nullptr)
        {
            AnterMovement->Velocity.Z = 0.0f;   
        }
    }
}

/*
    GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,TEXT("Anter prev. location: ") + GetActorLocation().ToString());
    GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,TEXT("Dot product") + FString::SanitizeFloat(FVector::DotProduct(Dist,FVector::UpVector)));
    GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,TEXT("Platform Centre: ") + PlatformCentre.ToString());
    GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,TEXT("Platform Surface Centre: ") + PlatformSurfaceCentre.ToString());
*/