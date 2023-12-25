#pragma once
#include "Pawn/AnterPaperCharacter.h"
#include "Components/SceneComponent.h"

#include "EnemyAIComponent.generated.h"

//This class describes a generic, base AI component for enemies. 
//Its main goal is to represent the behavior of enemies, and control it at tick time.

UCLASS(Blueprintable, BlueprintType)
class ANTER_TERMITE_MAYHEM_API UEnemyAIComponent : public UActorComponent
{
    GENERATED_BODY()
    public:

        UEnemyAIComponent();

        //Main Tick component. We inject here the controls over shooting and 
        //Other possible evaluations.
        //This must be called by means of Super::TickComponent() on child classes of UEnemyAIComponent
        virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction) override;

        virtual void BeginPlay() override;

        virtual void SetTargetLocation();

        //Throws a random number and then calls Shoot() on weapon component
        virtual void EvaluateShoot();

        //We call this to make a check on Anter's position. Should be overridden in derived classes
        UFUNCTION()
        void SetAnterPointer();

    protected:

        //Pointer used to point to 
        UPROPERTY()
        AAnterPaperCharacter* AnterPtr;

        UPROPERTY()
        FVector TargetLocation;

        UPROPERTY(EditDefaultsOnly)
        float RandomThresholdLimitForShooting;

        UAnterWeaponComponent* EnemyWeapon;
};

