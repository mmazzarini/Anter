#include "ActorComponents/AnterFSMComponent.h"
#include "Pawn/AnterPawn.h"

UAnterFSMComponent::UAnterFSMComponent()
: 
AnterFSM(nullptr)
{
    Anter = GetOwner<AAnterPawn>();
    /*AnterFSM = CreateDefaultSubobject<UAnterFSM>(this);
    if(AnterFSM != nullptr)
    {
        AnterFSM->AnterInitializer(this);
    }
    */
}
