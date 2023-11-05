#pragma once

#include "UI/AnterInteractiveWidgetInterface.h"
#include "Components/Button.h"

#include "AnterBaseButton.generated.h"

//Anter base button class. Can be used and derived for menu buttons and pause menu buttons.

UCLASS(Blueprintable, BlueprintType)
class ANTER_TERMITE_MAYHEM_API UAnterBaseButton : public UButton, public IAnterInteractiveWidgetInterface
{

    GENERATED_BODY()

public:

    UAnterBaseButton(const FObjectInitializer& ObjectInitializer);

    UFUNCTION()
    void FireAction();

    virtual void InitializeAnterWidget() override;

    virtual void UninitializeAnterWidget() override;

protected:

    UPROPERTY(EditInstanceOnly, Category = "Interaction")
    FString ButtonAction = "NoAction";
};
