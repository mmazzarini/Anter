#include "ActorComponents/LevelManagerComponent.h"
#include "SceneUtilities/SceneStructs.h"
#include "AnterGameModes/AnterBaseLevelGameMode.h"
#include "SceneActors/Managers/SceneActorManagerBase.h"
#include "Kismet/GameplayStatics.h"

ULevelManagerComponent::ULevelManagerComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;  
}

void ULevelManagerComponent::BeginPlay() 
{
    Super::BeginPlay();
    SetupLevelElements();
    //BindToLevelRestartPoints();
    //This binds all managers to delegate from game mode
    BindManagers();
}

void ULevelManagerComponent::SetupLevelElements()
{
    OwnerGameMode = Cast<AAnterBaseLevelGameMode>(GetOwner());
    GenerateCheckpoints();
}

void ULevelManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    DeleteBindings();
    Super::EndPlay(EndPlayReason);
}

void ULevelManagerComponent::GenerateCheckpoints()
{
    TArray<AActor*> Checkpoints;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALevelCheckpoint::StaticClass(), Checkpoints);
    for(auto* Checkpoint : Checkpoints)
    {
        if(ALevelCheckpoint* CastedCheckpoint = Cast<ALevelCheckpoint>(Checkpoint))
        {
            CastedCheckpoint->OnActivatedCheckpointDelegate.AddDynamic(this,&ThisClass::OnCheckpointActivated);
            TPair<ALevelCheckpoint*,bool> CheckpointPair(CastedCheckpoint,false);
            LevelCheckpoints.Add(CheckpointPair);       
        }
    }
}

void ULevelManagerComponent::DeleteBindings()
{
    for(const TPair<ALevelCheckpoint*, bool >& Checkpoint : LevelCheckpoints)
    {
        if(Checkpoint.Key != nullptr)
        {
            Checkpoint.Key->OnActivatedCheckpointDelegate.RemoveDynamic(this,&ThisClass::OnCheckpointActivated);
        }
    }
}

void ULevelManagerComponent::OnCheckpointActivated(ALevelCheckpoint* InCheckpoint)
{
    /* Update current checkpoint ptr */
    if(InCheckpoint != nullptr && OwnerGameMode != nullptr)
    {
        APlayerController* CurrentPC;
        TArray<AActor*> CurrentPCs;
        UGameplayStatics::GetAllActorsOfClass(OwnerGameMode->GetWorld(), APlayerController::StaticClass(), CurrentPCs);
        if(CurrentPCs.Num() > 0)
        {
            CurrentPC = Cast<APlayerController>(CurrentPCs[0]);
            if(CurrentPC != nullptr)
            {
                AActor* LevelStart = OwnerGameMode->FindPlayerStart(CurrentPC,AnterPlayerStartString);
                if(LevelStart != nullptr)
                {
                    FVector OldLocation = LevelStart->GetActorLocation();
                    LevelStart->SetActorLocation(InCheckpoint->GetActorLocation() + FVector::UpVector*100.0f);
                    FVector NewLocation = LevelStart->GetActorLocation();
                    FVector DiffLocation = OldLocation-NewLocation;
                }
            }
        }
        CurrentCheckpoint = InCheckpoint;
        OnActivatedOneCheckpointDelegate.Broadcast(InCheckpoint);
    }
}

void ULevelManagerComponent::GetLevelGoalReference()
{
    /*Get the level goal reference*/
    TArray<AActor*> LevelGoalsArray;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), LevelGoalClass, LevelGoalsArray);
    if(LevelGoalsArray.Num() > 0)
    {
        LevelGoal = LevelGoalsArray[0];
    }
}

void ULevelManagerComponent::BindManagers()
{
    TArray<AActor*> ActorsArray;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), ActorsArray);
    for(auto* MyActor : ActorsArray)
    {
        if(ASceneActorManagerBase* MyCastedActor = Cast<ASceneActorManagerBase>(MyActor))
        {
            MyCastedActor->BindActorCreation(this);
        }
    }            
}