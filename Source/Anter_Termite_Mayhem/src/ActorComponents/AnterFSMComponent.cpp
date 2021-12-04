#include "ActorComponents/AnterFSMComponent.h"
#include "Pawn/AnterPaperCharacter.h"

UAnterFSMComponent::UAnterFSMComponent()
: 
AnterFSM(nullptr)
{
    Anter = GetOwner<APaperCharacter>();
    /*AnterFSM = CreateDefaultSubobject<UAnterFSM>(this);
    if(AnterFSM != nullptr)
    {
        AnterFSM->AnterInitializer(this);
    }
    */
}
