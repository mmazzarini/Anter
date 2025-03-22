#include "SceneActors/Platforms/AnimatedMovingPlatform.h"
#include "ActorComponents/MovingActorMovementSupportComponent.h"
#include "ActorComponents/BaseEnemyMovementComponent.h"

AAnimatedMovingPlatform::AAnimatedMovingPlatform()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bTickEvenWhenPaused = true;
}

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

    BounceTimeLineComp = Cast<UTimelineComponent>(AddComponentByClass(UTimelineComponent::StaticClass(), false, GetActorTransform(), false));
    IdleTimeLineComp = Cast<UTimelineComponent>(AddComponentByClass(UTimelineComponent::StaticClass(), false, GetActorTransform(), false));
    BindTimelineFunctions();

    InitialPlatformLocation = GetActorLocation();
}

void AAnimatedMovingPlatform::Tick(float DeltaTime)
{   
    Super::Tick(DeltaTime);
    if(CandidateCollision != nullptr)
    {
        if(ActorMovement != nullptr && !(ActorMovement->GetIsMovementActive()) && !GetIsPlaying())
        {
            //Start animations only if it was not playing loop
            StartShakeAnimation();
            SetIsPlaying(true);
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

        //TO BE CONTINUED HERE: WE NEED TO SET THE GO BACK TO POSITION ACTION AFTER IDLE ANIMATION
        //if(ActorMovement->Get)
        if (ActorMovement->GetLoopBehavior() == EEnemyLoopBehavior::LoopsAtArrive)
        {
            StartIdleAnimation();
        }
    }
}

bool AAnimatedMovingPlatform::GetIsPlaying()
{
    return bIsPlayingAnimation;
}

void AAnimatedMovingPlatform::SetIsPlaying(bool bInPlaying)
{
    bIsPlayingAnimation = bInPlaying;
}

void AAnimatedMovingPlatform::StartIdleAnimation()
{
    if (IdleTimeLineComp != nullptr)
    {
        IdleTimeLineComp->Play();
    }
}

void AAnimatedMovingPlatform::StartShakeAnimation()
{
    if (BounceTimeLineComp != nullptr)
    {
        BounceTimeLineComp->Play();
    }
}

void AAnimatedMovingPlatform::ResetMotionAfterAnimation() const
{
    if (ActorMovement != nullptr)
    {
        ActorMovement->SetIsMovementActive(true);
    }
    if (ActorBaseMotion != nullptr)
    {
        ActorBaseMotion->ResetSpeed();
    }
}

void AAnimatedMovingPlatform::SetupTimelineAnimation(UTimelineComponent* InTimelineComp, UCurveFloat* InCurve, FOnTimelineFloat& InExecTimeline, FOnTimelineEventStatic& InEndedTimeline)
{
    if (InTimelineComp != nullptr)
    {
        InTimelineComp->AddInterpFloat(InCurve, InExecTimeline);
        InTimelineComp->SetTimelineFinishedFunc(InEndedTimeline);
    }
    ActorVerticalPivot = GetActorLocation().Z;
}

void AAnimatedMovingPlatform::BindTimelineFunctions()
{
    VerticalTimelineFunc.BindUFunction(this, FName("OnVerticalDisplacementFloatUpdated"));
    BounceAnimationFinishedFunc.BindUFunction(this, FName("OnBounceAnimationFinished"));
    IdleTimelineFunc.BindUFunction(this, FName("OnIdleDisplacementFloatUpdated"));
    IdleAnimationFinishedFunc.BindUFunction(this, FName("OnIdleAnimationFinished"));
    SetupTimelineAnimation(BounceTimeLineComp, VerticalCurve, VerticalTimelineFunc, BounceAnimationFinishedFunc);
    SetupTimelineAnimation(IdleTimeLineComp, IdleCurve, IdleTimelineFunc, IdleAnimationFinishedFunc);
}

void AAnimatedMovingPlatform::OnVerticalDisplacementFloatUpdated(float InDisplacement)
{
    float NewZ = ActorVerticalPivot + InDisplacement;
    FVector BouncePosition = FVector(GetActorLocation().X, GetActorLocation().Y, NewZ);
    SetActorLocation(BouncePosition);
    GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Red, FString(TEXT("NewZ: ") + FString::SanitizeFloat(NewZ) + TEXT(": NewPosZ: ") + FString::SanitizeFloat(GetActorLocation().Z)));
}

void AAnimatedMovingPlatform::OnBounceAnimationFinished()
{
    if (BounceTimeLineComp != nullptr)
    {
        BounceTimeLineComp->SetPlaybackPosition(0.0f,false, false);
        BounceTimeLineComp->Stop();
    }
    ResetMotionAfterAnimation();
}

void AAnimatedMovingPlatform::OnIdleDisplacementFloatUpdated(float InDisplacement)
{
    //Do nothing - or implement otherwise
}

void AAnimatedMovingPlatform::OnIdleAnimationFinished()
{
    if (IdleTimeLineComp != nullptr)
    {
        IdleTimeLineComp->SetPlaybackPosition(0.0f, false, false);
        IdleTimeLineComp->Stop();
    }

    SetActorLocation(InitialPlatformLocation);
    if (ActorMovement != nullptr)
    {
        ActorMovement->ResetMovement();
        SetIsPlaying(false);
    }
}
