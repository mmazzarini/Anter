#pragma once 

#include "SceneActors/Platforms/BasePlatform.h"

#include "MovingPlatform.generated.h"

UCLASS(Blueprintable,BlueprintType)
class ANTER_TERMITE_MAYHEM_API AMovingPlatform : public ABasePlatform
{
    GENERATED_BODY()

public:

    AMovingPlatform(){}

    void BeginPlay() override {Super::BeginPlay();};
    
};