#pragma once

#include "Components/SceneComponent.h"
#include "SceneActors/AnterFire.h"

#include "AnterWeaponComponent.generated.h"

/*
*
* Class that defines the basic properties of the Weapon component to be attached to the Pawn.
* It is primarily used to shoot lasers/bullets.
*
*/

UCLASS(Blueprintable, BlueprintType)
class ANTER_TERMITE_MAYHEM_API UAnterWeaponComponent : public USceneComponent
{
    GENERATED_BODY()
    public:

        UAnterWeaponComponent();

        UFUNCTION(BlueprintCallable)
        void ShootLaser();

    protected:
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<AAnterFire> LaserSubClass;

    UPROPERTY(EditAnywhere)
    float WeaponSpawnRange = 30.0f;

};

