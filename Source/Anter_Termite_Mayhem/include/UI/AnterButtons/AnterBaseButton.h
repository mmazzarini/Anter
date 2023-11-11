#pragma once

#include "UI/AnterInteractiveWidgetInterface.h"
#include "Blueprint/UserWidget.h"

#include "AnterBaseButton.generated.h"

//Anter base button class. Can be used and derived for menu buttons and pause menu buttons.

UCLASS(Blueprintable, BlueprintType)
class ANTER_TERMITE_MAYHEM_API UAnterBaseButton : public UUserWidget, public IAnterInteractiveWidgetInterface
{

    GENERATED_BODY()

public:

    UAnterBaseButton(const FObjectInitializer& ObjectInitializer);

    //UFUNCTION(BlueprintCallable)
    //void FireAction();

    virtual void InitializeAnterWidget() override;

    virtual void UninitializeAnterWidget() override;

    virtual FReply NativeOnMouseButtonUp(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent) override;

protected:

    UPROPERTY(EditInstanceOnly, Category = "Interaction")
    FString ButtonAction = "NoAction";
};
