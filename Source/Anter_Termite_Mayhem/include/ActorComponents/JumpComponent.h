#pragma once
#include "Components/ActorComponent.h"
#include "TimerManager.h" 

#include "JumpComponent.generated.h"

class AActor;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnJumpRequestedDelegate, FVector);

UCLASS(Blueprintable, BlueprintType)
class ANTER_TERMITE_MAYHEM_API  UJumpComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	void BeginPlay() override;

	void BindToActorMovement();

	UFUNCTION()
	void RequestJump();

	void DestroyComponent(bool bPromoteChildren = false) override;

	UFUNCTION()
	void ResetTimer();

	FOnJumpRequestedDelegate OnJumpRequested;

protected:

	UPROPERTY(EditDefaultsOnly)
	FVector JumpStrength = FVector(1000.0f, 0.0f, 0.0f);

	FTimerDelegate JumpTimerDelegate;

	FTimerHandle JumpHandle;

	UPROPERTY(EditInstanceOnly)
	float JumpTimer = 5.f;

	UPROPERTY(EditInstanceOnly)
	float JumpDelay = 0.f;

private:

	void ClearTimer();

	TWeakObjectPtr<AActor> OwningActor;
};
