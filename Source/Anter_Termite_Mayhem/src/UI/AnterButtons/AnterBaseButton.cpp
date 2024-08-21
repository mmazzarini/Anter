#include "UI/AnterButtons/AnterBaseButton.h"

UAnterBaseButton::UAnterBaseButton(const FObjectInitializer& ObjectInitializer)
    :Super(ObjectInitializer)
{
}

FReply UAnterBaseButton::NativeOnMouseButtonUp(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
    Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
    OnActionFired.Broadcast(ButtonAction);
    return FReply::Handled();
}

/*
FReply UAnterBaseButton::NativeOnKeyDown(const FGeometry & InGeometry, const FKeyEvent & InKeyEvent)
{
    Super::NativeOnKeyDown(InGeometry, InKeyEvent);
    return FReply::Handled();
}
*/


FReply UAnterBaseButton::NativeOnKeyUp(const FGeometry & InGeometry, const FKeyEvent & InKeyEvent)
{
    if(InKeyEvent.GetKey().ToString().Equals(InteractionString))
    {
        OnActionFired.Broadcast(ButtonAction);
    }
    return Super::NativeOnKeyUp(InGeometry, InKeyEvent);
}


void UAnterBaseButton::InitializeAnterWidget()
{
    //OnClicked.AddDynamic(this,&UAnterBaseButton::FireAction);
}

void UAnterBaseButton::UninitializeAnterWidget()
{
    //OnClicked.RemoveDynamic(this,&UAnterBaseButton::FireAction);
}

    /*
FReply UAnterBaseButton::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{

    Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
    ForegroundColor = FocusColor;
    return FReply::Handled();

}
    */
    /*
void UAnterBaseButton::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{

    Super::NativeOnFocusLost(InFocusEvent);
    ForegroundColor = UnfocusColor;

}
    */