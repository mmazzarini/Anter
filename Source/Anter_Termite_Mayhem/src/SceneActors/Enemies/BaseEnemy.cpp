#include "SceneActors/Enemies/BaseEnemy.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "SceneActors/Enemies/BaseEnemyBoundary.h"
#include "SceneActors/AnterFire.h"
#include "ActorComponents/BaseEnemyMovementComponent.h"
#include "ActorComponents/DamageComponent.h"

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

    FVector CurrentPositionToReach = CurrentPivotPositions[PivotArrayIndex];
    float EnemyPivotDist = FVector::Dist(GetActorLocation(),CurrentPositionToReach);
    if(CurrentPivotPositions.Num()>0 && EnemyPivotDist < PivotDistanceThreshold)
    {
        MoveToNextPivot();
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
    AAnterFire* AnterFire = Cast<AAnterFire>(OtherActor);
    if(AnterFire != nullptr)
    {
        UDamageComponent* FireDamage = Cast<UDamageComponent>(OtherActor->FindComponentByClass(UDamageComponent::StaticClass()));
        if(FireDamage != nullptr && FireDamage->IsPawnDamage() && BaseEnemyHealth != nullptr)
        {
            BaseEnemyHealth->IncreaseHealth(FireDamage->GetDamageValue());
            AnterFire->Destroy();
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
            CurrentPivotPositions.Add(InPosition);
        }
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
    FVector NewLocation = CurrentPivotPositions[PivotArrayIndex];
    FVector LocationDistance = NewLocation - CurrentLocation;
    
    // Normalize distance vector
    if(LocationDistance.Size() != 0 && BaseEnemyMovement != nullptr)
    {
        FVector NewGeometry;
        NewGeometry.X = LocationDistance.X/LocationDistance.Size();
        NewGeometry.Y = LocationDistance.Z/LocationDistance.Size();
        FVector2D New2DGeometry(NewGeometry.X,NewGeometry.Y);
        /*
        //Fix movement reset
        BaseEnemyMovement->Velocity.X = 0.0f;
        BaseEnemyMovement->Velocity.Y = 0.0f;
        BaseEnemyMovement->Velocity.Z = 0.0f;
        */
        BaseEnemyMovement->SetMovement(New2DGeometry);
    }
}

void ABaseEnemy::SetLoopBehavior(EEnemyLoopBehavior InLoopBehavior)
{
    LoopBehavior = InLoopBehavior;
}