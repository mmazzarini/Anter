#include "FSMState/GameFSMState.h"
#include "Blueprint/UserWidget.h"
#include "UI/AnterWidgets/AnterBasePage.h"
#include "FSM/GameFSM.h"

UGameFSMState::UGameFSMState()
:
OwnerFSM(nullptr)
{   
}

void UGameFSMState::SetOwnerFSM(UGameFSM* InOwnerFSM)
{
    OwnerFSM = InOwnerFSM;
}

void UGameFSMState::StartState()
{

}

void UGameFSMState::CreatePage()
{
    MainPage = CreateWidget<UAnterBasePage>(GetWorld(),MainPageClass,MainPageName);
    check(MainPage != nullptr);
    StartPage();
}

void UGameFSMState::StartPage()
{
    if(MainPage != nullptr)
    {
        MainPage->Initialize();
    }
}
void UGameFSMState::TransitionToState(FString InState)
{
    if(OwnerFSM != nullptr)
    {
        OwnerFSM->TransitionToState(InState);
    }
}

void UGameFSMState::OnActionExecuted()
{
    //This should handle execution of actions triggered by a bound widget
}

FString UGameFSMState::GetFSMStateID()
{
    return FSMStateID;
}

void UGameFSMState::EndState()
{
    //this->BeginDestroy();
    //Do operations to End FSMState's execution (e.g. unbind from page etc)
}

