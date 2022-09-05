#include "Pawn/AnterPaperCharacter.h"
#include "GameFramework/Character.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ActorComponents/AnterMovementSupportComponent.h"
#include "ActorComponents/CollisionSupportComponent.h"
#include "ActorComponents/AnterInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerControllers/AnterPlayerController.h"
#include "AnterCameras/AnterCameraActor.h"
#include "Engine/GameEngine.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/PrimitiveComponent.h"
#include "SceneActors/Platforms/BasePlatform.h"

AAnterPaperCharacter::AAnterPaperCharacter()
{

    AnterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AnterMesh"));
    AnterMesh->SetupAttachment(RootComponent);

    AnterMovementSupport = CreateDefaultSubobject<UAnterMovementSupportComponent>(TEXT("AnterMovementSupport"));
    AnterMovementSupport->SetupAttachment(RootComponent);

    Spring = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring"));
    Spring->SetupAttachment(RootComponent);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(Spring);

    AnterHealth = CreateDefaultSubobject<UHealthComponent>(TEXT("AnterHealth"));
    AnterHealth->SetupAttachment(RootComponent);

    AnterWeapon = CreateDefaultSubobject<UAnterWeaponComponent>(TEXT("AnterWeapon"));
    AnterWeapon->SetupAttachment(RootComponent);

    AnterBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AnterBox"));
    AnterBox->SetupAttachment(RootComponent);

    AnterCollisionSupport = CreateDefaultSubobject<UCollisionSupportComponent>(TEXT("AnterCollisionSupport"));
    AnterCollisionSupport->SetupAttachment(RootComponent);

} 

PRAGMA_DISABLE_OPTIMIZATION
void AAnterPaperCharacter::Tick(float DeltaTime)
{
    AdjustVelocity();
    //Ensure no displacement from Y = 0 plane
    FVector LocationTemp = GetActorLocation();
    SetActorLocation(FVector(GetActorLocation().X,0.0f,GetActorLocation().Z));
    ConstrainJump();
}
PRAGMA_ENABLE_OPTIMIZATION

void AAnterPaperCharacter::OnDeathEvent()
{
    if(AnterHealth != nullptr)
    {
        AnterHealth->GetDeathReachedDelegate().RemoveDynamic(this,&AAnterPaperCharacter::OnDeathEvent);
    }
    if(AnterMesh != nullptr && AnterCollisionSupport != nullptr)
    {
        if(AnterMesh->OnComponentBeginOverlap.IsBound())
        {
            AnterMesh->OnComponentBeginOverlap.RemoveDynamic(AnterCollisionSupport,&UCollisionSupportComponent::ProcessCollisionGeometry);
        }
        if(AnterMesh->OnComponentEndOverlap.IsBound())
        {
            AnterMesh->OnComponentEndOverlap.RemoveDynamic(this,&AAnterPaperCharacter::OnColliderUnhit);
        }
    }
    Destroy();
    RegisteredVerticalPlatformCollisions.Empty();
}

PRAGMA_DISABLE_OPTIMIZATION
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
    ResetGeometron();

    if(AnterCollisionSupport != nullptr)
    {
        AnterCollisionSupport->RegisterInterfaceOwnerCharacter(this);
    }

    //Anter centre is not really revelevant at BeginPlay, but AnterSize is. 
    GetActorBounds(true,AnterCentre,AnterSize,false);
}
PRAGMA_ENABLE_OPTIMIZATION

