#pragma once

#include "UI/AnterWidgets/AnterBasePage.h"
#include "SceneActors/Managers/LevelInfoManager.h"

#include "AnterMapNavigationPage.generated.h"

//Base class describing base page for menu and pause menu.
UCLASS()
class ANTER_TERMITE_MAYHEM_API UAnterMapNavigationPage : public UAnterBasePage
{
    GENERATED_BODY()

public: 

    UFUNCTION(BlueprintImplementableEvent)
    void FillLevelInfo(const FString& InLevelName, const FLevelInfo& InLevelInfo);

    UFUNCTION(BlueprintImplementableEvent)
    void CollapseLevelInfo();

protected:

    //This bind widget property is useful to give info about level data: level name, objects collected, records etc...
    UPROPERTY(meta = (BindWidgetOptional))
    UWidget* LevelInfoWidget;

};
