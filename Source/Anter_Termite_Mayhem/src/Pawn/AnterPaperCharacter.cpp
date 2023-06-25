#include "Pawn/AnterPaperCharacter.h"
#include "GameFramework/Character.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ActorComponents/AnterMovementSupportComponent.h"
#include "ActorComponents/CollisionSupportComponent.h"
#include "ActorComponents/DamageComponent.h"
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
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerState.h"
#include "SceneActors/Enemies/BaseEnemy.h"
#include "Materials/MaterialInterface.h"
#include "GameSpecificStaticLibrary/GameSpecificStaticLibrary.h"

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

    AnterFloorHanging = CreateDefaultSubobject<UAnterFloorHangingComponent>(TEXT("AnterFloorHanging"));
    AnterFloorHanging->SetupAttachment(RootComponent);

} 

void AAnterPaperCharacter::Tick(float DeltaTime)
{
    AdjustVelocity();
    //Ensure no displacement from Y = 0 plane
    FVector LocationTemp = GetActorLocation();
    SetActorLocation(FVector(GetActorLocation().X,0.0f,GetActorLocation().Z));
    ConstrainJump();
}

void AAnterPaperCharacter::OnDeathEvent()
{
    if(AnterHealth != nullptr)
    {
        AnterHealth->OnDeathReached.RemoveDynamic(this,&AAnterPaperCharacter::OnDeathEvent);
        AnterHit.RemoveDynamic(AnterHealth,&UHealthComponent::IncreaseHealth);
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
    RegisteredVerticalPlatformCollisions.Empty();
    Destroy();
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

    if(AnterCollisionSupport != nullptr)
    {
        AnterCollisionSupport->RegisterInterfaceOwnerCharacter(this);
    }

    //Anter centre is not really revelevant at BeginPlay, but AnterSize is. 
    GetActorBounds(true,AnterCentre,AnterSize,false);

    if(AnterWeapon != nullptr)
    {
        AnterWeapon->SetLaserDirection(FVector(1.0f,0.0f,0.0f));
    }

    if(AnterMesh != nullptr)
    {
        TArray<UMaterialInterface*> Materials;
        AnterMesh->GetUsedMaterials(Materials,false);
        DefaultMaterial = ( Materials.Num() > 0 ? Materials[0] : nullptr );
    }
}

void AAnterPaperCharacter::SetBindings()
{
    if(AnterHealth != nullptr)
    {
        AnterHealth->OnDeathReached.AddDynamic(this,&AAnterPaperCharacter::OnDeathEvent);
        AnterHit.AddDynamic(AnterHealth,&UHealthComponent::IncreaseHealth);
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
            //InInputComponent->BindAxis("RightMovement",AnterWeapon,&UAnterWeaponComponent::OnOwnerMoving);
            InInputComponent->BindAction("Fire",IE_Pressed, AnterWeapon, &UAnterWeaponComponent::ShootLaser);   
        }

        if(AnterFloorHanging != nullptr)
        {
            InInputComponent->BindAction("FloorHanging",IE_Pressed,AnterFloorHanging,&UAnterFloorHangingComponent::AttemptRayCastByLineTrace);
            InInputComponent->BindAction("FloorHanging",IE_Released,AnterFloorHanging,&UAnterFloorHangingComponent::DetachFromUpsideDownPlatform);
        }
    }

}

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

            /* Update Weapon component */
            if(InAxisValue != 0.0f)
            {
                UpdateWeaponDirection(InAxisValue);
            }
        }

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

void AAnterPaperCharacter::HandleJump()
{
    UCharacterMovementComponent* AnterMovement = Cast<UCharacterMovementComponent>(FindComponentByClass<UCharacterMovementComponent>());
    if(AnterMovement != nullptr)
    {
        if(bCanAnterJump == true)
        {
            ProcessJump(JumpScale, AnterMovement);
        }
    }
}

void AAnterPaperCharacter::HandleCollision(const FCollisionGeometry& CollisionGeometry, AActor* OtherActor)
{
    UDamageComponent* PotentialDamage = Cast<UDamageComponent>(OtherActor->FindComponentByClass<UDamageComponent>());
    if(UGameSpecificStaticLibrary::IsHealthDamageType(this,OtherActor))
    {
        //First check: Colliding object was an enemy/dangerous obstacle/laser
        HandleDamage(OtherActor);
        //If dangerous actor was a laser, destroy it
        if(AAnterFire* OtherFire = Cast<AAnterFire>(OtherActor))
        {
            OtherFire->Destroy();
        }
        return;
    }
    else
    {
        //Then we check if it is platform
        ABasePlatform* Platform = Cast<ABasePlatform>(OtherActor);
        if(Platform != nullptr)
        {     
            //Colliding object was a platform
            HandlePlatform(CollisionGeometry,Platform);
            return;
        }
    }
}   

