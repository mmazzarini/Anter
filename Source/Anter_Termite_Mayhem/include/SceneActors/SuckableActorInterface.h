#pragma once

#include "UObject/Interface.h"
#include "SceneUtilities/SceneStructs.h"

#include "SuckableActorInterface.generated.h"

/*
This Interface can be inherited to specify behaviours of scene actors, such as management of collision and 
other interactive functionalities 
*/

UINTERFACE()
class USuckableActorInterface : public UInterface
{
    GENERATED_BODY()

};

class ISuckableActorInterface
{
    GENERATED_BODY()

public:

    UFUNCTION()
    virtual void Activate() = 0;

    virtual void UpdateSuckProgress(float InPercentage) =0;

    UFUNCTION()
    virtual bool GetIsActivated() const = 0;

protected:

    bool bIsActivated = false;
};