#include "SceneActors/Managers/BackgroundManager.h"
#include "AnterGameModes/AnterBaseLevelGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/PlayerController.h"

ABackgroundManager::ABackgroundManager()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;
    PrimaryActorTick.TickGroup = TG_PostUpdateWork;
}

void ABackgroundManager::BeginPlay()
{
    Super::BeginPlay();

    if(AAnterBaseLevelGameMode* LevelGMode = Cast<AAnterBaseLevelGameMode>(UGameplayStatics::GetGameMode(this)))
    {
        LevelGMode->PlayerRestartedDelegate.AddDynamic(this,&ABackgroundManager::OnPlayerRestarted);
    }

    if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
    {
        CameraManager = PC->PlayerCameraManager;
    }
    
    if (CameraManager.IsValid())
    {
        InitialCameraLocation = CameraManager->GetCameraLocation();
    }

    BackgroundLevelToTileMaps.Empty();

    for (TActorIterator<AActor>BckgIt(GetWorld()); BckgIt; ++BckgIt)
    {
        if (AActor* BkgTile = *BckgIt)
        {
            for (TPair<uint32, FBckgLayerProperties>& BckgLevelInfo : BackgroundLevelToParallaxes)
            {
                if (BkgTile->ActorHasTag(FName(BckgLevelInfo.Value.ParallaxTag)))
                {
                    BackgroundLevelToTileMaps.Add(BckgLevelInfo.Key, BkgTile);
                    BckgLevelInfo.Value.InitialPosition = BkgTile->GetActorLocation();
                    break;
                }
            }
        }
    }

}

void ABackgroundManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if (CameraManager.IsValid())
    {

        CurrentCameraLocation = CameraManager->GetCameraLocation();

        if (!bParallaxInitialized)
        {
            InitialCameraLocation = CurrentCameraLocation;
            bParallaxInitialized = true;
            return;
        }

        for (const TPair<uint32, FBckgLayerProperties> ParallaxLevel : BackgroundLevelToParallaxes)
        {
            if (TWeakObjectPtr<AActor>* BckgTilePtr = BackgroundLevelToTileMaps.Find(ParallaxLevel.Key))
            {
                TWeakObjectPtr <AActor> BckgTile = *BckgTilePtr;
                if (BckgTile.IsValid())
                {
                    float YLoc = BckgTile->GetActorLocation().Y;
                    float XLoc = ParallaxLevel.Value.InitialPosition.X + (CurrentCameraLocation.X - InitialCameraLocation.X) * ParallaxLevel.Value.ParallaxFactor;
                    float ZLoc = ParallaxLevel.Value.InitialPosition.Z + (CurrentCameraLocation.Z - InitialCameraLocation.Z) * ParallaxLevel.Value.ParallaxFactor;
                    BckgTile->SetActorLocation(FVector(XLoc, YLoc, ZLoc));
                }

            }
        }
    }

    
}

void ABackgroundManager::OnPlayerRestarted()
{
    // unused? ( . )___(O)
}