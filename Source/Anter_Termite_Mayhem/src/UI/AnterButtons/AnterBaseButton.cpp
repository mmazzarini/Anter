#include "UI/AnterButtons/AnterBaseButton.h"

UAnterBaseButton::UAnterBaseButton(const FObjectInitializer& ObjectInitializer)
    :Super(ObjectInitializer)
{
}

void UAnterBaseButton::FireAction()
{
    OnActionFired.Broadcast(ButtonAction);
}

void UAnterBaseButton::InitializeAnterWidget()
{
    OnClicked.AddDynamic(this,&UAnterBaseButton::FireAction);
}

void UAnterBaseButton::UninitializeAnterWidget()
{
    OnClicked.RemoveDynamic(this,&UAnterBaseButton::FireAction);
}