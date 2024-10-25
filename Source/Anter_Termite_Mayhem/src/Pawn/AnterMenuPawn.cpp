#include "Pawn/AnterMenuPawn.h"
#include "Components/InputComponent.h"
#include "SceneActors/Markers/MenuMapMarker.h"
#include "Kismet/GameplayStatics.h"

AAnterMenuPawn::AAnterMenuPawn()
:
Super()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AAnterMenuPawn::BeginPlay()
{
    Super::BeginPlay();
}

void AAnterMenuPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if(bIsMovingToMarker)
    {
        MoveToNextMarker();
    }
}

void AAnterMenuPawn::SetupPlayerInputComponent(UInputComponent* InInputComponent)
{
    Super::SetupPlayerInputComponent(InInputComponent);
    if(InInputComponent != nullptr)
    {
        InInputComponent->BindAction("MapDown",IE_Pressed,this,&ThisClass::HandleMoveDown);
        InInputComponent->BindAction("MapUp",IE_Pressed,this,&ThisClass::HandleMoveUp);
        InInputComponent->BindAction("MapLeft",IE_Pressed,this,&ThisClass::HandleMoveLeft);
        InInputComponent->BindAction("MapRight",IE_Pressed,this,&ThisClass::HandleMoveRight);
        InInputComponent->BindAction("MapSelectLevel",IE_Pressed,this,&ThisClass::HandleLevelSelected);
    }
}

void AAnterMenuPawn::MoveToNextMarker()
{
    if(MarkerRef.IsValid())
    {
        FVector DistToMarker = MarkerRef->GetActorLocation() - GetActorLocation();
        FVector DistToMarkerXY = FVector(DistToMarker.X, DistToMarker.Y, 0.0f);
        if(DistToMarkerXY.Size() < MarkerDistanceThreshold)
        {
            bIsMovingToMarker = false;
            SetActorLocation(MarkerRef->GetActorLocation() + FVector(0.0f,0.0f,100.f));

            OnMapLevelHit.Broadcast(CurrentMarkerTag);
        }
        else
        {
            SetActorLocation(GetActorLocation() + DistToMarkerXY/DistToMarkerXY.Size()*NavigationSpeed);
        }
    }
}

void AAnterMenuPawn::HandleMoveDown()
{
    if(!bIsMovingToMarker)
    {
        if(InternalNavigationMap.Contains("Down"))
        {
            NavigateToNextMarker(InternalNavigationMap["Down"]);
        }    
    }
}

void AAnterMenuPawn::HandleMoveUp()
{
    if(!bIsMovingToMarker)
    {
        if(InternalNavigationMap.Contains("Up"))
        {
            NavigateToNextMarker(InternalNavigationMap["Up"]); 
        }
    }
}

void AAnterMenuPawn::HandleMoveLeft()
{
    if(!bIsMovingToMarker)
    {
        if(InternalNavigationMap.Contains("Left"))
        {
            NavigateToNextMarker(InternalNavigationMap["Left"]);  
        }
    }   
}

void AAnterMenuPawn::HandleMoveRight()
{
    if(!bIsMovingToMarker)
    {
        if(InternalNavigationMap.Contains("Right"))
        {
            NavigateToNextMarker(InternalNavigationMap["Right"]);
        }
    }
}

void AAnterMenuPawn::HandleLevelSelected()
{
    //Travel to level
    if(MarkerRef.IsValid())
    {
        if(AMenuMapMarker* CastedMapMarker = Cast<AMenuMapMarker>(MarkerRef.Get()))
        {
            if (CastedMapMarker->Tags.Num() > 0)
            {
                OnMapLevelSelected.Broadcast(CastedMapMarker->Tags[0].ToString());
            }
            if(!bIsMovingToMarker && !(CastedMapMarker->GetLevelPath().Equals("")))
            {
                GetWorld()->ServerTravel(CastedMapMarker->GetLevelPath());
            }
        }
    }
}

void AAnterMenuPawn::NavigateToNextMarker(const FString& InStartingMarkerTag)
{
    CurrentMarkerTag = InStartingMarkerTag;
    MarkerRef = RetrieveTargetMarker(InStartingMarkerTag);
    if(MarkerRef != nullptr)
    {
        if(AMenuMapMarker* CastedMapMarker = Cast<AMenuMapMarker>(MarkerRef.Get()))
        {
            InternalNavigationMap.Empty();
            InternalNavigationMap = CastedMapMarker->GetMenuNavigation();
        }
        bIsMovingToMarker = true;
    }
}

AActor* AAnterMenuPawn::RetrieveTargetMarker(const FString& InStartingMarkerTag)
{
    TArray<AActor*> TargetMapMarkerActors;
    AActor* MarkerActor = nullptr;
    UGameplayStatics::GetAllActorsWithTag(this,FName(InStartingMarkerTag),TargetMapMarkerActors);
    if(TargetMapMarkerActors.Num() > 0)
    {
        MarkerActor = TargetMapMarkerActors[0];
    }
    return MarkerActor;
}