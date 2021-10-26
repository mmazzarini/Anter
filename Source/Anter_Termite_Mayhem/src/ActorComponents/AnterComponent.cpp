#include "ActorComponents/AnterComponent.h"
#include "Pawn/AnterPawn.h"

UAnterComponent::UAnterComponent()
: 
AnterFSM(nullptr)
{
    Anter = GetOwner<AAnterPawn>();
    bAutoActivate = true; 
}

UAnterComponent::~UAnterComponent()
{

}
