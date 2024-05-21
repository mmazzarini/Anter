#include "FSMState/MenuPlaceAnterAndCameraFSMState.h"
#include "Pawn/AnterMenuPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"

void UMenuPlaceAnterAndCameraFSMState::StartState()
{
    Super::StartState();

    CreateAnter();

    PlaceAnter(); 
}

void UMenuPlaceAnterAndCameraFSMState::PlaceAnter()
{
    TArray<AActor*> TargetActors;
    TArray<AActor*> TargetCameraActors;
    AActor* TargetActor = nullptr;
    AActor* TargetCameraActor = nullptr;
    UGameplayStatics::GetAllActorsWithTag(this,AnterTargetMarkerTag,TargetActors);
    UGameplayStatics::GetAllActorsWithTag(this,AnterTargetMarkerTag,TargetCameraActors);
    if(TargetActors.Num() > 0)
    {
        TargetActor = TargetActors[0];
    }
    if(TargetCameraActors.Num() > 0)
    {
        TargetCameraActor = TargetCameraActors[0];
    }

    if(AnterMenuPawnRef.IsValid() && TargetActor != nullptr)
    {
        AnterMenuPawnRef->SetActorLocation(TargetActor->GetActorLocation());
    }

    if(AnterCameraActorRef.IsValid() && TargetCameraActor != nullptr)
    {
        AnterCameraActorRef->SetActorLocation(TargetCameraActor->GetActorLocation());
    }

    if(bShouldActivateCamera)
    {
        if(APlayerController* PC = UGameplayStatics::GetPlayerController(this,0))
        {
            if(AnterCameraActorRef.IsValid())
            {
                PC->SetViewTarget(AnterCameraActorRef.Get());
            }
        }
    }
}

void UMenuPlaceAnterAndCameraFSMState::CreateAnter()
{
    if(!AnterMenuPawnRef.IsValid())
    {
        TArray<AActor*> AnterPawnArray;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAnterMenuPawn::StaticClass(), AnterPawnArray);
        if(AnterPawnArray.Num())
        {
            AnterMenuPawnRef = Cast<AAnterMenuPawn>(AnterPawnArray[0]);
        }
    }  

    if(!AnterCameraActorRef.IsValid())
    {
        TArray<AActor*> AnterCameraArray;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), AnterCameraArray);
        if(AnterCameraArray.Num())
        {
            AnterCameraActorRef = Cast<ACameraActor>(AnterCameraArray[0]);
        }
    }  
}