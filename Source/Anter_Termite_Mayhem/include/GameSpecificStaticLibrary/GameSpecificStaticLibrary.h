#pragma once

/*
This header file and its corresponding cpp are necessary to include all 
Static library objects that are used in the game.
The GameSpecificStaticLibrary is the class encapsulating all these.
*/

#include "GameSpecificStaticLibrary.generated.h"

class UHealthComponent;
class UDamageComponent;
class AActor;

UCLASS()
class ANTER_TERMITE_MAYHEM_API UGameSpecificStaticLibrary : public UObject 
{

    GENERATED_BODY()

public:

    static bool IsHealthDamageType(AActor* InDamagedActor, AActor* InDamagingActor);

};