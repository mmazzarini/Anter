#pragma once

#include "UObject/Interface.h"
#include "SceneUtilities/SceneStructs.h"

#include "AnterInteractiveWidgetInterface.generated.h"

/*
This Interface can be inherited to initialize all buttons and widgets in a page.
*/

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActionFiredDelegate, FString, ActionNameParam);

UINTERFACE()
class UAnterInteractiveWidgetInterface : public UInterface
{
    GENERATED_BODY()

};

class IAnterInteractiveWidgetInterface 
{
    GENERATED_BODY()

public:

    /*Virtual pure methods to be implemented in derived classes (e.g. buttons in pages)*/
    UFUNCTION()
    virtual void InitializeAnterWidget() = 0;

    UFUNCTION()
    virtual void UninitializeAnterWidget() = 0;

    //Delegate
    FOnActionFiredDelegate OnActionFired;

};