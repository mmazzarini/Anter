#pragma once

#include "GameFramework/Actor.h"
#include "LevelCheckpoint.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_One_Param(FCheckpointActivatedDelegate, ALevelCheckpoint*, CurrentCheckpointPtr)

UCLASS(Blueprintable,BlueprintType)
class ANTER_TERMITE_MAYHEM_API ALevelCheckpoint : public AActor
{
    
    GENERATED_BODY()

public:

    ALevelCheckpoint(){}

    FCheckpointActivatedDelegate OnActivatedCheckpointDelegate;

};