#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "HealthContainer.generated.h"

//This class is a base class that defines the Health structure of a character
//It can be possessed by a character/pawn via ActorComponent properties, that use this Container, depending on the 
//character/actor and its design.

UCLASS()
class ANTER_TERMITE_MAYHEM_API UHealthContainer : public UObject
{
    GENERATED_BODY()

    public:

        UHealthContainer();

        ~UHealthContainer();

    //Tick is needed because the container may update the health at each step, depending on the implementation.

    virtual void Tick(float DeltaTime) override;

    //This is needed to update the health of the container associated to the Health Component

    virtual void UpdateHealth();

    void SetHealth(int32 InHealth);

    int32 GetHealth(int32 InHealth);
   

    protected:

    //All pawns/characters have integer-like health
    int32 CoreHealth;
        
}