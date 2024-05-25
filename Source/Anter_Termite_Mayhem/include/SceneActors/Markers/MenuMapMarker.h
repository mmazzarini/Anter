#pragma once

#include "GameFramework/Actor.h"

#include "MenuMapMarker.generated.h"


UCLASS(BlueprintType,Blueprintable)
class ANTER_TERMITE_MAYHEM_API AMenuMapMarker : public AActor
{
    GENERATED_BODY()

public:

    const TMap<FString,FString> GetMenuNavigation() const;

    const FString& GetLevelPath() const;

protected:

    //Map coupling the direction name with the destination marker.
    UPROPERTY(EditInstanceOnly)
    TMap<FString,FString> MenuNavigationMap;

    UPROPERTY(EditInstanceOnly)
    FString LevelPath = "";
};