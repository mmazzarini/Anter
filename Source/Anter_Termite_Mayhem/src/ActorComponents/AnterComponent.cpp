#pragma once

#include "ActorComponents/AnterComponent.h"
#include "Pawn/AnterPawn.h"

UAnterComponent::UAnterComponent()
: 
AnterFSM(nullptr)
{
    Anter = GetOwner<AAnterPawn>();
    bAutoActivate = true; 
    AnterFSM = NewObject<UAnterFSM>(this);
}

UAnterComponent::~UAnterComponent()
{

}
