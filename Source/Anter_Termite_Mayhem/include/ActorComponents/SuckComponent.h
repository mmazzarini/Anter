#pragma once

#include "UObject/Object.h"
#include "Components/SceneComponent.h"
#include "SceneUtilities/SceneStructs.h"
#include "TimerManager.h"

#include "SuckComponent.generated.h"

class AActor;

/*
*
* Class that defines the basic properties of Health component attached to a Pawn.
* It uses a CoreHealth variable to repreent health.
* This class can be derived to satisfy the specific behavior of the Pawn class.
*
*/

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMaxSuckReachedDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSuckingValueUpdated, float, SuckingFractionValue);

UCLASS(Blueprintable, BlueprintType)
class ANTER_TERMITE_MAYHEM_API USuckComponent : public USceneComponent
{
    GENERATED_BODY()

public:

    USuckComponent();

    virtual void BeginPlay() override;

    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    //This function updates the core health of the health container
    UFUNCTION()
    void StartIncreaseSuckingDuration();

    //This function is used to calculate the new value of health and change it 
    UFUNCTION()
    void ResetSucking();
    
    void SetCanSuck(bool bInCanSuck);

    bool GetCanSuck() const;

    bool GetIsSucking() const;

    UPROPERTY(EditDefaultsOnly)
    float SuckMaxDuration = 10.0f;

    UPROPERTY(EditDefaultsOnly)
    float DeltaSuckingDuration = 0.5f;

    UPROPERTY(EditDefaultsOnly)
    float TimerDelay = 0.1f; 
    
    UPROPERTY(EditDefaultsOnly)
    float TimerRate = 0.1f;
    
    FOnMaxSuckReachedDelegate OnMaxSuckReached;

    FOnSuckingValueUpdated OnSuckValueUpdate;

protected:

    UFUNCTION()
    void OnTimerEnded();

    FTimerHandle SuckTimerHandle;

private:

    void IncreaseSuckValue();

    void OnMaxSuckDurationReached();

    void UpdateSuckingProgressionValue();

    bool bCanSuck = false;

    bool bIsSucking = false;

    float SuckingDuration = 0.0f;

    AActor* OwningActor;
};