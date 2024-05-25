#include "FSMState/StartMenuMapNavigationFSMState.h"
#include "Pawn/AnterMenuPawn.h"
#include "Kismet/GameplayStatics.h"

void UStartMenuMapNavigationFSMState::StartState()
{
    Super::StartState();
    StartMenuMapNavigation();
}

void UStartMenuMapNavigationFSMState::StartMenuMapNavigation()
{
    AAnterMenuPawn* AnterPawn = nullptr;
    TArray<AActor*> AnterPawnArray;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAnterMenuPawn::StaticClass(), AnterPawnArray);
    if(AnterPawnArray.Num())
    {
        AnterPawn = Cast<AAnterMenuPawn>(AnterPawnArray[0]);
    }
    if(AnterPawn != nullptr)
    {
        AnterPawn->NavigateToNextMarker(StartingLevelLabel);
    }
}