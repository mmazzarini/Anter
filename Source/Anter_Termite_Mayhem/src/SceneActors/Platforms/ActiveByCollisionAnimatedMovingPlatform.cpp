#include "SceneActors/Platforms/ActiveByCollisionAnimatedMovingPlatform.h"
#include "Components/CapsuleComponent.h"
#include "ActorComponents/MovingActorMovementSupportComponent.h"

void AActiveByCollisionAnimatedMovingPlatform::OnStartShakeAnimation()
{
	Super::OnStartShakeAnimation();
    if (ActorMovement != nullptr && !(ActorMovement->GetIsMovementActive()) && !GetIsEngaged())
    {
        //Start animations only if it was not playing loop
        StartShakeAnimation();
    }
}
