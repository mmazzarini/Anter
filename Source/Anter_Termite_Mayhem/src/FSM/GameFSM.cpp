// Copyright Epic Games, Inc. All Rights Reserved.

#include "FSM/GameFSM.h"
#include "FSMState/GameFSMState.h"

/*
UGameFSM::UGameFSM(const FObjectInitializer& ObjectInitializer)
{
    //Operations at ctor. 
}


UGameFSM::~UGameFSM()z
{
}
*/

void UGameFSM::InitializeFSM(UObject* ContextObject)
{
    CurrentState.Reset();
    RegisterFSMStates();
    //SetupFSMStates();
    SetCurrentState(InitialStateString);
    if(ContextObject != nullptr)
    {
        OwnerComponent = ContextObject;
    }
    StartFSM();
}

void UGameFSM::SetCurrentState(FString InStateID)
{
    if(CurrentState.IsValid())
    {
        CurrentState->EndState();
        CurrentState.Reset();
    }

    UGameFSMState** FoundState = InternalArrayOfStates.FindByPredicate([=](UGameFSMState* FSMState)
    {
        return FSMState->GetFSMStateID() == InStateID;
    });

    if(FoundState != nullptr && *FoundState != nullptr)
    {
        CurrentState = *FoundState;
    }  
    else
    {
        CurrentState.Reset();
    }
}
 
//Create the FSM States
void UGameFSM::RegisterFSMStates()
{
    if(InternalArrayOfStates.Num()!=0)
    {
        for(UGameFSMState* FSMState : InternalArrayOfStates)
        {
            if(FSMState != nullptr)
            {
                FSMState->SetOwnerFSM(this);
            }
        }
    }
}

/*
void UGameFSM::SetupFSMStates()
{
    for(UGameFSMState* InternalState : InternalArrayOfStates)
    {
        if(InternalState != nullptr)
        {
            FFSMStateNavigationHandler* RequestedElem = FSMNavigator.FindByPredicate([&InternalState](const FFSMStateNavigationHandler NavigatorElem)
            {
                return NavigatorElem.StateID == InternalState->GetFSMStateID();
            });
            if(RequestedElem != nullptr)
            {
                InternalState->SetMapOfStateTransitions(RequestedElem->ActionTransitionMap);
            }
        }
    }
}
*/

void UGameFSM::TransitionToState(FString InStateID)
{

    SetCurrentState(*InStateID);
    /*
    FString CurrID = (CurrentState != nullptr ? CurrentState->GetFSMStateID() : "");

    FFSMStateNavigationHandler* NavElementRef = FSMNavigator.FindByPredicate([CurrID](const FFSMStateNavigationHandler& NavElem)
    {
        return NavElem.StateID == CurrID;
    });
    if(NavElementRef != nullptr)
    {
        if(FString* RequestedString = NavElementRef->ActionTransitionMap.Find(InState))
        {
            SetCurrentState(*RequestedString);
        }
        
    }
    */

    if(CurrentState != nullptr)
    {
        CurrentState->StartState();        
    }
}

void UGameFSM::StartFSM()
{
    if(CurrentState.IsValid())
    {
        CurrentState->StartState();
    }
}