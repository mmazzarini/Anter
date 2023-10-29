// Copyright Epic Games, Inc. All Rights Reserved.

#include "FSM/GameFSM.h"
#include "FSMState/GameFSMState.h"

/*
UGameFSM::UGameFSM(const FObjectInitializer& ObjectInitializer)
{
    //Operations at ctor. 
}


UGameFSM::~UGameFSM()
{
}
*/

void UGameFSM::InitializeFSM(UObject* ContextObject)
{
    CurrentState.Reset();
    CreateFSMStates();
    SetCurrentState(InitialStateString);
    if(ContextObject != nullptr)
    {
        OwnerComponent = ContextObject;
    }
}

void UGameFSM::SetCurrentState(FString InState)
{
    if(CurrentState.IsValid())
    {
        CurrentState->EndState();
        CurrentState.Reset();
    }

    FFSMStateSpecifier* CurrentPair = InternalArrayOfStates.FindByPredicate([=](FFSMStateSpecifier StateSpecifier)
    {
        return StateSpecifier.StateID == InState;
    });

    if(CurrentPair != nullptr)
    {
        CurrentState = CurrentPair->FSMState;
    }  
    else
    {
        CurrentState.Reset();
    }
}
 
//Create the FSM States
void UGameFSM::CreateFSMStates()
{
    if(ArrayOfTSubclassStates.Num()!=0)
    {
        for(auto FSMStateSubclass : ArrayOfTSubclassStates)
        {
            AddFSMState(FSMStateSubclass);
        }
    }
 
}

void UGameFSM::AddFSMState(const TSubclassOf<UGameFSMState>& InSubclass)
{
    FFSMStateSpecifier StateSpecifier;
    StateSpecifier.FSMState = NewObject<UGameFSMState>(this, InSubclass);
    check(StateSpecifier.FSMState != nullptr);
    StateSpecifier.FSMState->SetOwnerFSM(this);
    StateSpecifier.StateID = StateSpecifier.FSMState->GetFSMStateID();
    InternalArrayOfStates.Push(StateSpecifier);
}

void UGameFSM::SetupFSMStates()
{
    /*

    */
}

void UGameFSM::TransitionToState(FString InNewState)
{
    //TODO implement
}

void UGameFSM::StartFSM()
{
    if(CurrentState.IsValid())
    {
        CurrentState->StartState();
    }
}