#pragma once

#include "UObject/Interface.h"

#include "EnemyManagerInterface.generated.h"

UINTERFACE()
class UEnemyManagerInterface : public UInterface
{
    GENERATED_BODY()
};

class IEnemyManagerInterface
{
    GENERATED_BODY()
public:

    UFUNCTION()
    virtual void FillEnemyPositions() = 0;

    UFUNCTION()
    virtual void InjectEnemyLoopBehavior() = 0;
};