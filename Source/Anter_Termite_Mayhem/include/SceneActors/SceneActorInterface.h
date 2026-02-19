#pragma once

#include "UObject/Interface.h"
#include "SceneUtilities/SceneStructs.h"

#include "SceneActorInterface.generated.h"

/*
This Interface can be inherited to specify behaviours of scene actors, such as management of collision and 
other interactive functionalities 
*/


//Little structure to be referenced by Pawn in order to impose its local geometry while moving
USTRUCT()
struct FGeometron
{
    GENERATED_BODY();
    float X = 1.0f;
    float Z = 0.0f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FActorGeometryChangeDelegate, float, XDirGeometryParam, float, ZDirGeometryParam);

UINTERFACE()
class USceneActorInterface : public UInterface
{
    GENERATED_BODY()

};

class ISceneActorInterface 
{
    GENERATED_BODY()

public:

    /* Pure virtual functions */
    UFUNCTION()
    virtual void HandleCollision(const FCollisionGeometry& InCollisionGeometry, AActor* OtherActor) = 0;

    UFUNCTION()
    virtual void UpdateWeaponDirection(float InLaserDirection) = 0;

    UFUNCTION()
    virtual void ProcessRayCastGeometry(const FGeometron& InGeometron, bool bHitVertically, bool bHitHorizontallyFront, bool bHitHorizontallyBack) = 0;

protected:

    UFUNCTION()
    virtual void HandleDamage(AActor* OtherActor) = 0;
};