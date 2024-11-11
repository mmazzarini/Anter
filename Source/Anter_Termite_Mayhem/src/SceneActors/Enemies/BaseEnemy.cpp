#include "SceneActors/Enemies/BaseEnemy.h"
#include "SceneActors/Platforms/BasePlatform.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "SceneActors/Enemies/BaseEnemyBoundary.h"
#include "SceneActors/AnterFire.h"
#include "ActorComponents/BaseEnemyMovementComponent.h"
#include "ActorComponents/DamageComponent.h"
#include "GameSpecificStaticLibrary/GameSpecificStaticLibrary.h"

ABaseEnemy::ABaseEnemy()
{
    BaseEnemyMovement = CreateDefaultSubobject<UBaseEnemyMovementComponent>(TEXT("BaseEnemyMovement"));
    BaseEnemyMovement->SetupAttachment(RootComponent);
    
    BaseEnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseEnemyMesh"));
    BaseEnemyMesh->SetupAttachment(RootComponent);

    BaseEnemyHealth = CreateDefaultSubobject<UHealthComponent>(TEXT("BaseEnemyHealth"));
    BaseEnemyHealth->SetupAttachment(RootComponent);

    BaseEnemyBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BaseEnemyBox"));
    BaseEnemyBox->SetupAttachment(RootComponent);

    BaseEnemyCollisionSupport = CreateDefaultSubobject<UCollisionSupportComponent>(TEXT("BaseEnemyCollisionSupport"));
    BaseEnemyCollisionSupport->SetupAttachment(RootComponent);

    /*
    if(bCanShoot)
    {
        BaseEnemyWeaponComponent = CreateDefaultSubobject<UAnterWeaponComponent>(TEXT("BaseEnemyCollisionSupport"));
    }
    */


    PivotState = EEnemyPivotState::IsWaitingToBeFilled;
}

void ABaseEnemy::UpdateAttack()
{
    
}

void ABaseEnemy::UpdateMovement()
{
    if(PivotState == EEnemyPivotState::HasBeenFilled)
    {
        StartToMove();
    }

    if(ensure(CurrentPivotPositions.Num()))
    {
        FVector CurrentPositionToReach = CurrentPivotPositions[PivotArrayIndex];
        float EnemyPivotDist = FVector::Dist(GetActorLocation(),CurrentPositionToReach);
        if(EnemyPivotDist < PivotDistanceThreshold)
        {
            MoveToNextPivot();
        }
    }
}

void ABaseEnemy::BeginPlay()
{
    Super::BeginPlay();
    if(BaseEnemyMovement != nullptr)
    {
        BaseEnemyMovement->Initialize();
    }

    SetBindings();

    if(BaseEnemyCollisionSupport != nullptr)
    {
        BaseEnemyCollisionSupport->RegisterInterfaceOwnerCharacter(this);
    }
    //We set this at the beginning
    CurrentPivotPositions = PivotPositions;

    ResetGravity();
}

void ABaseEnemy::Tick(float DeltaSeconds)
{
    UpdateMovement();
}

void ABaseEnemy::StartToMove()
{
    PivotState = EEnemyPivotState::HasStartedMoving; 
    MoveToNextPivot();
}

void ABaseEnemy::MoveToNextPivot()
{
    //We first switch orientation if necessary, then we move to next pivot by updating the velocity.
    if(PivotArrayIndex == GetCurrentPivotPositionsArray().Num()-1)
    {
        SwitchOrientation();
    }

    PivotArrayIndex++;
    AdjustVelocity();
}

void ABaseEnemy::SwitchOrientation()
{
    if(LoopBehavior == EEnemyLoopBehavior::GoesBackward)
    {
        CurrentPivotPositions = (( CurrentPivotPositions == PivotPositions ) ? ReversePivotPositions : PivotPositions);
    }
    PivotArrayIndex = 0;
}

