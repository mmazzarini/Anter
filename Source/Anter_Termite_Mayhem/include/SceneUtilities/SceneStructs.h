#pragma once

#include "SceneStructs.generated.h"

/*
Class that describes the geometrical properties of the colliding platform and of the
*/

USTRUCT()
struct FCollisionGeometry
{
    GENERATED_BODY()

    FCollisionGeometry()
    {}

    FVector PlatformCentre = FVector(0.0f,0.0f,0.0f);
    FVector PlatformSize  = FVector(0.0f,0.0f,0.0f);
    FVector PlatformSurfaceCentre = FVector(0.0f,0.0f,0.0f);
    FVector PlatformBottomCentre = FVector(0.0f,0.0f,0.0f);
    FVector PlatformRightSideCentre = FVector(0.0f,0.0f,0.0f);
    FVector PlatformLeftSideCentre = FVector(0.0f,0.0f,0.0f);
    FVector SideDist = FVector(0.0f,0.0f,0.0f);
    FVector TopDist = FVector(0.0f,0.0f,0.0f);
    FVector BottomDist = FVector(0.0f,0.0f,0.0f);
    FVector RotatedNormal = FVector(0.0f,0.0f,0.0f);

    float PlatformLength = 0.0f;
    float PlatformHeight = 0.0f;
};

UENUM(BlueprintType)
enum class EEnemyLoopBehavior : uint8
{
    LoopsAtArrive,
    GoesBackward
};

/*Used to identify the type of damage produced by actors*/
UENUM(BlueprintType)
enum class EHealthHarmlessType : uint8
{
    NoDamage,
    YouLaserDamage,
    EnemyLaserDamage,
    AnterDamage,
    EnemyDamage,
    CrateDamage,
    SqueezingDamage
};

/*
Enum to control the pivot arrays filling state of the enemy
*/
UENUM()
enum class EEnemyPivotState : uint8
{
    IsWaitingToBeFilled,
    HasBeenFilled,
    HasStartedMoving
};

/*
Enum to represent the running orientation of the enemy
*/
UENUM()
enum class EEnemyRunningOrientation : uint8
{
    IsRunningForward,
    IsRunningBackward
};