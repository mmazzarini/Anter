#pragma once

#include "GameFramework/Actor.h"

#include "BaseDestroyable.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDestroyedObject, ABaseDestroyable, DestroyedObjectPtr)
{

}

UCLASS(Blueprintable,BlueprintType)
class ANTER_TERMITE_MAYHEM_API ABaseDestroyable : public AActor
{
    GENERATED_BODY()

public:

    ABaseDestroyable();



};