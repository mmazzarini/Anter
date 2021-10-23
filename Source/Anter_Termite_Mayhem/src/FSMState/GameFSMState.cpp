// Copyright Epic Games, Inc. All Rights Reserved.

#include "FSMState/GameFSMState.h"

UGameFSMState::UGameFSMState()
{
}

UGameFSMState::UGameFSMState(UGameFSM* InitOwnerFSM)
: OwnerFSM(InitOwnerFSM)
{   
}

void UGameFSMState::TransitionToState(FString InState)
{
}

void UGameFSMState::OnActionExecuted()
{
}

void UGameFSMState::SetTrialFloat(float InFloat)
{
    TrialFloat = InFloat;
}

FString UGameFSMState::GetFSMStateID()
{
    return FSMStateID;
}

void UGameFSMState::KillState()
{
    //this->BeginDestroy();
}

