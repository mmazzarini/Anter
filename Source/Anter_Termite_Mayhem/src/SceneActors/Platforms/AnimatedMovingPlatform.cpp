#include "SceneActors/Platforms/AnimatedMovingPlatform.h"
#include "ActorComponents/MovingActorMovementSupportComponent.h"
#include "ActorComponents/BaseEnemyMovementComponent.h"

void AAnimatedMovingPlatform::BeginPlay()
{
    Super::BeginPlay();
    ActorMovement = FindComponentByClass<UMovingActorMovementSupportComponent>();
    if(ActorMovement != nullptr)
    {
        ActorMovement->SetIsMovementActive(false);
        ActorMovement->OnMovementCompleted.AddDynamic(this,&AAnimatedMovingPlatform::OnMovementComplete);
    }

    ActorBaseMotion = FindComponentByClass<UBaseEnemyMovementComponent>();
    if(ActorBaseMotion != nullptr)
    {
        ActorBaseMotion->PauseSpeed();
    }
}

void AAnimatedMovingPlatform::Tick(float DeltaTime)
{   
    Super::Tick(DeltaTime);
    if(CandidateCollision != nullptr)
    {
        if(ActorMovement != nullptr && ActorMovement->GetIsMovementActive() == false)
        {
            ActorMovement->SetIsMovementActive(true);
            if(ActorBaseMotion != nullptr)
            {
                ActorBaseMotion->ResetSpeed();
            }
        }
    }
}

void AAnimatedMovingPlatform::OnMovementComplete()
{
    if(ActorMovement != nullptr)
    {
        ActorMovement->SetIsMovementActive(false);
    }
    if(ActorBaseMotion != nullptr)
    {
        ActorBaseMotion->PauseSpeed();
    }
}
/*
Todo
We need to implement:
- collision system to bind with pawn collision event
- wirh collision, you check if the motion has started or not (we need other event)
- we need animations to bind and to proceed
- we need a control to reset state to starting state after or the loop has been executed.

- DOUBT:Do we need to control the logic for looping back?
*/
