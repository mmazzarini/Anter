#include "SceneActors/Enemies/BaseEnemy.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "SceneActors/Enemies/BaseEnemyBoundary.h"

#include "ActorComponents/BaseEnemyMovementComponent.h"

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
    
    PivotState = EEnemyPivotState::IsWaitingToBeFilled;
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
    
    if(PivotState == EEnemyPivotState::HasBeenFilled)
    {
        StartToMove();
    }
    /*
    AAIController* EnemyController = Cast<AAIController>(GetController());
    if(EnemyController != nullptr)
    {
        EPathFollowingRequestResult::Type EnemyMovingStatus = EnemyController->MoveToLocation(CurrentPivotPositions[PivotArrayIndex],100.0f);
        if(EnemyMovingStatus == EPathFollowingRequestResult::Type::AlreadyAtGoal)    
        {
                MoveToNextPivot();
        }
    }
    */
    FVector CurrentPositionToReach = CurrentPivotPositions[PivotArrayIndex];
    float EnemyPivotDist = FVector::Dist(GetActorLocation(),CurrentPositionToReach);
    if(CurrentPivotPositions.Num()>0 && EnemyPivotDist < 50.0f)
    {
        MoveToNextPivot();
    }
    //UpdateMovement();
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
    CurrentPivotPositions = (( CurrentPivotPositions == PivotPositions ) ? ReversePivotPositions : PivotPositions);
    PivotArrayIndex = 0;
}

void ABaseEnemy::HandleCollision(const FCollisionGeometry& InCollisionGeometry, AActor* OtherActor) 
{
    ABaseEnemyBoundary* EnemyBoundary = Cast<ABaseEnemyBoundary>(OtherActor);
    if(EnemyBoundary != nullptr)
    {
        if(RootComponent != nullptr)
        {
            RootComponent->ComponentVelocity.Set(0.0f,0.0f,0.0f);
        }
        MoveToNextPivot();
    }
}

void ABaseEnemy::SetBindings()
{ 
    if(BaseEnemyBox != nullptr && BaseEnemyMovement != nullptr)
    {
        BaseEnemyBox->OnComponentBeginOverlap.AddDynamic(BaseEnemyMovement,&UBaseEnemyMovementComponent::OnCollided);
    }
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

/*
void ABaseEnemy::HandleMovement(FVector2D InMovementDirection, float InMovementSpeed)
{
    FVector InputMovement3D = FVector(InMovementDirection.X,InMovementDirection.Y,0.0f);
    AddMovementInput(InputMovement3D,InMovementSpeed);
}
*/