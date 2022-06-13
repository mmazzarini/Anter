#pragma once

#include "UObject/Interface.h"
#include "SceneUtilities/SceneStructs.h"

#include "SceneActorInterface.generated.h"

/*
This Interface can be inherited to specify behaviours of scene actors, such as management of collision and 
other interactive functionalities 
*/

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

};