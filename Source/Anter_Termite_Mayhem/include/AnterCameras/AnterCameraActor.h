#pragma once

#include "Camera/CameraActor.h"

#include "AnterCameraActor.generated.h"

/*
*
* Base class representing Anter camera
*
*/

UCLASS(Blueprintable, BlueprintType)
class ANTER_TERMITE_MAYHEM_API AAnterCameraActor : public ACameraActor
{
    GENERATED_BODY()

    void Tick(float DeltaTime) override;

};