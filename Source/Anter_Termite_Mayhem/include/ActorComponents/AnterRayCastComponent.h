#pragma once

#include "Components/SceneComponent.h"
#include "SceneActors/SceneActorInterface.h"

#include "AnterRayCastComponent.generated.h"

UCLASS(Blueprintable, BlueprintType)
class ANTER_TERMITE_MAYHEM_API UAnterRayCastComponent : public USceneComponent
{
	GENERATED_BODY()

public: 

	void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void RegisterInterfaceOwnerCharacter();

	UAnterRayCastComponent();

protected:

	void RayCast();
	
	void ResetRayCastFlags();
	void VerticalRayCast();
	void HorizontalBackRayCast();
	void HorizontalFrontRayCast();
	void SendGeometryAndFlags();
	void ProcessHorizontallyBlockingGeometry(const TArray<FHitResult>& ObjectsHit, bool bFront);
	void ProcessVerticallyBlockingGeometry(const TArray<FHitResult>& ObjectsHit);
	
	//#TODO Refactor using tscriptinterface
	ISceneActorInterface* OwnerActorInterfacePtr;

	UPROPERTY()
	FVector OwnerActorCentre = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY()
	FVector OwnerActorSize = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditDefaultsOnly)
	float VerticalSizeCorrection = 1.2f;

	UPROPERTY(EditDefaultsOnly)
	float HorizontalSizeCorrection = 1.2f;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AActor>> CollidingActorClassesToCheck;

	UPROPERTY(EditDefaultsOnly, meta=(ClampMin = "0.6", ClampMax="1.0"))
	float GeometryScalarProductTolerance = 0.6f;

private:

	bool bHitVertically = false;
	bool bHitHorizontallyFront = false;
	bool bHitHorizontallyBack = false;
	bool bBlockedHorizontallyFront = false;
	bool bBlockedHorizontallyBack = false;
	FVector RotatedNormal = FVector(1.0f, 0.0f, 0.0f);
	FVector ImpactPoint;
};