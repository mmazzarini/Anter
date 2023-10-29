#pragma once

#include "CommonButtonBase.h"

#include "AnterBaseButton.generated.h"

//Anter base button class. Can be used and derived for menu buttons and pause menu buttons.

UCLASS(Blueprintable, BlueprintType)
class ANTER_TERMITE_MAYHEM_API UAnterBaseButton : public UCommonButtonBase
{

    GENERATED_BODY()

public:

    UAnterBaseButton();
};

