#include "FSMState/GameFSMState.h"
#include "Blueprint/UserWidget.h"
#include "UI/AnterWidgets/AnterBasePage.h"
#include "FSM/GameFSM.h"

UGameFSMState::UGameFSMState(const FObjectInitializer& ObjectInitializer)
:
Super(ObjectInitializer),
OwnerFSM(nullptr)
{   
}

void UGameFSMState::SetOwnerFSM(UGameFSM* InOwnerFSM)
{
    OwnerFSM = InOwnerFSM;
}

void UGameFSMState::StartState()
{
    CreatePage();
}

void UGameFSMState::CreatePage()
{
    if(MainPage == nullptr)
    {
        MainPage = CreateWidget<UAnterBasePage>(GetWorld(),MainPageClass,MainPageName);
    }
    check(MainPage != nullptr);
    StartPage();
}

void UGameFSMState::StartPage()
{
    if(MainPage != nullptr)
    {
        MainPage->AddToViewport();
        MainPage->InitializePage();
        MainPage->ActionDelegate.AddDynamic(this,&UGameFSMState::OnActionExecuted);
    }
}

void UGameFSMState::TransitionToState(FString InState)
{
    if(OwnerFSM != nullptr)
    {
        if(FString* RequestedString = MapOfStateTransitions.Find(InState))
        {
            OwnerFSM->TransitionToState(*RequestedString);
        }
    }
}

void UGameFSMState::OnActionExecuted(FString InTriggerName)
{
    TransitionToState(InTriggerName);
    //This should handle execution of actions triggered by a bound widget
}

FString UGameFSMState::GetFSMStateID()
{
    return FSMStateID;
}

void UGameFSMState::EndState()
{
    if(MainPage != nullptr)
    {
        MainPage->ActionDelegate.RemoveDynamic(this,&UGameFSMState::OnActionExecuted);
        MainPage->UninitializePage();
        MainPage->RemoveFromViewport();
    }
}

void UGameFSMState::SetMapOfStateTransitions(const TMap<FString, FString >& InMapOfStateTransitions)
{
    for(auto& StateElem : InMapOfStateTransitions)
    {
        MapOfStateTransitions.Add(StateElem);
    }
}
