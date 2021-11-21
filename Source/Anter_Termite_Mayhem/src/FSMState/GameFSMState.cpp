#include "FSMState/GameFSMState.h"

UGameFSMState::UGameFSMState()
:
OwnerFSM(nullptr)
{   
}

void UGameFSMState::SetOwnerFSM(UGameFSM* InOwnerFSM)
{
    OwnerFSM = InOwnerFSM;
}

void UGameFSMState::TransitionToState(FString InState)
{
}

void UGameFSMState::OnActionExecuted()
{
}

FString UGameFSMState::GetFSMStateID()
{
    return FSMStateID;
}

void UGameFSMState::KillState()
{
    //this->BeginDestroy();
}

