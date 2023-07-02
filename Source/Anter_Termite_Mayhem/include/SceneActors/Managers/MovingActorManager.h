#pragma once 

#include "GameFramework/Actor.h"
#include "SceneUtilities/SceneStructs.h"
#include "MovingActorManager.generated.h"

/*
Manager class for moving actors. This can be used for enemies as well.
*/

class UMovingActorMovementSupportComponent;


UCLASS(Blueprintable,BlueprintType)
class ANTER_TERMITE_MAYHEM_API AMovingActorManager : public AActor
{
    GENERATED_BODY()

    AMovingActorManager(){}

public:

    void BeginPlay() override;

    void FillMovingActorPositions();

    void InjectMovingActorBehavior();

protected:

    //EnemyPositions represents the array of enemy pivot positions, calculated relative to the Manager ActorLocation 
    UPROPERTY(EditAnywhere)
    TArray<FVector> MovingActorPositions;

    UPROPERTY()
    AActor* MovingActor;

    UPROPERTY(EditAnywhere)
    TSubclassOf<AActor> MovingActorClass;

    UPROPERTY(EditInstanceOnly)
    float MovingActorPivotDistanceThreshold;

    UPROPERTY(EditInstanceOnly)
    EEnemyLoopBehavior MovingActorLoopBehavior;

    UMovingActorMovementSupportComponent* MovingActorMovementSupport;

};