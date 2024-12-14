#include "ActorComponents/SuckComponent.h"
#include "GameFramework/Actor.h"
 

USuckComponent::USuckComponent()
    :
    SuckingDuration(0.0f)
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;
}

void USuckComponent::BeginPlay()
{
    Super::BeginPlay();
    OwningActor = GetOwner();
}

void USuckComponent::StartIncreaseSuckingDuration()
{
    if (bCanSuck)
    {
        bIsSucking = true;
        if (OwningActor != nullptr)
        {
            OwningActor->GetWorldTimerManager().SetTimer(SuckTimerHandle, this, &USuckComponent::OnTimerEnded, TimerRate, false, TimerDelay);
        }
    }
}
 
void USuckComponent::ResetSucking()
{
    if (OwningActor != nullptr)
    {
        OwningActor->GetWorldTimerManager().ClearTimer(SuckTimerHandle);
    }
    bIsSucking = false;
    SuckingDuration = 0.0f;
    UpdateSuckingProgressionValue();
}

void USuckComponent::SetCanSuck(bool bInCanSuck)
{
    bCanSuck = bInCanSuck;
}

bool USuckComponent::GetCanSuck() const
{
    return bCanSuck;
}

bool USuckComponent::GetIsSucking() const
{
    return bIsSucking;
}

void USuckComponent::OnTimerEnded()
{
    if (OwningActor != nullptr)
    {
        IncreaseSuckValue();
        if (SuckingDuration >= SuckMaxDuration)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("MAX Duration reached!!"));
            OnMaxSuckDurationReached();
        }
        else
        {
            OwningActor->GetWorldTimerManager().ClearTimer(SuckTimerHandle);
            OwningActor->GetWorldTimerManager().SetTimer(SuckTimerHandle, this, &USuckComponent::OnTimerEnded, TimerRate, false, TimerDelay);
            double CurrentTimeClock = FPlatformTime::Seconds();
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Reset timer Time: ") + FString::SanitizeFloat(CurrentTimeClock));

        }
    }
}

void USuckComponent::IncreaseSuckValue()
{
    SuckingDuration += DeltaSuckingDuration;
    UpdateSuckingProgressionValue();
}

void USuckComponent::OnMaxSuckDurationReached()
{
    OnMaxSuckReached.Broadcast();
    ResetSucking();
    bIsSucking = false;
    bCanSuck = false;
}

void USuckComponent::UpdateSuckingProgressionValue()
{
    OnSuckValueUpdate.Broadcast(SuckingDuration / SuckMaxDuration);
}

void USuckComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}