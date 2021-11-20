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

void UGameFSM::AnterInitializer(UAnterComponent* ContextObject)
{
    CreateFSMStates();
    GetFSMStateIdentifiers();
    SetCurrentState(InitialStateString);
    if(ContextObject != nullptr)
    {
        OwnerComponent = ContextObject;
    }
}

void UGameFSM::SetCurrentState(FString InState)
{
    FFSMStateSpecifier* CurrentPair = InternalArrayOfStates.FindByPredicate([=](FFSMStateSpecifier StateSpecifier)
    {
        return StateSpecifier.StateID == InState;
    });
    if(CurrentPair)
    {
        CurrentState = CurrentPair->FSMState;
    }  
    else
    {
        CurrentState = nullptr;
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
    StateSpecifier.StateID = "";
    StateSpecifier.FSMState = NewObject<UGameFSMState>(this, InSubclass);
    InternalArrayOfStates.Push(StateSpecifier);
}

void UGameFSM::SetupFSMStates()
{
    /*

    */
}

void UGameFSM::GetFSMStateIdentifiers()
{
    for(auto FSMSpecifier : InternalArrayOfStates)
    {
        UGameFSMState* FSMState = FSMSpecifier.FSMState;
        if(FSMState != nullptr)
        {
            FSMSpecifier.StateID = FSMState->GetFSMStateID();
        }
    }
}