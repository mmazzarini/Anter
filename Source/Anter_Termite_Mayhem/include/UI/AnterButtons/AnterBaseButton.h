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

    //virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;

   //virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;

    virtual FReply NativeOnMouseButtonUp(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent) override;

    //virtual FReply NativeOnKeyDown ( const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

    virtual FReply NativeOnKeyUp ( const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;



protected:

    UPROPERTY(EditInstanceOnly, Category = "Interaction")
    FString ButtonAction = "NoAction";

    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Interaction")
    FLinearColor FocusColor = FLinearColor::Transparent;

    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Interaction")
    FLinearColor UnfocusColor = FLinearColor::Transparent;

    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Interaction")
    FLinearColor StartingColor = FLinearColor::Transparent;

    UPROPERTY(EditInstanceOnly, Category = "Interaction")
    FString InteractionString = "Enter";
};
