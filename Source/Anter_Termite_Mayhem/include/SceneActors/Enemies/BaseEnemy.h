#pragma once

#include "GameFramework/Actor.h"

/*
Base Enemy class
An Enemy that is able to move right/left or up/down according to what is 
set by user on Blueprint defaults
Can be derived to obtain all the specified enemies
*/

UCLASS(BlueprintType,Blueprintable)
class ANTER_TERMITE_MAYHEM_API ABaseEnemy : public APaperCharacter
{

    GENERATED_BODY()

public:

    ABaseEnemy();

    

};