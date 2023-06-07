#pragma once

#include "UObject/Object.h"
#include "Components/SceneComponent.h"

#include "DamageComponent.generated.h"

/*
*
* Class that defines the basic damage behavior transmitted to what collides the owner pawn.
*
*/

UCLASS(Blueprintable, BlueprintType)
class ANTER_TERMITE_MAYHEM_API UDamageComponent : public USceneComponent
{
    GENERATED_BODY()

public:

    UDamageComponent();

    //Getter for is damage by pawn 
    bool IsPawnDamage(){return bIsPawnDamageComponent;}

    bool GetDamageValue(){return DamageValue;}

private:

    UPROPERTY(EditInstanceOnly)
    bool bIsPawnDamageComponent = false;

    UPROPERTY(EditInstanceOnly)
    float DamageValue = 1.0f;

};