#pragma once

#include "PaperCharacter.h"

#include "AnterMenuPawn.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMapLevelHitDelegate, FString, LevelName);

UCLASS(BlueprintType)
class ANTER_TERMITE_MAYHEM_API AAnterMenuPawn : public APaperCharacter
{
    GENERATED_BODY()
    
public:

    AAnterMenuPawn();

    void BeginPlay() override;

    void SetupPlayerInputComponent(UInputComponent* InInputComponent) override;

    void MoveToNextMarker();

    //Movement functions
    void HandleMoveDown();

    //Movement functions
    void HandleMoveUp();

    //Movement functions
    void HandleMoveLeft();

    //Movement functions
    void HandleMoveRight();

    void HandleLevelSelected();

    void NavigateToNextMarker(const FString& InStartingMarkerTag);

    void Tick(float DeltaTime) override;

    FMapLevelHitDelegate OnMapLevelHit;

protected:

    AActor* RetrieveTargetMarker(const FString& InStartingMarkerTag);

    UPROPERTY(EditDefaultsOnly)
    float NavigationSpeed = 50.f;

    UPROPERTY(EditDefaultsOnly)
    float MarkerDistanceThreshold = 50.f;

private:

    UPROPERTY(EditDefaultsOnly)
    float ThresholdToNextPivot = 50.0f;

    //Map coupling the direction name with the destination marker.
    TMap<FString,FString> InternalNavigationMap;

    bool bIsMovingToMarker = false;

    TWeakObjectPtr<AActor> MarkerRef;

};
