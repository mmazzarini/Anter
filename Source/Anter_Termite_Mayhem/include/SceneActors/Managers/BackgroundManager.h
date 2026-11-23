#pragma once

#include "GameFramework/Actor.h"
#include "BackgroundManager.generated.h"

class APlayerCameraManager;

USTRUCT()
struct ANTER_TERMITE_MAYHEM_API FBckgLayerProperties
{
    GENERATED_BODY()

    UPROPERTY(EditInstanceOnly)
    float ParallaxFactor;

    UPROPERTY(EditInstanceOnly)
    FString ParallaxTag;

    FVector InitialPosition;
};

UCLASS(Abstract)
class ANTER_TERMITE_MAYHEM_API ABackgroundManager : public AActor
{
    GENERATED_BODY()
public:

    ABackgroundManager();

    void BeginPlay() override;

    void Tick(float DeltaTime) override;

protected:

    UFUNCTION()
    void OnPlayerRestarted();

    UPROPERTY(EditInstanceOnly)
    TMap<uint32, FBckgLayerProperties> BackgroundLevelToParallaxes;

    UPROPERTY()
    TMap<uint32, TWeakObjectPtr<AActor>> BackgroundLevelToTileMaps;


    TWeakObjectPtr<APlayerCameraManager> CameraManager;

    FVector CurrentCameraLocation;

    FVector InitialCameraLocation;
};