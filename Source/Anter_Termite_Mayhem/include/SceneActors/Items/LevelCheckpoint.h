#pragma once

#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

#include "LevelCheckpoint.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCheckpointActivatedDelegate, ALevelCheckpoint*, CurrentCheckpointPtr);

//Base class for checkpoint. 
//Behavior: if the checkpoint is triggered (trigger placeholder event: contact with Anter): the checkpoint broadcasts an OnActivated event
//The broadcaster may be used to activate the checkpoint status
//implementation otherwise is similar to LevelGoal

UCLASS(Blueprintable,BlueprintType)
class ANTER_TERMITE_MAYHEM_API ALevelCheckpoint : public AActor
{
    
    GENERATED_BODY()

public:

    ALevelCheckpoint();

    void ActivateCheckpoint();

    FCheckpointActivatedDelegate OnActivatedCheckpointDelegate;

    UFUNCTION(BlueprintImplementableEvent)
    void UpdateCheckpointSuckProgress(float InPercentage);

    bool GetIsActivated() const;

protected:

    UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
    UStaticMeshComponent* LevelMesh;

    UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
    UBoxComponent* LevelBox;

    bool bIsActivated = false;

};