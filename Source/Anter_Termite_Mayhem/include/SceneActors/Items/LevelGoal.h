#pragma once

#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

#include "LevelGoal.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelGoalReached);

UCLASS(Blueprintable,BlueprintType)
class ANTER_TERMITE_MAYHEM_API ALevelGoal : public AActor
{

    GENERATED_BODY()

public:

    ALevelGoal();

    void BeginPlay() override;

    void SetBindings();

    FOnLevelGoalReached LevelGoalReached;

protected:

    UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
    UStaticMeshComponent* LevelMesh;

    UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
    UBoxComponent* LevelBox;

    UFUNCTION()
    void OnCollidedWithActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};