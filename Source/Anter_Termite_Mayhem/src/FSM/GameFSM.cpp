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
    CreateFSMStates();
    SetupFSMStates();
    SetCurrentState(InitialStateString);
    if(ContextObject != nullptr)
    {
        OwnerComponent = ContextObject;
    }
    StartFSM();
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
    if(MapOfTSubclassStates.Num()!=0)
    {
        for(auto FSMStatePair : MapOfTSubclassStates)
        {
            AddFSMState(FSMStatePair.Key, FSMStatePair.Value);
        }
    }
 
}

void UGameFSM::AddFSMState(const FString& InClassID, const TSubclassOf<UGameFSMState>& InSubclass)
{
    //Let's build the internal Array of states, by adding a new state and ID
    FFSMStateSpecifier StateSpecifier;
    StateSpecifier.FSMState = NewObject<UGameFSMState>(this, InSubclass);
    check(StateSpecifier.FSMState != nullptr);
    StateSpecifier.FSMState->SetOwnerFSM(this);
    StateSpecifier.StateID = InClassID;
    // #TODO Remove  this, check it is not useful.
    StateSpecifier.FSMState->SetFSMStateID(InClassID);
    InternalArrayOfStates.Push(StateSpecifier);
}

void UGameFSM::SetupFSMStates()
{
    for(FFSMStateSpecifier InternalState : InternalArrayOfStates)
    {
        if(InternalState.FSMState != nullptr)
        {
            FFSMStateNavigationHandler* RequestedElem = FSMNavigator.FindByPredicate([&InternalState](const FFSMStateNavigationHandler NavigatorElem)
            {
                return NavigatorElem.StateID == InternalState.StateID;
            });
            if(RequestedElem != nullptr)
            {
                InternalState.FSMState->SetMapOfStateTransitions(RequestedElem->ActionTransitionMap);
            }
        }
    }
}

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