#pragma once

#include "GameFramework/Pawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "PaperCharacter.h"
#include "Engine/StaticMeshActor.h"

#include "BasePlatform.generated.h"

//Class representing a base platform literaly doing nothing!

UCLASS(Blueprintable,BlueprintType)
class ANTER_TERMITE_MAYHEM_API ABasePlatform : public APawn
{
    GENERATED_BODY()

public:

    ABasePlatform();

    void BeginPlay() override;
    
    UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
    UStaticMeshComponent* PlatformMesh;

    //UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
    //UBoxComponent* PlatformBox;
    
    UPROPERTY()
    FString InnerMeshName = "";
    
};