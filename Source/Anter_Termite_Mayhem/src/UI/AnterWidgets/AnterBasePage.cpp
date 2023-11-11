#include "UI/AnterWidgets/AnterBasePage.h"
#include "Components/PanelWidget.h"
#include "UI/AnterInteractiveWidgetInterface.h"

void UAnterBasePage::InitializePage()
{
    if(PagePanel != nullptr)
    {
        TArray<UWidget*> PageChildWidgets = PagePanel->GetAllChildren();
        for(UWidget* ChildWidget : PageChildWidgets)
        {
            if(IAnterInteractiveWidgetInterface* CastedChild = Cast<IAnterInteractiveWidgetInterface>(ChildWidget))
            {
                CastedChild->InitializeAnterWidget();
                CastedChild->OnActionFired.AddDynamic(this,&UAnterBasePage::OnActionExecuted);
            }
        }
    }
}

void UAnterBasePage::UninitializePage()
{
    if(PagePanel != nullptr)
    {
        TArray<UWidget*> PageChildWidgets = PagePanel->GetAllChildren();
        for(UWidget* ChildWidget : PageChildWidgets)
        {
            if(IAnterInteractiveWidgetInterface* CastedChild = Cast<IAnterInteractiveWidgetInterface>(ChildWidget))
            {
                CastedChild->UninitializeAnterWidget();
                CastedChild->OnActionFired.RemoveDynamic(this,&UAnterBasePage::OnActionExecuted);
            }
            //ChildWidget->RemoveFromParent();
        }
    }
}

void UAnterBasePage::OnActionExecuted(FString InActionName)
{
    ActionDelegate.Broadcast(InActionName);
}