#include "AnterCameras/AnterMenuCameraActor.h"
#include "Pawn/AnterMenuPawn.h"
#include "Kismet/GameplayStatics.h"

AAnterMenuCameraActor::AAnterMenuCameraActor()
:Super()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AAnterMenuCameraActor::BeginPlay()
{
    Super::BeginPlay();
    TArray<AActor*> AnterPawnArray;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAnterMenuPawn::StaticClass(), AnterPawnArray);
    if(AnterPawnArray.Num())
    {
            AnterMenuPawnRef = Cast<AAnterMenuPawn>(AnterPawnArray[0]);
    }
}

void AAnterMenuCameraActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if(AnterMenuPawnRef.IsValid())
    {
        SetActorLocation(AnterMenuPawnRef->GetActorLocation() + FVector(0.0f,0.0f,VerticalHeightFromPawn));
    }
    
}