#pragma once

#include "Components/SceneComponent.h"
#include "SceneActors/AnterFire.h"
#include "TimerManager.h"

#include "AnterWeaponComponent.generated.h"

/*
*
* Class that defines the basic properties of the Weapon component to be attached to the Pawn.
* It is primarily used to shoot lasers/bullets.
*
*/


//Weapon recharged dyn multicast delegate
//NewWeaponValue: the new value of shots available in the weapon
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponRecharged, uint32, NewWeaponValue);

//Weapon uncharged dyn multicast delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponUncharged);




UCLASS(Blueprintable, BlueprintType)
class ANTER_TERMITE_MAYHEM_API UAnterWeaponComponent : public USceneComponent
{
    GENERATED_BODY()
    public:

        UAnterWeaponComponent();

        UFUNCTION(BlueprintCallable)
        virtual void ShootLaser();

        UFUNCTION()
        void OnTimerEnded();

        //Updates the Internal Ammos Counter.
        //NB InAmmosDifference can be both positive and negative, and must be specified.
        void UpdateAmmos(float InAmmosDifference);

        //void OnOwnerMoving(float InAxisValue);

        void SetLaserDirection(FVector InLaserDirection){LaserDirection = InLaserDirection;}

        bool CanShoot(){return bCanShoot;}

        UFUNCTION(BlueprintCallable)
        float GetAmmosCounter() const;

protected:

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<AAnterFire> LaserSubClass;

    UPROPERTY(EditAnywhere)
    float WeaponSpawnRange = 30.0f;

    UPROPERTY(EditAnywhere)
    float InFireRate = 0.2f;

    UPROPERTY(EditAnywhere, Category="Sequential Fire")
    float InSequentialFireRate = 0.2f;

    UPROPERTY(EditDefaultsOnly, Category="Sequential Fire")
    int32 MaxSequentialShots = 5;

    UFUNCTION(BlueprintCallable)
    void SetCanShoot(bool InCanShoot);

    bool bCanShoot;

    FTimerDelegate FireTimerDelegate;

    FTimerHandle FireTimerHandle;

    FTimerDelegate SequentialFireTimerDelegate;

    FTimerHandle SequentialFireTimerHandle;

    //Can assume only 1 or -1, to flag character direction
    //float MovementDirectionScaleFactor;

    //Vector representing the module-1-scaled geoemtry of the laser movement
    FVector LaserDirection;

    //Only for internal representation, can be accessed through Getter/Setter.
    float InternalAmmosCounter = 50.0f;

    UPROPERTY(EditDefaultsOnly, Category="Weapon Counting")
    float MaxWeaponCounter = 100.0f;

    UPROPERTY(EditDefaultsOnly, Category="Weapon Counting")
    bool ShouldUseWeaponCounter = false;

private:

    int32 InternalSequentialShotCounter = 0;

};

