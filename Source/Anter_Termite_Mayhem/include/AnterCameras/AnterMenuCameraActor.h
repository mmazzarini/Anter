#pragma once

#include "Camera/CameraActor.h"

#include "AnterMenuCameraActor.generated.h"

/*M
*
* Base class representing Anter camera
*
*/

class AAnterMenuPawn;

UCLASS(Blueprintable, BlueprintType)
class ANTER_TERMITE_MAYHEM_API AAnterMenuCameraActor : public ACameraActor
{
    GENERATED_BODY()

public:

    AAnterMenuCameraActor();

    void BeginPlay() override;

    void Tick(float DeltaTime) override;

protected:

    UPROPERTY(EditInstanceOnly)
    float VerticalHeightFromPawn = 1000.f;

private:

    TWeakObjectPtr<AAnterMenuPawn> AnterMenuPawnRef;

};