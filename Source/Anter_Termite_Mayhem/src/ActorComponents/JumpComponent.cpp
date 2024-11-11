#include "ActorComponents/JumpComponent.h"
#include "ActorComponents/BaseEnemyMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/Actor.h"
#include "SceneActors/Enemies/BaseEnemy.h"

void UJumpComponent::BeginPlay()
{
	Super::BeginPlay();
	BindToActorMovement();
}

void UJumpComponent::BindToActorMovement()
{
	if (GetOwner() != nullptr)
	{
		if(ABaseEnemy* OwningEnemy = Cast<ABaseEnemy>(GetOwner()))
		{ 
			OwningEnemy->OnGravityNullified.AddDynamic(this, &UJumpComponent::ResetTimer);
		}
	}
}

void UJumpComponent::RequestJump()
{
	OnJumpRequested.Broadcast(JumpStrength);
	if (ABaseEnemy* OwningEnemy = Cast<ABaseEnemy>(GetOwner()))
	{
		OwningEnemy->ResetGravity();
	}
}

void UJumpComponent::DestroyComponent(bool bPromoteChildren)
{
	if (GetOwner() != nullptr)
	{
		if (ABaseEnemy* OwningEnemy = Cast<ABaseEnemy>(GetOwner()))
		{
			OwningEnemy->OnGravityNullified.RemoveDynamic(this, &UJumpComponent::RequestJump);
		}
	}
	Super::DestroyComponent(bPromoteChildren);
}

void UJumpComponent::ResetTimer()
{
	ClearTimer();
	if (GetOwner() != nullptr)
	{
		GetOwner()->GetWorldTimerManager().SetTimer(JumpHandle, this, &UJumpComponent::RequestJump, JumpTimer, true, JumpDelay);
	}
}

void UJumpComponent::ClearTimer()
{
	if (GetOwner() != nullptr)
	{
		GetOwner()->GetWorldTimerManager().ClearTimer(JumpHandle);
	}
}