void AAnterPaperCharacter::SetBindings()
{
    if(AnterHealth != nullptr)
    {
        AnterHealth->GetDeathReachedDelegate().AddDynamic(this,&AAnterPaperCharacter::OnDeathEvent);
    }

    if(AnterMesh != nullptr && AnterCollisionSupport != nullptr)
    {
        AnterMesh->OnComponentBeginOverlap.AddDynamic(AnterCollisionSupport,&UCollisionSupportComponent::ProcessCollisionGeometry);
        AnterMesh->OnComponentEndOverlap.AddDynamic(this,&AAnterPaperCharacter::OnColliderUnhit);
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

void AAnterPaperCharacter::SetupPlayerInputComponent(UInputComponent* InInputComponent)
{   
    Super::SetupPlayerInputComponent(InInputComponent);
    UCharacterMovementComponent* AnterMovement = Cast<UCharacterMovementComponent>(FindComponentByClass<UCharacterMovementComponent>());
    if((InInputComponent != nullptr))
    {
        InInputComponent->BindAxis("RightMovement",this,&AAnterPaperCharacter::HandleRightMovement);
        InInputComponent->BindAction("Jump",IE_Pressed,this,&AAnterPaperCharacter::HandleJump);
        if(AnterWeapon != nullptr)
        {
            InInputComponent->BindAxis("RightMovement",AnterWeapon,&UAnterWeaponComponent::OnOwnerMoving);
            InInputComponent->BindAction("Fire",IE_Pressed, AnterWeapon, &UAnterWeaponComponent::ShootLaser);   
        }
    }

}

PRAGMA_DISABLE_OPTIMIZATION
void AAnterPaperCharacter::HandleRightMovement(float InAxisValue)
{
    UCharacterMovementComponent* AnterMovement = Cast<UCharacterMovementComponent>(FindComponentByClass<UCharacterMovementComponent>());
    if(AnterMovement != nullptr)
    {
        FVector MovementVectorX = FVector(InAxisValue*AnterGeometron.X,0.0f,0.0f);
        FVector MovementVectorZ = FVector(0.0f,0.0f,InAxisValue*AnterGeometron.Z);
        if(InAxisValue != 0.0f)
        {
            //Input is given by the player

            if((InAxisValue >0.0f && bIsRightUnlocked) || (InAxisValue <0.0f && bIsLeftUnlocked) || (AnterMovement->Velocity.X >0.0f && bIsRightUnlocked) || (AnterMovement->Velocity.X <0.0f && bIsLeftUnlocked))
            {
                if(AnterGeometron.Z == 0.0f)
                {
                    LastVelocityX = AnterMovement->Velocity.X;
                    AddMovementInput(MovementVectorX,MovementMultiplier);
                }
                else
                {
                    AnterMovement->Velocity.X = 0.0f;
                    AnterMovement->Velocity.Z = 0.0f;
                    FVector NewLocation = GetActorLocation() + (MovementVectorX.X + MovementVectorZ.Z)*MovementMultiplier;
                    SetActorLocation(NewLocation);
                    LastVelocityX = MovementVectorX.X;
                }
            }
            else
            {
                AnterMovement->Velocity.X = 0.0f;
                if(AnterGeometron.Z != 0.0f)
                {
                    AnterMovement->Velocity.Z = 0.0f;
                }
            }           
        }

        //30 jan 2022 we need to go on from here

        else
        {
            //The player gives no input: handling braking and slowing down
            if(AnterGeometron.Z == 0.0f)
            {
                AddMovementInput(FVector(-1.0f*AnterMovement->Velocity.X*FrictionScale*AnterGeometron.X,0.0f,0.0f));

                //X braking
                if(abs(AnterMovement->Velocity.X) >= VelocityThreshold)
                {
                    AnterMovement->AddImpulse(FVector(0.0f,0.0f,+1.0f*AnterMovement->Velocity.Z*abs(AnterGeometron.Z)*ZBrake));
                }
                else
                {
                    AnterMovement->Velocity.X = 0.0f; 
                }
            }
            else
            {
                AnterMovement->Velocity.X = 0.0f;
                AnterMovement->Velocity.Z = 0.0f;
                if(LastVelocityX >= VelocityThreshold)
                {
                    FVector NewLocation = GetActorLocation() + (FVector::RightVector.X*AnterGeometron.X + FVector::UpVector.Z*AnterGeometron.Z)*LastVelocityX;
                    SetActorLocation(NewLocation);
                    LastVelocityX /= 2.0f;
                }
            }
        }
    }
}
PRAGMA_ENABLE_OPTIMIZATION

void AAnterPaperCharacter::HandleJump()
{
    UCharacterMovementComponent* AnterMovement = Cast<UCharacterMovementComponent>(FindComponentByClass<UCharacterMovementComponent>());
    if(AnterMovement != nullptr && bCanAnterJump == true)
    {
        FVector JumpVector = FVector(0.0f,0.0f,JumpScale);
        UE_LOG(LogTemp, Warning,TEXT("Jump Impulse is %f"), JumpVector.Z);
        AnterMovement->AddImpulse(JumpVector);
        SetCanJump(false);
    }
}

PRAGMA_DISABLE_OPTIMIZATION
void AAnterPaperCharacter::HandleCollision(const FCollisionGeometry& CollisionGeometry, AActor* OtherActor)
{
    ABasePlatform* Platform = Cast<ABasePlatform>(OtherActor);
    if(Platform != nullptr)
    {   
        //Colliding object was a platform
        HandlePlatform(CollisionGeometry,Platform);
        return;
    }
}   
PRAGMA_ENABLE_OPTIMIZATION

PRAGMA_DISABLE_OPTIMIZATION
void AAnterPaperCharacter::OnColliderUnhit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    ABasePlatform* Platform = Cast<ABasePlatform>(OtherActor);
    if(Platform != nullptr)
    {
        UStaticMeshComponent* OtherMesh = Cast<UStaticMeshComponent>(OtherComp);
        if(OtherMesh != nullptr)
        {
            //First, Deregister platform from colliding platforms
            DeregisterPlatformCollision(Platform);
            UCharacterMovementComponent* AnterMovement = Cast<UCharacterMovementComponent>(FindComponentByClass<UCharacterMovementComponent>());
            if(AnterMovement != nullptr)
            {
                //If there is no vertically colliding platform in the registered platforms array, then free fall
                if(FindAnyCollisionOfType(EPlatformCollisionType::IsVerticallyColliding) == false)
                {
                    SetIsFalling(true);
                    AnterMovement->GravityScale = InputGravityScale;
                    ResetGeometron();
                }
                
                if(FindAnyCollisionOfType(EPlatformCollisionType::IsCollidingFromLeft) == false)
                {
                    SetRightMovementFree(true);                    
                }

                if(FindAnyCollisionOfType(EPlatformCollisionType::IsCollidingFromRight) == false)
                {
                    SetLeftMovementFree(true);
                }
            }
        }
    }
}
PRAGMA_ENABLE_OPTIMIZATION

void AAnterPaperCharacter::SetCanJump(bool InCanJump)
{
    bCanAnterJump = InCanJump;
}

void AAnterPaperCharacter::ConstrainJump()
{
    UCharacterMovementComponent* AnterMovement = Cast<UCharacterMovementComponent>(FindComponentByClass<UCharacterMovementComponent>());
    if(AnterMovement != nullptr)
    {
        if(AnterMovement->Velocity.Z < ZVelocityThresholdToJump)
        {
            SetCanJump(false);
        }
    }
}

void AAnterPaperCharacter::SetIsFalling(bool InIsFalling)
{
    bIsFalling = InIsFalling;
}


PRAGMA_DISABLE_OPTIMIZATION
void AAnterPaperCharacter::RegisterPlatformCollision(AActor* InPlatformToAdd, EPlatformCollisionType InPlatformCollisionType)
{
    auto PlatformIndex = RegisteredVerticalPlatformCollisions.IndexOfByPredicate([InPlatformToAdd](TPair <AActor*,EPlatformCollisionType> PlatformPair)
    {
        return PlatformPair.Key == InPlatformToAdd;
    }
    );
    //Add if the platform is not present already in the array
    if(PlatformIndex == INDEX_NONE)
    {
        TPair<AActor*,EPlatformCollisionType> PlatformToAdd = TPair<AActor*,EPlatformCollisionType>(InPlatformToAdd,InPlatformCollisionType);
        RegisteredVerticalPlatformCollisions.Add(PlatformToAdd);        
    }
}
PRAGMA_ENABLE_OPTIMIZATION

PRAGMA_DISABLE_OPTIMIZATION
void AAnterPaperCharacter::DeregisterPlatformCollision(AActor* InPlatformToRemove)
{
    auto PlatformIndex = RegisteredVerticalPlatformCollisions.IndexOfByPredicate([InPlatformToRemove](TPair <AActor*,EPlatformCollisionType> PlatformPair)
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


bool AAnterPaperCharacter::FindAnyCollisionOfType(EPlatformCollisionType InPlatformCollisionTypeToFind)
{
    TPair <AActor*,EPlatformCollisionType>* PlatformToFind = RegisteredVerticalPlatformCollisions.FindByPredicate([=](TPair <AActor*,EPlatformCollisionType> PlatformPair)
    {
        return PlatformPair.Value == InPlatformCollisionTypeToFind;
    }
    );
    return (PlatformToFind != nullptr);
}

PRAGMA_DISABLE_OPTIMIZATION
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
PRAGMA_ENABLE_OPTIMIZATION

void AAnterPaperCharacter::ImposeGeometry(float InAngle)
{
    FMath::SinCos(&(AnterGeometron.Z),&(AnterGeometron.X),InAngle);
}

void AAnterPaperCharacter::ResetGeometron()
{
    ImposeGeometry(0.0f);
}

void AAnterPaperCharacter::HandlePlatform(const FCollisionGeometry& CollisionGeometry, AActor* Platform)
{
    //if(FVector::DotProduct(CollisionGeometry.TopDist,CollisionGeometry.RotatedNormal)/FMath::Abs(FVector::DotProduct(CollisionGeometry.TopDist,CollisionGeometry.RotatedNormal)) >= VerticalTolerance)
    if(CollisionGeometry.TopDist.Z >= -(AnterSize.Z))
    {
        /* Impact was from top: pawn is standing on platform. */ 

        //Register impact with vertical colliding platform
        RegisterPlatformCollision(Platform,EPlatformCollisionType::IsVerticallyColliding);

        UCharacterMovementComponent* AnterMovement = Cast<UCharacterMovementComponent>(FindComponentByClass<UCharacterMovementComponent>());
        if(AnterMovement != nullptr)
        {
            SetIsFalling(false);
            SetCanJump(true);
            AnterMovement->GravityScale = 0.0f;
        }
        //Floor impenetrability condition
        FVector NewLocation = GetActorLocation();
        //if(AnterBox != nullptr)
        //{
            //FVector AnterBoxExtent = AnterBox->GetScaledBoxExtent();
            NewLocation = FVector(GetActorLocation().X ,GetActorLocation().Y,CollisionGeometry.PlatformSurfaceCentre.Z + AnterSize.Z/2.0f*VerticalImpenetrabilityFactor);// AnterSize.Z*VerticalImpenetrabilityFactor); // AnterSize.Z*VerticalImpenetrabilityFactor);// + AnterSize.Z/2.0f*VerticalImpenetrabilityFactor);
        //}
        SetActorLocation(NewLocation);
    } 
    else
    {
        if(FVector::DotProduct(CollisionGeometry.BottomDist,-CollisionGeometry.RotatedNormal) < VerticalTolerance)
        {
            /* Impact was from side or bottom */
            FVector PlatformRightSideCentre = CollisionGeometry.PlatformCentre + FVector::XAxisVector*CollisionGeometry.PlatformLength;
            FVector PlatformLeftSideCentre = CollisionGeometry.PlatformCentre - FVector::XAxisVector*CollisionGeometry.PlatformLength;
            FVector SideDist = this->GetActorLocation()-CollisionGeometry.PlatformCentre;
            if(SideDist.X > 0.0f)
            {
                //Impact coming from right
                if((FVector::DotProduct(SideDist,FVector::XAxisVector) > HorizontalTolerance))
                {
                    //Impenetrability
                    SetLeftMovementFree(false);
                    RegisterPlatformCollision(Platform,EPlatformCollisionType::IsCollidingFromRight);
                }
            }
            else if(SideDist.X < 0.0f)
            {
                //Impact coming from left
                if((FVector::DotProduct(SideDist,-FVector::XAxisVector) > HorizontalTolerance))
                {
                    //Impenetrability
                    SetRightMovementFree(false);
                    RegisterPlatformCollision(Platform,EPlatformCollisionType::IsCollidingFromLeft);
                }
            }
        }
    }
}