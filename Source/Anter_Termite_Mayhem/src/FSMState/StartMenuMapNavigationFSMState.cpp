#include "FSMState/StartMenuMapNavigationFSMState.h"
#include "Kismet/GameplayStatics.h"
#include "Pawn/AnterMenuPawn.h"
#include "SceneActors/Managers/LevelInfoManager.h"
#include "UI/AnterWidgets/AnterMapNavigationPage.h"

void UStartMenuMapNavigationFSMState::StartState()
{
    Super::StartState();
    StartMenuMapNavigation();
}

void UStartMenuMapNavigationFSMState::EndState()
{
    if (AnterPawn.IsValid())
    {
        AnterPawn->OnMapLevelHit.RemoveDynamic(this, &UStartMenuMapNavigationFSMState::OnMapLevelHit);
        AnterPawn->OnMapLevelSelected.RemoveDynamic(this, &UStartMenuMapNavigationFSMState::OnMapLevelSelected);
    }
    Super::EndState();
}

void UStartMenuMapNavigationFSMState::StartMenuMapNavigation()
{
    TArray<AActor*> AnterPawnArray;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAnterMenuPawn::StaticClass(), AnterPawnArray);
    if(AnterPawnArray.Num())
    {
        AnterPawn = Cast<AAnterMenuPawn>(AnterPawnArray[0]);
    }
    if(AnterPawn.IsValid())
    {
        AnterPawn->OnMapLevelHit.AddDynamic(this, &UStartMenuMapNavigationFSMState::OnMapLevelHit);
        AnterPawn->OnMapLevelSelected.AddDynamic(this, &UStartMenuMapNavigationFSMState::OnMapLevelSelected);
        AnterPawn->NavigateToNextMarker(StartingLevelLabel);
    }
}

void UStartMenuMapNavigationFSMState::OnMapLevelHit(FString InLevelName)
{
    if (ULevelInfoManager* LevelInfoManager = ULevelInfoManager::Get(this))
    {
        const TMap<FString, FLevelInfo>& LevelMap = LevelInfoManager->GetLevelsMap();
        if (const FLevelInfo* CurrentLevelInfo = LevelMap.Find(InLevelName))
        {
            if (UAnterMapNavigationPage* CurrentPage = Cast<UAnterMapNavigationPage>(MainPage))
            {
                CurrentPage->FillLevelInfo(InLevelName, *CurrentLevelInfo);
            }
        }
        else
        {
            if (UAnterMapNavigationPage* CurrentPage = Cast<UAnterMapNavigationPage>(MainPage))
            {
                CurrentPage->CollapseLevelInfo();
            }
        }
    }
}

void UStartMenuMapNavigationFSMState::OnMapLevelSelected(FString InLevelName)
{
    if (ULevelInfoManager* LevelInfoManager = ULevelInfoManager::Get(this))
    {
        LevelInfoManager->SetCurrentLevelName(InLevelName);
    }
}
