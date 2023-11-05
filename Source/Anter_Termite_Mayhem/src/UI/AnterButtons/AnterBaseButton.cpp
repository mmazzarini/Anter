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
    OnPressed.AddDynamic(this,&UAnterBaseButton::FireAction);
}

void UAnterBaseButton::UninitializeAnterWidget()
{
    OnPressed.RemoveDynamic(this,&UAnterBaseButton::FireAction);
}