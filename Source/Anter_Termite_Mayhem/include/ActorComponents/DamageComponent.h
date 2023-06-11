#pragma once

#include "UObject/Object.h"
#include "Components/SceneComponent.h"
#include "SceneUtilities/SceneStructs.h"

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

    float GetDamageValue(){return DamageValue;}

    EHealthHarmlessType GetTypeOfDamage(){return TypeOfDamage;}

private:

    UPROPERTY(EditDefaultsOnly)
    EHealthHarmlessType TypeOfDamage = EHealthHarmlessType::NoDamage;

    UPROPERTY(EditDefaultsOnly)
    float DamageValue = -1.0f;

};