void ABaseEnemy::HandleCollision(const FCollisionGeometry& InCollisionGeometry, AActor* OtherActor) 
{
    if(UGameSpecificStaticLibrary::IsHealthDamageType(this,OtherActor))
    {
        HandleDamage(OtherActor);
    }

    if (ABasePlatform* CurrentPlatform = Cast<ABasePlatform>(OtherActor))
    {
        FVector EnemyCentre;
        FVector EnemySize;
        GetActorBounds(true, EnemyCentre, EnemySize, false);
        if (BaseEnemyMesh != nullptr)
        {
            if (InCollisionGeometry.TopDist.Z >= -(EnemySize.Z))
            {
                NullifyGravity();
                SetActorLocation(InCollisionGeometry.PlatformSurfaceCentre + EnemySize.Z / 2.f);
            }
        }
    }
}
   
void ABaseEnemy::SetBindings()
{ 
    if(BaseEnemyMesh !=nullptr && BaseEnemyCollisionSupport != nullptr)
    {
        BaseEnemyMesh->OnComponentBeginOverlap.AddDynamic(BaseEnemyCollisionSupport,&UCollisionSupportComponent::ProcessCollisionGeometry);
    }
}


void ABaseEnemy::FillPositionArrays(TArray<FVector> InPositions)
{
    PivotPositions.Empty();
    ReversePivotPositions.Empty();
        
    if(InPositions.Num() != 0)
    {
        for(FVector InPosition : InPositions)
        {
            PivotPositions.Add(InPosition);
        }
        CurrentPivotPositions = PivotPositions;
        //reverse loop
        for(int32 PositionIndex = InPositions.Num()-1; PositionIndex >= 0; PositionIndex--)
        {
            ReversePivotPositions.Add(InPositions[PositionIndex]);
        }
    }
    PivotState = EEnemyPivotState::HasBeenFilled;
}

void ABaseEnemy::AdjustVelocity()
{
    FVector CurrentLocation = GetActorLocation();
    FVector LocationDistance  = FVector(0.0f,0.0f,0.0f);
    if(ensure(CurrentPivotPositions.Num()))
    {
        FVector NewLocation = CurrentPivotPositions[PivotArrayIndex];
        LocationDistance = NewLocation - CurrentLocation;
    }
    
    // Normalize distance vector
    if(LocationDistance.Size() != 0 && BaseEnemyMovement != nullptr)
    {
        FVector NewGeometry;
        NewGeometry.X = LocationDistance.X/LocationDistance.Size();
        NewGeometry.Y = LocationDistance.Z/LocationDistance.Size();
        FVector2D New2DGeometry(NewGeometry.X,NewGeometry.Y);
        BaseEnemyMovement->SetMovement(New2DGeometry);
    }
}

void ABaseEnemy::SetLoopBehavior(EEnemyLoopBehavior InLoopBehavior)
{
    LoopBehavior = InLoopBehavior;
}

void ABaseEnemy::ResetMovement()
{
    PivotArrayIndex = 0;
    if(ensure(CurrentPivotPositions.Num()))
    {
        SetActorLocation(CurrentPivotPositions[0]);
        StartToMove();
    }
}

void ABaseEnemy::ResetGravity()
{
    if (bShouldHaveGravity)
    {
        if (UCharacterMovementComponent* EnemyMovement = Cast<UCharacterMovementComponent>(GetComponentByClass(UCharacterMovementComponent::StaticClass())))
        {
            EnemyMovement->GravityScale = EnemyGravityScale;
        }
    }
}

void ABaseEnemy::NullifyGravity()
{
    if (bShouldHaveGravity)
    {
        if (UCharacterMovementComponent* EnemyMovement = Cast<UCharacterMovementComponent>(GetComponentByClass(UCharacterMovementComponent::StaticClass())))
        {
            EnemyMovement->GravityScale = 0.0f;
            EnemyMovement->Velocity.Z = 0.0f;
        }
    }
    OnGravityNullified.Broadcast();
}

void ABaseEnemy::HandleDamage(AActor* InDamagingActor)
{
    UDamageComponent* Damage = (InDamagingActor != nullptr) ? Cast<UDamageComponent>(InDamagingActor->FindComponentByClass(UDamageComponent::StaticClass())) : nullptr;
    if(BaseEnemyHealth != nullptr && Damage != nullptr)
    {
        BaseEnemyHealth->IncreaseHealth(Damage->GetDamageValue());
        AAnterFire* AnterFire = Cast<AAnterFire>(InDamagingActor);
        if(AnterFire != nullptr)
        {  
            AnterFire->Destroy();
        }
    }
}