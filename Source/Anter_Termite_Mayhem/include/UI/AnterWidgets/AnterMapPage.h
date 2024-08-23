#pragma once

#include "UI/AnterWidgets/AnterBasePage.h"

#include "AnterMapPage.generated.h"

//Base class describing base page for menu and pause menu.
UCLASS()
class ANTER_TERMITE_MAYHEM_API UAnterMapPage : public UAnterBasePage
{
    GENERATED_BODY()

public: 

    void NativeConstruct() override;

};
