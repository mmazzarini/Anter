#pragma once

#include "SceneActors/Platforms/AnimatedMovingPlatform.h"

#include "ActiveByCollisionAnimatedMovingPlatform.generated.h"

//Lethal moving platform

UCLASS(Blueprintable,BlueprintType)
class ANTER_TERMITE_MAYHEM_API AActiveByCollisionAnimatedMovingPlatform : public AAnimatedMovingPlatform
{
    GENERATED_BODY()

protected:

    void OnStartShakeAnimation() override;
};