void AAnterPaperCharacter::UpdateWeaponDirection(float InLaserDirection)
{
    if(AnterWeapon != nullptr)
    {
        FVector LaserDirection = FVector(InLaserDirection > 0.0f ? 1.0f : - 1.0f, 0.0f, 0.0f);
        AnterWeapon->SetLaserDirection(LaserDirection);
    }
}

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

bool AAnterPaperCharacter::FindAnyCollisionOfType(EPlatformCollisionType InPlatformCollisionTypeToFind)
{
    TPair <AActor*,EPlatformCollisionType>* PlatformToFind = RegisteredVerticalPlatformCollisions.FindByPredicate([=](TPair <AActor*,EPlatformCollisionType> PlatformPair)
    {
        return PlatformPair.Value == InPlatformCollisionTypeToFind;
    }
    );
    return (PlatformToFind != nullptr);
}

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

void AAnterPaperCharacter::HandleDamage(AActor* InDamagingActor) 
{
    UCharacterMovementComponent* AnterMovement = Cast<UCharacterMovementComponent>(FindComponentByClass<UCharacterMovementComponent>());
    //Base functionality: jump vertically and for a few deactivate hittability.
    UDamageComponent* EnemyDamage = (InDamagingActor != nullptr) ? Cast<UDamageComponent>(InDamagingActor->FindComponentByClass<UDamageComponent>()) : nullptr;
    if(AnterHitStatus == EAnterHitableStatus::CanBeHit && AnterMovement != nullptr && EnemyDamage != nullptr)
    {
        //Set timer for a few secs invincibility
        GetWorldTimerManager().SetTimer(UnhittableTimerHandle, this, &AAnterPaperCharacter::OnUnhittableTimerEnded, UnhittableTimerDuration, false, UnhittableTimerDuration);
        AnterHitStatus = EAnterHitableStatus::CannotBeHit;
        //Get some extra kick to the pawn in the direction opposite to the line connecting pawn and enemy
        FVector KickToReceive = GetActorLocation()-InDamagingActor->GetActorLocation();
        //Normalize Kick to unity
        KickToReceive /= KickToReceive.Size();
        bool bIsAnterDescending = AnterMovement->Velocity.Z < 0.0f;
        //Revert z-kick if it is negative, to avoid penetrating terrains if kicked
        KickToReceive.Z *= ((KickToReceive.Z < 0.0f) ? -1.0f : 1.0f);
        //Give different kick multiplier depending on original vertical movement (higher when descending to contrast vertical negative momentum)
        KickToReceive *= (bIsAnterDescending ? DescendingJumpScaleMultiplier : AscendingJumpScaleMultiplier);
        //Add extra z kick to avoid problems with jump
        KickToReceive.Z += KickVerticalScaleAddition;
        HandleKick(KickToReceive, AnterMovement);
        if(GEngine != nullptr)
        {
            GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,TEXT("Anter Has been hit!"));
        }
        UE_LOG(LogTemp, Warning,TEXT("Anter Has been hit!"));
        if(AnterMesh != nullptr && FlickeringMaterial != nullptr)
        {
            AnterMesh->SetMaterial(0, FlickeringMaterial);
        }

        //Update health now
        AnterHit.Broadcast(EnemyDamage->GetDamageValue());
    }
}

// For debug purposes TODO delete
void AAnterPaperCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);
    APlayerState* DebugPlayerState = Controller->PlayerState;
    if(DebugPlayerState != nullptr)
    {
        uint64 MyPlayerAddress = (uint64)DebugPlayerState;
        DebugPlayerState->StartTime = 0.0f;
    }
}

void AAnterPaperCharacter::OnUnhittableTimerEnded()
{
    AnterHitStatus = EAnterHitableStatus::CanBeHit;
    if(GEngine != nullptr)
    {
        GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,TEXT("Anter Can be hit again!"));
    }
    if(AnterMesh != nullptr && DefaultMaterial != nullptr)
    {
        AnterMesh->SetMaterial(0, DefaultMaterial);
    }

};

void AAnterPaperCharacter::ProcessJump(float InJumpValue, UCharacterMovementComponent* InAnterMovement)
{
    FVector JumpVector = FVector(0.0f,0.0f,InJumpValue);
    //We kick the pawn on the vertical direction with a JumpVector and using its movement component
    if(InAnterMovement != nullptr)
    {
        InAnterMovement->AddImpulse(JumpVector);
    }
    SetCanJump(false);
}

void AAnterPaperCharacter::HandleKick(FVector InKickToReceive, UCharacterMovementComponent* InAnterMovement)
{
    if(InAnterMovement != nullptr)
    {
        InAnterMovement->AddImpulse(InKickToReceive);
        //FVector JumpVector = FVector(0.0f,0.0f,JumpScale*JumpScaleMultiplier);
        //AnterMovement->AddImpulse(JumpVector);
        //SetCanJump(false);
        SetCanJump(false);
    }
}