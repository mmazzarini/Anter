#pragma once 

#include "SceneActors/Platforms/BasePlatform.h"

#include "MovingPlatform.generated.h"

class AAnterPaperCharacter;

UCLASS(Blueprintable,BlueprintType)
class ANTER_TERMITE_MAYHEM_API AMovingPlatform : public ABasePlatform
{
    GENERATED_BODY()

public:

    AMovingPlatform();

    void BeginPlay() override;

    void Tick(float DeltaTime) override;

    void UpdateMovement();

protected:

    FVector OldPosition;

    FVector NewPosition;

    AAnterPaperCharacter* Anter;

    
};