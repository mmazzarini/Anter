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
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/PrimitiveComponent.h"

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
    ConstrainJump();
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
    ResetGeometron();
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
        FVector MovementVectorX = FVector(InAxisValue*AnterGeometron.X,0.0f,0.0f);
        FVector MovementVectorZ = FVector(0.0f,0.0f,InAxisValue*AnterGeometron.Z);
        if(InAxisValue != 0.0f)
        {
        //Input is given by the player
            if((InAxisValue >0.0f && bIsRightUnlocked) || (InAxisValue <0.0f && bIsLeftUnlocked) || (AnterMovement->Velocity.X >0.0f && bIsRightUnlocked) || (AnterMovement->Velocity.X <0.0f && bIsLeftUnlocked))
            {
                AddMovementInput(MovementVectorX,MovementMultiplier);
                //AddMovementInput(MovementVectorZ,ZMultiplier);
                if((FMath::Asin(AnterGeometron.Z) >= 0.0f && InAxisValue >0.0f) || (FMath::Asin(AnterGeometron.Z) < 0.0f && InAxisValue <0.0f))
                {
                    AnterMovement->AddImpulse(FVector(0.0f,0.0f,abs(MovementVectorZ.Z))*ZAscendingMultiplier);
                }
                else
                {
                    AnterMovement->AddImpulse(FVector(0.0f,0.0f,-abs(MovementVectorZ.Z))*ZDiscendingMultiplier);
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
        else
        {
        //The player gives no input: handling braking and slowing down

            if(abs(AnterMovement->Velocity.X) >= VelocityThreshold)
            {
                AddMovementInput(FVector(-1.0f*AnterMovement->Velocity.X*FrictionScale*AnterGeometron.X,0.0f,0.0f));
                if(AnterGeometron.Z != 0.0f)
                {
                    if((FMath::Asin(AnterGeometron.Z) >= 0.0f && AnterMovement->Velocity.X >0.0f) || (FMath::Asin(AnterGeometron.Z) < 0.0f && AnterMovement->Velocity.X <0.0f))
                    {
                        AnterMovement->AddImpulse(FVector(0.0f,0.0f,+1.0f*AnterMovement->Velocity.X*abs(AnterGeometron.Z)*ZBrake));  
                    }
                    else
                    {
                        AnterMovement->AddImpulse(FVector(0.0f,0.0f,+1.0f*AnterMovement->Velocity.X*abs(AnterGeometron.Z)*ZBrake));  
                    }
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


void AAnterPaperCharacter::OnColliderHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    if(OtherActor != nullptr)
    {
        if(OtherActor->GetName().Contains("Floor"))
        {

            FVector PlatformCentre = FVector(0.0f,0.0f,0.0f);
            FVector PlatformSize = FVector(0.0f,0.0f,0.0f);
            OtherActor->GetActorBounds(true,PlatformCentre,PlatformSize,false);
            
            UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(OtherActor->FindComponentByClass<UStaticMeshComponent>());
            float PlatformLength = 0.0f;
            float PlatformHeight = 0.0f;

            if(MeshComponent != nullptr)
            {
                UStaticMesh* StaticMesh = MeshComponent->GetStaticMesh();
                if(StaticMesh != nullptr)
                {
                    FVector IntrinsicSize = StaticMesh->GetBounds().GetBox().GetExtent();
                    FVector PlatformScale = OtherActor->GetTransform().GetScale3D();
                    PlatformLength = IntrinsicSize.X*PlatformScale.X;   
                    PlatformHeight =  IntrinsicSize.Z*PlatformScale.Z;    
                }
            }

            FVector PlatformSurfaceCentre = PlatformCentre + OtherActor->GetActorRotation().RotateVector(FVector::UpVector*PlatformHeight); 
            FVector PlatformBottomCentre =  PlatformCentre - OtherActor->GetActorRotation().RotateVector(FVector::UpVector*PlatformHeight);
            float PlatformAngle = OtherActor->GetTransform().GetRotation().GetAngle();
            float PlatformCosine = 1.0f;
            float PlatformSine = 0.0f;
            FMath::SinCos(&PlatformSine,&PlatformCosine,PlatformAngle);
            //Pawn geometrical references
            FVector AnterSize = FVector(0.0f,0.0f,0.0f);
            FVector AnterCentre = FVector(0.0f,0.0f,0.0f);
            this->GetActorBounds(true,AnterCentre,AnterSize,false);
            FVector TopDist = this->GetActorLocation()-PlatformSurfaceCentre;
            FVector BottomDist = this->GetActorLocation()-PlatformBottomCentre;
            //Check geometry of collision to decide which impact     
            FVector RotatedNormal = OtherActor->GetTransform().GetRotation().RotateVector(FVector::UpVector);
            GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,TEXT("Platform angle: ") +  FString::SanitizeFloat(PlatformAngle));
            if(FVector::DotProduct(TopDist,RotatedNormal) >= VerticalTolerance)
            {
                /* Impact was from top: pawn is standing on platform. */ 

                //Register impact with vertical colliding platform
                RegisterPlatformCollision(OtherActor,EPlatformCollisionType::IsVeritcallyColliding);

                UCharacterMovementComponent* AnterMovement = Cast<UCharacterMovementComponent>(FindComponentByClass<UCharacterMovementComponent>());
                if(AnterMovement != nullptr)
                {
                    SetIsFalling(false);
                    SetCanJump(true);
                    AnterMovement->GravityScale = 0.0f;
                    ImposeGeometry(PlatformAngle);
                }
                //Floor impenetrability condition
                float ProjectedPlatformZ = PlatformCentre.Z + UKismetMathLibrary::DegTan(PlatformAngle/UKismetMathLibrary::GetPI()*180.)*(GetActorLocation().X - PlatformCentre.X);

                FVector NewLocation = FVector(GetActorLocation().X ,GetActorLocation().Y,ProjectedPlatformZ + AnterSize.Z/VerticalImpenetrabilityFactor + PlatformHeight/PlatformCosine);
                SetActorLocation(NewLocation);
                GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,TEXT("Platform centre Z: ") + FString::SanitizeFloat(PlatformCentre.Z));
                GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,TEXT("Platform proj Z: ") + FString::SanitizeFloat(ProjectedPlatformZ));
            } 
            else
            {
                if(FVector::DotProduct(BottomDist,-RotatedNormal) < VerticalTolerance)
                {

                    /* Impact was from side or bottom */
                    FVector PlatformRightSideCentre = PlatformCentre + FVector::XAxisVector*PlatformLength;
                    FVector PlatformLeftSideCentre = PlatformCentre - FVector::XAxisVector*PlatformLength;
                    FVector SideDist = this->GetActorLocation()-PlatformCentre;
                    if(SideDist.X > 0.0f)
                    {
                        //Impact coming from right
                        if((FVector::DotProduct(SideDist,FVector::XAxisVector) > HorizontalTolerance))
                        {
                            //Impenetrability
                            SetLeftMovementFree(false);
                            RegisterPlatformCollision(OtherActor,EPlatformCollisionType::IsCollidingFromRight);
                        }
                    }
                    else if(SideDist.X < 0.0f)
                    {
                        //Impact coming from left
                        if((FVector::DotProduct(SideDist,-FVector::XAxisVector) > HorizontalTolerance))
                        {
                            //Impenetrability
                            SetRightMovementFree(false);
                            RegisterPlatformCollision(OtherActor,EPlatformCollisionType::IsCollidingFromLeft);
                        }
                    }
                }
            }
        }
    }
}   

void AAnterPaperCharacter::OnColliderUnhit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if(OtherActor != nullptr)
    {
        if(OtherActor->GetName().Contains("Floor"))
        {

            //First, Deregister platform from colliding platforms
            DeregisterPlatformCollision(OtherActor);

            UCharacterMovementComponent* AnterMovement = Cast<UCharacterMovementComponent>(FindComponentByClass<UCharacterMovementComponent>());
            if(AnterMovement != nullptr)
            {
                //If there is no vertically colliding platform in the registered platforms array, then free fall
                if(FindAnyCollisionOfType(EPlatformCollisionType::IsVeritcallyColliding) == false)
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
    TPair<AActor*,EPlatformCollisionType> PlatformToAdd = TPair<AActor*,EPlatformCollisionType>(InPlatformToAdd,InPlatformCollisionType);
    RegisteredVerticalPlatformCollisions.Add(PlatformToAdd);
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

PRAGMA_DISABLE_OPTIMIZATION
bool AAnterPaperCharacter::FindAnyCollisionOfType(EPlatformCollisionType InPlatformCollisionTypeToFind)
{
    TPair <AActor*,EPlatformCollisionType>* PlatformToFind = RegisteredVerticalPlatformCollisions.FindByPredicate([=](TPair <AActor*,EPlatformCollisionType> PlatformPair)
    {
        return PlatformPair.Value == InPlatformCollisionTypeToFind;
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

void AAnterPaperCharacter::ImposeGeometry(float InAngle)
{
    FMath::SinCos(&(AnterGeometron.Z),&(AnterGeometron.X),InAngle);
}

void AAnterPaperCharacter::ResetGeometron()
{
    ImposeGeometry(0.0f);
}
