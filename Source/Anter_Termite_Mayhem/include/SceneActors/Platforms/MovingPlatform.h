#pragma once 

#include "SceneActors/Platforms/BasePlatform.h"
#include "Pawn/AnterPaperCharacter.h"

#include "MovingPlatform.generated.h"

UCLASS(Blueprintable,BlueprintType)
class ANTER_TERMITE_MAYHEM_API AMovingPlatform : public ABasePlatform
{
    GENERATED_BODY()

public:

    AMovingPlatform();

    void Tick(float DeltaTime) override;

    void UpdateMovement();

    virtual void Setup();

protected:

    FVector OldPosition;

    FVector NewPosition;

    TWeakObjectPtr<AAnterPaperCharacter> AnterRef;

    TPair<AActor*,EPlatformCollisionType>* CandidateCollision;
    
};
