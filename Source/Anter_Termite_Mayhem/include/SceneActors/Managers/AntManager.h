#pragma once 

#include "GameFramework/Actor.h"

#include "AntManager.generated.h"

/*
Manager class for crates. It is used to manage crate spawning and destruction state in between checkpoints 
*/

class AAnterBaseAnt;

UCLASS(Blueprintable,BlueprintType)
class ANTER_TERMITE_MAYHEM_API AAntManager : public AActor
{
    GENERATED_BODY()

public:

    virtual void BeginPlay() override;

    AAntManager(){}

protected:

    UPROPERTY()
    AAnterBaseAnt* Ant;

    UPROPERTY(EditAnywhere)
    TSubclassOf<AAnterBaseAnt> AntClass;
};