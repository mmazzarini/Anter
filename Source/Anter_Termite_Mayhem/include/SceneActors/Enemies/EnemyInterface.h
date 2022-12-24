#pragma once

#include "UObject/Interface.h"

#include "EnemyInterface.generated.h"

UINTERFACE()
class UEnemyInterface : public UInterface
{
    GENERATED_BODY()
};

class IEnemyInterface
{
    GENERATED_BODY()
public:

    //Generic Enemy movement-update pure virtual function
    UFUNCTION()
    virtual void UpdateMovement() = 0;

    //Generic Enemy attack-update pure virtual function
    UFUNCTION()
    virtual void UpdateAttack() = 0;
};