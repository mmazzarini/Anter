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

void UAnterBaseButton::InitializeAnterWidget()
{
    //OnClicked.AddDynamic(this,&UAnterBaseButton::FireAction);
}

void UAnterBaseButton::UninitializeAnterWidget()
{
    //OnClicked.RemoveDynamic(this,&UAnterBaseButton::FireAction);
}