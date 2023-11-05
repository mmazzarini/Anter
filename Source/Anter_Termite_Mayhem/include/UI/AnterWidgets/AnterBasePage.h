#pragma once

#include "Blueprint/UserWidget.h"

#include "AnterBasePage.generated.h"

class UPanelWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActionExecuted, FString, ActionParameter);

//Base class describing base page for menu and pause menu.
UCLASS(Blueprintable, BlueprintType)
class ANTER_TERMITE_MAYHEM_API UAnterBasePage : public UUserWidget
{
    GENERATED_BODY()

public: 

    FOnActionExecuted ActionDelegate;

    void InitializePage();

    void UninitializePage();

    UFUNCTION()
    void OnActionExecuted(FString InActionName);

    UPROPERTY(BlueprintReadWrite,meta=(BindWidget))
    UPanelWidget* PagePanel;

};
